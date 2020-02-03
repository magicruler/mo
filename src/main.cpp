#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "game.h"

#include "texture.h"
#include "render_target.h"

#include "editor_window.h"
#include "editor_window_system.h"

#include "shader.h"
#include "editor_scene_view.h"

#ifndef __glad_h_
#include <glad/glad.h>
#endif
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "input.h"

#include <cstdio>
#include "time_manager.h"

#include "string_utils.h"

#ifdef _WIN32
#include <direct.h>
#else
#include<unistd.h>  
#endif

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow *window = nullptr;

int main(int, char **args)
{
    auto path = args[0];
	std::string environmentPath = "";

	#ifdef _WIN32
	auto tokens = StringUtils::Split(path, "\\");
	

	for (int i = 0; i < tokens.size() - 1; i++)
	{
		environmentPath += (tokens[i] + "\\");
	}
    environmentPath += "assets\\";

	_chdir(environmentPath.c_str());
	#else
	auto tokens = StringUtils::Split(path, "/");
	for (int i = 0; i < tokens.size() - 1; i++)
	{
		environmentPath += (tokens[i] + "/");
	}
    environmentPath += "assets/";

	chdir(environmentPath.c_str());
	#endif

	spdlog::info("Environment Path Is {}", environmentPath);
    
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        return 0;
    }

    //TestOrdinalSpeed();

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    window = glfwCreateWindow(1400, 900, "Mo", nullptr, nullptr);
    if (window == NULL)
    {
        return 0;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    bool err = gladLoadGL() == 0;
    if (err)
    {
        spdlog::debug("Failed to initialize OpenGL loader!");
        return 0;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    // io.ConfigDockingWithShift = false;
    // io.ConfigWindowsResizeFromEdges = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    static bool p_open = true;

    Input::Init(window);
    Game::Init();

    while (!glfwWindowShouldClose(window))
    {  
        Time::Update();

        glfwPollEvents();

        Game::Render();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui::ShowDemoWindow(&p_open);

        Game::Update();

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
        
        Time::LimitFPS();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    Game::End();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}