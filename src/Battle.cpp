
#include <Battle.hpp>

#include <ctime>

Battle::Battle(StateStack& stack, Context context) :
State(stack, context),
isInBattle(false),
gen(std::random_device()() + std::time(nullptr))
{
}

Battle::~Battle()
{
}

void Battle::draw()
{
}

bool Battle::update(sf::Time dt)
{
    return true;
}

bool Battle::handleEvent(const sf::Event& event)
{
    return true;
}

