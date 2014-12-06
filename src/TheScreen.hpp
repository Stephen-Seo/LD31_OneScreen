
#ifndef THE_SCREEN_HPP
#define THE_SCREEN_HPP

#define PLAYER_SPEED 35.0f
#define PLAYER_JUMP 50.0f
#define GRAVITY 2.0f
#define SLOPE_BUMP 2.0f

#include <engine/state.hpp>

#include <SFML/Graphics.hpp>

class TheScreen : public State
{
public:
    TheScreen(StateStack& stack, Context context);
    ~TheScreen();

    void draw();
    bool update(sf::Time dt);
    bool handleEvent(const sf::Event& event);

private:
    sf::Image bgImage;
    sf::Sprite bgSprite;
    bool up;
    bool down;
    bool left;
    bool right;
    bool jump;

    sf::RectangleShape player;
    bool grounded;
    sf::Vector2f velocity;

    sf::Vector2u startLocation;

    bool jetpack;

    /*
     * collision bitfield map
        0000 0000 - black
        0000 0001 - red
        0000 0010 - green
        0000 0100 - blue
        0000 0011 - yellow
        0000 0110 - cyan
        0000 0101 - magenta
        0000 0111 - white
    */
    std::unique_ptr<char[]> collisionMap;

    void playerMovement(sf::Time dt);
    void playerGravity(sf::Time dt);
    void applyVelocity(sf::Time dt);
    void save(bool changeStart);
    void resetCollisionMap();
};

#endif
