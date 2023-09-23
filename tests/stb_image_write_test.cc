#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <lib_stb/stb_image.hpp>
#include <lib_stb/stb_image_write.hpp>

#include <array>

#include "test_utils/test_environment.hpp"

const std::filesystem::path test_images = "./tests/images";
const std::filesystem::path rgbw_cmyk = "rgbw-cmyk.png";

TEST_CASE("Throws wrong file format", "[lib_stb, stb_image_write]")
{
    constexpr int width = 1;
    constexpr int height = 1;
    constexpr int channels = 4;
    constexpr std::array<unsigned char, width * height * channels> data{};
    CHECK_THROWS(stbImageWrite("", width, height, channels, data.data()));
    CHECK_THROWS(stbImageWrite("./", width, height, channels, data.data()));
    CHECK_THROWS(stbImageWrite("./test", width, height, channels, data.data()));
    CHECK_THROWS(
        stbImageWrite("./test.txt", width, height, channels, data.data()));
}

TEST_CASE("Successful write", "[lib_stb, stb_image_write]")
{
    constexpr int width = 1;
    constexpr int height = 1;
    constexpr int channels = 4;
    constexpr std::array<unsigned char, width * height * channels> data{};

    std::filesystem::path image_name = GENERATE("./test.png",
                                                "./test.bmp",
                                                "./test.tga",
                                                "./test.jpg",
                                                "./test.jpeg");

    TestEnvironment environment{};

    int result =
        stbImageWrite(image_name, width, height, channels, data.data());

    CHECK(result != 0);
    CHECK(std::filesystem::exists(image_name));
}

TEST_CASE("Read write", "[lib_stb, stb_image_write]")
{
    const StbImage rgbw_cmyk_image(test_images / rgbw_cmyk);

    TestEnvironment environment{};

    int result = stbImageWrite(rgbw_cmyk,
                               rgbw_cmyk_image.getWidth(),
                               rgbw_cmyk_image.getHeight(),
                               rgbw_cmyk_image.getChannels(),
                               rgbw_cmyk_image.getData());
    CHECK(result != 0);

    const StbImage rgbw_cmyk_read(rgbw_cmyk);
    CHECK(rgbw_cmyk_image.getWidth() == rgbw_cmyk_read.getWidth());
    CHECK(rgbw_cmyk_image.getHeight() == rgbw_cmyk_read.getHeight());
    CHECK(rgbw_cmyk_image.getChannels() == rgbw_cmyk_read.getChannels());
    auto data_size = rgbw_cmyk_image.getWidth() * rgbw_cmyk_image.getHeight() *
                     rgbw_cmyk_image.getChannels();
    for (int i = 0; i < data_size; i++)
    {
        CHECK(rgbw_cmyk_image.getData()[i] == rgbw_cmyk_image.getData()[i]);
    }
}
