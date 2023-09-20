#include <catch2/catch_test_macros.hpp>

#include "test_utils/test_environment.hpp"

TEST_CASE("Create Delete environment", "[test_environment]")
{
    CHECK_NOTHROW(TestEnvironment());
}

TEST_CASE("new path set", "[test_environment]")
{
    TestEnvironment environment{};
    CHECK_FALSE(environment.getNewPath().empty());
}

TEST_CASE("verify environment gets deleted", "[test_environment]")
{
    std::filesystem::path environment_path{};

    {
        TestEnvironment environment{};
        environment_path = environment.getNewPath();
        REQUIRE(std::filesystem::exists(environment_path));
    }

    CHECK_FALSE(std::filesystem::exists(environment_path));
}

TEST_CASE("always new path", "[test_environment]")
{
    TestEnvironment environment1{};
    TestEnvironment environment2{};
    REQUIRE(environment1.getNewPath() != environment2.getNewPath());
}
