#include "Engine/Renderer.h"

#include "Engine/Camera.h"
#include "Engine/ComponentMesh.h"
#include "Engine/ComponentTexture.h"
#include "Engine/ComponentTransform.h"
#include "Engine/GameObject.h"
#include "Engine/Log.h"
#include "Engine/Scene.h"
#include "GLInclude.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <cmath>

namespace Engine
{
namespace
{
constexpr const char* kVertexShader = R"(#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 vNormal;
out vec2 vTexCoord;

uniform mat4 model_matrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model_matrix * vec4(position, 1.0);
    vNormal = mat3(transpose(inverse(model_matrix))) * normal;
    vTexCoord = texCoord;
}
)";

// De moment es pinta la normal com a color; amb les textures s'hi afegirà el mostreig.
constexpr const char* kFragmentShader = R"(#version 460 core
in vec3 vNormal;
in vec2 vTexCoord;
out vec4 color;

uniform sampler2D ourTexture;
uniform int use_texture;

void main()
{
    if (use_texture == 1)
    {
        color = texture(ourTexture, vTexCoord);
    }
    else
    {
        // Sense textura: es pinta la normal com a color.
        color = vec4(normalize(vNormal) * 0.5 + 0.5, 1.0);
    }
}
)";

GLuint CompileShader(GLenum type, const char* source)
{
    const GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == 0)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        ENGINE_LOGF("Shader compilation error: {}", infoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
} // namespace

bool Renderer::Init()
{
    const GLuint vs = CompileShader(GL_VERTEX_SHADER, kVertexShader);
    const GLuint fs = CompileShader(GL_FRAGMENT_SHADER, kFragmentShader);
    if (vs == 0 || fs == 0)
    {
        return false;
    }

    m_program = glCreateProgram();
    glAttachShader(m_program, vs);
    glAttachShader(m_program, fs);
    glLinkProgram(m_program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint success = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        GLchar infoLog[1024];
        glGetProgramInfoLog(m_program, sizeof(infoLog), nullptr, infoLog);
        ENGINE_LOGF("Shader link error: {}", infoLog);
        return false;
    }

    m_locModel = glGetUniformLocation(m_program, "model_matrix");
    m_locView = glGetUniformLocation(m_program, "view");
    m_locProjection = glGetUniformLocation(m_program, "projection");
    m_locSampler = glGetUniformLocation(m_program, "ourTexture");
    m_locUseTexture = glGetUniformLocation(m_program, "use_texture");

    m_checkers = Texture::CreateCheckers();

    ResizeTarget(800, 600);
    ENGINE_LOG("Renderer initialised");
    return m_fbo != 0;
}

void Renderer::Shutdown()
{
    m_checkers.reset();
    DestroyTarget();
    glDeleteProgram(m_program);
    m_program = 0;
}

void Renderer::DestroyTarget()
{
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteTextures(1, &m_colorTex);
    glDeleteRenderbuffers(1, &m_depthRbo);
    m_fbo = m_colorTex = m_depthRbo = 0;
}

void Renderer::ResizeTarget(int width, int height)
{
    if (width <= 0 || height <= 0 || (width == m_width && height == m_height))
    {
        return;
    }

    DestroyTarget();
    m_width = width;
    m_height = height;

    glGenTextures(1, &m_colorTex);
    glBindTexture(GL_TEXTURE_2D, m_colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &m_depthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTex, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthRbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        ENGINE_LOG("Framebuffer is not complete");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::RenderScene(const Scene& scene, const Camera& camera)
{
    if (m_fbo == 0)
    {
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_width, m_height);
    glClearColor(m_settings.clearColor.r, m_settings.clearColor.g, m_settings.clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Opcions de la finestra Configuracio
    const auto setCapability = [](GLenum capability, bool enabled)
    {
        if (enabled)
        {
            glEnable(capability);
        }
        else
        {
            glDisable(capability);
        }
    };
    setCapability(GL_DEPTH_TEST, m_settings.depthTest);
    setCapability(GL_CULL_FACE, m_settings.cullFace);
    setCapability(GL_BLEND, m_settings.blend);
    glCullFace(m_settings.cullBackFaces ? GL_BACK : GL_FRONT);
    glFrontFace(m_settings.frontFaceCounterClockwise ? GL_CCW : GL_CW);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, m_settings.wireframe ? GL_LINE : GL_FILL);

    const float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
    const glm::mat4 view = camera.ViewMatrix();
    const glm::mat4 projection = camera.ProjectionMatrix(aspect);

    glUseProgram(m_program);
    glUniformMatrix4fv(m_locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(m_locProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1i(m_locSampler, 0); // unitat de textura 0

    DrawGameObject(scene.GetRoot());

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // el wireframe només afecta l'escena, no l'editor
    glDisable(GL_BLEND);

    glUseProgram(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawGameObject(const GameObject& gameObject) const
{
    // Un GameObject desactivat amaga també tot el seu subarbre.
    if (!gameObject.IsActive())
    {
        return;
    }

    const ComponentMesh* meshComponent = gameObject.GetComponent<ComponentMesh>();
    if (meshComponent != nullptr && meshComponent->IsActive() && meshComponent->GetMesh() != nullptr)
    {
        const glm::mat4& model = gameObject.GetTransform().GetGlobalMatrix();
        glUniformMatrix4fv(m_locModel, 1, GL_FALSE, glm::value_ptr(model));

        const Texture* texture = nullptr;
        const ComponentTexture* textureComponent = gameObject.GetComponent<ComponentTexture>();
        if (textureComponent != nullptr && textureComponent->IsActive())
        {
            texture = textureComponent->UseCheckers() ? m_checkers.get() : textureComponent->GetTexture().get();
        }

        glUniform1i(m_locUseTexture, texture != nullptr ? 1 : 0);
        if (texture != nullptr)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture->GetId());
        }

        meshComponent->GetMesh()->Draw();

        if (texture != nullptr)
        {
            glBindTexture(GL_TEXTURE_2D, 0); // sempre deslliguem la textura després de fer-la servir
        }
    }

    for (const auto& child : gameObject.GetChildren())
    {
        DrawGameObject(*child);
    }
}
} // namespace Engine
