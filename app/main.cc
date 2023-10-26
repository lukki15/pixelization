#include <filesystem>
#include <iostream>
#include <string>

#include <cxxopts.hpp>

#include <lib_resize/resize.hpp>
#include <lib_stb/stb_image.hpp>
#include <lib_stb/stb_image_write.hpp>
#include <lib_utils/image_view.hpp>

#include "config.hpp"

int main(int argc, char **argv)
{
    const auto welcome_message =
        std::string(project_name) + " v" + std::string(project_version);

    cxxopts::Options options(project_name.data(), welcome_message);

    options.add_options()       // add options
        ("help", "Print usage") // help
        ("i,input", "Input image file", cxxopts::value<std::string>())   //
        ("o,output", "Output image file", cxxopts::value<std::string>()) //
        ("w,width", "Output image width", cxxopts::value<int>())         //
        ("h,height", "Output image height", cxxopts::value<int>())       //
        ("nearest_neighbor", "Resize with nearest-neighbor")             //
        ("bi_linear", "Resize with bi-linear")                           //
        ("bi_cubic", "Resize with bi-cubic");

    auto result = options.parse(argc, argv);

    if (argc == 1 || result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return 0;
    }

    auto input_path = result["input"].as<std::string>();
    auto output_path = result["output"].as<std::string>();
    StbImage input(input_path);
    ConstImageView input_view(input.getWidth(),
                              input.getHeight(),
                              input.getChannels(),
                              input.getData());

    auto output_width = result["width"].as<int>();
    auto output_height = result["height"].as<int>();
    auto output_size =
        static_cast<size_t>(output_width * output_height * input.getChannels());
    std::vector<StbImage::PixelData> output_data(output_size);
    ImageView output_view{output_width,
                          output_height,
                          input.getChannels(),
                          output_data.data()};

    if (result.count("nearest_neighbor"))
    {
        resizeNearestNeighbor(input_view, output_view);
    }
    else if (result.count("bi_linear"))
    {
        resizeBiLinear(input_view, output_view);
    }
    else if (result.count("bi_cubic"))
    {
        resizeBiCubic(input_view, output_view);
    }
    else
    {
        throw std::runtime_error("no resize algorithm selected");
    }

    int write_result = stbImageWrite(output_path,
                                     output_view.getWidth(),
                                     output_view.getHeight(),
                                     output_view.getChannels(),
                                     output_view.data());
    if (write_result == 0)
    {
        throw std::runtime_error("failed to write output");
    }

    return 0;
}
