#include <catch2/catch_test_macros.hpp>

#include <lib_resize/resize.hpp>
#include <lib_utils/image_view.hpp>

#include <cmath>
#include <limits>

#include "image.hpp"

TEST_CASE("different channels", "[lib_resize, resize, nearest_neighbor]")
{
    InputImage<1, 1, 3> input;
    OutputImage<1, 1, 4> output;

    REQUIRE_THROWS(resizeNearestNeighbor(input, output));
}

TEST_CASE("Empty input image", "[lib_resize, resize, nearest_neighbor]")
{
    constexpr InputImage<0, 0, 4> input;
    OutputImage<1, 1, 4> output;

    REQUIRE_THROWS(resizeNearestNeighbor(input, output));
}

TEST_CASE("Empty output image", "[lib_resize, resize, nearest_neighbor]")
{
    InputImage<1, 1, 4> input;
    OutputImage<0, 0, 4> output;

    REQUIRE_THROWS(resizeNearestNeighbor(input, output));
}

TEST_CASE("input is output", "[lib_resize, resize, nearest_neighbor]")
{
    InputImage<10, 10, 4> input;
    OutputImage<10, 10, 4> output;

    for (size_t i = 0; i < input.size(); i++)
    {
        input.image_array[i] = static_cast<decltype(input)::ImageData>(
            (i) % std::numeric_limits<decltype(input)::ImageData>::max());
    }

    resizeNearestNeighbor(input, output);

    for (size_t i = 0; i < input.size(); i++)
        CHECK(input.image_array[i] == output.image_array[i]);
}

TEST_CASE("input is double output", "[lib_resize, resize, nearest_neighbor]")
{
    InputImage<10, 10, 4> input;
    OutputImage<5, 5, 4> output;

    for (size_t i = 0; i < input.size(); i++)
    {
        input.image_array[i] = static_cast<decltype(input)::ImageData>(
            (i) % std::numeric_limits<decltype(input)::ImageData>::max());
    }

    resizeNearestNeighbor(input, output);

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

TEST_CASE("input is half output", "[lib_resize, resize, nearest_neighbor]")
{
    InputImage<5, 5, 4> input;
    OutputImage<10, 10, 4> output;

    for (size_t i = 0; i < input.size(); i++)
    {
        input.image_array[i] = static_cast<decltype(input)::ImageData>(
            (i) % std::numeric_limits<decltype(input)::ImageData>::max());
    }

    resizeNearestNeighbor(input, output);

    for (int y = 0; y < output.getHeight(); y++)
    {
        for (int x = 0; x < output.getWidth(); x++)
        {
            auto input_x = std::min((x + 1) / 2, input.getWidth() - 1);
            auto input_y = std::min((y + 1) / 2, input.getHeight() - 1);
            for (int c = 0; c < output.getChannels(); c++)
            {
                CHECK(input.get(input_x, input_y)[c] == output.get(x, y)[c]);
            }
        }
    }
}

TEST_CASE("input offset", "[lib_resize, resize, nearest_neighbor]")
{
    InputImage<10, 10, 4> input;
    OutputImage<3, 3, 4> output;

    for (size_t i = 0; i < input.size(); i++)
    {
        input.image_array[i] = static_cast<decltype(input)::ImageData>(
            (i) % std::numeric_limits<decltype(input)::ImageData>::max());
    }

    resizeNearestNeighbor(input, output);

    for (int y = 0; y < output.getHeight(); y++)
    {
        for (int x = 0; x < output.getWidth(); x++)
        {
            auto input_x = static_cast<int>(std::round(x * 10.0 / 3));
            auto input_y = static_cast<int>(std::round(y * 10.0 / 3));
            for (int c = 0; c < output.getChannels(); c++)
            {
                CHECK(input.get(input_x, input_y)[c] == output.get(x, y)[c]);
            }
        }
    }
}

TEST_CASE("input double with half height",
          "[lib_resize, resize, nearest_neighbor]")
{
    InputImage<10, 10, 4> input;
    OutputImage<5, 20, 4> output;

    for (size_t i = 0; i < input.size(); i++)
    {
        input.image_array[i] = static_cast<decltype(input)::ImageData>(
            (i) % std::numeric_limits<decltype(input)::ImageData>::max());
    }

    resizeNearestNeighbor(input, output);

    for (int y = 0; y < output.getHeight(); y++)
    {
        for (int x = 0; x < output.getWidth(); x++)
        {
            auto input_x = x * 2;
            auto input_y = std::min((y + 1) / 2, input.getHeight() - 1);
            for (int c = 0; c < output.getChannels(); c++)
            {
                CHECK(input.get(input_x, input_y)[c] == output.get(x, y)[c]);
            }
        }
    }
}
