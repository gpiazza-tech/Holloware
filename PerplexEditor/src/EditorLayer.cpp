#include <pch.h>
#include "EditorLayer.h"

#include "Meta/UserData.h"

#include <Perplex/Scene/SceneManager.h>
#include <Perplex/Platform/SystemUtils.h>
#include <Perplex/Serialization/JsonHelper.h>
#include <Perplex/Core/KeyCodes.h>
#include <Perplex/Core/Window.h>
#include <Perplex/Core/FileIO.h>
#include <Perplex/Scene/SceneManager.h>

#include <glm/fwd.hpp>
#include <imgui/imgui.h>

#include <filesystem>
#include <string>
#include <functional>

namespace Perplex
{
    namespace fs = std::filesystem;
    static const fs::path s_UserDataPath{ PerplexAppUserPath() / "PerplexEditor/UserData.json" };

    static void Screenshot(const pxr::ImageBuffer& imageBuffer)
    {
        HW_PROFILE_FUNCTION();

        std::filesystem::path targetDir{ Application::Get().GetGame().RootDirectory / "screenshots" };
        if (!FileIO::TryCreateDirectories(targetDir))
        {
            HW_CORE_WARN("Failed to take screenshot!");
            return;
        }

        std::string timeStr = std::to_string((double)std::chrono::system_clock::now().time_since_epoch().count());
        pxr::SavePNG(targetDir / (timeStr + ".png"), imageBuffer);

        HW_CORE_INFO("Screenshot taken!");
    }

    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
    {
    }

    void EditorLayer::OnAttach()
    {
        HW_PROFILE_FUNCTION();

        JsonHelper::ObjectFromFile(m_UserData, s_UserDataPath);

        m_Dockspace = Dockspace{};
        m_EditorCamera = EditorCamera{};
        m_ContentBrowserPanel = ContentBrowserPanel{};
        m_ContentBrowserPanel.SetOpenFileCallback(HW_BIND_EVENT_FN(EditorLayer::OnFileOpen));

        m_PlayIcon = CreateRef<pxr::TextureBuffer>(Application::Get().EngineRes("textures/play_icon.png"));
        m_StopIcon = CreateRef<pxr::TextureBuffer>(Application::Get().EngineRes("textures/pause_icon.png"));

        m_ProjectPanel.Focus();

        ImGuiUtilities::SetGlobalStyles();
    }

    void EditorLayer::OnDetach()
    {
        HW_PROFILE_FUNCTION();

        JsonHelper::ObjectToFile(m_UserData, s_UserDataPath);
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        HW_PROFILE_FUNCTION();

        m_FrameMS = ts.GetMilliseconds();

        m_ViewportPanel.OnUpdate();

        // Resizing
        if (m_ViewportPanel.IsResizing())
            OnResize();

        // Update
        if (m_SceneState == SceneState::Edit)
        {
            if (m_ViewportPanel.IsFocused()) { m_EditorCamera.OnUpdate(ts); }
            // m_SceneRenderer.RenderEditor(m_ActiveScene, m_EditorCamera);
        }
        else if (m_SceneState == SceneState::Play)
        {
            SceneManager::Get().ActiveScene()->Update(ts);
            // m_SceneRenderer.Render(m_ActiveScene);
        }

        glm::vec2 mousePos = m_SceneRenderer.ScreenToWorldPosition(m_ViewportPanel.GetMousePosition());
        Input::SetMouseWorldPosition({ mousePos.x, mousePos.y });

        SceneManager::Get().OnUpdateEnd();
    }

