
#ifndef SOUND_PLAYER_HPP
#define SOUND_PLAYER_HPP

#include <list>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include "resourceHolder.hpp"
#include "resourceIdentifiers.hpp"

class SoundPlayer : private sf::NonCopyable
{
public:
    SoundPlayer();
    void play(sf::SoundBuffer& soundBuffer);
    void play(sf::SoundBuffer& soundBuffer, sf::Vector2f position);
    void removeStoppedSounds();
    void setListenerPosition(sf::Vector2f position);
    sf::Vector2f getListenerPosition() const;
private:
    std::list<sf::Sound> mSounds;
};

#endif
