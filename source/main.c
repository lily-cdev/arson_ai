#include <sublinker.h>
#include <town.h>
#include <engine.h>

struct ai_core Core = {
	.Running = true,
	.Debug = true,
	.Manual = true,
	.Framerate = 60,
	.Roll_Coefficient = 0.1
};
struct ai_engine Engine = { };
struct ai_tank Tank = {
	.Torque_Multi = 1,
	.Tread_Radius = 0.21f,
	.Max_Speed = 23.7f,
	.Pos = { AI_WIDTH * 0.5f, AI_HEIGHT * 0.5f },
	.Angle = M_PI * -0.5f,
	.Health = 200.0f,
	.Fuel = 200.0f
};

//0.02c = gravel; 0.1c = dirt; 0.04c = rock; 0.08c soil; 0.3c sand

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer("arson ai", px(AI_WIDTH), px(AI_HEIGHT), 0, &Core.Window, &Core.Renderer);
	SDL_SetRenderDrawBlendMode(Core.Renderer, SDL_BLENDMODE_BLEND);
	Engine.Material_Map[M_None] = 0.0f;
	Engine.Material_Map[M_Straw] = 0.1f;
	Engine.Material_Map[M_Clay] = 0.2f;
	Engine.Material_Map[M_Wood] = 0.3f;
	Engine.Material_Map[M_Stone] = 0.4f;
	Engine.Material_Map[M_Flesh] = 0.5f;
	Reseed_State();
	Load_All();
	Generate_Map();
	Draw_Noise(T_Dirt);
	Draw_Fire();
	Tick_State();
	Core.Temperature = Core.State % 100;
	Tick_State();
	Core.Epoch = (float)(Core.State % 90) + 150.0f;
	Tank.Pos = (Point_f){ Engine.Center.X, Engine.Center.Y };
	while (Core.Running) {
		float Start = SDL_GetTicks();
		Poll_Events();
        Apply_Forces();
		Set_Renderer(255, 255, 255);
		SDL_RenderClear(Core.Renderer);
		Clear_Renderer();
		SDL_RenderTexture(Core.Renderer, Core.Noise, NULL, NULL);
		Render_Town();
		Draw_Tiles();
		Fire_Flamethrower();
		Collide_Tiles();
		Run_Flamethrower();
		for (int C1 = 0; C1 < AI_WIDTH * 4; C1++) {
			for (int C2 = 0; C2 < AI_HEIGHT * 4; C2++) {
				if (Engine.Flamemap[C1][C2] > 0) {
					Engine.Flamemap[C1][C2] -= 2.0f / Core.Framerate;
					Tick_State();
					SDL_FRect Target = { C1 * 4, C2 * 4, 4, 4 };
					SDL_SetTextureAlphaMod(Textures.Fire[Core.State & 7], (uint8_t)(Engine.Flamemap[C1][C2] *
						255.0f));
					SDL_RenderTexture(Core.Renderer, Textures.Fire[Core.State & 7], NULL, &Target);
				}
			}
		}
		SDL_FRect Tank_Rect = { px(Tank.Pos.X), px(Tank.Pos.Y), px(1), px(1) };
		SDL_RenderTextureRotated(Core.Renderer, Textures.Tank, NULL, &Tank_Rect, (Tank.Angle * 180.0f /
			M_PI) + 90, NULL, SDL_FLIP_NONE);
		Draw_Debug();
		Draw_Wind();
		Handle_Sensors();
		SDL_RenderPresent(Core.Renderer);
		float Time = SDL_GetTicks() - Start;
		SDL_Delay(max(0, (1000 / Core.Framerate) - Time));
	}
	SDL_DestroyWindow(Core.Window);
	SDL_DestroyRenderer(Core.Renderer);
	Free_All();
	SDL_Quit();
	return 0;
}