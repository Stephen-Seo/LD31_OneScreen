
#ifndef SOUND_PLAYER_HPP
#define SOUND_PLAYER_HPP

#include <map>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include "resourceHolder.hpp"
#include "resourceIdentifiers.hpp"

class SoundPlayer : private sf::NonCopyable
{
public:
    SoundPlayer();
    int create(sf::SoundBuffer& soundBuffer);
    void play(int id, bool looping=false);
    void play(int id, sf::Vector2f position, bool looping=false);
    void pause(int id);
    void destroy(int id);
    void removeStoppedSounds();
    void stopAllSounds();
    void stopLoopingSounds();
    void setListenerPosition(sf::Vector2f position);
    sf::Vector2f getListenerPosition() const;
private:
    std::map<int, sf::Sound> mSounds;
    int id;
};

#endif
