#include "Engine/Texture.h"

#include "Engine/Log.h"
#include "Engine/ResourceStats.h"
#include "GLInclude.h"
#include "PathUtil.h"

#include <IL/il.h>

#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>
#include <iterator>
#include <vector>

#ifndef GL_TEXTURE_MAX_ANISOTROPY
    #define GL_TEXTURE_MAX_ANISOTROPY 0x84FE
#endif
#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY
    #define GL_MAX_TEXTURE_MAX_ANISOTROPY 0x84FF
#endif

namespace Engine
{
namespace
{
std::vector<const Texture*>& Registry()
{
    static std::vector<const Texture*> textures;
    return textures;
}

std::string LowerExtension(const std::string& path)
{
    const std::size_t dot = path.find_last_of('.');
    if (dot == std::string::npos)
    {
        return {};
    }
    std::string extension = path.substr(dot);
    std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return extension;
}

ILenum DevilTypeFromExtension(const std::string& extension)
{
    if (extension == ".png")
    {
        return IL_PNG;
    }
    if (extension == ".dds")
    {
        return IL_DDS;
    }
    if (extension == ".jpg" || extension == ".jpeg")
    {
        return IL_JPG;
    }
    if (extension == ".tga")
    {
        return IL_TGA;
    }
    if (extension == ".bmp")
    {
        return IL_BMP;
    }
    return IL_TYPE_UNKNOWN;
}

GLint ToGl(TextureWrap wrap)
{
    switch (wrap)
    {
    case TextureWrap::MirroredRepeat:
        return GL_MIRRORED_REPEAT;
    case TextureWrap::ClampToEdge:
        return GL_CLAMP_TO_EDGE;
    case TextureWrap::ClampToBorder:
        return GL_CLAMP_TO_BORDER;
    case TextureWrap::Repeat:
    default:
        return GL_REPEAT;
    }
}

GLint ToGl(TextureMinFilter filter)
{
    switch (filter)
    {
    case TextureMinFilter::Nearest:
        return GL_NEAREST;
    case TextureMinFilter::Linear:
        return GL_LINEAR;
    case TextureMinFilter::NearestMipmapNearest:
        return GL_NEAREST_MIPMAP_NEAREST;
    case TextureMinFilter::LinearMipmapNearest:
        return GL_LINEAR_MIPMAP_NEAREST;
    case TextureMinFilter::NearestMipmapLinear:
        return GL_NEAREST_MIPMAP_LINEAR;
    case TextureMinFilter::LinearMipmapLinear:
    default:
        return GL_LINEAR_MIPMAP_LINEAR;
    }
}

GLint ToGl(TextureMagFilter filter)
{
    return filter == TextureMagFilter::Nearest ? GL_NEAREST : GL_LINEAR;
}

std::uint32_t UploadRgba(const unsigned char* pixels, int width, int height, bool pixelated)
{
    GLuint id = 0;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixelated ? GL_NEAREST : GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    if (!pixelated)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return id;
}
} // namespace

void Texture::InitLoader()
{
    ilInit();
    // Origen a baix a l'esquerra, com OpenGL i les UV que dona Assimp: així no cal girar res.
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ENGINE_LOGF("DevIL initialised (version {})", ilGetInteger(IL_VERSION_NUM));
}

void Texture::ShutdownLoader()
{
    ilShutDown();
}

TextureSettings& Texture::GetSettings()
{
    static TextureSettings settings;
    return settings;
}

void Texture::ApplySettings()
{
    for (const Texture* texture : Registry())
    {
        texture->ApplyParameters();
    }
}

float Texture::MaxAnisotropy()
{
    static const float value = []
    {
        while (glGetError() != GL_NO_ERROR)
        {
        }
        GLfloat maxValue = 1.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxValue);
        return (glGetError() == GL_NO_ERROR && maxValue >= 1.0f) ? maxValue : 1.0f;
    }();
    return value;
}

const std::vector<const Texture*>& Texture::GetAll()
{
    return Registry();
}

