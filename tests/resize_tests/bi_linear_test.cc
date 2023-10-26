#include <catch2/catch_test_macros.hpp>

#include <lib_resize/resize.hpp>
#include <lib_utils/image_view.hpp>

#include <cmath>
#include <limits>

#include "image.hpp"

TEST_CASE("different channels", "[lib_resize, resize, bi_linear]")
{
    InputImage<1, 1, 3> input;
    OutputImage<1, 1, 4> output;

    REQUIRE_THROWS(resizeBiLinear(input, output));
}

TEST_CASE("Empty input image", "[lib_resize, resize, bi_linear]")
{
    constexpr InputImage<0, 0, 4> input;
    OutputImage<1, 1, 4> output;

    REQUIRE_THROWS(resizeBiLinear(input, output));
}

TEST_CASE("Empty output image", "[lib_resize, resize, bi_linear]")
{
    InputImage<1, 1, 4> input;
    OutputImage<0, 0, 4> output;

    REQUIRE_THROWS(resizeBiLinear(input, output));
}

TEST_CASE("input is output", "[lib_resize, resize, bi_linear]")
{
    InputImage<10, 10, 4> input;
    OutputImage<10, 10, 4> output;

    for (size_t i = 0; i < input.size(); i++)
    {
        input.image_array[i] = static_cast<decltype(input)::ImageData>(
            (i) % std::numeric_limits<decltype(input)::ImageData>::max());
    }

    resizeBiLinear(input, output);

    for (size_t i = 0; i < input.size(); i++)
        CHECK(input.image_array[i] == output.image_array[i]);
}

TEST_CASE("input is double output", "[lib_resize, resize, bi_linear]")
{
    InputImage<10, 10, 4> input;
    OutputImage<5, 5, 4> output;

    for (size_t i = 0; i < input.size(); i++)
    {
        input.image_array[i] = static_cast<decltype(input)::ImageData>(
            (i) % std::numeric_limits<decltype(input)::ImageData>::max());
    }

    resizeBiLinear(input, output);

    for (int y = 0; y < output.getHeight(); y++)
    {
        for (int x = 0; x < output.getWidth(); x++)
        {
            for (int c = 0; c < output.getChannels(); c++)
            {
                CHECK(input.get(x * 2, y * 2)[c] == output.get(x, y)[c]);
            }
        }
    }
}

TEST_CASE("input is half output", "[lib_resize, resize, bi_linear]")
{
    InputImage<5, 5, 4> input;
    OutputImage<10, 10, 4> output;

    for (size_t i = 0; i < input.size(); i++)
    {
        input.image_array[i] = static_cast<decltype(input)::ImageData>(
            (i) % std::numeric_limits<decltype(input)::ImageData>::max());
    }

    resizeBiLinear(input, output);

    for (int y = 0; y < output.getHeight(); y++)
    {
        for (int x = 0; x < output.getWidth(); x++)
        {
            auto input_x = x / 2;
            auto input_y = y / 2;

            auto next_x = std::min(input_x + 1, input.getWidth() - 1);
            auto next_y = std::min(input_y + 1, input.getHeight() - 1);

            auto pixel_top_left = input.get(input_x, input_y);
            auto pixel_top_right = input.get(input_x, next_y);
            auto pixel_bottom_left = input.get(next_x, input_y);
            auto pixel_bottom_right = input.get(next_x, next_y);

            for (int c = 0; c < output.getChannels(); c++)
            {
                if (x % 2 == 0 && y % 2 == 0)
                {
                    CHECK(pixel_top_left[c] == output.get(x, y)[c]);
                }
                else if (x % 2 == 1 && y % 2 == 1)
                {
                    auto value_sum =
                        static_cast<unsigned int>(pixel_top_left[c]) +
                        pixel_top_right[c] + pixel_bottom_left[c] +
                        pixel_bottom_right[c];
                    auto value =
                        static_cast<decltype(input)::ImageData>(value_sum / 4);
                    CHECK(value == output.get(x, y)[c]);
                }
                else if (x % 2 == 1 && y % 2 == 0)
                {
                    auto value_sum =
                        static_cast<unsigned int>(pixel_top_left[c]) +
                        pixel_bottom_left[c];
                    auto value =
                        static_cast<decltype(input)::ImageData>(value_sum / 2);
                    CHECK(static_cast<int>(value) == output.get(x, y)[c]);
                }
                else
                {
                    auto value_sum =
                        static_cast<unsigned int>(pixel_top_left[c]) +
                        pixel_top_right[c];
                    auto value =
                        static_cast<decltype(input)::ImageData>(value_sum / 2);
                    CHECK(static_cast<int>(value) == output.get(x, y)[c]);
                }
            }
        }
    }
}
TEST_CASE("input offset", "[lib_resize, resize, bi_linear]")
{
    InputImage<10, 10, 4> input;
    OutputImage<3, 3, 4> output;

    for (size_t i = 0; i < input.size(); i++)
    {
        input.image_array[i] = static_cast<decltype(input)::ImageData>(
            (i) % std::numeric_limits<decltype(input)::ImageData>::max());
    }

    resizeBiLinear(input, output);

    for (int y = 0; y < output.getHeight(); y++)
    {
        for (int x = 0; x < output.getWidth(); x++)
        {
            auto input_x = x * 10 / 3;
            auto input_y = y * 10 / 3;

            auto pixel_top_left = input.get(input_x, input_y);
            auto pixel_top_right = input.get(input_x, input_y + 1);
            auto pixel_bottom_left = input.get(input_x + 1, input_y);
            auto pixel_bottom_right = input.get(input_x + 1, input_y + 1);

            for (int c = 0; c < output.getChannels(); c++)
            {
                if (x == 0 && y == 0)
                {
                    CHECK(pixel_top_left[c] == output.get(x, y)[c]);
                }
                else if (x % 3 == y % 3)
                {
                    auto value_top =
                        x % 2 == 1 ? pixel_top_left[c] : pixel_top_right[c];
                    auto value_bottom = y % 2 == 1 ? pixel_bottom_left[c]
                                                   : pixel_bottom_right[c];
                    auto value_sum =
                        static_cast<unsigned int>(pixel_top_left[c]) +
                        pixel_top_right[c] + pixel_bottom_left[c] +
                        pixel_bottom_right[c] + value_top + value_bottom;
                    auto value =
                        static_cast<decltype(input)::ImageData>(value_sum / 6) +
                        (x % 3) * 2 - 3;
                    CHECK(static_cast<int>(value) ==
                          static_cast<int>(output.get(x, y)[c]));
                }
                else
                {
                    // TODO
                }
            }
        }
    }
}
