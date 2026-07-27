#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>
#include <cassert>
#include <iostream>

#define QUIT "/q"
#define FORWARD "/w"
#define STOP "/b"
#define BACK "/s"
#define LEFT "/a"
#define RIGHT "/d"

#define Send(msg) send(client, msg, strlen(msg) * sizeof(char), 0)

int main() {
    std::string ip = "";
    int port = 0;

    std::cout << "Enter the ip address (don't include port) :- ";
    std::cin >> ip;
    std::cout << "Enter port :- ";
    std::cin >> port;

    std::vector<std::string> logs;
    WSADATA data{};
    SOCKET client{};
    
    // WinSock setup
    {
        WSAStartup(MAKEWORD(2, 2), &data);

        client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        sockaddr_in addr{};
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        addr.sin_port = htons(port);
        addr.sin_family = AF_INET;

        if(connect(client, (sockaddr*)&addr, sizeof(addr)) != 0) {
            std::cerr << "Failed to connect to server!" << std::endl;
            return -1;
        }
    }

    // Window
    assert(glfwInit() && "Failed to init glfw!");

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(vidMode->width, vidMode->height, "Client for Robotic Car", monitor, nullptr);
    assert(window && "Failed to create window!");
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        assert(false && "Failed to load OpenGL!");
    }

    // ImGui 
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.Fonts->AddFontFromFileTTF("consolas.ttf", 20.0f);
        io.Fonts->Build();
    
        ImGui::StyleColorsDark();


        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 12.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        style.WindowPadding = ImVec2(0.0f, 0.0f);
        style.FrameBorderSize = 3;
        style.FramePadding = ImVec2(5, 5);
        style.FrameRounding = 6;
        style.TabRounding = 6;
        style.GrabRounding = 6;
        style.PopupRounding = 6;
        style.ChildRounding = 6;
        style.WindowRounding = 6;
        style.ScrollbarRounding = 6;
        ImVec4* colors = style.Colors;
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.13f, 0.13f, 0.13f, 0.00f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.13f, 0.13f, 0.13f, 0.94f);
        colors[ImGuiCol_Border]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.14f, 0.14f, 0.14f, 0.74f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.33f, 0.33f, 0.33f, 0.54f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.31f, 0.31f, 0.31f, 0.40f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.23f, 0.23f, 0.23f, 0.75f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.12f, 0.12f, 0.12f, 0.51f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.13f, 0.13f, 0.13f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.40f, 0.40f, 0.41f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.39f, 0.39f, 0.40f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
        colors[ImGuiCol_Button]                 = ImVec4(0.25f, 0.24f, 0.24f, 0.40f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.29f, 0.29f, 0.29f, 0.31f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.29f, 0.29f, 0.29f, 0.31f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.39f, 0.39f, 0.39f, 0.78f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.16f, 0.16f, 0.16f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.20f, 0.20f, 0.20f, 0.67f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.27f, 0.28f, 0.28f, 0.95f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.27f, 0.27f, 0.27f, 0.80f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.28f, 0.28f, 0.28f, 0.86f);
        colors[ImGuiCol_TabSelected]            = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
        colors[ImGuiCol_TabSelectedOverline]    = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_TabDimmed]              = ImVec4(0.18f, 0.19f, 0.21f, 0.97f);
        colors[ImGuiCol_TabDimmedSelected]      = ImVec4(0.17f, 0.19f, 0.22f, 1.00f);
        colors[ImGuiCol_TabDimmedSelectedOverline]  = ImVec4(0.19f, 0.17f, 0.17f, 1.00f);
        colors[ImGuiCol_DockingPreview]         = ImVec4(0.20f, 0.29f, 0.41f, 0.70f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    }
   
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glfwShowWindow(window);
    while(!glfwWindowShouldClose(window)) {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            break;

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2((float)vidMode->width, (float)vidMode->height));

        ImGui::Begin("Main Panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

        ImGui::Text("Robotic Car Control Panel");
        ImGui::Separator();

        // ======================================================
        // Connection
        // ======================================================

        ImGui::Text("Connection");
        ImGui::BulletText("Status : Connected");
        ImGui::BulletText("IP     : %s", ip.c_str());
        ImGui::BulletText("Port   : %d", port);

        ImGui::Spacing();
        ImGui::Separator();

        // ======================================================
        // Controls
        // ======================================================

        ImGui::Text("Controls");

        ImGui::Dummy(ImVec2(0, 5));

        ImGui::SetCursorPosX(vidMode->width/2 + 60);
        ImGui::Button("Forward", ImVec2(100, 35));

        ImGui::Dummy(ImVec2(0, 5));

        ImGui::SetCursorPosX(vidMode->width/2 - 50);
        ImGui::Button("Left", ImVec2(100, 35));

        ImGui::SameLine();

        ImGui::Button("Stop", ImVec2(100, 35));

        ImGui::SameLine();

        ImGui::Button("Right", ImVec2(100, 35));

        ImGui::Dummy(ImVec2(0, 5));

        ImGui::SetCursorPosX(vidMode->width/2 + 60);
        ImGui::Button("Backward", ImVec2(100, 35));

        ImGui::Spacing();
        ImGui::Separator();

        // ======================================================
        // Keyboard Controls
        // ======================================================

        ImGui::Text("Keyboard");
        ImGui::BulletText("W - Forward");
        ImGui::BulletText("A - Left");
        ImGui::BulletText("S - Backward");
        ImGui::BulletText("D - Right");
        ImGui::BulletText("B - Brake/Stop car");
        ImGui::BulletText("ESC - Exit");

        ImGui::Spacing();
        ImGui::Separator();

        // ======================================================
        // Status
        // ======================================================

        const char* state = "Idle";

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            Send(FORWARD);
            state = "Moving Forward";

            if(logs.empty() || logs.back() != state)
                logs.emplace_back(state);
        }

        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            Send(BACK);
            state = "Moving Backward";

            if(logs.empty() || logs.back() != state)
                logs.emplace_back(state);
        }

        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            Send(LEFT);
            state = "Turning Left";

            if(logs.empty() || logs.back() != state)
                logs.emplace_back(state);
        }

        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            Send(RIGHT);
            state = "Turning Right";

            if(logs.empty() || logs.back() != state)
                logs.emplace_back(state);
        }

        if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
            Send(STOP);
            state = "Stopping";

            if(logs.empty() || logs.back() != state)
                logs.emplace_back(state);
        }
        
        ImGui::Text("Logs");
        ImGui::Separator();


        if (logs.size() > 20)
            logs.erase(logs.begin());

        for(const auto& log : logs) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s", log.c_str());
        }

        ImGui::End();

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    Send(QUIT);

    closesocket(client);
    WSACleanup();

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
