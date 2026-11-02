#include "Engine/ModelImporter.h"

#include "Engine/ComponentMesh.h"
#include "Engine/ComponentTexture.h"
#include "Engine/ComponentTransform.h"
#include "Engine/GameObject.h"
#include "Engine/Log.h"
#include "Engine/Scene.h"
#include "PathUtil.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <algorithm>
#include <cctype>
#include <chrono>
#include <filesystem>
#include <memory>
#include <system_error>
#include <vector>

namespace Engine::ModelImporter
{
namespace
{
namespace fs = std::filesystem;

aiLogStream g_logStream{};
bool g_logAttached = false;
std::string g_resourcesRoot;

struct ImportContext
{
    ImportContext(const aiScene& importedScene, fs::path directory)
        : scene(importedScene),
          modelDirectory(std::move(directory)),
          meshes(importedScene.mNumMeshes),
          textures(importedScene.mNumMaterials),
          textureTried(importedScene.mNumMaterials, false)
    {
    }

    const aiScene& scene;
    fs::path modelDirectory;
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::vector<std::shared_ptr<Texture>> textures;
    std::vector<bool> textureTried;
};

void AssimpLogCallback(const char* message, char* user)
{
    std::string text(message);
    while (!text.empty() && (text.back() == '\n' || text.back() == '\r'))
    {
        text.pop_back();
    }
    ENGINE_LOGF("[Assimp] {}", text);
}

std::string ToLowerAscii(std::string text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return text;
}

bool IsTriangleMesh(const aiMesh& mesh)
{
    return (mesh.mPrimitiveTypes & aiPrimitiveType_TRIANGLE) != 0 && mesh.mNumFaces > 0;
}

std::shared_ptr<Mesh> ConvertMesh(const aiMesh& source)
{
    const bool hasNormals = source.HasNormals();
    const bool hasUvs = source.HasTextureCoords(0);

    std::vector<Vertex> vertices;
    vertices.reserve(source.mNumVertices);
    for (unsigned int i = 0; i < source.mNumVertices; ++i)
    {
        Vertex vertex{};
        vertex.position = glm::vec3(source.mVertices[i].x, source.mVertices[i].y, source.mVertices[i].z);
        vertex.normal = hasNormals ? glm::vec3(source.mNormals[i].x, source.mNormals[i].y, source.mNormals[i].z) : glm::vec3(0.0f, 1.0f, 0.0f);
        vertex.uv = hasUvs ? glm::vec2(source.mTextureCoords[0][i].x, source.mTextureCoords[0][i].y) : glm::vec2(0.0f);
        vertices.push_back(vertex);
    }

    std::vector<std::uint32_t> indices;
    indices.reserve(static_cast<std::size_t>(source.mNumFaces) * 3);
    for (unsigned int i = 0; i < source.mNumFaces; ++i)
    {
        const aiFace& face = source.mFaces[i];
        if (face.mNumIndices != 3)
        {
            ENGINE_LOG("WARNING: geometry face with != 3 indices, skipped");
            continue;
        }
        indices.insert(indices.end(), {face.mIndices[0], face.mIndices[1], face.mIndices[2]});
    }

    const std::string name = source.mName.length > 0 ? source.mName.C_Str() : "Mesh";
    return Mesh::Create(name, std::move(vertices), std::move(indices));
}

std::shared_ptr<Mesh> GetOrCreateMesh(ImportContext& context, unsigned int index)
{
    if (context.meshes[index] == nullptr)
    {
        context.meshes[index] = ConvertMesh(*context.scene.mMeshes[index]);
    }
    return context.meshes[index];
}

fs::path SearchByName(const fs::path& root, const std::string& lowerFileName)
{
    std::error_code error;
    if (root.empty() || !fs::is_directory(root, error))
    {
        return {};
    }
    for (const auto& entry : fs::recursive_directory_iterator(root, error))
    {
        if (entry.is_regular_file(error) && ToLowerAscii(ToUtf8(entry.path().filename())) == lowerFileName)
        {
            return entry.path();
        }
    }
    return {};
}

fs::path FindTextureFile(const std::string& referenced, const fs::path& modelDirectory)
{
    const std::size_t separator = referenced.find_last_of("/\\");
    const std::string fileName = separator == std::string::npos ? referenced : referenced.substr(separator + 1);
    if (fileName.empty())
    {
        return {};
    }

    std::error_code error;
    const fs::path next(modelDirectory / FromUtf8(fileName));
    if (fs::is_regular_file(next, error))
    {
        return next;
    }
    const fs::path asGiven = FromUtf8(referenced);
    if (fs::is_regular_file(asGiven, error))
    {
        return asGiven;
    }

    const std::string lowerName = ToLowerAscii(fileName);
    fs::path found = SearchByName(modelDirectory, lowerName);
    if (found.empty())
    {
        found = SearchByName(FromUtf8(g_resourcesRoot), lowerName);
    }
    return found;
}

std::shared_ptr<Texture> GetOrLoadTexture(ImportContext& context, unsigned int materialIndex)
{
    if (materialIndex >= context.textures.size())
    {
        return nullptr;
    }
    if (context.textureTried[materialIndex])
    {
        return context.textures[materialIndex];
    }
    context.textureTried[materialIndex] = true;

    const aiMaterial* material = context.scene.mMaterials[materialIndex];
    if (aiGetMaterialTextureCount(material, aiTextureType_DIFFUSE) == 0)
    {
        return nullptr;
    }

    aiString path;
    if (aiGetMaterialTexture(material, aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr) != aiReturn_SUCCESS)
    {
        return nullptr;
    }

    const std::string referenced = path.C_Str();
    if (referenced.empty() || referenced.front() == '*')
    {
        ENGINE_LOGF("Embedded textures are not supported ('{}')", referenced);
        return nullptr;
    }

    const fs::path file = FindTextureFile(referenced, context.modelDirectory);
    if (file.empty())
    {
        ENGINE_LOGF("Texture '{}' referenced by the model was not found", referenced);
        return nullptr;
    }

    context.textures[materialIndex] = Texture::LoadFromFile(ToUtf8(file));
    return context.textures[materialIndex];
}

void AttachMesh(GameObject& gameObject, unsigned int meshIndex, ImportContext& context)
{
    const aiMesh& source = *context.scene.mMeshes[meshIndex];
    gameObject.AddComponent<ComponentMesh>(GetOrCreateMesh(context, meshIndex));

    if (std::shared_ptr<Texture> texture = GetOrLoadTexture(context, source.mMaterialIndex))
    {
        gameObject.AddComponent<ComponentTexture>(std::move(texture));
    }
}

void ImportNode(const aiNode& node, GameObject& parent, ImportContext& context)
{
    GameObject& gameObject = parent.CreateChild(node.mName.length > 0 ? node.mName.C_Str() : "Node");

    aiVector3D scaling;
    aiQuaternion rotation;
    aiVector3D position;
    node.mTransformation.Decompose(scaling, rotation, position);

    ComponentTransform& transform = gameObject.GetTransform();
    transform.SetPosition(glm::vec3(position.x, position.y, position.z));
    transform.SetRotation(glm::quat(rotation.w, rotation.x, rotation.y, rotation.z));
    transform.SetScale(glm::vec3(scaling.x, scaling.y, scaling.z));

    std::vector<unsigned int> meshIndices;
    for (unsigned int i = 0; i < node.mNumMeshes; ++i)
    {
        if (IsTriangleMesh(*context.scene.mMeshes[node.mMeshes[i]]))
        {
            meshIndices.push_back(node.mMeshes[i]);
        }
    }

    if (meshIndices.size() == 1)
    {
        AttachMesh(gameObject, meshIndices.front(), context);
    }
    else
    {
        for (const unsigned int index : meshIndices)
        {
            const aiMesh& source = *context.scene.mMeshes[index];
            GameObject& child = gameObject.CreateChild(source.mName.length > 0 ? source.mName.C_Str() : "Mesh");
            AttachMesh(child, index, context);
        }
    }

    for (unsigned int i = 0; i < node.mNumChildren; ++i)
    {
        ImportNode(*node.mChildren[i], gameObject, context);
    }
}
}

void Init()
{
    if (g_logAttached)
    {
        return;
    }
    g_logStream.callback = AssimpLogCallback;
    g_logStream.user = nullptr;
    aiAttachLogStream(&g_logStream);
    g_logAttached = true;
    ENGINE_LOG("Assimp log stream attached");
}

void Shutdown()
{
    if (g_logAttached)
    {
        aiDetachAllLogStreams();
        g_logAttached = false;
    }
}

void SetResourcesRoot(const std::string& utf8Path)
{
    g_resourcesRoot = utf8Path;
}

GameObject* Load(Scene& scene, const std::string& utf8Path, GameObject* parent)
{
    const auto start = std::chrono::steady_clock::now();
    ENGINE_LOGF("Importing '{}'", utf8Path);

    const unsigned int flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType;
    const aiScene* imported = aiImportFile(utf8Path.c_str(), flags);
    if (imported == nullptr || imported->mRootNode == nullptr || !imported->HasMeshes())
    {
        ENGINE_LOGF("Error loading '{}': {}", utf8Path, aiGetErrorString());
        if (imported != nullptr)
        {
            aiReleaseImport(imported);
        }
        return nullptr;
    }

    ImportContext context(*imported, FromUtf8(utf8Path).parent_path());
    GameObject& owner = (parent != nullptr) ? *parent : scene.GetRoot();
    ImportNode(*imported->mRootNode, owner, context);

    GameObject& root = *owner.GetChildren().back();
    root.SetName(ToUtf8(FromUtf8(utf8Path).stem()));

    aiReleaseImport(imported);

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
    ENGINE_LOGF("Model '{}' loaded in {} ms", root.GetName(), elapsed.count());
    return &root;
}
}