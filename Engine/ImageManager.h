#pragma once

#include <vector>
#include <SFML\Graphics.hpp>
#include <memory>
#include <unordered_map>

// Manage loaded image assets
class ImageManager {
private:
  std::vector<std::unique_ptr<sf::Texture>> _textures;
  std::unordered_map<int, int> _idToIdx;

public:
  ImageManager() {}

  ~ImageManager() {}

  void addImage(std::unique_ptr<sf::Texture> image, int id);

  sf::Texture* getTexture(int id);

  void loadTileset(const std::string& filename);
};