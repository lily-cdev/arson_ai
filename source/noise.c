#include <sublinker.h>

void Draw_Noise(Terrain Floor) {
	SDL_Surface* Noise = SDL_CreateSurface(px(AI_WIDTH), px(AI_HEIGHT), SDL_PIXELFORMAT_RGBA8888);
	SDL_LockSurface(Noise);
    const SDL_PixelFormatDetails* Format = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888);
    uint32_t Table[32] = { };
    SDL_Color Min;
    SDL_Color Max;
    switch (Floor) {
    case T_Dirt:
        Min = (SDL_Color){ 97, 74, 37, SDL_ALPHA_OPAQUE };
        Max = (SDL_Color){ 118, 85, 43, SDL_ALPHA_OPAQUE };
        break;
    case T_Soil:
        Min = (SDL_Color){ 64, 41, 5, SDL_ALPHA_OPAQUE };
        Max = (SDL_Color){ 182, 159, 123, SDL_ALPHA_OPAQUE };
        break;
    default:
        break;
    }
    for (int C1 = 0; C1 < 32; C1++) {
        Table[C1] = SDL_MapRGBA(Format, NULL, Min.r + ((Max.r - Min.r) * C1 / 32), Min.g + ((Max.g - Min.g) *
            C1 / 32), Min.b + ((Max.b - Min.b) * C1 / 32), SDL_ALPHA_OPAQUE);
    }
	uint32_t* Pixels = (uint32_t*)Noise->pixels;
	for (int C1 = 0; C1 < px(AI_WIDTH) * px(AI_HEIGHT); C1++, Pixels++) {
        Tick_State();
        *Pixels = Table[Core.State & 31];
    }
	SDL_UnlockSurface(Noise);
	Core.Noise = SDL_CreateTextureFromSurface(Core.Renderer, Noise);
	SDL_DestroySurface(Noise);
}

void Draw_Fire() {
    const SDL_PixelFormatDetails* Details = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888);
    uint32_t Table[4] = {
        SDL_MapRGBA(Details, NULL, 255, 90, 0, SDL_ALPHA_OPAQUE),
        SDL_MapRGBA(Details, NULL, 255, 154, 0, SDL_ALPHA_OPAQUE),
        SDL_MapRGBA(Details, NULL, 255, 206, 0, SDL_ALPHA_OPAQUE),
        SDL_MapRGBA(Details, NULL, 154, 45, 25, SDL_ALPHA_OPAQUE)
    };
    SDL_Surface* Carriers[8];
    for (int C1 = 0; C1 < 8; C1++) {
        Carriers[C1] = SDL_CreateSurface(4, 4, SDL_PIXELFORMAT_RGBA8888);
        SDL_LockSurface(Carriers[C1]);
        uint32_t* Pixels = (uint32_t*)Carriers[C1]->pixels;
        for (int C2 = 0; C2 < sqr(4); C2++, Pixels++) {
            Tick_State();
            *Pixels = Table[Core.State & 3];
        }
        SDL_UnlockSurface(Carriers[C1]);
        Textures.Fire[C1] = SDL_CreateTextureFromSurface(Core.Renderer, Carriers[C1]);
        SDL_DestroySurface(Carriers[C1]);
    }
}