    void EditorLayer::OnImGuiRender()
    {
        HW_PROFILE_FUNCTION();

        Ref<Scene> activeScene = SceneManager::Get().ActiveScene();

        // Scene
        switch (m_SceneState)
        {
        case SceneState::Edit:
            m_SceneRenderer.RenderEditor(activeScene, m_EditorCamera);
            break;
        case SceneState::Play:
            m_SceneRenderer.Render(activeScene);
            break;
        default:
            break;
        }

        // ImGui
        m_Dockspace.Begin();

        UI_MenuBar();

        m_SceneHierarchyPanel.OnImGuiRender();
        m_ContentBrowserPanel.OnImGuiRender();
        m_PerpixelPanel.OnImGuiRender(activeScene, m_SceneHierarchyPanel.GetSelectedEntity());
        m_ViewportPanel.OnImGuiRender(m_SceneRenderer.GetMainFramebufferTexture());

        // Debug Panels
        m_SpriteRegistryPanel.Render();

        UI_Stats();
        UI_Toolbar();

        m_Dockspace.End();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_EditorCamera.OnEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseButtonPressedEvent>(HW_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
        dispatcher.Dispatch<KeyPressedEvent>(HW_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        /* Entity Selection
        if (m_ViewportHovered && e.GetMouseButton() == 0)
            m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
        */

        return false;
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        if (e.GetKeyCode() == HW_KEY_F8)
            Screenshot(m_SceneRenderer.FetchFramebufferPixels());
        if (e.GetKeyCode() == HW_KEY_F7)
            Screenshot(Application::Get().GetWindow().FetchWindowPixels());

        return false;
    }

    void EditorLayer::UI_MenuBar()
    {
        // MENU BAR
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                {
                    Application::Get().Close();
                }

                if (ImGui::MenuItem("Save"))
                {
                    static fs::path filepath = Application::Get().GetGame().AssetsDirectory / "scenes/scene.pxs";
                    m_SavePopup.Open(filepath.string(), [](const std::string& newStr)
                        {
                            SceneManager::Get().SaveScene(fs::path{ newStr });
                            ImGui::CloseCurrentPopup();
                        });
                }

                if (ImGui::MenuItem("Load"))
                {
                    static fs::path filepath = Application::Get().GetGame().AssetsDirectory / "scenes/scene.pxs";
                    m_LoadPopup.Open(filepath.string(), [](const std::string& newStr) 
                        {
                            Asset sceneAsset = Asset(fs::path{ newStr });
                            SceneManager::Get().LoadScene(sceneAsset);
                        });
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Debug"))
            {
                if (ImGui::MenuItem("Open Application User Folder"))
                    OpenFilesystemGui(PerplexAppUserPath());

                if (ImGui::MenuItem("Open Application Data Folder"))
                    OpenFilesystemGui(PerplexAppDataPath());

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        m_SavePopup.Update();
        m_LoadPopup.Update();
    }

    void EditorLayer::UI_Stats()
    {
        ImGui::Begin("Stats");
        ImGui::Text("FPS: %.3f", 1000.0f / m_FrameMS);
        ImGui::Dummy({ 0.0f, 10.0f });

        const pxr::RenderStats& renderStats = pxr::Renderer::GetStats();
        ImGui::Text("Quads: %i", renderStats.Quads);
        ImGui::Text("Draw Calls: %i", renderStats.DrawCalls);

        ImGui::End();
    }

    void EditorLayer::UI_Toolbar()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 0.3f));

        ImGui::Begin("Tool Bar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        float size = ImGui::GetWindowHeight() - 5.0f;

        Ref<pxr::TextureBuffer> icon = (m_SceneState == SceneState::Play) ? m_StopIcon : m_PlayIcon;
        ImTextureRef iconTextureRef = ImTextureRef(icon->GetID());

        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
        ImGui::SetCursorPosY((ImGui::GetWindowContentRegionMax().y * 0.5f) - (size * 0.5f));
        if (ImGui::ImageButton("12309487", iconTextureRef, { size, size }))
        {
            if (m_SceneState == SceneState::Edit)
                OnScenePlay();
            else if (m_SceneState == SceneState::Play)
                OnSceneStop();
        }

        ImGui::End();

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
    }

    void EditorLayer::OnScenePlay()
    {
        ImGuiUtilities::SetRuntimeStyles();

        m_SceneState = SceneState::Play;
        SceneManager::Get().Play();
    }

    void EditorLayer::OnSceneStop()
    {
        ImGuiUtilities::SetEditorStyles();

        m_SceneState = SceneState::Edit;
        SceneManager::Get().Stop();
        SceneManager::Get().Reset();
    }

    void EditorLayer::OnResize()
    {
        const glm::vec2& viewportSize = m_ViewportPanel.GetSize();
        m_EditorCamera.OnResize(viewportSize.x, viewportSize.y);
        m_SceneRenderer.Resize((int)viewportSize.x, (int)viewportSize.y);
    }

    void EditorLayer::OnAssetImported(Asset asset)
    {
        //if (asset.GetPath().extension() == ".c")
        //    m_Interpreter.OnScriptAssetReimported(m_ActiveScene, asset);
    }

    void EditorLayer::OnFileOpen(const std::filesystem::path& path)
    {
        fs::path startCommandPath = NativePath(path);
        std::string command = "\"" + startCommandPath.string() + "\"";
        std::system(command.c_str());
    }
}
