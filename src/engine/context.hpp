
#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <SFML/Graphics.hpp>

#include "resourceIdentifiers.hpp"

class ResourceManager;
class MusicPlayer;
class SoundPlayer;
class Engine;
class BattleContext;


struct Context
{
    Context(sf::RenderWindow& window,
            ResourceManager& resourceManager,
            MusicPlayer& mPlayer,
            SoundPlayer& sPlayer,
            Engine& ecEngine,
            bool& isQuitting,
            BattleContext& battleContext);

    sf::RenderWindow* window;
    ResourceManager* resourceManager;
    MusicPlayer* mPlayer;
    SoundPlayer* sPlayer;
    Engine* ecEngine;
    bool* isQuitting;
    BattleContext* battleContext;
};

#endif
