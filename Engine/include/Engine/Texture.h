#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace Engine
{
// Textura 2D a la VRAM. DevIL només es fa servir dins de Texture.cpp.
// Ha de destruir-se mentre el context d'OpenGL encara existeix.
class Texture
{
public:
    // Inicialitza/tanca DevIL (cal cridar-ho un cop, després de crear el context).
    static void InitLoader();
    static void ShutdownLoader();

    // Carrega PNG/DDS/... des d'una ruta UTF-8. Retorna nullptr si falla.
    static std::shared_ptr<Texture> LoadFromFile(const std::string& utf8Path);
    // Textura procedural de quadrats blancs i negres.
    static std::shared_ptr<Texture> CreateCheckers();

    ~Texture();
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    std::uint32_t GetId() const { return m_id; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    const std::string& GetPath() const { return m_path; }

private:
    Texture() = default;

    std::uint32_t m_id = 0;
    int m_width = 0;
    int m_height = 0;
    std::string m_path;
};
} // namespace Engine
