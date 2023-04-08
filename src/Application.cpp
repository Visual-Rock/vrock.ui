#include "vrock/ui/Application.hpp"

#include <vrock/log/Logger.hpp>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

namespace vrock::ui
{
    auto Application::run( ApplicationConfig config, std::shared_ptr<ImGuiBaseWidget> root ) -> int
    {
        logger->log->info( "initializing Window" );
        GLFWwindow *window;

        /* Initialize the library */
        if ( !glfwInit( ) )
            return -1;

        logger->log->info( "creating Window" );
        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow( config.width, config.height, config.application_name.c_str(), NULL, NULL );
        if ( !window )
        {
            glfwTerminate( );
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent( window );
        
        logger->log->info( "initializing ImGui" );

        rename = [&] ( std::string title ) { glfwSetWindowTitle( window, title.c_str() ); };

        IMGUI_CHECKVERSION( );
        ImGui::CreateContext( );
        ImGuiIO &io = ImGui::GetIO( );
        ImGui_ImplGlfw_InitForOpenGL( window, true );
        io.ConfigFlags = config.config_flags;
        io.BackendFlags = config.backend_flags;
        ImGui_ImplOpenGL3_Init( "#version 330" );

        logger->log->info( "initializing Main Window" );
        root->setup( );

        /* Loop until the user closes the window */
        while ( !glfwWindowShouldClose( window ) )
        {
            /* Render here */
            glClear( GL_COLOR_BUFFER_BIT );

            ImGui_ImplOpenGL3_NewFrame( );
            ImGui_ImplGlfw_NewFrame( );
            ImGui::NewFrame( );

            root->render( );

            ImGui::Render( );
            ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );
            /* Swap front and back buffers */
            glfwSwapBuffers( window );
            /* Poll for and process events */
            glfwPollEvents( );
        }
        logger->log->info( "cleanup" );
        rename = [&] ( std::string title ) { logger->log->info( "can't rename window after cleanup!" ); };
        root->terminate( );
        ImGui_ImplOpenGL3_Shutdown( );
        ImGui_ImplGlfw_Shutdown( );
        ImGui::DestroyContext( );

        glfwTerminate( );
        return 0;
    }

    auto Application::rename_window( std::string title ) -> void
    {
        rename( title );
    }
} // namespace vrock::ui
