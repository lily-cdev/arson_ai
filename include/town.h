#pragma once
#include <sublinker.h>

typedef enum {
    B_Invalid = -1,
    B_None,
    B_Hut,
    B_Path,
    B_Tree,
    B_Well,
    B_Barn,
    B_LHut,
    B_Clayhut
} Buildings;

struct ai_town {
    Buildings Map[AI_WIDTH][AI_HEIGHT];
    Dir Rotations[AI_WIDTH][AI_HEIGHT];
};

void Generate_Map();
void Render_Town();
void Set_Hut(Point Pos);
void Set_LHutV(Point Pos);
void Set_LHutH(Point Pos);
void Set_BarnV(Point Pos);
void Set_BarnH(Point Pos);

extern struct ai_town Town;