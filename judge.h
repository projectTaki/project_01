#pragma once

class Judge
{
public:
    static bool  rectHitCheck(VECTOR2 pos1, VECTOR2 size1, 
                              VECTOR2 pos2, VECTOR2 size2);
    static void titleJudge();
    static void gameJudge();
    static void overJudge();
    static void clearJudge();
};