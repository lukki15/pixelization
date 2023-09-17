#include <filesystem>
#include <memory>

class StbImage
{
public:
    StbImage(const std::filesystem::path &path, int desired_channels = 0);
    StbImage(const StbImage &) = delete;
    StbImage &operator=(const StbImage &) = delete;
    StbImage(StbImage &&) = delete;
    StbImage &operator=(StbImage &&) = delete;
    ~StbImage() = default;

    int getWidth() const noexcept;
    int getHeight() const noexcept;
    int getChannels() const noexcept;

    const unsigned char *getData() const noexcept;

private:
    static void dataDeleter(unsigned char *img_data) noexcept;
    using Data = std::unique_ptr<unsigned char, decltype(&dataDeleter)>;

    int width{};
    int height{};
    int channels{}; // # 8-bit components per pixel

    Data data{nullptr, dataDeleter};
};
