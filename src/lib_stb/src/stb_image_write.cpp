#include <lib_stb/stb_image_write.hpp>

#include <optional>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

int stbImageWrite(const std::filesystem::path &path,
                  int width,
                  int height,
                  int channels,
                  const unsigned char *data,
                  std::optional<int> quality)
{
    const auto extension = path.extension();
    const auto path_string = path.string();

    if (extension == ".png")
    {
        int stride_in_bytes = width * channels;
        return stbi_write_png(path_string.c_str(),
                              width,
                              height,
                              channels,
                              data,
                              stride_in_bytes);
    }
    else if (extension == ".bmp")
    {
        return stbi_write_bmp(path_string.c_str(),
                              width,
                              height,
                              channels,
                              data);
    }
    else if (extension == ".tga")
    {
        return stbi_write_tga(path_string.c_str(),
                              width,
                              height,
                              channels,
                              data);
    }
    else if (extension == ".jpg" || extension == ".jpeg")
    {
        return stbi_write_jpg(path_string.c_str(),
                              width,
                              height,
                              channels,
                              data,
                              quality.value_or(80));
    }

    throw std::runtime_error("extension not supported");
}
