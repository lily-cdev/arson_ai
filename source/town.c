#include <town.h>

struct ai_town Town = { };

//note: each tile is one square meter

bool Check_Building(int X, int Y) {
	if (Town.Map[X - 1][Y - 3] == B_Hut && Town.Rotations[X - 1][Y - 3] == D_Down) {
		return true;
	}
	if (Town.Map[X - 2][Y] == B_Hut && Town.Rotations[X - 2][Y] == D_Up) {
		return true;
	}
	if (Town.Map[X][Y - 1] == B_Hut && Town.Rotations[X][Y - 1] == D_Left) {
		return true;
	}
	if (Town.Map[X - 3][Y - 2] == B_Hut && Town.Rotations[X - 3][Y - 2] == D_Right) {
		return true;
	}
	if (Town.Map[X][Y - 1] == B_Barn && Town.Rotations[X][Y - 1] == D_Left) {
		return true;
	}
	if (Town.Map[X][Y - 7] == B_Barn && Town.Rotations[X][Y - 7] == D_Left) {
		return true;
	}
	if (Town.Map[X - 5][Y - 1] == B_Barn && Town.Rotations[X - 5][Y - 1] == D_Right) {
		return true;
	}
	if (Town.Map[X - 5][Y - 7] == B_Barn && Town.Rotations[X - 5][Y - 7] == D_Right) {
		return true;
	}
	if (Town.Map[X - 1][Y] == B_Barn && Town.Rotations[X - 1][Y] == D_Up) {
		return true;
	}
	if (Town.Map[X - 7][Y] == B_Barn && Town.Rotations[X - 7][Y] == D_Up) {
		return true;
	}
	if (Town.Map[X - 1][Y - 5] == B_Barn && Town.Rotations[X - 1][Y - 5] == D_Down) {
		return true;
	}
	if (Town.Map[X - 7][Y - 5] == B_Barn && Town.Rotations[X - 7][Y - 5] == D_Down) {
		return true;
	}
	if (Town.Map[X - 4][Y - 4] == B_LHut && Town.Rotations[X - 4][Y - 4] == D_Down) {
		return true;
	}
	if (Town.Map[X - 2][Y] == B_LHut && Town.Rotations[X - 2][Y] == D_Up) {
		return true;
	}
	if (Town.Map[X][Y - 4] == B_LHut && Town.Rotations[X][Y - 4] == D_Left) {
		return true;
	}
	if (Town.Map[X - 4][Y - 2] == B_LHut && Town.Rotations[X - 4][Y - 2] == D_Right) {
		return true;
	}
	return false;
}

#define check_path(XO, YO) (Pos.X + (XO) >= 0 && Pos.X + (XO) < AI_WIDTH && Pos.Y + (YO) >= 0 && Pos.Y + \
	(YO) < AI_HEIGHT && (Town.Map[Pos.X + (XO)][Pos.Y + (YO)] == B_Path || Check_Building(Pos.X + (XO), \
	Pos.Y + (YO))))
void Render_Path(Point Pos) {
	Clear_Renderer();
	SDL_FRect Tile = { px(Pos.X), px(Pos.Y), px(1), px(1) };
	if (check_path(-1, 0) && check_path(1, 0) && check_path(0, -1) && check_path(0, 1)) {
		SDL_RenderTexture(Core.Renderer, Textures.Path[0], NULL, &Tile);
		return;
	}
	if (check_path(-1, 0) && check_path(1, 0) && check_path(0, 1)) {
		SDL_RenderTexture(Core.Renderer, Textures.Path[2], NULL, &Tile);
		return;
	}
	if (check_path(-1, 0) && check_path(1, 0) && check_path(0, -1)) {
		SDL_RenderTextureRotated(Core.Renderer, Textures.Path[2], NULL, &Tile, 180, NULL, SDL_FLIP_NONE);
		return;
	}
	if (check_path(-1, 0) && check_path(0, 1) && check_path(0, -1)) {
		SDL_RenderTextureRotated(Core.Renderer, Textures.Path[2], NULL, &Tile, 90, NULL, SDL_FLIP_NONE);
		return;
	}
	if (check_path(1, 0) && check_path(0, 1) && check_path(0, -1)) {
		SDL_RenderTextureRotated(Core.Renderer, Textures.Path[2], NULL, &Tile, 270, NULL, SDL_FLIP_NONE);
		return;
	}
	if (check_path(-1, 0) && check_path(1, 0)) {
		SDL_RenderTextureRotated(Core.Renderer, Textures.Path[1], NULL, &Tile, 90, NULL, SDL_FLIP_NONE);
		return;
	}
	if (check_path(0, -1) && check_path(0, 1)) {
		SDL_RenderTexture(Core.Renderer, Textures.Path[1], NULL, &Tile);
		return;
	}
	if (check_path(-1, 0) && check_path(0, -1)) {
		SDL_RenderTextureRotated(Core.Renderer, Textures.Path[4], NULL, &Tile, 180, NULL, SDL_FLIP_NONE);
		return;
	}
	if (check_path(-1, 0) && check_path(0, 1)) {
		SDL_RenderTextureRotated(Core.Renderer, Textures.Path[4], NULL, &Tile, 90, NULL, SDL_FLIP_NONE);
		return;
	}
	if (check_path(1, 0) && check_path(0, -1)) {
		SDL_RenderTextureRotated(Core.Renderer, Textures.Path[4], NULL, &Tile, 270, NULL, SDL_FLIP_NONE);
		return;
	}
	if (check_path(1, 0) && check_path(0, 1)) {
		SDL_RenderTexture(Core.Renderer, Textures.Path[4], NULL, &Tile);
		return;
	}
	if (check_path(-1, 0)) {
		SDL_RenderTextureRotated(Core.Renderer, Textures.Path[3], NULL, &Tile, 90, NULL, SDL_FLIP_NONE);
		return;
	}
	if (check_path(1, 0)) {
		SDL_RenderTextureRotated(Core.Renderer, Textures.Path[3], NULL, &Tile, 270, NULL, SDL_FLIP_NONE);
		return;
	}
	if (check_path(0, 1)) {
		SDL_RenderTexture(Core.Renderer, Textures.Path[3], NULL, &Tile);
		return;
	}
	if (check_path(0, -1)) {
		SDL_RenderTextureRotated(Core.Renderer, Textures.Path[3], NULL, &Tile, 180, NULL, SDL_FLIP_NONE);
		return;
	}
}
#undef check_path

