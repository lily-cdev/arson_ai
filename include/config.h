#pragma once
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
#define AI_TILESIZE 16
#define AI_WIDTH 48
#define AI_HEIGHT 36
#define AI_SENSORS 48

typedef struct {
    int X;
    int Y;
} Point;

typedef struct {
    float X;
    float Y;
} Point_f;

typedef enum {
    D_Left,
    D_Up,
    D_Right,
    D_Down
} Dir;

typedef enum {
    T_Dirt,
    T_Rock,
    T_Soil,
    T_Gravel,
    T_Sand
} Terrain;

enum Material {
    M_None,
    M_Straw,
    M_Clay,
    M_Wood,
    M_Stone,
    M_Flesh
};

typedef struct {
    bool Collider;
    int Ignition_Pt;
    bool Crushable;
    bool Triggered;
    bool Alight;
    bool Flammable;
    int Heat;
    float Time;
    int Material;
} Tile;

typedef struct {
    float Angle;
    float Depth;
    float Material;
    float Heat;
} Raycast;

typedef struct {
    float Temperature;
    float Wind;
    float Wind_Angle;
    float Firing;
    float Track1_Force;
    float Track2_Force;
    Raycast Sensors[AI_SENSORS];
    float Bias;
    float Health;
    float Fuel;
} Input_Group;