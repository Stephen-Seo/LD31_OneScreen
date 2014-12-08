
#ifndef RESOURCE_IDENTIFIERS_HPP
#define RESOURCE_IDENTIFIERS_HPP

#include <cstdlib>
#include <set>

namespace sf
{
    class Texture;
    class Font;
    class SoundBuffer;
}

namespace Textures
{
    enum ID
    {
        BG,
        SAVE,
        BGUI,
        LAMP
    };
}

namespace Fonts
{
    enum ID
    {
        DJVSMono
    };
}

/*
  Only use Music identifiers if you intend to use the MusicPlayer, which does not use a ResourcePacker.
  If you intend to use the ResourcePacker, load music files as SoundBuffers via the Sound enum values.
*/
namespace Music
{
    enum ID
    {
    };
}

namespace Sound
{
    enum ID
    {
        OneScreenBeat,
        Text
    };
}

namespace GameResources
{
    enum LoadingMode
    {
        DEFAULT,
        PACKFILE
    };
}

template<class Resource, class Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::SoundBuffer, Sound::ID> SoundBufferHolder;

typedef std::set<Textures::ID> TextureSet;
typedef std::set<Fonts::ID> FontSet;
typedef std::set<Sound::ID> SoundSet;

struct ResourcesSet
{
    ResourcesSet();

    ResourcesSet(TextureSet& tset, FontSet& fset, SoundSet& sset);

    TextureSet* tset;
    FontSet* fset;
    SoundSet* sset;
};
#endif
