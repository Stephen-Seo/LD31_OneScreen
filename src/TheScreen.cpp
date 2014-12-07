
#include <TheScreen.hpp>

#include <iostream>

#include <engine/resourceIdentifiers.hpp>
#include <engine/resourceManager.hpp>
#include <engine/soundPlayer.hpp>
#include <GameUtility.hpp>

TheScreen::TheScreen(StateStack& stack, Context context) :
State(stack, context),
up(false),
down(false),
left(false),
right(false),
jump(false),
player(sf::Vector2f(9.0f, 9.0f)),
grounded(false),
jetpack(true),
collisionMap(new char[720*480])
{
    // check if "save" data exists
    tset.insert(Textures::SAVE);
    bool saveExists = true;
    try
    {
        getContext().resourceManager->loadResources(getNeededResources());
    }
    catch (const std::exception& e)
    {
        saveExists = false;
        tset.erase(Textures::SAVE);
    }

    // Registering resources for this state
    tset.insert(Textures::BG);
    sset.insert(Sound::OneScreenBeat);

    // Loading registered resources
    getContext().resourceManager->loadResources(getNeededResources());

    // Getting loaded resources
    sf::Texture* bgTexture = nullptr;

    if(saveExists)
    {
        bgTexture = &getContext().resourceManager->getTexture(Textures::SAVE);
    }
    else
    {
        bgTexture = &getContext().resourceManager->getTexture(Textures::BG);
    }

    bgImage = bgTexture->copyToImage();
    bgSprite.setTexture(*bgTexture);

    // setup player
    player.setFillColor(sf::Color::Magenta);
    std::unique_ptr<sf::Vector2u> startCoordinate = GameUtility::findColor(bgImage, sf::Color::Yellow);
    if(startCoordinate.get() == nullptr)
    {
        if(saveExists)
        {
            std::cout << "FATAL ERROR: Save image is invalid!\n" << std::flush;
            *(getContext().isQuitting) = true;
        }
        else
        {
            std::cout << "FATAL ERROR: Background image is invalid!\n" << std::flush;
            *(getContext().isQuitting) = true;
        }
    }
    else
    {
        player.setPosition(startCoordinate->x, startCoordinate->y);
    }

    // setup collisionMap
    sf::Color color;
    for(unsigned int j = 0; j < 480; ++j)
    {
        for(unsigned int i = 0; i < 720; ++i)
        {
            color = bgImage.getPixel(i, j);
            if(color == sf::Color::White)
            {
                collisionMap[i + j * 720] = 0x1 | 0x2 | 0x4;
            }
            else if(color == sf::Color::Black)
            {
                collisionMap[i + j * 720] = 0x0;
            }
            else if(color == sf::Color::Red)
            {
                collisionMap[i + j * 720] = 0x1;
            }
            else if(color == sf::Color::Green)
            {
                collisionMap[i + j * 720] = 0x2;
            }
            else if(color == sf::Color::Blue)
            {
                collisionMap[i + j * 720] = 0x4;
            }
            else if(color == sf::Color::Yellow)
            {
                collisionMap[i + j * 720] = 0x1 | 0x2;
                startLocation.x = i;
                startLocation.y = j;
            }
            else if(color == sf::Color::Cyan)
            {
                collisionMap[i + j * 720] = 0x2 | 0x4;
                jetpack = false;
            }
            else if(color == sf::Color::Magenta)
            {
                collisionMap[i + j * 720] = 0x1 | 0x4;
            }
        }
    }

    // start music
    sf::SoundBuffer* osb = &getContext().resourceManager->getSoundBuffer(Sound::OneScreenBeat);
    getContext().sPlayer->play(*osb, true);
}

TheScreen::~TheScreen()
{
}

void TheScreen::draw()
{
    getContext().window->draw(bgSprite);

    getContext().window->draw(player);
}

bool TheScreen::update(sf::Time dt)
{
    playerGravity(dt);
    playerMovement(dt);
    applyVelocity(dt);
    return false;
}

bool TheScreen::handleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        *(getContext().isQuitting) = true;
    }
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W)
    {
        up = true;
    }
    else if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::W)
    {
        up = false;
    }
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
    {
        down = true;
    }
    else if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::S)
    {
        down = false;
    }
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
    {
        left = true;
    }
    else if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::A)
    {
        left = false;
    }
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
    {
        right = true;
    }
    else if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::D)
    {
        right = false;
    }
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
    {
        jump = true;
    }
    else if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
    {
        jump = false;
    }

    return false;
}

