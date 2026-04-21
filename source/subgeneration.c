#include <town.h>
#include <engine.h>

#define E (Tile){ }
#define S (Tile){ .Collider = true, .Ignition_Pt = 750, .Material = M_Wood }
#define H (Tile){ .Collider = true, .Ignition_Pt = 500, .Material = M_Straw }

void Load_Collision(const char* Path, Point Size, Tile Yield[Size.X][Size.Y], Tile First, Tile Second) {
	SDL_Surface* Carrier = Load_BMP(Path);
	SDL_LockSurface(Carrier);
	uint32_t* Data = (uint32_t*)Carrier->pixels;
	const SDL_PixelFormatDetails* Details = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888);
	for (int Y = 0; Y < Size.Y; Y++) {
		for (int X = 0; X < Size.X; X++) {
			SDL_Color Pixel;
			SDL_GetRGBA(Data[(Y * Size.X) + X], Details, NULL, &Pixel.r, &Pixel.g, &Pixel.b, &Pixel.a);
			if (Pixel.r == 255) {
				Yield[X][Y] = E;
			} else if (Pixel.g == 255) {
				Yield[X][Y] = First;
			} else if (Pixel.b == 255) {
				Yield[X][Y] = Second;
			}
		}
	}
	SDL_UnlockSurface(Carrier);
	SDL_DestroySurface(Carrier);
}

void Set_Hut(Point Pos) {
	Tile Yield[16][16] = { };
	Load_Collision("assets/hut_4x4_collision.bmp", (Point){ 16, 16 }, Yield, S, E);
	Set((Point){ 16, 16 }, (Point){ Pos.X * 4, Pos.Y * 4 }, Yield);
};

void Set_LHutV(Point Pos) {
	Tile Yield[20][28] = { };
	Load_Collision("assets/hut_5x7_collision.bmp", (Point){ 20, 28 }, Yield, S, E);
	if (Town.Rotations[pt(Pos)] == D_Right) {
		Tile Applied_Yield[20][28] = { };
		for (int C1 = 0; C1 < 20; C1++) {
			for (int C2 = 0; C2 < 28; C2++) {
				Applied_Yield[C1][C2] = Yield[C1][27 - C2];
			}
		}
		Set((Point){ 20, 28 }, (Point){ Pos.X * 4, Pos.Y * 4 }, Applied_Yield);
		return;
	}
	Set((Point){ 20, 28 }, (Point){ Pos.X * 4, Pos.Y * 4 }, Yield);
};

void Set_LHutH(Point Pos) {
	Tile Yield[28][20] = { };
	Load_Collision("assets/hut_7x5_collision.bmp", (Point){ 28, 20 }, Yield, S, E);
	if (Town.Rotations[pt(Pos)] == D_Down) {
		Tile Applied_Yield[28][20] = { };
		for (int C1 = 0; C1 < 28; C1++) {
			memcpy(Applied_Yield[C1], Yield[27 - C1], sizeof(Tile) * 20);
		}
		Set((Point){ 28, 20 }, (Point){ Pos.X * 4, Pos.Y * 4 }, Applied_Yield);
		return;
	}
	Set((Point){ 28, 20 }, (Point){ Pos.X * 4, Pos.Y * 4 }, Yield);
};

void Set_BarnV(Point Pos) {
	Tile Yield[24][36] = { };
	Load_Collision("assets/barn_6x9_collision.bmp", (Point){ 24, 36 }, Yield, S, H);
	Set((Point){ 24, 36 }, (Point){ Pos.X * 4, Pos.Y * 4 }, Yield);
}

void Set_BarnH(Point Pos) {
	Tile Yield[36][24] = { };
	Load_Collision("assets/barn_9x6_collision.bmp", (Point){ 36, 24 }, Yield, S, H);
	Set((Point){ 36, 24 }, (Point){ Pos.X * 4, Pos.Y * 4 }, Yield);
}
#undef S
#undef E
#undef H