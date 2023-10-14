#include <lib_resize/resize.hpp>

#include <lib_utils/image_view.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <vector>

void checkParameters(const ConstImageView &input, const ImageView &output)
{
    if (input.getChannels() != output.getChannels())
    {
        throw std::runtime_error("mismatched channels");
    }
    if (input.size() == 0 || output.size() == 0)
    {
        throw std::runtime_error("input or output can not be zero");
    }
}

void resizeNearestNeighbor(const ConstImageView &input, ImageView &output)
{
    checkParameters(input, output);

    double delta_width = (1.0 * input.getWidth()) / output.getWidth();
    double delta_height = (1.0 * input.getHeight()) / output.getHeight();

    for (int y = 0; y < output.getHeight(); y++)
    {
        int input_y = static_cast<int>(std::round(y * delta_height));
        for (int x = 0; x < output.getWidth(); x++)
        {
            int input_x = static_cast<int>(std::round(x * delta_width));
            const auto *input_pixel = input.get(input_x, input_y);
            auto *output_pixel = output.get(x, y);
            std::copy_n(input_pixel, input.getChannels(), output_pixel);
        }
    }
}

template <typename I, typename O>
static void linearInterpolation(int channels,
                                const I *left,
                                const I *right,
                                double offset,
                                O *output)
{
    assert(0 <= offset);
    assert(offset <= 1);

    for (int channel = 0; channel < channels; channel++)
    {
        output[channel] = static_cast<O>(left[channel] * (1.0 - offset) +
                                         right[channel] * offset);
    }
}

void resizeBiLinear(const ConstImageView &input, ImageView &output)
{
    checkParameters(input, output);

    double delta_width = (1.0 * input.getWidth()) / output.getWidth();
    double delta_height = (1.0 * input.getHeight()) / output.getHeight();

    std::vector<double> top_pixel(static_cast<size_t>(input.getChannels()));
    std::vector<double> bottom_pixel(static_cast<size_t>(input.getChannels()));

    for (int y = 0; y < output.getHeight(); y++)
    {
        auto input_y = y * delta_height;
        int input_y_pixel = static_cast<int>(input_y);
        auto y_offset = input_y - input_y_pixel;

        for (int x = 0; x < output.getWidth(); x++)
        {
            auto input_x = x * delta_width;
            int input_x_pixel = static_cast<int>(input_x);
            auto x_offset = input_x - input_x_pixel;

            auto *output_pixel = output.get(x, y);

            auto pixel_top_left = input.get(input_x_pixel, input_y_pixel);
            auto pixel_top_right = input.get(input_x_pixel + 1, input_y_pixel);
            auto pixel_bottom_left =
                input.get(input_x_pixel, input_y_pixel + 1);
            auto pixel_bottom_right =
                input.get(input_x_pixel + 1, input_y_pixel + 1);

            linearInterpolation(input.getChannels(),
                                pixel_top_left,
                                pixel_top_right,
                                x_offset,
                                top_pixel.data());
            linearInterpolation(input.getChannels(),
                                pixel_bottom_left,
                                pixel_bottom_right,
                                x_offset,
                                bottom_pixel.data());
            linearInterpolation(input.getChannels(),
                                top_pixel.data(),
                                bottom_pixel.data(),
                                y_offset,
                                output_pixel);
        }
    }
}
