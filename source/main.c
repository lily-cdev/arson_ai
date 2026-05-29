#include <sublinker.h>
#include <town.h>
#include <engine.h>
#include <ai.h>

struct ai_core Core = {
	.Running = true,
	.Debug = false,
	.Framerate = 24
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

float Roll_Table[6] = { 0.02f, 0.1f, 0.04f, 0.08f, 0.3f, 0.2f };
float Traction_Table[6] = { 0.5f, 0.6f, 0.7f, 0.45f, 0.2f, 0.6f };

int main(int argc, char* argv[]) {
	if (argc != 6) {
		return -1;
	}
	Core.Manual = (argv[3][0] == 'y');
	if (Core.Manual) {
		Reseed_State();
	} else {
		Core.State = (uint32_t)atoi(argv[4]);
	}
	srand((unsigned int)atoi(argv[5]));
	Tick_State();
	Core.Wind_Delta = ((Core.State % 100) - 50) * 0.01f;
	Tick_State();
	Core.Speed_Delta = (Core.State % 200) * 0.00002f;
	Tick_State();
	int Terrain = Core.State % 6;
	Core.Roll_Coefficient = Roll_Table[Terrain];
	Core.Traction_Coefficient = Traction_Table[Terrain];
	Read_Network(argv[2][0] == 'y');
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer("arson ai", px(AI_WIDTH), px(AI_HEIGHT), 0, &Core.Window, &Core.Renderer);
	SDL_SetRenderDrawBlendMode(Core.Renderer, SDL_BLENDMODE_BLEND);
	Engine.Material_Map[M_None] = 0.0f;
	Engine.Material_Map[M_Straw] = 0.1f;
	Engine.Material_Map[M_Clay] = 0.2f;
	Engine.Material_Map[M_Wood] = 0.3f;
	Engine.Material_Map[M_Stone] = 0.4f;
	Engine.Material_Map[M_Flesh] = 0.5f;
	Engine.Material_Map[M_Fire] = 0.6f;
	Load_All();
	Generate_Map();
	Draw_Noise(Terrain);
	Draw_Fire();
	Tick_State();
	Core.Temperature = Core.State % 100;
	Tick_State();
	Core.Epoch = 60.0f;
	Tank.Pos = (Point_f){ Engine.Center.X, Engine.Center.Y };
	bool Benchmarks[3] = { 0 };
	while (Core.Running && Tank.Health > 0) {
		float Start = SDL_GetTicks();
		for (int C1 = 0; C1 < 3; C1++) {
			if (Core.Epoch > (C1 + 1) * 15.0f || Benchmarks[C1]) {
				continue;
			}
			Benchmarks[C1] = true;
			Core.Wind_Angle = fmodf(Core.Wind_Angle + Core.Wind_Delta, M_PI * 2);
		}
		Core.Wind = min(max((Core.Speed_Delta / Core.Framerate) + Core.Wind, 0.0f), 0.5f);
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
		Tank.Max_Dist = max(Tank.Max_Dist, sqrtf(sqr(Tank.Pos.X - Engine.Center.X) + sqr(Tank.Pos.Y -
			Engine.Center.Y)));
		if (Tank.Pos.X <= 0 || Tank.Pos.Y <= 0 || Tank.Pos.X >= AI_WIDTH || Tank.Pos.Y >= AI_HEIGHT) {
			Tank.Health = 0;
		}
		for (int C1 = 0; C1 < AI_WIDTH * 4; C1++) {
			for (int C2 = 0; C2 < AI_HEIGHT * 4; C2++) {
				if (Engine.Flamemap[C1][C2] > 0) {
					Engine.Flamemap[C1][C2] = max(Engine.Flamemap[C1][C2] - (2.0f / Core.Framerate), 0);
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
		Core.Epoch -= 1.0f / Core.Framerate;
		if (Core.Epoch <= 0) {
			Core.Running = false;
		}
		char Carrier[128];
		snprintf(Carrier, sizeof(Carrier), "arson ai %i - %.2f - %s - %i/%i", atoi(argv[1]), Core.Epoch,
			(argv[2][0] == 'y') ? "mutated" : "unmutated", atoi(argv[4]), atoi(argv[5]));
		SDL_SetWindowTitle(Core.Window, Carrier);
		SDL_Delay(max(0, (1000 / Core.Framerate) - Time));
	}
	Save_Network(atoi(argv[1]));
	SDL_DestroyWindow(Core.Window);
	SDL_DestroyRenderer(Core.Renderer);
	Free_All();
	SDL_Quit();
	return 0;
}