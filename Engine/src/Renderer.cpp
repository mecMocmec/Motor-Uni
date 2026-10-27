#include "Engine/Renderer.h"

#include "Engine/Log.h"
#include "GLInclude.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
namespace
{
constexpr const char* kVertexShader = R"(#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 vColor;

uniform mat4 model_matrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model_matrix * vec4(position, 1.0);
    vColor = color;
}
)";

constexpr const char* kFragmentShader = R"(#version 460 core
in vec3 vColor;
out vec4 color;

void main()
{
    color = vec4(vColor, 1.0);
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

// 8 vèrtexs únics: posició (xyz) + color (rgb)
constexpr float kCubeVertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // 0
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 1
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // 2
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 3
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // 4
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, // 5
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, // 6
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // 7
};

// Triangles en sentit antihorari vistos des de fora
constexpr unsigned int kCubeIndices[] = {
    4, 5, 6, 4, 6, 7, // +z
    1, 0, 3, 1, 3, 2, // -z
    5, 1, 2, 5, 2, 6, // +x
    0, 4, 7, 0, 7, 3, // -x
    7, 6, 2, 7, 2, 3, // +y
    0, 1, 5, 0, 5, 4, // -y
};
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

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kCubeVertices), kCubeVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kCubeIndices), kCubeIndices, GL_STATIC_DRAW);
    m_indexCount = static_cast<int>(sizeof(kCubeIndices) / sizeof(kCubeIndices[0]));

    constexpr GLsizei stride = 6 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    ResizeTarget(800, 600);
    ENGINE_LOG("Renderer initialised");
    return m_fbo != 0;
}

void Renderer::Shutdown()
{
    DestroyTarget();
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteProgram(m_program);
    m_vbo = m_ebo = m_vao = m_program = 0;
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

void Renderer::RenderScene(float timeSeconds)
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

    const float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
    const glm::mat4 model = glm::rotate(glm::mat4(1.0f), timeSeconds, glm::normalize(glm::vec3(0.5f, 1.0f, 0.0f)));
    const glm::mat4 view = glm::lookAt(glm::vec3(2.5f, 2.0f, 3.5f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);

    glUseProgram(m_program);
    glUniformMatrix4fv(m_locModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(m_locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(m_locProjection, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glUseProgram(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
} // namespace Engine
