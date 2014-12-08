
#include <Battle.hpp>

#include <ctime>

#include <engine/resourceManager.hpp>
#include <engine/soundPlayer.hpp>
#include <BattleContext.hpp>
#include <GameUtility.hpp>

#ifndef N_DEBUG
    #include <iostream>
#endif

Battle::Battle(StateStack& stack, Context context) :
State(stack, context),
isInBattle(false),
gen(std::random_device()() + std::time(nullptr)),
bgCover(sf::Vector2f(720.0f,480.0f)),
hp(sf::Vector2f(248.0f, 4.0f)),
hpGhost(sf::Vector2f(248.0f, 4.0f)),
sp(sf::Vector2f(248.0f, 4.0f)),
hpgSpeed(0.0f),
battleState(PRE_DIALOG),
mousePressed(false),
dialogText(),
dialogBG(sf::Vector2f(360, 100)),
text(""),
subIndex(0),
textSpeed(0.0f)
{
    // register resources for this state
    tset.insert(Textures::BGUI);
    fset.insert(Fonts::DJVSMono);
    sset.insert(Sound::Text);

    // load resources
    getContext().resourceManager->loadResources(getNeededResources());

    // setup sprites
    sf::Texture* battleGui = &getContext().resourceManager->getTexture(Textures::BGUI);

    attack.setTexture(*battleGui);
    special.setTexture(*battleGui);
    pStrike.setTexture(*battleGui);
    heal.setTexture(*battleGui);
    hsBar.setTexture(*battleGui);

    attack.setTextureRect(sf::IntRect(0, 0, 144, 96));
    special.setTextureRect(sf::IntRect(144, 0, 144, 96));
    pStrike.setTextureRect(sf::IntRect(288, 0, 144, 96));
    heal.setTextureRect(sf::IntRect(432, 0, 144, 96));
    hsBar.setTextureRect(sf::IntRect(0, 96, 288, 24));

    attack.setPosition(150.0f, 10.0f);
    special.setPosition(390.0f, 10.0f);
    pStrike.setPosition(150.0f, 10.0f);
    heal.setPosition(390.0f, 10.0f);

    bgCover.setFillColor(sf::Color(0, 0, 0, 100));

    // setup dialog text
    sf::Font* dvFont = &getContext().resourceManager->getFont(Fonts::DJVSMono);

    dialogText.setFont(*dvFont);
    dialogText.setCharacterSize(18);
    dialogText.setColor(sf::Color::Black);

    dialogText.setPosition(200.0f, 30.0f);

    // setup dialog bg
    dialogBG.setFillColor(sf::Color::White);
    dialogBG.setPosition(180.0f, 25.0f);

    // setup health/special bar
    hsBar.setPosition(216.0f, 456.0f);

    hp.setPosition(238.0f, 461.0f);
    hp.setFillColor(sf::Color::Green);

    hpGhost.setPosition(238.0f, 461.0f);
    hpGhost.setFillColor(sf::Color::Red);

    sp.setPosition(238.0f, 472.0f);
    sp.setFillColor(sf::Color::Cyan);

    // setup boss info
    currentBoss = getContext().battleContext->bossId;
    getContext().battleContext->bossId += 1;

    getContext().battleContext->bHP = 100 + currentBoss * 35;
    getContext().battleContext->bSP = 20;
    getContext().battleContext->bATK = 80;
    getContext().battleContext->bDEF = 5;
    text = "A Lamp appears to fight!";

    // setup boss sprite
    bossSprite.setTexture(getContext().resourceManager->getTexture(Textures::LAMP));
    sf::FloatRect bossBounds = bossSprite.getLocalBounds();
    bossSprite.setOrigin(bossBounds.width / 2.0f, bossBounds.height / 2.0f);
    bossSprite.setPosition(360.0f, 240.0f);

    // setup text sfx
    sf::SoundBuffer* textSfx = &getContext().resourceManager->getSoundBuffer(Sound::Text);
    textSfxID = getContext().sPlayer->create(*textSfx);
}

Battle::~Battle()
{
}

