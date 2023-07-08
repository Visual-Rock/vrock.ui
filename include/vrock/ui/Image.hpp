#pragma once

#include <cstdint>
#include <string>

#include "vulkan/vulkan.h"

namespace vrock::ui
{
    enum class VROCKUI_API ImageFormat
    {
        None = 0,
        RGBA,
        RGBA32F
    };

    class VROCKUI_API Image
    {
    public:
        explicit Image( const std::string &path );
        Image( std::uint32_t, std::uint32_t, ImageFormat, const void *data );
        ~Image( );

        auto resize( std::uint32_t, std::uint32_t ) -> void;

        auto set_data( const void *data ) -> void;

        [[nodiscard]] auto get_descriptor_set( ) const -> VkDescriptorSet;

        auto get_width( ) const -> std::uint32_t;
        auto get_height( ) const -> std::uint32_t;

    private:
        auto allocate( std::size_t ) -> void;
        auto release( ) -> void;

        std::string filepath;
        std::uint32_t width = 0, height = 0;

        ImageFormat format = ImageFormat::None;

        VkImage image = nullptr;
        VkImageView image_view = nullptr;
        VkDeviceMemory memory = nullptr;
        VkSampler sampler = nullptr;

        VkBuffer buffer = nullptr;
        VkDeviceMemory device_memory = nullptr;
        std::size_t aligned_size = 0;

        VkDescriptorSet descriptor = nullptr;
    };
} // namespace vrock::ui
