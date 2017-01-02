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

#include "STP/Core/Tile.hpp"
#include "STP/Core/TileSet.hpp"

#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

#include "SFML/Graphics/RenderTarget.hpp"

namespace tmx {

Tile::Tile() :
        texture_(nullptr) {
}

Tile::Tile(const sf::Texture* texture, sf::Vector2f texture_pos, sf::Vector2f tile_size) :
    properties_(std::make_shared<Properties>()),
    texture_(texture),
    visible(true) {
    SetupCoords(texture_pos, tile_size);
}

Tile::Tile(const Image& image) :
    properties_(std::make_shared<Properties>()),
    texture_(image.GetTexture()),
    visible(true) {
    SetupCoords(sf::Vector2f(0.f, 0.f), sf::Vector2f(image.GetWidth(), image.GetHeight()));
}

bool Tile::empty() const {
    if (!texture_) return true;
    else return false;
}

void Tile::SetPosition(const sf::Vector2f& pos) {
    sf::Vector2f tile_size = GetSize();

    vertices_[0].position = sf::Vector2f(pos.x, pos.y);
    vertices_[1].position = sf::Vector2f(pos.x + tile_size.x, pos.y);
    vertices_[2].position = sf::Vector2f(pos.x + tile_size.x, pos.y + tile_size.y);
    vertices_[3].position = sf::Vector2f(pos.x, pos.y + tile_size.y);
}

Tile& Tile::Flip(unsigned int flags) {
    flags_ = flags;
    if (flags & TileFlip::FLIP_DIAGONAL) {
        std::swap(vertices_[1].texCoords, vertices_[3].texCoords);
        SetPosition(vertices_[0].position);
    }
    if (flags & TileFlip::FLIP_HORIZONTAL) {
        std::swap(vertices_[0].texCoords, vertices_[1].texCoords);
        std::swap(vertices_[2].texCoords, vertices_[3].texCoords);
    }
    if (flags & TileFlip::FLIP_VERTICAL) {
        std::swap(vertices_[0].texCoords, vertices_[3].texCoords);
        std::swap(vertices_[1].texCoords, vertices_[2].texCoords);
    }
    return *this;
}

sf::Vector2f Tile::GetSize() const {
    if (flags_ & TileFlip::FLIP_DIAGONAL) {
        return sf::Vector2f(
            std::abs(vertices_[1].texCoords.y - vertices_[0].texCoords.x),
            std::abs(vertices_[2].texCoords.x - vertices_[0].texCoords.y)
        );
    } else {
        return sf::Vector2f(
            std::abs(vertices_[1].texCoords.x - vertices_[0].texCoords.x),
            std::abs(vertices_[2].texCoords.y - vertices_[0].texCoords.y)
        );
    }
}

const sf::Texture* Tile::GetTexture() const {
    return texture_;
}

void Tile::SetColor(const sf::Color& color) {
    vertices_[0].color = color;
    vertices_[1].color = color;
    vertices_[2].color = color;
    vertices_[3].color = color;
}

void Tile::AddProperty(const std::string& name, const std::string& value) {
    properties_->AddProperty(name, value);
}

std::string& Tile::GetPropertyValue(const std::string& name) {
    return properties_->GetPropertyValue(name);
}

void Tile::SetupCoords(sf::Vector2f texture_pos, sf::Vector2f tile_size)
{
    vertices_[0].texCoords = sf::Vector2f(texture_pos.x, texture_pos.y);
    vertices_[1].texCoords = sf::Vector2f(texture_pos.x + tile_size.x, texture_pos.y);
    vertices_[2].texCoords = sf::Vector2f(texture_pos.x + tile_size.x, texture_pos.y + tile_size.y);
    vertices_[3].texCoords = sf::Vector2f(texture_pos.x, texture_pos.y + tile_size.y);

    SetPosition(sf::Vector2f(0.f, 0.f));
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (texture_ && visible) {
        states.texture = texture_;
        target.draw(vertices_, 4, sf::Quads, states);
    }
}

}  // namespace tmx