void Render_Town() {
	for (int C1 = 0; C1 < AI_WIDTH; C1++) {
		for (int C2 = 0; C2 < AI_HEIGHT; C2++) {
			switch (Town.Map[C1][C2]) {
				case B_Tree:
					Set_Renderer(0, 255, 0);
					SDL_FRect Tree = { px(C1), px(C2), px(1), px(1) };
					SDL_RenderFillRect(Core.Renderer, &Tree);
					break;
				case B_Well:
					SDL_FRect Well = { px(C1), px(C2), px(1), px(1) };
					SDL_RenderTexture(Core.Renderer, Textures.Well, NULL, &Well);
					break;
				case B_Path:
					Render_Path((Point){ .X = C1, .Y = C2 });
					break;
				case B_Hut:
					SDL_FRect Hut = { px(C1), px(C2), px(4), px(4) };
					SDL_RenderTextureRotated(Core.Renderer, Textures.Hut, NULL, &Hut, Town.Rotations[C1][
						C2] * 90, NULL, SDL_FLIP_NONE);
					break;
				case B_Barn:				
					SDL_FRect Barn = { px(C1), px(C2), px(6), px(9) };
					if (!evn(Town.Rotations[C1][C2])) {
						Barn.x += px(1.5);
						Barn.y -= px(1.5);
					}
					SDL_RenderTextureRotated(Core.Renderer, Textures.Barn, NULL, &Barn, Town.Rotations[C1][
						C2] * 90, NULL, SDL_FLIP_NONE);
					break;
				case B_LHut:
					SDL_FRect LHut = { px(C1), px(C2), px(5), px(7) };
					if (!evn(Town.Rotations[C1][C2])) {
						LHut.x += px(1);
						LHut.y -= px(1);
					}
					SDL_RenderTextureRotated(Core.Renderer, Textures.L_Hut, NULL, &LHut, Town.Rotations[C1][
						C2] * 90, NULL, SDL_FLIP_NONE);
					break;
				case B_Clayhut:
					SDL_FRect Clayhut = { px(C1), px(C2), px(5), px(5) };
					SDL_RenderTextureRotated(Core.Renderer, Textures.C_Hut, NULL, &Clayhut, Town.Rotations[
						C1][C2] * 90, NULL, SDL_FLIP_NONE);
					break;
				default:
					break;
			}
			Clear_Renderer();
		}
	}
	for (int C1 = 0; C1 < AI_WIDTH - 1; C1++) {
		for (int C2 = 0; C2 < AI_HEIGHT - 1; C2++) {
			if (Town.Map[C1][C2] == B_Path && Town.Map[C1][C2 + 1] == B_Path && Town.Map[C1 + 1][C2] ==
				B_Path && Town.Map[C1 + 1][C2 + 1] == B_Path) {
				Set_Renderer(118, 85, 43);
				SDL_FRect Filler = { px(C1 + 0.5f), px(C2 + 0.5f), px(1), px(1) };
				SDL_RenderFillRect(Core.Renderer, &Filler);
				Clear_Renderer();
			}
		}
	}
}