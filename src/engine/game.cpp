
#include "game.hpp"

#include <TheScreen.hpp>
#include <Battle.hpp>

// set packfile name/filepath if one is being used
#define PACKFILE_NAME ""

// set to true if a packfile is being used
#define IS_USING_PACKFILE false

// if not using cmake to build and using the ResourcePacker lib,
// define ResourcePacker_FOUND


#if defined(ResourcePacker_FOUND)
#else
#  define IS_USING_PACKFILE false
#endif

#if IS_USING_PACKFILE == true
#  define RESOURCE_MANAGER_MODE GameResources::PACKFILE
#else
#  define RESOURCE_MANAGER_MODE GameResources::DEFAULT
#endif

Game::Game()
: window(sf::VideoMode(720,480), "LD31_RPGPlatformer"),
resourceManager(&stateStack, RESOURCE_MANAGER_MODE, PACKFILE_NAME),
mPlayer(),
sPlayer(),
stateStack(Context(window, resourceManager, mPlayer, sPlayer, ecEngine, isQuitting, battleContext)),
isQuitting(false)
{
    registerResources();
    registerStates();

    frameTime = sf::seconds(1.f / 60.f);
}

void Game::run()
{
    sf::Clock clock;
    sf::Time lastUpdateTime = sf::Time::Zero;
    while (window.isOpen() && !isQuitting)
    {
        lastUpdateTime += clock.restart();
        while (lastUpdateTime > frameTime)
        {
            lastUpdateTime -= frameTime;
            processEvents();
            update(frameTime);
        }
        draw();
    }

    if(window.isOpen())
        window.close();
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        stateStack.handleEvent(event);
        if(event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update(sf::Time deltaTime)
{
    stateStack.update(deltaTime);
}

void Game::draw()
{
    window.clear();
    stateStack.draw();
    window.display();
}

// register resources via resourceManager
// Resource IDs must be listed in resourceIdentifiers.hpp
void Game::registerResources()
{
    resourceManager.registerTexture(Textures::BG, "res/background.png");
    resourceManager.registerTexture(Textures::SAVE, "res/save.png");
    resourceManager.registerTexture(Textures::BGUI, "res/battleGui.png");
    resourceManager.registerTexture(Textures::LAMP, "res/lamp.png");

    resourceManager.registerSoundBuffer(Sound::OneScreenBeat, "res/LD31_00.ogg");
    resourceManager.registerSoundBuffer(Sound::Text, "res/LD31_sfx_text.ogg");
    resourceManager.registerSoundBuffer(Sound::Jetpack, "res/LD31_sfx_jetpack.ogg");
    resourceManager.registerSoundBuffer(Sound::YOUWIN, "res/LD31_YOUWIN.ogg");

    resourceManager.registerFont(Fonts::DJVSMono, "res/DejaVuSansMono.ttf");
}

// register states via stateStack
// State IDs must be listed in stateIdentifiers.hpp
void Game::registerStates()
{
    stateStack.registerState<TheScreen>(States::TheScreen);
    stateStack.registerState<Battle>(States::Battle);

    stateStack.pushState(States::TheScreen);
}
