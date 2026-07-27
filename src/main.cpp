#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cstring>
#include <cassert>
#include <iostream>

#define QUIT "/q"
#define FORWARD "/w"
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
    
        ImGui::StyleColorsDark();
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

        ImGui::Begin("Main Panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

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

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 70);
        if (ImGui::Button("Forward", ImVec2(100, 35)))
            Send(FORWARD);

        ImGui::Dummy(ImVec2(0, 5));

        if (ImGui::Button("Left", ImVec2(100, 35)))
            Send(LEFT);

        ImGui::SameLine();

        if (ImGui::Button("Stop", ImVec2(100, 35)))
            Send(QUIT);

        ImGui::SameLine();

        if (ImGui::Button("Right", ImVec2(100, 35)))
            Send(RIGHT);

        ImGui::Dummy(ImVec2(0, 5));

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 70);
        if (ImGui::Button("Backward", ImVec2(100, 35)))
            Send(BACK);

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
        }

        else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            Send(BACK);
            state = "Moving Backward";
        }

        else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            Send(LEFT);
            state = "Turning Left";
        }

        else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            Send(RIGHT);
            state = "Turning Right";
        }

        ImGui::Text("Robot Status");
        ImGui::TextColored(ImVec4(0,1,0,1), "%s", state);

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
