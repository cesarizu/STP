////////////////////////////////////////////////////////////
//
// The MIT License (MIT)
//
// STP - SFML TMX Parser
// Copyright (c) 2013-2016 Manuel Sabogal
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////

#include "STP/Core/Layer.hpp"
#include "STP/Core/TileMap.hpp"

#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

#include "SFML/Graphics/RenderTarget.hpp"

namespace tmx {

Layer::Layer() {}

Layer::Layer(const std::string& name, unsigned int width,
             unsigned int height, float opacity, bool visible,
             MapOrientation orientation) :
        MapObject(name, width, height, opacity, visible),
        orientation_(orientation) {
    // Reserve space for each vector to avoid reallocate
    tiles_.reserve(width * height);
    unsigned char alpha = static_cast<unsigned char>(255 * opacity);
    color_.a = alpha;
}

unsigned int Layer::GetWidth() const {
    return width_;
}

unsigned int Layer::GetHeight() const {
    return height_;
}

Tile& Layer::GetTile(unsigned int x, unsigned int y) {
    if (x >= width_ || y >= height_) {
        char error[100];
        sprintf(error, "Error: tile (%u, %u) out of range.\n", x, y);
        throw std::out_of_range(error);
    }

    unsigned int pos = (y * width_) + x;

    return tiles_[pos];
}

void Layer::SetColor(const sf::Color& color) {
    color_.r = color.r;
    color_.g = color.g;
    color_.b = color.b;
    for (auto& tile : tiles_) {
        tile.SetColor(color_);
    }
}

void Layer::SetOpacity(float opacity) {
    color_.a = static_cast<unsigned char>(255 * opacity);
    for (auto& tile : tiles_) {
        tile.SetColor(color_);
    }
}

void Layer::draw(sf::RenderTarget& target, sf::RenderStates /* states */) const {
    if (visible) {
        for (unsigned int i = 0; i < tiles_.size(); ++i)
            target.draw(tiles_[i]);
    }
}

}  // namespace tmx
