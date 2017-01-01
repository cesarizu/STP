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

#include "STP/Core/TileMap.hpp"
#include "STP/Core/Parser.hpp"

#include <cstdio>
#include <string>

#include "pugixml.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

namespace tmx {

TileMap::TileMap() :
        orientation_(MapOrientation::ORTHOGONAL),
        renderorder_(MapRenderOrder::RIGHT_DOWN),
        width_(0),
        height_(0),
        tilewidth_(0),
        tileheight_(0) {
}

TileMap::TileMap(MapOrientation orientation,
                 MapRenderOrder renderorder,
                 unsigned int width,
                 unsigned int height,
                 unsigned int tilewidth,
                 unsigned int tileheight) :
        orientation_(orientation),
        renderorder_(renderorder),
        width_(width),
        height_(height),
        tilewidth_(tilewidth),
        tileheight_(tileheight) {
}

TileSet* TileMap::GetTileSet(unsigned int gid) {
    if (gid == 0) return nullptr;
    for (auto tileset: tilesets_){
        unsigned int first_gid = tileset->GetFirstGID();
        unsigned int last_gid = first_gid + tileset->GetTileCount() - 1;
        if (first_gid <= gid && gid <= last_gid) {
            return tileset.get();
        }
    }
    return nullptr;
}

TileSet& TileMap::GetTileSet(const std::string& tileset_name) {
    return *(tilesets_hash_[tileset_name]);
}

Layer& TileMap::GetLayer(const std::string& layer_name) {
    return layers_[layer_name];
}

ObjectGroup& TileMap::GetObjectGroup(const std::string& objectgroup_name) {
    return object_groups_[objectgroup_name];
}

ImageLayer& TileMap::GetImageLayer(const std::string& imagelayer_name) {
    return image_layers_[imagelayer_name];
}

void TileMap::ShowObjects(bool show) {
    for (auto& object_group : object_groups_)
        object_group.second.visible = show;
}

unsigned int TileMap::GetWidth() const {
    return width_;
}

unsigned int TileMap::GetHeight() const {
    return height_;
}

unsigned int TileMap::GetTileWidth() const {
    return tilewidth_;
}

unsigned int TileMap::GetTileHeight() const {
    return tileheight_;
}

MapOrientation TileMap::GetOrientation() const {
    return orientation_;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates /* states */) const {
    for (unsigned int i = 0; i < map_objects_.size(); ++i) {
        if (map_objects_[i]->visible == true)
            target.draw(*map_objects_[i]);
    }
}

}  // namespace tmx
