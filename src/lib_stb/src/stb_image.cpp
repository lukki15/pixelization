#include "lib_stb/stb_image.hpp"

#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

StbImage::StbImage(const std::filesystem::path &path, int desired_channels)
{
    if (path.empty() || !path.has_filename() || !std::filesystem::exists(path))
    {
        throw std::runtime_error("image does not exists");
    }

    const std::string path_string = path.string();
    data = Data(stbi_load(path_string.c_str(),
                          &width,
                          &height,
                          &channels,
                          desired_channels),
                StbImage::dataDeleter);
    if (data.get() == nullptr)
    {
        throw std::runtime_error("failed to load image");
    }
}

int StbImage::getWidth() const noexcept
{
    return width;
}

int StbImage::getHeight() const noexcept
{
    return height;
}

int StbImage::getChannels() const noexcept
{
    return channels;
}

const unsigned char *StbImage::getData() const noexcept
{
    return data.get();
}

void StbImage::dataDeleter(unsigned char *img_data) noexcept
{
    stbi_image_free(img_data);
}
