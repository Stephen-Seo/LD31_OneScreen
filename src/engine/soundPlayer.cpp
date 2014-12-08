
#include "soundPlayer.hpp"

#include <algorithm>

namespace
{
    const float Listenerz = 300.0f;
    const float Attenuation = 8.0f;
    const float MinDistance2D = 200.0f;
    const float MinDistance3D =
        std::sqrt(MinDistance2D*MinDistance2D + Listenerz*Listenerz);
}

SoundPlayer::SoundPlayer() :
id(0)
{}

int SoundPlayer::create(sf::SoundBuffer& soundBuffer)
{
    sf::Sound newSound(soundBuffer);

    std::map<int, sf::Sound>::iterator check;
    do
    {
        check = mSounds.find(id++);
    } while (check != mSounds.end());

    int newID = id;

    mSounds.insert(std::make_pair(newID, newSound));

    return newID;
}

void SoundPlayer::play(int sid, bool looping)
{
    play(sid, getListenerPosition(), looping);
}

void SoundPlayer::play(int sid, sf::Vector2f position, bool looping)
{
    auto sound = mSounds.find(sid);
    if(sound == mSounds.end())
        return;

    sound->second.setLoop(looping);

    sound->second.setPosition(position.x, -position.y, 0.0f);
    sound->second.setAttenuation(Attenuation);
    sound->second.setMinDistance(MinDistance3D);

    sound->second.play();
}

void SoundPlayer::pause(int sid)
{
    auto sound = mSounds.find(sid);
    if(sound == mSounds.end())
        return;

    if(sound->second.getStatus() == sf::Sound::Playing)
    {
        sound->second.pause();
    }
}

void SoundPlayer::destroy(int sid)
{
    mSounds.erase(sid);
}

void SoundPlayer::removeStoppedSounds()
{
    for(auto iter = mSounds.begin(); iter != mSounds.end(); ++iter)
    {
        if(iter->second.getStatus() == sf::Sound::Stopped)
        {
            iter = mSounds.erase(iter);
            --iter;
        }
    }
}

void SoundPlayer::stopAllSounds()
{
    for(auto iter = mSounds.begin(); iter != mSounds.end(); ++iter)
    {
        if(iter->second.getStatus() == sf::Sound::Playing)
        {
            iter->second.stop();
        }
    }
}

void SoundPlayer::stopLoopingSounds()
{
    for(auto iter = mSounds.begin(); iter != mSounds.end(); ++iter)
    {
        if(iter->second.getStatus() == sf::Sound::Playing &&
           iter->second.getLoop())
        {
            iter->second.stop();
        }
    }
}

void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
    sf::Listener::setPosition(position.x, -position.y, Listenerz);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
    sf::Vector3f pos = sf::Listener::getPosition();
    return sf::Vector2f(pos.x,-pos.y);
}
