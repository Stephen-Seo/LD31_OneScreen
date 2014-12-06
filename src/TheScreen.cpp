
#include <TheScreen.hpp>

#include <engine/resourceIdentifiers.hpp>
#include <engine/resourceManager.hpp>

TheScreen::TheScreen(StateStack& stack, Context context) :
State(stack, context)
{
    // Registering resources for this state
    tset.insert(Textures::BG);

    // Loading registered resources
    getContext().resourceManager->loadResources(getNeededResources());

    // Getting loaded resources
    sf::Texture* bgTexture = &getContext().resourceManager->getTexture(Textures::BG);

    bgImage = bgTexture->copyToImage();
    bgSprite.setTexture(*bgTexture);
}

TheScreen::~TheScreen()
{
}

void TheScreen::draw()
{
    getContext().window->draw(bgSprite);
}

bool TheScreen::update(sf::Time dt)
{
    return false;
}

bool TheScreen::handleEvent(const sf::Event& event)
{
    return false;
}