void Battle::draw()
{
    getContext().window->draw(bgCover);

    getContext().window->draw(bossSprite);

    switch(battleState)
    {
    case PRE_DIALOG:
    case ATTACKING:
    case DEFENDING:
    case POST_TURN:
        getContext().window->draw(dialogBG);
        getContext().window->draw(dialogText);
        break;
    case MAIN_MENU:
        getContext().window->draw(attack);
        getContext().window->draw(special);
        break;
    case SPECIAL_MENU:
        getContext().window->draw(pStrike);
        getContext().window->draw(heal);
        break;
    default:
        break;
    }

    drawStatus();
}

bool Battle::update(sf::Time dt)
{
    checkDialogText(dt);
    checkHPSP(dt);
    if(subIndex == text.size() && mousePressed)
    {
        mousePressed = false;
        switch(battleState)
        {
        case PRE_DIALOG:
            changeState(MAIN_MENU);
            choice = NONE;
            break;
        case ATTACKING:
            changeState(DEFENDING, getDefendingText());
            break;
        case DEFENDING:
            changeState(POST_TURN, getPostTurnText());
            break;
        case POST_TURN:
            if(getContext().battleContext->bHP <= 0 ||
               (getContext().battleContext->pHP <= 0 && hpGhost.getSize().x <= 0.3f))
            {
                requestStackPop();
                getContext().battleContext->battleEnded = true;
            }
            else
            {
                changeState(MAIN_MENU);
            }
            break;
        default:
            break;
        }
    }
    return false;
}

bool Battle::handleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        mousePressed = true;

        switch(battleState)
        {
        case MAIN_MENU:
            if(GameUtility::isWithin(event.mouseButton.x, event.mouseButton.y, attack.getGlobalBounds()))
            {
                choice = ATTACK;
                changeState(ATTACKING, getAttackingText());
                mousePressed = false;
            }
            else if(GameUtility::isWithin(event.mouseButton.x, event.mouseButton.y, special.getGlobalBounds()))
            {
                changeState(SPECIAL_MENU);
                mousePressed = false;
            }
            break;
        case SPECIAL_MENU:
            if(GameUtility::isWithin(event.mouseButton.x, event.mouseButton.y, pStrike.getGlobalBounds()))
            {
                choice = S_ATTACK;
                changeState(ATTACKING, getAttackingText());
                mousePressed = false;
            }
            else if(GameUtility::isWithin(event.mouseButton.x, event.mouseButton.y, heal.getGlobalBounds()))
            {
                choice = HEAL;
                changeState(ATTACKING, getAttackingText());
                mousePressed = false;
            }
            break;
        default:
            break;
        }
    }
    else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        mousePressed = false;
    }

    return true;
}

void Battle::checkDialogText(sf::Time dt)
{
    textSpeed += dt.asSeconds();
    if(mousePressed && subIndex < text.size())
    {
        subIndex = text.size();
        dialogText.setString(text);
        mousePressed = false;
    }
    else if(subIndex < text.size() && textSpeed >= TEXT_SPEED)
    {
        dialogText.setString(text.substr(0, ++subIndex));
        textSpeed = 0.0f;
        getContext().sPlayer->play(textSfxID);
    }
}

void Battle::changeState(BattleState newState, std::string newText)
{
    battleState = newState;
    dialogText.setString("");
    text = newText;
    subIndex = 0;
    textSpeed = 0.0f;
}

void Battle::drawStatus()
{
    getContext().window->draw(hsBar);

    getContext().window->draw(hpGhost);
    getContext().window->draw(hp);
    getContext().window->draw(sp);
}

void Battle::checkHPSP(sf::Time dt)
{
    float pHP = (float)getContext().battleContext->pHP;
    if(pHP < 0.0f)
    {
        pHP = 0.0f;
    }
    hp.setSize(sf::Vector2f(pHP / (float)getContext().battleContext->pHPM * 248.0f, 4.0f));

    hpgSpeed += dt.asSeconds();
    if(hp.getSize().x < hpGhost.getSize().x && hpgSpeed >= HPG_SPEED)
    {
        hpGhost.setSize(hpGhost.getSize() - sf::Vector2f(1.0f, 0.0f));
        hpgSpeed = 0.0f;
    }
    else if(hp.getSize().x > hpGhost.getSize().x)
    {
        hpGhost.setSize(hp.getSize());
    }

    float pSP = (float)getContext().battleContext->pSP;
    if(pSP < 0.0f)
    {
        pSP = 0.0f;
    }
    sp.setSize(sf::Vector2f(pSP / (float)getContext().battleContext->pSPM * 248.0f, 4.0f));
}

