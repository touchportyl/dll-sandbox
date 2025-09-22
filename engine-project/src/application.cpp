#include "application.h"

#include <windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h> // always put glad before glfw

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "FlexScripting/scriptregistry.h"
#include "Utilities/filelist.h"
#include "intmanager.h"

#include <iostream>

namespace FlexEngine
{
  GLFWwindow* m_glfwwindow = nullptr;
  ImGuiContext* m_imguicontext = nullptr;
  HMODULE hModule = NULL;
  std::string devenvpath = "E:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\IDE\\devenv.com";
  Path m_scriptingslnpath;
  std::string selectedScriptName = "";
  script_interface* runningScript = nullptr;

  Application::Application()
  {
    if (!glfwInit())
    {
      std::cerr << "Failed to initialize GLFW" << std::endl;
      return;
    }

    m_glfwwindow = glfwCreateWindow(800, 600, "FlexEngine", nullptr, nullptr);

    if (!m_glfwwindow)
    {
      std::cerr << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return;
    }

    glfwMakeContextCurrent(m_glfwwindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cerr << "Failed to initialize GLAD" << std::endl;
      return;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    IMGUI_CHECKVERSION();
    m_imguicontext = ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui::SetCurrentContext(m_imguicontext);

    ImGuiIO& io = ImGui::GetIO();

    // set config flags
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange; // disable internal mouse cursor handling

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(m_glfwwindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
  }

  Application::~Application()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(m_imguicontext);
    glfwDestroyWindow(m_glfwwindow);
    glfwTerminate();
  }

  void Application::Run()
  {
    while (!glfwWindowShouldClose(m_glfwwindow))
    {
      glfwPollEvents();

      // Quit on Control+Q
      if (glfwGetKey(m_glfwwindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
          glfwGetKey(m_glfwwindow, GLFW_KEY_Q) == GLFW_PRESS)
      {
        glfwSetWindowShouldClose(m_glfwwindow, true);
      }

      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      // Start the Dear ImGui frame
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      // Scripting GUI
      ImGui::SetNextWindowSizeConstraints(ImVec2(300, 200), ImVec2(FLT_MAX, FLT_MAX));
      if (ImGui::Begin("Scripting"))
      {
        if (ImGui::Button("Link scripting solution"))
        {
          auto file = FileList::Browse(
            "Link scripting solution",
            Path::current(),
            "scripting-project.sln",
            L"All Files\0*.*\0Solution Files\0*.sln\0",
            false, false, 1UL
          );
          if (!file.empty() && file[0].exists() && file[0].extension() == ".sln")
          {
            // Valid file selected
            m_scriptingslnpath = file[0];
          }
          else
          {
            std::cout << "No file selected." << std::endl;
          }
        }
        ImGui::Text(m_scriptingslnpath.empty() ? "No solution linked" : m_scriptingslnpath.string().c_str());
        ImGui::Separator();
        if (ImGui::Button("Link devenv.com executable"))
        {
          auto file = FileList::Browse(
            "Link MSBuild executable",
            Path::current(),
            "devenv.com",
            L"All Files\0*.*\0Executable Files\0*.com\0",
            false, false, 1UL
          );
          if (!file.empty() && file[0].exists() && file[0].filename() == "devenv.com")
          {
            // Valid file selected
            devenvpath = file[0].string();
          }
          else
          {
            std::cout << "No file selected." << std::endl;
          }
        }
        ImGui::Text(devenvpath.empty() ? "devenv.com not linked" : devenvpath.c_str());
        ImGui::Separator();

        static int choice = 0;
        ImGui::Text("Debug mode for specifically doing the load, build, and unload steps separately.");
        ImGui::RadioButton("Enabled", &choice, 1);
        ImGui::SameLine();
        ImGui::RadioButton("Disabled", &choice, 0);
        ImGui::Separator();

        if (choice == 1)
        {
          // Debug mode enabled

          if (ImGui::Button("Load scripts"))
          {
            // Unload previous DLL first if it exists
            if (hModule)
            {
              if (runningScript)
              {
                // Stop and clean up script instance
                runningScript->Stop();
                delete runningScript;
                runningScript = nullptr;
              }
              ScriptRegistry::Clear(); // Clear the old registry
              FreeLibrary(hModule);
              hModule = NULL;
              selectedScriptName = "";
              std::cout << "Previous DLL unloaded for reload." << std::endl;
            }

            std::cout << "Loading scripting-project.dll..." << std::endl;
            hModule = LoadLibraryA("scripting-project.dll");
            if (hModule == NULL)
            {
              std::cerr << "Failed to load scripting-project.dll. Error code: " << GetLastError() << std::endl;
            }
            else
            {
              ScriptRegistry::RegisterAllScripts(hModule);
              std::cout << "scripting-project.dll loaded successfully." << std::endl;
            }
          }
          ImGui::SameLine();
          if (ImGui::Button("Build scripts"))
          {
            std::cout << "Building scripts..." << std::endl;
            if (devenvpath.empty())
            {
              std::cerr << "devenv.com path not set. Please link the devenv.com executable." << std::endl;
            }
            else if (!m_scriptingslnpath.exists())
            {
              std::cerr << "Scripts not found. Please link a valid scripting solution." << std::endl;
            }
            else
            {
              // Unload previous DLL before building a new one
              if (hModule)
              {
                if (runningScript)
                {
                  runningScript->Stop();
                  delete runningScript;
                  runningScript = nullptr;
                }
                ScriptRegistry::Clear();
                FreeLibrary(hModule);
                hModule = NULL;
                selectedScriptName = "";
                std::cout << "Unloaded previous DLL for rebuild." << std::endl;
              }

              // Full command line: devenv.com + solution/project + args
              #if _DEBUG
              std::string cmdLine = "\"" + devenvpath + "\" \"" + m_scriptingslnpath.string() + "\" /Build Debug";
              #else
              std::string cmdLine = "\"" + devenvpath + "\" \"" + m_scriptingslnpath.string() + "\" /Build Release";
              #endif

              // Use std::vector for a dynamically sized buffer
              std::vector<char> cmd(cmdLine.begin(), cmdLine.end());
              cmd.push_back('\0');

              STARTUPINFOA si{};
              si.cb = sizeof(si);
              PROCESS_INFORMATION pi{};

              // Launch devenv.com
              if (!CreateProcessA(
                nullptr,
                cmd.data(),
                nullptr, nullptr,
                FALSE, 0, nullptr,
                m_scriptingslnpath.parent_path().string().c_str(),
                &si, &pi))
              {
                std::cerr << "Failed to launch Visual Studio! Error: " << GetLastError() << std::endl;
              }
              else
              {
                WaitForSingleObject(pi.hProcess, INFINITE);

                DWORD exitCode = 0;
                GetExitCodeProcess(pi.hProcess, &exitCode);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);

                if (exitCode != 0)
                  std::cerr << "Failed to build scripts. devenv.com exited with code: " << exitCode << std::endl;
                else
                  std::cout << "Scripts built successfully." << std::endl;
              }
            }
          }
          ImGui::SameLine();
          if (ImGui::Button("Unload scripts"))
          {
            if (hModule)
            {
              std::cout << "Unloading scripting-project.dll..." << std::endl;
              if (runningScript)
              {
                // Stop and clean up script instance before unloading DLL
                runningScript->Stop();
                delete runningScript;
                runningScript = nullptr;
              }
              if (FreeLibrary(hModule))
              {
                std::cout << "scripting-project.dll unloaded successfully." << std::endl;
                hModule = NULL;
                ScriptRegistry::Clear();
                selectedScriptName = "";
              }
              else
              {
                std::cerr << "Failed to unload scripting-project.dll. Error code: " << GetLastError() << std::endl;
              }
            }
            else
            {
              std::cout << "scripting-project.dll is not loaded." << std::endl;
            }
          }
        }
        else
        {
          // Debug mode disabled

          if (ImGui::Button("Hot-reload scripts"))
          {
            std::cout << "Hot-reloading scripting-project.dll..." << std::endl;
            // Unload previous DLL first if it exists
            if (hModule)
            {
              if (runningScript)
              {
                // Stop and clean up script instance
                runningScript->Stop();
                delete runningScript;
                runningScript = nullptr;
              }
              ScriptRegistry::Clear(); // Clear the old registry
              FreeLibrary(hModule);
              hModule = NULL;
              selectedScriptName = "";
              std::cout << "Previous DLL unloaded for reload." << std::endl;
            }
            std::cout << "Building scripts..." << std::endl;
            if (devenvpath.empty())
            {
              std::cerr << "devenv.com path not set. Please link the devenv.com executable." << std::endl;
            }
            else if (!m_scriptingslnpath.exists())
            {
              std::cerr << "Scripts not found. Please link a valid scripting solution." << std::endl;
            }
            else
            {
              // Full command line: devenv.com + solution/project + args
              #if _DEBUG
              std::string cmdLine = "\"" + devenvpath + "\" \"" + m_scriptingslnpath.string() + "\" /Build Debug";
              #else
              std::string cmdLine = "\"" + devenvpath + "\" \"" + m_scriptingslnpath.string() + "\" /Build Release";
              #endif
              // Use std::vector for a dynamically sized buffer
              std::vector<char> cmd(cmdLine.begin(), cmdLine.end());
              cmd.push_back('\0');
              STARTUPINFOA si{};
              si.cb = sizeof(si);
              PROCESS_INFORMATION pi{};
              // Launch devenv.com
              if (!CreateProcessA(
                nullptr,
                cmd.data(),
                nullptr, nullptr,
                FALSE, 0, nullptr,
                m_scriptingslnpath.parent_path().string().c_str(),
                &si, &pi))
              {
                std::cerr << "Failed to launch Visual Studio! Error: " << GetLastError() << std::endl;
              }
              else
              {
                WaitForSingleObject(pi.hProcess, INFINITE);
                DWORD exitCode = 0;
                GetExitCodeProcess(pi.hProcess, &exitCode);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                if (exitCode != 0)
                  std::cerr << "Failed to build scripts. devenv.com exited with code: " << exitCode << std::endl;
                else
                {
                  std::cout << "Scripts built successfully." << std::endl;
                  std::cout << "Loading scripting-project.dll..." << std::endl;
                  hModule = LoadLibraryA("scripting-project.dll");
                  if (hModule == NULL)
                  {
                    std::cerr << "Failed to load scripting-project.dll. Error code: " << GetLastError() << std::endl;
                  }
                  else
                  {
                    //ScriptRegistry::RegisterAllScripts(hModule);
                    ScriptReg::RegisterAllScripts(hModule);

                    std::cout << "scripting-project.dll loaded successfully." << std::endl;
                  }
                }
              }
            }
          }
        }

        ImGui::Text(hModule ? "scripting-project.dll loaded" : "scripting-project.dll not loaded");
        ImGui::Separator();

        // List registered scripts
        if (ImGui::CollapsingHeader("Scripts", ImGuiTreeNodeFlags_DefaultOpen))
        {
          for (const std::string& name : ScriptRegistry::GetScriptNames())
          {
            bool isSelected = (name == selectedScriptName);

            if (ImGui::Selectable(name.c_str(), isSelected))
            {
              selectedScriptName = name;

              // clean up old script
              if (runningScript)
              {
                runningScript->Stop(); // Safely stop the old script
                delete runningScript;
              }
              runningScript = nullptr;

              // create new script instance
              runningScript = ScriptRegistry::Create(g_flex_interface, name);
              if (runningScript)
              {
                // runningScript->Start(); // auto-run when selected
              }
            }

            if (isSelected)
            {
              ImGui::SetItemDefaultFocus();
            }
          }
        }

        // Display selected script details
        if (runningScript)
        {
          ImGui::Separator();
          ImGui::Text("Selected Script: %s", selectedScriptName.c_str());
          if (ImGui::Button("Start"))
          {
            runningScript->Start();
          }
          ImGui::SameLine();
          if (ImGui::Button("Stop"))
          {
            runningScript->Stop();
          }
        }

        ImGui::End();
      }

      // End the Dear ImGui frame
      ImGuiIO& io = ImGui::GetIO();
      int width, height;
      glfwGetWindowSize(m_glfwwindow, &width, &height);
      io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      // Update and Render additional Platform Windows
      if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
      {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
      }

      glfwSwapBuffers(m_glfwwindow);
    }
  }
}