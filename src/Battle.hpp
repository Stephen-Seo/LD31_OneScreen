
#ifndef BATTLE_HPP
#define BATTLE_HPP

#include <engine/state.hpp>

#include <random>

class Battle : public State
{
public:
    Battle(StateStack& stack, Context context);
    ~Battle();

    void draw();
    bool update(sf::Time dt);
    bool handleEvent(const sf::Event& event);

private:
    bool isInBattle;
    std::mt19937 gen;

};

#endif
