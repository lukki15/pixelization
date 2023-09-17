#include <catch2/catch_test_macros.hpp>

#include <lib_stb/stb_image.hpp>

TEST_CASE("Throws no file found", "[lib_stb, stb_image]")
{
    CHECK_THROWS(StbImage(""));
    CHECK_THROWS(StbImage("./"));
    CHECK_THROWS(StbImage("./test.png"));
}

TEST_CASE("Open successful", "[lib_stb, stb_image]")
{
    CHECK_NOTHROW(StbImage("./rgbw-cmyk.png"));
}
