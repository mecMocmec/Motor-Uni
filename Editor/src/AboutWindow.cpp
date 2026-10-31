#include "AboutWindow.h"

#include "ProjectInfo.h"

#include <Engine/SystemInfo.h>

#include <imgui.h>

#include <string>

namespace Editor
{
namespace
{
constexpr const char* kMitLicenseBody =
    "Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated "
    "documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the "
    "rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit "
    "persons to whom the Software is furnished to do so, subject to the following conditions:\n\n"
    "The above copyright notice and this permission notice shall be included in all copies or substantial portions of the "
    "Software.\n\n"
    "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE "
    "WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR "
    "COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR "
    "OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.";

std::string CopyrightLine()
{
    std::string line = "Copyright (c) 2026 ";
    bool first = true;
    for (const ProjectInfo::Member& member : ProjectInfo::kMembers)
    {
        line += first ? "" : ", ";
        line += member.name;
        first = false;
    }
    return line;
}
} // namespace

void AboutWindow::Draw(const Engine::SystemInfo& info, bool& open) const
{
    ImGui::SetNextWindowSize(ImVec2(460.0f, 520.0f), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("About", &open))
    {
        ImGui::Text("%s", ProjectInfo::kEngineName);
        ImGui::Text("Version %s", ProjectInfo::kVersion);
        ImGui::TextWrapped("%s", ProjectInfo::kShortDescription);
        if (ImGui::Button("Open GitHub repository"))
        {
            ProjectInfo::OpenRepositoryPage("");
        }

        ImGui::Separator();
        ImGui::Text("Team");
        for (const ProjectInfo::Member& member : ProjectInfo::kMembers)
        {
            ImGui::BulletText("%s  (github.com/%s)", member.name, member.github);
        }

        ImGui::Separator();
        ImGui::Text("Libraries used");
        ImGui::BulletText("SDL3 %s - window, input and OpenGL context", info.sdlVersion.c_str());
        ImGui::BulletText("OpenGL %s", info.openGlVersion.c_str());
        ImGui::BulletText("glad - OpenGL function loader");
        ImGui::BulletText("Dear ImGui %s - editor interface", ImGui::GetVersion());
        ImGui::BulletText("GLM %s - math", info.glmVersion.c_str());
        ImGui::BulletText("Assimp %s - FBX model import", info.assimpVersion.c_str());
        ImGui::BulletText("DevIL %s - texture loading (PNG, DDS)", info.devilVersion.c_str());

        ImGui::Separator();
        ImGui::Text("License: MIT");
        if (ImGui::CollapsingHeader("MIT License text"))
        {
            ImGui::TextWrapped("%s", CopyrightLine().c_str());
            ImGui::TextWrapped("%s", kMitLicenseBody);
        }
    }
    ImGui::End();
}
} // namespace Editor
