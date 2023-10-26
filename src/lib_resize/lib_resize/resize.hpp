#pragma once

#include <lib_utils/image_view.hpp>

void resizeNearestNeighbor(const ConstImageView &input, ImageView &output);

void resizeBiLinear(const ConstImageView &input, ImageView &output);

void resizeBiCubic(const ConstImageView &input, ImageView &output);
