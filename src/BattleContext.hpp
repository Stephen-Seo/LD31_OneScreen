
#ifndef BATTLE_CONTEXT_HPP
#define BATTLE_CONTEXT_HPP

struct BattleContext
{
    unsigned char bossId = 0;

    int pHP = 1000;
    int pHPM = 1000;
    int pSP = 50;
    int pSPM = 50;
    int pATK = 20;
    int pDEF = 20;

    int bHP;
    int bSP;
    int bATK;
    int bDEF;

    bool battleEnded = false;
};

#endif
