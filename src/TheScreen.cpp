
#include <TheScreen.hpp>

TheScreen::TheScreen(StateStack& stack, Context context) :
State(stack, context)
{
}

TheScreen::~TheScreen()
{
}

void TheScreen::draw()
{
}

bool TheScreen::update(sf::Time dt)
{
}

bool TheScreen::handleEvent(const sf::Event& event)
{
}