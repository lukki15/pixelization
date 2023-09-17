#include <catch2/catch_test_macros.hpp>

#include <lib_stb/stb_image.hpp>

const std::filesystem::path test_images = "./tests/images";
const std::filesystem::path empty = test_images / "empty.png";
const std::filesystem::path rgbw_cmyk = test_images / "rgbw-cmyk.png";

TEST_CASE("Throws no file found", "[lib_stb, stb_image]")
{
    CHECK_THROWS(StbImage(""));
    CHECK_THROWS(StbImage("./"));
    CHECK_THROWS(StbImage("./test.png"));
}

TEST_CASE("Open empty image", "[lib_stb, stb_image]")
{
    CHECK_THROWS(StbImage(empty));
}

TEST_CASE("Open successful", "[lib_stb, stb_image]")
{
    CHECK_NOTHROW(StbImage(rgbw_cmyk));
}

TEST_CASE("check dimensions", "[lib_stb, stb_image]")
{
    constexpr int expected_width = 4;
    constexpr int expected_height = 2;
    constexpr int expected_channels = 4;

    const StbImage image(rgbw_cmyk);
    CHECK(image.getWidth() == expected_width);
    CHECK(image.getHeight() == expected_height);
    CHECK(image.getChannels() == expected_channels);
}

TEST_CASE("pixel values", "[lib_stb, stb_image]")
{
    const StbImage image(rgbw_cmyk);
    REQUIRE(image.getData() != nullptr);

    const auto getPixel = [&image](int x, int y) {
        int index = (y * image.getWidth() + x) * image.getChannels();
        return &image.getData()[index];
    };
    constexpr auto comparePixel =
        [](const unsigned char *pixel, int red, int green, int blue) {
            CHECK(pixel[0] == red);
            CHECK(pixel[1] == green);
            CHECK(pixel[2] == blue);
        };

    const auto *red = getPixel(0, 0);
    comparePixel(red, 255, 0, 0);
    const auto *green = getPixel(1, 0);
    comparePixel(green, 0, 255, 0);
    const auto *blue = getPixel(2, 0);
    comparePixel(blue, 0, 0, 255);
    const auto *white = getPixel(3, 0);
    comparePixel(white, 255, 255, 255);

    const auto *cyan = getPixel(0, 1);
    comparePixel(cyan, 0, 255, 255);
    const auto *magenta = getPixel(1, 1);
    comparePixel(magenta, 255, 0, 255);
    const auto *yellow = getPixel(2, 1);
    comparePixel(yellow, 255, 255, 0);
    const auto *black = getPixel(3, 1);
    comparePixel(black, 0, 0, 0);
}

TEST_CASE("alpha values", "[lib_stb, stb_image]")
{
    const StbImage image(rgbw_cmyk);
    REQUIRE(image.getData() != nullptr);

    const auto *data = image.getData();
    for (int y = 0; y < image.getHeight(); y++)
    {
        for (int x = 0; x < image.getWidth(); x++)
        {
            constexpr int alpha_offset = 3;
            int index =
                (y * image.getWidth() + x) * image.getChannels() + alpha_offset;
            constexpr unsigned char non_translucent = 255;
            CHECK(data[index] == non_translucent);
        }
    }
}
