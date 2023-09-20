#include <atomic>
#include <ctime>
#include <filesystem>

class TestEnvironment
{
public:
    TestEnvironment()
    {
        init();
        auto random_variable = std::rand();

        std::string folder_name = "test_" + std::to_string(random_variable);
        new_path = std::filesystem::temp_directory_path() / folder_name;

        std::filesystem::create_directory(new_path);
        std::filesystem::current_path(new_path);
    }
    ~TestEnvironment()
    {
        std::filesystem::current_path(original_path);
        std::filesystem::remove(new_path);
    }

    const std::filesystem::path &getNewPath() const
    {
        return new_path;
    }

private:
    void init()
    {
        if (!initalized.test_and_set())
        {
            // TODO get randomness seed from Catch2
            std::srand(static_cast<unsigned int>(std::time(nullptr)));
        }
    }
    static inline std::atomic_flag initalized = ATOMIC_FLAG_INIT;

    const std::filesystem::path original_path{std::filesystem::current_path()};
    std::filesystem::path new_path{};
};