void TheScreen::playerMovement(sf::Time dt)
{
    // move left/right
    if(left && !right)
    {
        velocity.x = -PLAYER_SPEED;
    }
    else if(right && !left)
    {
        velocity.x = PLAYER_SPEED;
    }
    else
    {
        velocity.x = 0.0f;
    }

    // jumping
    if(jump && (grounded || jetpack))
    {
        grounded = false;
        velocity.y = -PLAYER_JUMP;
    }
}

void TheScreen::playerGravity(sf::Time dt)
{
    velocity.y += GRAVITY;
}

void TheScreen::applyVelocity(sf::Time dt)
{
    // apply velocity based on collision
    sf::Vector2f prevPosition = player.getPosition();

    // x axis
    player.move(velocity.x * dt.asSeconds(), 0.0f);
    std::unique_ptr<sf::Vector2u[]> edges = GameUtility::getEdges(player);

    char cData;

    for(int i = 0; i < 8; ++i)
    {
        cData = collisionMap[edges[i].x + edges[i].y * 720];
        if(cData == 0x2 || cData == 0x4 || cData == 0x6)
        {
            bgImage = GameUtility::removeColorAtPixel(edges[i], bgImage, collisionMap.get());
            save(cData == 0x2);
            if(cData == 0x6)
            {
                jetpack = true;
            }
        }
        else if(cData == 0)
        {
            player.setPosition(prevPosition);
            break;
        }
    }


    // y axis
    prevPosition = player.getPosition();
    player.move(0.0f, velocity.y * dt.asSeconds());
    edges = GameUtility::getEdges(player);

    for(int i = 0; i < 8; ++i)
    {
        cData = collisionMap[edges[i].x + edges[i].y * 720];
        if(cData == 0x2 || cData == 0x4 || cData == 0x6)
        {
            bgImage = GameUtility::removeColorAtPixel(edges[i], bgImage, collisionMap.get());
            save(cData == 0x2);
            if(cData == 0x6)
            {
                jetpack = true;
            }
        }
        else if(cData == 0)
        {
            if((i == 2 || i == 3) && i != 0 && i != 1 && i != 6 && velocity.x != 0.0f)
            {
                player.move(0.0f, -2.0f);
                edges = GameUtility::getEdges(player);

                bool safeBump = true;
                for(int j = 0; j < 8; ++j)
                {
                    if(collisionMap[edges[j].x + edges[j].y * 720] == 0)
                    {
                        safeBump = false;
                        break;
                    }
                }
                if(safeBump)
                {
                    break;
                }
            }

            player.setPosition(prevPosition);
            grounded = true;
            if(i != 0 && i != 1 && i != 6)
            {
                velocity.y = 0.0f;
            }
            break;
        }
    }
}

void TheScreen::save(bool changeStart)
{
    if(changeStart)
    {
        bgImage.setPixel(startLocation.x, startLocation.y, sf::Color::White);
        startLocation.x = (unsigned int) (player.getPosition().x + 0.5f);
        startLocation.y = (unsigned int) (player.getPosition().y + 0.5f);
        bgImage.setPixel(startLocation.x, startLocation.y, sf::Color::Yellow);
    }

    if(!bgImage.saveToFile("res/save.png"))
    {
        std::cout << "ERROR: Failed to save the game!\n" << std::flush;
        return;
    }

    if(tset.insert(Textures::SAVE).second)
    {
        try
        {
            getContext().resourceManager->loadResources(getNeededResources());
        }
        catch (const std::exception& e)
        {
            tset.erase(Textures::SAVE);
            std::cout << "ERROR: Failed to reload save.png!\n" << std::flush;
            return;
        }
    }
    else
    {
        tset.erase(Textures::SAVE);
        getContext().resourceManager->unloadCheckResources();
        tset.insert(Textures::SAVE);
        getContext().resourceManager->loadResources(getNeededResources());
    }

    sf::Texture* bg = &getContext().resourceManager->getTexture(Textures::SAVE);
    bgSprite.setTexture(*bg);
}
