// Main entry point for application.

#include "Engine.h"
#include "MapRenderer.h"
#include "LevelLoader.h"
#include "SFML\Graphics.hpp"
#include <iostream>

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;
constexpr int FPS = 30;
constexpr int TILE_SIZE2 = 32;
constexpr int NUM_LAYERS = 1;

void provider(int x, int y, int layer, sf::Color& color, sf::IntRect &src) {
  color = sf::Color::White;

  //std::cout << x << ", " << y << std::endl;

  //if (layer > 0) {
    //src = sf::IntRect(32 * 8, 0, TILE_SIZE2, TILE_SIZE2);
    //return;
  //}

  //src = (x % 2 == 0) ? sf::IntRect(0, 0, TILE_SIZE2, TILE_SIZE2) : sf::IntRect(32, 0, TILE_SIZE2, TILE_SIZE2);
  src = sf::IntRect(0, abs(x % 6) * 32, TILE_SIZE2, TILE_SIZE2);

  //src = sf::IntRect(0, 0, 32, 32);

  //color = sf::Color((x * 5) % 255, (y * 5) % 255, 50);
  //src = sf::IntRect(0, 0, 1, 1);
}

void provider2(int x, int y, int layer, sf::Color& color, sf::IntRect &src) {
  color = sf::Color::White;
  src = sf::IntRect(32 * 3, 0, TILE_SIZE2, TILE_SIZE2);
}

int main()
{
  ImageManager im;
  LevelLoader ll;
  ll.loadLevel("Jail.tmx", im);

  //Engine engine;
  //engine.start();

  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game");

  sf::Clock clock;
  sf::Time elapsed = elapsed.Zero;

  sf::View view = window.getDefaultView();

  sf::Texture texture;
  //texture.create(1, 1);
  if (!texture.loadFromFile("./Assets/World_A2.png")) {
    throw std::exception("blah");
  }

  MapRenderer map(&texture, provider, TILE_SIZE2, NUM_LAYERS);

  sf::Texture texture2;
  //texture.create(1, 1);
  if (!texture2.loadFromFile("./Assets/tileset.png")) {
    throw std::exception("blah");
  }
  MapRenderer map2(&texture2, provider2, TILE_SIZE2, NUM_LAYERS);

  sf::Vector2f prev;

  while (window.isOpen()) {

    // Process Events
    sf::Event event;
    while (window.pollEvent(event)) {
      sf::Vector2f ms, delta;
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        case sf::Event::Resized:
          view.setSize(event.size.width, event.size.height);
          view.setCenter(view.getSize() * 0.5f);
          break;
        case sf::Event::MouseMoved:
          ms = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
          delta = ms - prev;
          if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            view.setCenter(view.getCenter() - delta);
          prev = ms;
          break;
        case sf::Event::KeyPressed:
          map.refresh();
          map2.refresh();
          break;
      }

      // TODO: Handle GUI events
    }
    elapsed += clock.restart();

    // Avoid updating graphics too often
    if (elapsed.asMilliseconds() < 1000 / FPS) {
      sf::sleep(sf::milliseconds(10));
      continue;
    }

    elapsed -= sf::milliseconds(1000 / FPS);
    window.clear();
    window.setView(view);

    map.update(window);
    window.draw(map);

    map2.update(window);
    window.draw(map2);

    // TODO: Draw GUI
    window.display();
  }

  return 0;
}