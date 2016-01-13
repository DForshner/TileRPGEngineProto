#pragma once

#include "Tile.h"
#include "ImageManager.h"
#include "Level.h"
#include <vector>
#include <cassert>
#include <memory>
#include <SFML\Graphics.hpp>

class LevelLoader {
public:
  Level loadLevel(const std::string& filename, ImageManager& imageManager);
};