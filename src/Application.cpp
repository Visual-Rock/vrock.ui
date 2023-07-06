#include "vrock/ui/Application.hpp"

#include <utility>
#include <vrock/log/Logger.hpp>

#include <imgui_impl_glfw.h>

#if defined( VROCKUI_OPENGL )
#include <GLFW/glfw3.h>
#include <imgui_impl_opengl3.h>
#elif defined( VROCKUI_VULKAN )
#include "Vulkan.cpp"
#else
#error "No Renderer Selected"
#endif

#define SPECTRUM_USE_DARK_THEME
#include <spectrum.h>

#include "font.h"

#include <future>
#include <memory>

static void glfw_error_callback( int error, const char *description )
{
    vrock::log::get_logger( "ui" )->log->error( "GLFW Error {}: {}", error, description );
}

namespace vrock::ui
{
    auto Application::run( const ApplicationConfig &config, std::shared_ptr<ImGuiBaseWidget> root ) -> int
    {
        logger->log->debug( "initializing Window" );
        glfwSetErrorCallback( glfw_error_callback );
        GLFWwindow *window;

        /* Initialize the library */
        if ( !glfwInit( ) )
            return -1;

#if defined( VROCKUI_VULKAN )
        glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
#endif

        logger->log->debug( "creating Window" );
        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow( (int)config.width, (int)config.height, config.application_name.c_str( ), nullptr,
                                   nullptr );
        if ( !window )
        {
            glfwTerminate( );
            return -1;
        }

#if defined( VROCKUI_VULKAN )
        if ( !glfwVulkanSupported( ) )
        {
            log::get_logger( "ui" )->log->error( "Vulkan not supported!" );
            return -1;
        }
        uint32_t extensions_count = 0;
        const char **extensions = glfwGetRequiredInstanceExtensions( &extensions_count );
        SetupVulkan( extensions, extensions_count );

        VkSurfaceKHR surface;
        VkResult err = glfwCreateWindowSurface( g_Instance, window, g_Allocator, &surface );
        check_vk_result( err );

        int w, h;
        glfwGetFramebufferSize( window, &w, &h );
        ImGui_ImplVulkanH_Window *wd = &g_MainWindowData;
        SetupVulkanWindow( wd, surface, w, h );

        s_AllocatedCommandBuffers.resize( wd->ImageCount );
        s_ResourceFreeQueue.resize( wd->ImageCount );
#elif defined( VROCKUI_OPENGL )
        glfwMakeContextCurrent( window );
#endif

        logger->log->debug( "initializing ImGui" );

        rename = [ & ]( const std::string &title ) { glfwSetWindowTitle( window, title.c_str( ) ); };

        IMGUI_CHECKVERSION( );
        ImGui::CreateContext( );
        ImGuiIO &io = ImGui::GetIO( );
#if defined( VROCKUI_OPENGL )
        ImGui_ImplGlfw_InitForOpenGL( window, true );
#endif
        io.ConfigFlags = config.config_flags;
        io.BackendFlags = config.backend_flags;

#if defined( VROCKUI_VULKAN )
        // Setup Platform / Renderer backends
        ImGui_ImplGlfw_InitForVulkan( window, true );
        ImGui_ImplVulkan_InitInfo init_info = { };
        init_info.Instance = g_Instance;
        init_info.PhysicalDevice = g_PhysicalDevice;
        init_info.Device = g_Device;
        init_info.QueueFamily = g_QueueFamily;
        init_info.Queue = g_Queue;
        init_info.PipelineCache = g_PipelineCache;
        init_info.DescriptorPool = g_DescriptorPool;
        init_info.Subpass = 0;
        init_info.MinImageCount = g_MinImageCount;
        init_info.ImageCount = wd->ImageCount;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = g_Allocator;
        init_info.CheckVkResultFn = check_vk_result;
        ImGui_ImplVulkan_Init( &init_info, wd->RenderPass );
#elif defined( VROCKUI_OPENGL )
        ImGui_ImplOpenGL3_Init( "#version 330" );
#endif
        ImGui::Spectrum::StyleColorsSpectrum( );

        ImFont *font = io.Fonts->AddFontFromMemoryCompressedTTF( roboto_compressed_data, roboto_compressed_size, 18.0 );
        if ( font )
            io.FontDefault = font;

#if defined( VROCKUI_VULKAN )
        {
            // Use any command queue
            VkCommandPool command_pool = wd->Frames[ wd->FrameIndex ].CommandPool;
            VkCommandBuffer command_buffer = wd->Frames[ wd->FrameIndex ].CommandBuffer;

            err = vkResetCommandPool( g_Device, command_pool, 0 );
            check_vk_result( err );
            VkCommandBufferBeginInfo begin_info = { };
            begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            err = vkBeginCommandBuffer( command_buffer, &begin_info );
            check_vk_result( err );

            ImGui_ImplVulkan_CreateFontsTexture( command_buffer );

            VkSubmitInfo end_info = { };
            end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            end_info.commandBufferCount = 1;
            end_info.pCommandBuffers = &command_buffer;
            err = vkEndCommandBuffer( command_buffer );
            check_vk_result( err );
            err = vkQueueSubmit( g_Queue, 1, &end_info, VK_NULL_HANDLE );
            check_vk_result( err );

            err = vkDeviceWaitIdle( g_Device );
            check_vk_result( err );
            ImGui_ImplVulkan_DestroyFontUploadObjects( );
        }
#endif
        logger->log->debug( "initializing Main Window" );
        root->setup( );

        std::unique_ptr<std::future<bool>> close = nullptr;
        /* Loop until the user closes the window */
        while ( !should_close )
        {
            // close handling
            if ( glfwWindowShouldClose( window ) && close == nullptr )
            {
                close = std::make_unique<std::future<bool>>( std::async( close_handler_ ) );
                glfwSetWindowShouldClose( window, GLFW_FALSE );
            }
            if ( close != nullptr && close->valid( ) )
            {
                if ( close->wait_for( std::chrono::milliseconds( 0 ) ) == std::future_status::ready )
                {
                    should_close = close->get( );
                    close = nullptr;
                }
            }

#if defined( VROCKUI_VULKAN )
            glfwPollEvents( );
            // Resize swap chain?
            if ( g_SwapChainRebuild )
            {
                int width, height;
                glfwGetFramebufferSize( window, &width, &height );
                if ( width > 0 && height > 0 )
                {
                    ImGui_ImplVulkan_SetMinImageCount( g_MinImageCount );
                    ImGui_ImplVulkanH_CreateOrResizeWindow( g_Instance, g_PhysicalDevice, g_Device, &g_MainWindowData,
                                                            g_QueueFamily, g_Allocator, width, height,
                                                            g_MinImageCount );
                    g_MainWindowData.FrameIndex = 0;

                    // Clear allocated command buffers from here since entire pool is destroyed
                    s_AllocatedCommandBuffers.clear( );
                    s_AllocatedCommandBuffers.resize( g_MainWindowData.ImageCount );

                    g_SwapChainRebuild = false;
                }
            }

            // Start the Dear ImGui frame
            ImGui_ImplVulkan_NewFrame( );
#elif defined( VROCKUI_OPENGL )
            glClear( GL_COLOR_BUFFER_BIT );
            ImGui_ImplOpenGL3_NewFrame( );
#endif
            ImGui_ImplGlfw_NewFrame( );
            ImGui::NewFrame( );

            root->render( );

            ImGui::Render( );
#if defined( VROCKUI_VULKAN )
            ImDrawData *main_draw_data = ImGui::GetDrawData( );
            const bool main_is_minimized =
                ( main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f );
            if ( !main_is_minimized )
                FrameRender( wd, main_draw_data );

            // Update and Render additional Platform Windows
            if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
            {
                ImGui::UpdatePlatformWindows( );
                ImGui::RenderPlatformWindowsDefault( );
            }

            // Present Main Platform Window
            if ( !main_is_minimized )
                FramePresent( wd );
#elif defined( VROCKUI_OPENGL )
            ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );
            /* Swap front and back buffers */
            glfwSwapBuffers( window );
            /* Poll for and process events */
            glfwPollEvents( );
#endif
        }
        logger->log->debug( "cleanup" );
        rename = [ & ]( const std::string &title ) { logger->log->debug( "can't rename window after cleanup!" ); };
        root->terminate( );
#if defined( VROCKUI_VULKAN )
        // Cleanup
        err = vkDeviceWaitIdle( g_Device );
        check_vk_result( err );

        // Free resources in queue
        for ( auto &queue : s_ResourceFreeQueue )
        {
            for ( auto &func : queue )
                func( );
        }
        s_ResourceFreeQueue.clear( );

        ImGui_ImplVulkan_Shutdown( );
        ImGui_ImplGlfw_Shutdown( );
        ImGui::DestroyContext( );

        CleanupVulkanWindow( );
        CleanupVulkan( );
#elif defined( VROCKUI_OPENGL )
        ImGui_ImplOpenGL3_Shutdown( );
        ImGui_ImplGlfw_Shutdown( );
        ImGui::DestroyContext( );
#endif
        glfwDestroyWindow( window );
        glfwTerminate( );
        return 0;
    }

    auto Application::rename_window( const std::string &title ) -> void
    {
        rename( title );
    }

    auto Application::close_handler( std::function<bool( )> fn ) -> void
    {
        close_handler_ = fn;
    }
} // namespace vrock::ui
