
#ifndef GAME_UTILITY_HPP
#define GAME_UTILITY_HPP

#include <SFML/Graphics.hpp>
#include <memory>

namespace GameUtility
{
    // Returns a unique_ptr holding a coordinate of collision if any point
    // collides with the color. Otherwise, returns a unique_ptr holding
    // nothing.
//    std::unique_ptr<sf::Vector2u> pixelsCollidesWithColor(sf::Vector2u* coord, unsigned int size, sf::Image image, sf::Color color);

    // Returns a new image that has pixels changed to white at the given
    // coordinate.
    sf::Image removeColorAtPixel(sf::Vector2u coord, sf::Image image, char* collisionMap);

    // Returns the first instance of a specific color in the image. The
    // unique_ptr will hold nothing if the color was not found.
    std::unique_ptr<sf::Vector2u> findColor(sf::Image image, sf::Color color);

    // Returns corners/sides of rectangle shape as unique_ptr array.
    std::unique_ptr<sf::Vector2u[]> getEdges(sf::RectangleShape shape);
}

#endif
