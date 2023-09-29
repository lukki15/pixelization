#include <lib_resize/resize.hpp>

#include <lib_utils/image_view.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>

void resizeNearestNeighbor(const ConstImageView &input, ImageView &output)
{
    if (input.getChannels() != output.getChannels())
    {
        throw std::runtime_error("mismatched channels");
    }
    if (input.size() == 0 || output.size() == 0)
    {
        throw std::runtime_error("input or output can not be zero");
    }

    double delta_width = (1.0 * input.getWidth()) / output.getWidth();
    double delta_height = (1.0 * input.getHeight()) / output.getHeight();

    for (int y = 0; y < output.getHeight(); y++)
    {
        int input_y = static_cast<int>(y * delta_height);
        for (int x = 0; x < output.getWidth(); x++)
        {
            int input_x = static_cast<int>(x * delta_width);
            const auto *input_pixel = input.get(input_x, input_y);
            auto *output_pixel = output.get(x, y);
            std::copy_n(input_pixel, input.getChannels(), output_pixel);
        }
    }
}
