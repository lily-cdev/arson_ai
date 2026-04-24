#pragma once
#include <config.h>

#define px(Victim) ((Victim) * AI_TILESIZE)
#define pt(Victim) (Victim.X)][(Victim.Y)
#define evn(Victim) (((Victim) & 1) == 0)
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
#define sgn(Victim) ((Victim) > 0 ? 1 : ((Victim) < 0 ? -1 : 0))
#define sqr(Victim) ((Victim) * (Victim))

struct ai_core {
    bool Running;
    bool Debug;
    bool Manual;
    int Framerate;
    float Wind;
    float Wind_Angle;
    float Wind_Tick;
    float Roll_Coefficient;
    uint32_t State;
    SDL_Texture* Noise;
    SDL_Renderer* Renderer;
    SDL_Window* Window;
    float Epoch;
    float Time;
    int Temperature;
};

struct ai_textures {
    SDL_Texture* Tank;
    SDL_Texture* Path[5];
    SDL_Texture* Hut;
    SDL_Texture* Barn;
    SDL_Texture* Well;
    SDL_Texture* L_Hut;
    SDL_Texture* C_Hut;
    SDL_Texture* Fire[8];
};

struct ai_engine {
    Point Center;
    float Material_Map[6];
    Tile Tilemap[AI_WIDTH * 4][AI_HEIGHT * 4];
    float Flamemap[AI_WIDTH * 4][AI_HEIGHT * 4];
};

struct ai_tank {
    float Mass;
    const float Torque_Multi;
    const float Tread_Radius;
    float Tread_Force[2];
    float Max_Speed;
    Point_f Pos;
    float Angle;
    Point_f Heat_Cone[3];
    Point_f Min_Cone;
    Point_f Max_Cone;
    float Firing;
    float Health;
    float Fuel;
    Input_Group Sensors;
};

void Set_Renderer(uint8_t R, uint8_t G, uint8_t B);
void Clear_Renderer();
void Tick_State();
void Reseed_State();
void Load_All();
void Free_All();
void Poll_Events();
void Draw_Noise(Terrain Floor);
void Draw_Fire();
void Rotate_Clockwise(Point Edge, Tile Target[Edge.X][Edge.Y], float Angle);
void Set(Point Edge, Point Offset, Tile Victim[Edge.X][Edge.Y]);
void Run_Flamethrower();
bool Collide_Tri(SDL_FRect A, Point_f B[3]);
void Handle_Sensors();
SDL_Surface* Load_BMP(const char* Path);

extern struct ai_core Core;
extern struct ai_textures Textures;
extern struct ai_engine Engine;
extern struct ai_tank Tank;