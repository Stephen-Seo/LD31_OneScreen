
#ifndef BATTLE_HPP
#define BATTLE_HPP

#define TEXT_SPEED 0.055f
#define HPG_SPEED 0.05f

#include <engine/state.hpp>

#include <random>
#include <string>

class Battle : public State
{
public:
    enum BattleState
    {
        PRE_DIALOG,
        MAIN_MENU,
        SPECIAL_MENU,
        ATTACKING,
        DEFENDING,
        POST_TURN
    };

    enum PlayerChoice
    {
        NONE,
        ATTACK,
        S_ATTACK,
        HEAL
    };

    Battle(StateStack& stack, Context context);
    ~Battle();

    void draw();
    bool update(sf::Time dt);
    bool handleEvent(const sf::Event& event);

private:
    bool isInBattle;
    std::mt19937 gen;
    sf::RectangleShape bgCover;
    unsigned char currentBoss;

    sf::Sprite attack;
    sf::Sprite special;
    sf::Sprite pStrike;
    sf::Sprite heal;
    sf::Sprite hsBar;

    sf::RectangleShape hp;
    sf::RectangleShape hpGhost;
    sf::RectangleShape sp;
    float hpgSpeed;

    BattleState battleState;

    bool mousePressed;

    sf::Text dialogText;
    sf::RectangleShape dialogBG;
    std::string text;
    int subIndex;
    float textSpeed;
    int textSfxID;

    PlayerChoice choice;

    sf::Sprite bossSprite;

    void checkDialogText(sf::Time dt);
    void changeState(BattleState newState, std::string newText = "");
    void drawStatus();
    void checkHPSP(sf::Time dt);
    std::string getAttackingText();
    std::string getDefendingText();
    std::string getPostTurnText();
};

#endif
