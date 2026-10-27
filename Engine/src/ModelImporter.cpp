#include "Engine/ModelImporter.h"

#include "Engine/ComponentMesh.h"
#include "Engine/ComponentTransform.h"
#include "Engine/GameObject.h"
#include "Engine/Log.h"
#include "Engine/Scene.h"
#include "PathUtil.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <chrono>
#include <memory>
#include <vector>

namespace Engine::ModelImporter
{
namespace
{
using MeshCache = std::vector<std::shared_ptr<Mesh>>;

aiLogStream g_logStream{};
bool g_logAttached = false;

void AssimpLogCallback(const char* message, char* /*user*/)
{
    std::string text(message);
    while (!text.empty() && (text.back() == '\n' || text.back() == '\r'))
    {
        text.pop_back();
    }
    ENGINE_LOGF("[Assimp] {}", text);
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

// Una malla d'Assimp pot ser referenciada per diversos nodes: la convertim una sola vegada.
std::shared_ptr<Mesh> GetOrCreateMesh(const aiScene& scene, unsigned int index, MeshCache& cache)
{
    if (cache[index] == nullptr)
    {
        cache[index] = ConvertMesh(*scene.mMeshes[index]);
    }
    return cache[index];
}

void ImportNode(const aiScene& scene, const aiNode& node, GameObject& parent, MeshCache& cache)
{
    GameObject& gameObject = parent.CreateChild(node.mName.length > 0 ? node.mName.C_Str() : "Node");

    // Descomponem la matriu del node: posició, rotació (quaternió) i escala per separat.
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
        if (IsTriangleMesh(*scene.mMeshes[node.mMeshes[i]]))
        {
            meshIndices.push_back(node.mMeshes[i]);
        }
    }

    if (meshIndices.size() == 1)
    {
        gameObject.AddComponent<ComponentMesh>(GetOrCreateMesh(scene, meshIndices.front(), cache));
    }
    else
    {
        // Diverses malles al mateix node: un GameObject fill per cadascuna.
        for (const unsigned int index : meshIndices)
        {
            const aiMesh& source = *scene.mMeshes[index];
            GameObject& child = gameObject.CreateChild(source.mName.length > 0 ? source.mName.C_Str() : "Mesh");
            child.AddComponent<ComponentMesh>(GetOrCreateMesh(scene, index, cache));
        }
    }

    for (unsigned int i = 0; i < node.mNumChildren; ++i)
    {
        ImportNode(scene, *node.mChildren[i], gameObject, cache);
    }
}
} // namespace

void Init()
{
    if (g_logAttached)
    {
        return;
    }
    // Els missatges d'Assimp van a la nostra consola.
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

    MeshCache cache(imported->mNumMeshes);
    GameObject& owner = (parent != nullptr) ? *parent : scene.GetRoot();
    ImportNode(*imported, *imported->mRootNode, owner, cache);

    GameObject& root = *owner.GetChildren().back();
    root.SetName(ToUtf8(FromUtf8(utf8Path).stem()));

    aiReleaseImport(imported);

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
    ENGINE_LOGF("Model '{}' loaded in {} ms", root.GetName(), elapsed.count());
    return &root;
}
} // namespace Engine::ModelImporter
