#pragma once

#include <future>
#include <string>
#include <vector>

#include "ImGuiBaseWidget.hpp"

namespace vrock::ui
{
    struct Filter
    {
        std::string description;
        std::string filter;
    };

    auto VROCKUI_API open_file( const std::string &title, const std::string &default_path,
                                std::vector<Filter> filter_patterns ) -> std::string;

    auto VROCKUI_API open_multiple_files( const std::string &title, const std::string &default_path,
                                          std::vector<Filter> filter_patterns ) -> std::vector<std::string>;

    auto VROCKUI_API select_folder( const std::string &title, const std::string &default_path ) -> std::string;

    auto VROCKUI_API save_file( const std::string &title, const std::string &default_path,
                                std::vector<Filter> filter_patterns ) -> std::string;

    auto VROCKUI_API open_file_async( const std::string &title, const std::string &default_path,
                                      const std::vector<Filter> &filter_patterns ) -> std::future<std::string>;

    auto VROCKUI_API open_multiple_files_async( const std::string &title, const std::string &default_path,
                                                const std::vector<Filter> &filter_patterns )
        -> std::future<std::vector<std::string>>;

    auto VROCKUI_API select_folder_async( const std::string &title, const std::string &default_path )
        -> std::future<std::string>;

    auto VROCKUI_API save_file_async( const std::string &title, const std::string &default_path,
                                      const std::vector<Filter> &filter_patterns ) -> std::future<std::string>;
} // namespace vrock::ui