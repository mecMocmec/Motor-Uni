#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace Engine
{
enum class TextureWrap
{
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder
};

enum class TextureMinFilter
{
    Nearest,
    Linear,
    NearestMipmapNearest,
    LinearMipmapNearest,
    NearestMipmapLinear,
    LinearMipmapLinear
};

enum class TextureMagFilter
{
    Nearest,
    Linear
};

struct TextureSettings
{
    TextureWrap wrap = TextureWrap::Repeat;
    TextureMinFilter minFilter = TextureMinFilter::LinearMipmapLinear;
    TextureMagFilter magFilter = TextureMagFilter::Linear;
    float anisotropy = 1.0f;
};

class Texture
{
public:
    static void InitLoader();
    static void ShutdownLoader();

    static std::shared_ptr<Texture> LoadFromFile(const std::string& utf8Path);
    static std::shared_ptr<Texture> CreateCheckers();

    static TextureSettings& GetSettings();
    static void ApplySettings(); 
    static float MaxAnisotropy();
    static const std::vector<const Texture*>& GetAll();

    ~Texture();
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    std::uint32_t GetId() const { return m_id; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    const std::string& GetPath() const { return m_path; }
    std::uint64_t GetGpuBytes() const { return m_gpuBytes; }

private:
    Texture() = default;
    void Register();
    void ApplyParameters() const;

    std::uint32_t m_id = 0;
    int m_width = 0;
    int m_height = 0;
    std::string m_path;
    bool m_pixelated = false;
    std::uint64_t m_gpuBytes = 0;
};
} 
