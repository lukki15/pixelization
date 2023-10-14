#pragma once

#include <lib_utils/image_view.hpp>

#include <array>

template <size_t W, size_t H, size_t C>
struct Image
{
    using ImageData = unsigned char;
    std::array<ImageData, W * H * C> image_array{};
};

template <size_t W, size_t H, size_t C>
struct OutputImage : public Image<W, H, C>, public ImageView
{
    constexpr OutputImage()
        : ImageView(W, H, C, Image<W, H, C>::image_array.data())
    {
    }
};

template <size_t W, size_t H, size_t C>
struct InputImage : public Image<W, H, C>, public ConstImageView
{
    constexpr InputImage()
        : ConstImageView(W, H, C, Image<W, H, C>::image_array.data())
    {
    }
};
