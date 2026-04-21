#include <sublinker.h>

void Set_Renderer(uint8_t R, uint8_t G, uint8_t B) {
    SDL_SetRenderDrawColor(Core.Renderer, R, G, B, SDL_ALPHA_OPAQUE);
}

void Clear_Renderer() {
    SDL_SetRenderDrawColor(Core.Renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
}

void Tick_State() {
    Core.State = (Core.State * 2891336453u) + 747796405u;
    Core.State ^= Core.State >> 16;
}

void Reseed_State() {
    struct timespec Time;
    timespec_get(&Time, TIME_UTC);
    Core.State = (uint32_t)(Time.tv_nsec / 1000000);
}

void Rotate_Clockwise(Point Edge, Tile Target[Edge.X][Edge.Y], float Angle) {

}

void Set(Point Edge, Point Offset, Tile Victim[Edge.X][Edge.Y]) {
    for (int C1 = 0; C1 < Edge.X; C1++) {
        for (int C2 = 0; C2 < Edge.Y; C2++) {
            Engine.Tilemap[C1 + Offset.X][C2 + Offset.Y] = Victim[C1][C2];
        }
    }
}