
#ifndef THE_SCREEN_HPP
#define THE_SCREEN_HPP

#include <engine/state.hpp>

class TheScreen : public State
{
public:
    TheScreen(StateStack& stack, Context context);
    ~TheScreen();

    void draw();
    bool update(sf::Time dt);
    bool handleEvent(const sf::Event& event);

private:


};

#endif
