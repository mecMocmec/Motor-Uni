#include "Engine/Renderer.h"

#include "Engine/ComponentMesh.h"
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
constexpr float kFovDegrees = 60.0f;

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

void main()
{
    color = vec4(normalize(vNormal) * 0.5 + 0.5, 1.0);
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

    ResizeTarget(800, 600);
    ENGINE_LOG("Renderer initialised");
    return m_fbo != 0;
}

void Renderer::Shutdown()
{
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

void Renderer::RenderScene(const Scene& scene)
{
    if (m_fbo == 0)
    {
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Càmera fixa de moment; la càmera estil Unity arriba en un pas posterior.
    const float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
    const glm::mat4 view = glm::lookAt(m_cameraPosition, m_cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 projection = glm::perspective(glm::radians(kFovDegrees), aspect, m_near, m_far);

    glUseProgram(m_program);
    glUniformMatrix4fv(m_locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(m_locProjection, 1, GL_FALSE, glm::value_ptr(projection));

    DrawGameObject(scene.GetRoot());

    glUseProgram(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::FocusOn(const glm::vec3& center, float radius)
{
    radius = std::max(radius, 0.001f);
    const float distance = radius / std::sin(glm::radians(kFovDegrees) * 0.5f) * 1.2f;
    const glm::vec3 direction = glm::normalize(glm::vec3(0.6f, 0.45f, 0.8f));

    m_cameraTarget = center;
    m_cameraPosition = center + direction * distance;
    m_near = std::max(distance * 0.01f, 0.001f);
    m_far = distance * 100.0f + radius * 10.0f;
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
        meshComponent->GetMesh()->Draw();
    }

    for (const auto& child : gameObject.GetChildren())
    {
        DrawGameObject(*child);
    }
}
} // namespace Engine
