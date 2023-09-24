#include <catch2/catch_test_macros.hpp>

#include <lib_utils/image_view.hpp>

#include <array>
#include <numeric>

TEST_CASE("Constexpr", "[lib_utils, image_view]")
{
    constexpr ImageView image_view{};

    STATIC_CHECK(image_view.size() == 0);
    STATIC_CHECK(image_view.getWidth() == 0);
    STATIC_CHECK(image_view.getHeight() == 0);
    STATIC_CHECK(image_view.getChannels() == 0);
    STATIC_CHECK(image_view.data() == nullptr);
    STATIC_CHECK(image_view.get(0, 0) == nullptr);
}

template <size_t W, size_t H, size_t C>
static constexpr auto getArrayIota()
{
    std::array<unsigned char, W * H * C> array{};
    unsigned char i = 1;
    for (auto &v : array)
    {
        v = i++;
    }
    return array;
}

TEST_CASE("Constexpr Data", "[lib_utils, image_view]")
{
    constexpr int width = 2;
    constexpr int height = 3;
    constexpr int channels = 4;
    constexpr std::array data = getArrayIota<width, height, channels>();
    ImageView image_view{width, height, channels, data.data()};

    CHECK(image_view.size() == data.size());
    CHECK(image_view.getWidth() == width);
    CHECK(image_view.getHeight() == height);
    CHECK(image_view.getChannels() == channels);
    CHECK(image_view.data() == data.data());
    CHECK(image_view.get(0, 0) == data.data());
}

TEST_CASE("Get data", "[lib_utils, image_view]")
{
    constexpr int width = 2;
    constexpr int height = 3;
    constexpr int channels = 4;
    constexpr std::array data = getArrayIota<width, height, channels>();
    ImageView image_view{width, height, channels, data.data()};

    int i = 1;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            for (int c = 0; c < channels; c++)
                CHECK(image_view.get(x, y)[c] == i++);
        }
    }
}