std::string Battle::getAttackingText()
{
    std::string rText;

    switch(choice)
    {
    case ATTACK:
    {
        std::uniform_int_distribution<unsigned int> dis(0,9);
        std::uniform_int_distribution<int> var(-5,8);
        switch(dis(gen))
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        {
            int damage = getContext().battleContext->pATK + var(gen);
            rText = "You swing your arms and\ndeal " + std::to_string(damage) + " damage!!";
            getContext().battleContext->bHP -= damage;
            break;
        }
        case 7:
        case 8:
        {
            int damage = (getContext().battleContext->pATK + var(gen)) * 2;
            rText = "Your flailing arms turn\ninto a flurry of fists!\nYou deal " + std::to_string(damage) + " damage!!";
            getContext().battleContext->bHP -= damage;
            break;
        }
        case 9:
            rText = "Your flailing arms hit nothing!";
            break;
        }
        break;
    }
    case S_ATTACK:
    {
        if(getContext().battleContext->pSP >= 10)
        {
            int damage = getContext().battleContext->pATK * 1.5f;
            rText = "You precisely swing your arms\nand deal " + std::to_string(damage) + " damage!!";
            getContext().battleContext->bHP -= damage;
            getContext().battleContext->pSP -= 10;
        }
        else
        {
            int damage = getContext().battleContext->pATK * 0.8f;
            rText = "You don't have enough Special\nPoints! You swing your arms\nfeebly!" + std::to_string(damage) + " damage was dealt!";
            getContext().battleContext->bHP -= damage;
        }
        break;
    }
    case HEAL:
    {
        if(getContext().battleContext->pSP >= 20)
        {
            rText = "With superb concentration,\nyou meditate and recover!";
            getContext().battleContext->pHP += getContext().battleContext->pHPM * 0.35f;
            if(getContext().battleContext->pHP > getContext().battleContext->pHPM)
            {
                getContext().battleContext->pHP = getContext().battleContext->pHPM;
            }
            getContext().battleContext->pSP -= 20;
        }
        else
        {
            rText = "You don't have enough Special\nPoints! Instead, you think\nhappy thoughts.";
            getContext().battleContext->pHP += getContext().battleContext->pHPM * 0.07f;
        }
    }
    default:
        break;
    }

    return rText;
}

std::string Battle::getDefendingText()
{
    std::string rText;

    std::uniform_int_distribution<unsigned int> dis(0, 4);
    std::uniform_int_distribution<int> adis(-7, 3);
    switch(dis(gen))
    {
    case 0:
    case 1:
        rText = "The Lamp flashes at you!!";
        getContext().battleContext->pHP -= getContext().battleContext->bATK + adis(gen);
        break;
    case 2:
        rText = "The Lamp sits there!!";
        break;
    case 3:
        rText = "The Lamp turns off, only to\nturn back on again!!";
        break;
    case 4:
        if(getContext().battleContext->bSP > 8)
        {
            rText = "The Lamp flashes even stronger!!";
            getContext().battleContext->pHP -= getContext().battleContext->bATK + adis(gen) + 20;
            getContext().battleContext->bSP -= 8;
        }
        else
        {
            rText = "The Lamp tries to increase brightness,\nbut fails!!";
        }
        break;
    }

    return rText;
}

std::string Battle::getPostTurnText()
{
    std::string rText = "";

    if(getContext().battleContext->pHP <= 0 && hpGhost.getSize().x <= 0.3f)
    {
        rText = rText + "Oh noes, you has dead!";
    }
    else if(getContext().battleContext->bHP <= 0)
    {
        rText = rText + "The Lamp broke it's bulb!!";
    }
    else
    {
        rText = "The battle continues!";
    }

    return rText;
}
