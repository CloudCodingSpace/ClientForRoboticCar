#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <iostream>

int main() {
    std::string ip = "";
    int port = 0;

    std::cout << "Enter the ip address (don't include port) :- ";
    std::cin >> ip;
    std::cout << "Enter port :- ";
    std::cin >> port;

    WSADATA data{};
    WSAStartup(MAKEWORD(2, 2), &data);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in addr{};
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;

    if(connect(client, (sockaddr*)&addr, sizeof(addr)) != 0) {
        std::cerr << "Failed to connect to server!" << std::endl;
        return -1;
    }

    send(client, "Yooo!!", 6 * sizeof(char), 0);

    closesocket(client);
    WSACleanup();

    // Window
    assert(glfwInit() && "Failed to init glfw!");

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
   
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(vidMode->width, vidMode->height, "Client for Robotic Car", monitor, nullptr);
    assert(window && "Failed to create window!");
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        assert(false && "Failed to load OpenGL!");
    }

    // Init other stuff
   
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glfwShowWindow(window);
    while(!glfwWindowShouldClose(window)) {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            break;

        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    return 0;
}
