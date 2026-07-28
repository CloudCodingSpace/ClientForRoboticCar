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

const char* QUIT = "/q\n";
const char* FORWARD = "/w\n";
const char* STOP = "/b\n";
const char* BACK = "/s\n";
const char* LEFT = "/a\n";
const char* RIGHT = "/d\n";

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
        io.Fonts->AddFontFromFileTTF("consolas.ttf", 20.0f);
        io.Fonts->Build();
    
        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        style.Alpha = 1.0;
        style.WindowRounding = 3;
        style.GrabRounding = 1;
        style.GrabMinSize = 20;
        style.FrameRounding = 3;

        style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 0.65f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.73f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.27f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.65f, 0.65f, 0.46f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.01f, 1.00f, 1.00f, 0.43f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.62f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 1.00f, 0.33f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glfwShowWindow(window);
    const char* prevSignal = "  ";
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

        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Robotic Car Control Panel");
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

        if((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) && (strcmp(prevSignal, FORWARD)) != 0) {
            Send(FORWARD);
            state = "Moving Forward";
            prevSignal = FORWARD;

            if(logs.empty() || logs.back() != state)
                logs.emplace_back(state);
        }

        else if((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && (strcmp(prevSignal, FORWARD) != 0)) {
            Send(BACK);
            state = "Moving Backward";
            prevSignal = BACK;

            if(logs.empty() || logs.back() != state)
                logs.emplace_back(state);
        }

        else if((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && (strcmp(prevSignal, LEFT) != 0)) {
            Send(LEFT);
            state = "Turning Left";
            prevSignal = LEFT;

            if(logs.empty() || logs.back() != state)
                logs.emplace_back(state);
        }

        else if((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && (strcmp(prevSignal, RIGHT) != 0)) {
            Send(RIGHT);
            state = "Turning Right";
            prevSignal = RIGHT;

            if(logs.empty() || logs.back() != state)
                logs.emplace_back(state);
        }

        else if((glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) && (strcmp(prevSignal, STOP) != 0)) {
            Send(STOP);
            state = "Stopping";
            prevSignal = STOP;

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
