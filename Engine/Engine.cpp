
#include "Engine.h"
#include <SFML\Graphics.hpp>
#include <iostream>
#include <cassert>

void Engine::init() {
  std::unique_ptr<sf::RenderWindow> rw(new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT, BIT_DEPTH), "ProtoTileEngine"));
  _window = std::move(rw);
  if (!_window) {
    throw std::exception("Could not initialize engine.");
  }

  loadImages();
  loadLevel();
}

void Engine::loadImages() {
    //auto sprite = std::make_unique<sf::Texture>();
    //sprite->loadFromFile("./Assets/TestSprite.png");
    //_imageManager.addImage(std::move(sprite), 0);

    //auto img = std::make_unique<sf::Image>();
    //img->create(32, 32, sf::Color::Red);
    //auto sprite3 = std::make_unique<sf::Texture>();
    //sprite3->loadFromImage(*img);
    //_imageManager.addImage(std::move(sprite3), 1);

    //_imageManager.loadTileset("./Assets/tileset.xml");

    //_testTile = new Tile(_imageManager.getTexture(0));
}

void Engine::loadLevel() {
  _level.loadLevel("level1.xml", _imageManager);

  //for (int y = 0; y < MAP_HEIGHT; y++) {
    //for (int x = 0; x < MAP_WIDTH; x++) {

      //auto tx = (y % 4 == 0) ? _imageManager.getTexture(0) : _imageManager.getTexture(1);
      //auto tx = _imageManager.getTexture(y % 4);
      //auto tx = _imageManager.getTexture(0);
      //_level.addTile(x, y, std::move(std::make_unique<Tile>(tx)));

      //auto tile = new Tile(_imageManager.getTexture(1));
      //auto tile = new Tile(tx);
      //_level.addTile(x, y, std::unique_ptr<Tile>(tile));

      //std::unique_ptr<Tile> tile (new Tile(_imageManager.getTexture(0)));
      //std::unique_ptr<Tile> tile(new Tile(tx));

      //_level.addTile(x, y, std::move(tile));
    //}
  //}
}

void Engine::mainLoop() {
  while (_window->isOpen()) {
    processInput();
    update();
    renderFrame();
  }
}

void Engine::renderFrame() {
  _window->clear();

  auto camOffset = _camera.GetNearestTileOffset(TILE_SIZE);
  auto bounds = _camera.getTileBounds(TILE_SIZE);

  for (int y = 0, tileY = bounds.top; y < bounds.height; y++, tileY++) {
    for (int x = 0, tileX = bounds.left ; x < bounds.width; x++, tileX++) {
      //std::cout << "render: " << x << "," << y << "  " << tileX << "," << tileY << std::endl;

      if (tileX >= MAP_WIDTH || tileY >= MAP_HEIGHT || tileX < 0 || tileY < 0) {
        continue;
      }

      auto curr = _level.getTile(tileX, tileY);
      if (!curr) {
        continue;
      }

      auto screenX = (x * TILE_SIZE) - camOffset.x;
      auto screenY = (y * TILE_SIZE) - camOffset.y;

      if (screenX < 0 || screenX > WIDTH || screenY < 0 || screenY > HEIGHT) {
        continue;
      }

      curr->draw({ screenX, screenY }, *_window);
      //_testTile->draw({ 33, 33 }, *_window);
    }
  }

  _window->display();
}

void Engine::processInput() {
  sf::Event evt;
  while (_window->pollEvent(evt)) {
    if (evt.type == sf::Event::Closed) {
      _window->close();
    }

    if (evt.type == sf::Event::MouseButtonPressed && !_mouseDown) {
      auto x = _camera.GetPosition().x + sf::Mouse::getPosition().x;
      auto y = _camera.GetPosition().y + sf::Mouse::getPosition().y;
      _camera.ScrollCenterTo(x, y);
      _mouseDown = true;
    }

    if (evt.type == sf::Event::MouseButtonReleased) {
      _mouseDown = false;
    }
  }

}

void Engine::update() {
  _camera.Update();
}

void Engine::start() {
  init();
  mainLoop();
}