void Texture::Register()
{
    Registry().push_back(this);
    ResourceStats::OnTextureCreated(m_gpuBytes);
}

void Texture::ApplyParameters() const
{
    if (m_pixelated)
    {
        return; // la textura de quadrats sempre es veu a píxels
    }

    const TextureSettings& settings = GetSettings();
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToGl(settings.wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToGl(settings.wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToGl(settings.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToGl(settings.magFilter));

    if (MaxAnisotropy() > 1.0f)
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, std::clamp(settings.anisotropy, 1.0f, MaxAnisotropy()));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

std::shared_ptr<Texture> Texture::LoadFromFile(const std::string& utf8Path)
{
    const auto start = std::chrono::steady_clock::now();

    // Llegim el fitxer nosaltres i li donem els bytes a DevIL: així les rutes UTF-8 funcionen.
    std::ifstream file(FromUtf8(utf8Path), std::ios::binary);
    if (!file)
    {
        ENGINE_LOGF("Could not open texture '{}'", utf8Path);
        return nullptr;
    }
    const std::vector<char> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (bytes.empty())
    {
        ENGINE_LOGF("Texture '{}' is empty", utf8Path);
        return nullptr;
    }

    ILuint image = 0;
    ilGenImages(1, &image);
    ilBindImage(image);

    const bool loaded = ilLoadL(DevilTypeFromExtension(LowerExtension(utf8Path)), bytes.data(), static_cast<ILuint>(bytes.size())) == IL_TRUE &&
                        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE) == IL_TRUE;
    if (!loaded)
    {
        ENGINE_LOGF("DevIL could not load '{}' (error 0x{:X})", utf8Path, static_cast<unsigned int>(ilGetError()));
        ilDeleteImages(1, &image);
        return nullptr;
    }

    const int width = ilGetInteger(IL_IMAGE_WIDTH);
    const int height = ilGetInteger(IL_IMAGE_HEIGHT);

    std::shared_ptr<Texture> texture(new Texture());
    texture->m_width = width;
    texture->m_height = height;
    texture->m_path = utf8Path;
    texture->m_gpuBytes = static_cast<std::uint64_t>(width) * static_cast<std::uint64_t>(height) * 4ull * 4ull / 3ull; // RGBA8 + mipmaps
    texture->m_id = UploadRgba(ilGetData(), width, height, false);
    ilDeleteImages(1, &image);

    texture->Register();
    texture->ApplyParameters(); // fa servir les opcions globals actuals

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
    ENGINE_LOGF("Texture '{}' loaded ({}x{}) in {} ms", utf8Path, width, height, elapsed.count());
    return texture;
}

std::shared_ptr<Texture> Texture::CreateCheckers()
{
    constexpr int kSize = 64;
    std::vector<unsigned char> pixels(static_cast<std::size_t>(kSize) * kSize * 4);
    for (int i = 0; i < kSize; ++i)
    {
        for (int j = 0; j < kSize; ++j)
        {
            const unsigned char c = static_cast<unsigned char>((((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255);
            unsigned char* pixel = &pixels[(static_cast<std::size_t>(i) * kSize + j) * 4];
            pixel[0] = c;
            pixel[1] = c;
            pixel[2] = c;
            pixel[3] = 255;
        }
    }

    std::shared_ptr<Texture> texture(new Texture());
    texture->m_width = kSize;
    texture->m_height = kSize;
    texture->m_path = "(procedural checkers)";
    texture->m_pixelated = true;
    texture->m_gpuBytes = static_cast<std::uint64_t>(kSize) * kSize * 4ull;
    texture->m_id = UploadRgba(pixels.data(), kSize, kSize, true);
    texture->Register();
    return texture;
}

Texture::~Texture()
{
    std::erase(Registry(), this);
    ResourceStats::OnTextureDestroyed(m_gpuBytes);
    glDeleteTextures(1, &m_id);
}
} // namespace Engine
