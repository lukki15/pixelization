#pragma once

#include <cstddef>

template <typename Data>
class ImageViewBase
{
    constexpr auto getIndex(int x, int y) const noexcept
    {
        return (y * width_ + x) * channels_;
    }

public:
    constexpr ImageViewBase() = default;
    constexpr ImageViewBase(const ImageViewBase &image_view) = default;
    constexpr ImageViewBase(int width, int height, int channels, Data *data)
        : width_{width}, height_(height), channels_(channels), data_(data)
    {
    }

    constexpr size_t size() const noexcept
    {
        return static_cast<size_t>(width_ * height_ * channels_);
    }
    constexpr auto getWidth() const noexcept
    {
        return width_;
    }
    constexpr auto getHeight() const noexcept
    {
        return height_;
    }
    constexpr auto getChannels() const noexcept
    {
        return channels_;
    }

    constexpr Data *data() const noexcept
    {
        return data_;
    }
    constexpr Data *get(int x, int y) const noexcept
    {
        if (data_ == nullptr)
        {
            return nullptr;
        }
        int index = getIndex(x, y);
        return &data_[index];
    }

private:
    const int width_{};
    const int height_{};
    const int channels_{};
    Data *data_ = nullptr;
};

using ImageView = ImageViewBase<unsigned char>;
using ConstImageView = ImageViewBase<const unsigned char>;
