#include <lib_resize/resize.hpp>

#include <lib_utils/image_view.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <vector>

static void checkParameters(const ConstImageView &input,
                            const ImageView &output)
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

template <typename T>
static constexpr auto getPixelBounded(const ImageViewBase<T> &input,
                                      int x,
                                      int y)
{
    auto bounded_x = std::max(0, std::min(input.getWidth() - 1, x));
    auto bounded_y = std::max(0, std::min(input.getHeight() - 1, y));
    return input.get(bounded_x, bounded_y);
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
            const auto *input_pixel = getPixelBounded(input, input_x, input_y);
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

            auto pixel_top_left =
                getPixelBounded(input, input_x_pixel, input_y_pixel);
            auto pixel_top_right =
                getPixelBounded(input, input_x_pixel + 1, input_y_pixel);
            auto pixel_bottom_left =
                getPixelBounded(input, input_x_pixel, input_y_pixel + 1);
            auto pixel_bottom_right =
                getPixelBounded(input, input_x_pixel + 1, input_y_pixel + 1);

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

// https://www.paulinternet.nl/?page=bicubic
template <typename T>
static constexpr double cubicInterpolate(double t, T A, T B, T C, T D)
{
    double a = -A / 2.0 + (3.0 * B) / 2.0 - (3.0 * C) / 2.0 + D / 2.0;
    double b = static_cast<double>(A) - (5.0 * B) / 2.0 + 2.0 * C - D / 2.0;
    double c = -static_cast<double>(A) / 2.0 + C / 2.0;
    double d = B;

    return a * t * t * t + b * t * t + c * t + d;
}

void bicubicInterpolate(const ConstImageView &input,
                        unsigned char *output,
                        double input_x,
                        double input_y)
{
    auto x = static_cast<int>(input_x);
    auto y = static_cast<int>(input_y);
    double tx = input_x - x;
    double ty = input_y - y;

    const auto p00 = getPixelBounded(input, x - 1, y - 1);
    const auto p10 = getPixelBounded(input, x + 0, y - 1);
    const auto p20 = getPixelBounded(input, x + 1, y - 1);
    const auto p30 = getPixelBounded(input, x + 2, y - 1);

    const auto p01 = getPixelBounded(input, x - 1, y + 0);
    const auto p11 = getPixelBounded(input, x + 0, y + 0);
    const auto p21 = getPixelBounded(input, x + 1, y + 0);
    const auto p31 = getPixelBounded(input, x + 2, y + 0);

    const auto p02 = getPixelBounded(input, x - 1, y + 1);
    const auto p12 = getPixelBounded(input, x + 0, y + 1);
    const auto p22 = getPixelBounded(input, x + 1, y + 1);
    const auto p32 = getPixelBounded(input, x + 2, y + 1);

    const auto p03 = getPixelBounded(input, x - 1, y + 2);
    const auto p13 = getPixelBounded(input, x + 0, y + 2);
    const auto p23 = getPixelBounded(input, x + 1, y + 2);
    const auto p33 = getPixelBounded(input, x + 2, y + 2);

    for (int c = 0; c < input.getChannels(); c++)
    {
        auto col0 = cubicInterpolate(tx, p00[c], p10[c], p20[c], p30[c]);
        auto col1 = cubicInterpolate(tx, p01[c], p11[c], p21[c], p31[c]);
        auto col2 = cubicInterpolate(tx, p02[c], p12[c], p22[c], p32[c]);
        auto col3 = cubicInterpolate(tx, p03[c], p13[c], p23[c], p33[c]);

        auto value = cubicInterpolate(ty, col0, col1, col2, col3);
        auto clamped = std::max(0.0, std::min(255.0, value));
        output[c] = static_cast<unsigned char>(clamped);
    }
}

void resizeBiCubic(const ConstImageView &input, ImageView &output)
{
    checkParameters(input, output);

    double delta_width = (1.0 * input.getWidth()) / output.getWidth();
    double delta_height = (1.0 * input.getHeight()) / output.getHeight();

    for (int y = 0; y < output.getHeight(); y++)
    {
        auto input_y = y * delta_height;

        for (int x = 0; x < output.getWidth(); x++)
        {
            auto input_x = x * delta_width;

            auto *output_pixel = output.get(x, y);
            bicubicInterpolate(input, output_pixel, input_x, input_y);
        }
    }
}
