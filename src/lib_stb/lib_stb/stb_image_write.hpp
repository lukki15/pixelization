#pragma once

#include <filesystem>
#include <optional>

int stbImageWrite(const std::filesystem::path &path,
                  int width,
                  int height,
                  int channels,
                  const unsigned char *data,
                  std::optional<int> quality = {});
