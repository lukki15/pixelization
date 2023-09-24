#include <cstddef>

class ImageView
{
    constexpr auto getIndex(int x, int y) const noexcept
    {
        return (y * width_ + x) * channels_;
    }

public:
    using Data = unsigned char;

    constexpr ImageView() = default;
    constexpr ImageView(const ImageView &image_view) = default;
    constexpr ImageView(int width, int height, int channels, const Data *data)
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

    constexpr const Data *data() const noexcept
    {
        return data_;
    }
    constexpr const Data *get(int x, int y) const noexcept
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
    const Data *data_ = nullptr;
};
