#include "EditorLayer.h"

#include <iostream>

namespace Holloware
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
    {
    }

    void EditorLayer::OnAttach()
    {
        HW_PROFILE_FUNCTION();

        m_EditorCamera = EditorCamera();

        m_FaceTexture = Texture2D::Create("assets/textures/face.png");
        m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
        m_SpriteSheet = Texture2D::Create("assets/game/textures/tilemap_packed.png");

        m_Grass = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 6 }, { 16, 16 });

        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);

        m_ActiveScene = CreateRef<Scene>();

        // Entity
        Entity squareEntity1 = m_ActiveScene->CreateEntity("Red Square");
        squareEntity1.GetComponent<TransformComponent>().Translation = { 3.0f, 3.0f, 0.0f };
        squareEntity1.AddComponent<SpriteRendererComponent>(glm::vec4(0.8f, 0.3f, 0.2f, 1.0f));

        Entity squareEntity2 = m_ActiveScene->CreateEntity("Blue Square");
        squareEntity2.GetComponent<TransformComponent>().Translation = { -3.0f, 3.0f, 0.0f };
        squareEntity2.AddComponent<SpriteRendererComponent>(glm::vec4(0.3f, 0.4f, 0.8f, 1.0f));

        Entity squareEntity3 = m_ActiveScene->CreateEntity("Green Square");
        squareEntity3.GetComponent<TransformComponent>().Translation = { -3.0f, -3.0f, 0.0f };
        squareEntity3.AddComponent<SpriteRendererComponent>(glm::vec4(0.1f, 0.9f, 0.4f, 1.0f));

        Entity squareEntity4 = m_ActiveScene->CreateEntity("White Square");
        squareEntity4.GetComponent<TransformComponent>().Translation ={ 3.0f, -3.0f, 0.0f };
        squareEntity4.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        m_CameraEntity = m_ActiveScene->CreateEntity("Main Camera");
        auto& cc = m_CameraEntity.AddComponent<CameraComponent>();
        cc.Primary = true;

        class CameraController : public ScriptableEntity
        {
        public:
            void OnUpdate(Timestep ts)
            {
                auto& transform = GetComponent<TransformComponent>();
                float speed = 5.0f;

                if (Input::IsKeyPressed(HW_KEY_A))
                    transform.Translation.x -= speed * ts;
                if (Input::IsKeyPressed(HW_KEY_D))
                    transform.Translation.x += speed * ts;
                if (Input::IsKeyPressed(HW_KEY_W))
                    transform.Translation.y += speed * ts;
                if (Input::IsKeyPressed(HW_KEY_S))
                    transform.Translation.y -= speed * ts;
            }
        };
        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnDetach()
    {
        HW_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        HW_PROFILE_FUNCTION();

        m_frameMS = ts.GetMilliseconds();

        // Updating 
        if (m_ViewportFocused) { m_EditorCamera.OnUpdate(ts); }
        
        // Resizing
        if (m_ViewportSize != *((glm::vec2*)&m_ViewportPanelSize))
        {
            m_ViewportSize = { m_ViewportPanelSize.x, m_ViewportPanelSize.y };

            m_Framebuffer->Resize((uint32_t)m_ViewportPanelSize.x, (uint32_t)m_ViewportPanelSize.y);
            m_EditorCamera.OnResize(m_ViewportPanelSize.x, m_ViewportPanelSize.y);

            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        // Render
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        RenderCommand::Clear();

        // Clear our entity ID attachment to -1
        m_Framebuffer->ClearAttachment(1, -1);

        // Update Scene
        m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
        //m_ActiveScene->OnUpdateRuntime(ts);

        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        my = viewportSize.y - my;
        int mouseX = (int)mx;
        int mouseY = (int)my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
        {
            int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
            m_HoveredEntity = (pixelData == -1) ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
        }

        m_Framebuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender()
    {
        HW_PROFILE_FUNCTION();

        static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        // REMINDER: THIS IS A DEMO FOR ADVANCED USAGE OF DockSpace()!
        // MOST REGULAR APPLICATIONS WILL SIMPLY WANT TO CALL DockSpaceOverViewport(). READ COMMENTS ABOVE.
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        // Show demo options and help
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        /////////////////////////////////////////////////////////////////////////////////

        m_SceneHierarchyPanel.OnImGuiRender();
        m_ContentBrowserPanel.OnImGuiRender();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("ViewPort");
        auto viewportOffset = ImGui::GetCursorPos();

        m_ViewportPanelSize = ImGui::GetContentRegionAvail();

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, { m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
            {
                const wchar_t* path = (const wchar_t*) payload->Data;
                HW_CORE_INFO(std::filesystem::path(path).string().c_str());
            }
            ImGui::EndDragDropTarget();
        }

        auto windowSize = ImGui::GetWindowSize();
        ImVec2 minBound = ImGui::GetWindowPos();
        minBound.x += viewportOffset.x;
        minBound.y += viewportOffset.y;

        ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y - viewportOffset.y };
        m_ViewportBounds[0] = { minBound.x, minBound.y };
        m_ViewportBounds[1] = { maxBound.x, maxBound.y };

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::Begin("Stats");
        ImGui::Text("FPS: %.3f", 1000.0f / m_frameMS);

        std::string name = m_HoveredEntity ? m_HoveredEntity.GetComponent<TagComponent>().Tag : "None";

        ImGui::Text("Hovered Entity: %s", name.c_str());
        ImGui::End();

        ImGui::End(); // Dockspace End
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_EditorCamera.OnEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseButtonPressedEvent>(HW_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if (m_ViewportHovered && e.GetMouseButton() == 0)
        {
            m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
        }

        return false;
    }
}
