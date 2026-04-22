#include <sublinker.h>

void Run_Flamethrower() {
	if (Tank.Firing > 0) {
		Tank.Firing -= 1.0f / Core.Framerate;
	} else {
		return;
	}
	for (int C1 = 0; C1 < AI_WIDTH * 4; C1++) {
		for (int C2 = 0; C2 < AI_HEIGHT * 4; C2++) {
			SDL_FRect Target = { C1 * 4, C2 * 4, 4, 4 };
			if (!Collide_Tri(Target, Tank.Heat_Cone)) {
				continue;
			}
			Point_f Center = { Target.x + 2, Target.y + 2 };
			Center.X *= 0.0625f;
			Center.Y *= 0.0625f;
			int Distance = sqrtf(sqr((Center.X - Tank.Pos.X) * 4) + sqr((Center.Y - Tank.Pos.Y) * 4));
			float Angle = atan2f(Center.Y - Tank.Pos.Y, Center.X - Tank.Pos.X);
			bool Skip = false;
			for (int C3 = 0; C3 < Distance && !Skip; C3++) {
				Point_f Initial = { Tank.Pos.X * 4.0f, Tank.Pos.Y * 4.0f };
				Initial.X += cosf(Angle) * C3;
				Initial.Y += sinf(Angle) * C3;
				if (Initial.X < 0 || Initial.X >= AI_WIDTH * 4 || Initial.Y < 0 || Initial.Y >= AI_HEIGHT *
					4) {
					continue;
				}
				if (Engine.Tilemap[(int)floorf(Initial.X)][(int)floorf(Initial.Y)].Collider) {
					Skip = true;
				}
			}
			if (Skip) {
				continue;
			}
			if (Collide_Tri(Target, Tank.Heat_Cone) && !Engine.Tilemap[C1][C2].Collider) {
				Engine.Flamemap[C1][C2] = 1.0f;
			}
			if (!Engine.Tilemap[C1][C2].Triggered || Engine.Tilemap[C1][C2].Alight || !Engine.Tilemap[C1][
				C2].Flammable) {
				continue;
			}
			Tick_State();
			Engine.Tilemap[C1][C2].Heat += min(3700, Engine.Tilemap[C1][C2].Heat + 100 - (Core.State % 50));
			if (Engine.Tilemap[C1][C2].Heat >= Engine.Tilemap[C1][C2].Ignition_Pt) {
				Engine.Tilemap[C1][C2].Alight = true;
				Engine.Tilemap[C1][C2].Time = 7.0f;
			}
		}
	}
}

void Poll_Events() {
	SDL_Event Event;
	while (SDL_PollEvent(&Event)) {
		if (Event.type == SDL_EVENT_QUIT) {
			Core.Running = false;
		}
		if (Event.type == SDL_EVENT_KEY_DOWN) {
			if (Event.key.key == SDLK_K) {
				Core.Debug = !Core.Debug;
			}
			if (Core.Manual) {
				if (Event.key.key == SDLK_Q) {
					Tank.Tread_Force[0] = min(Tank.Tread_Force[0] + 0.1f, 1.0f);
				} else if (Event.key.key == SDLK_A) {
					Tank.Tread_Force[0] = max(Tank.Tread_Force[0] - 0.1f, -1.0f);
				}
				if (Event.key.key == SDLK_E) {
					Tank.Tread_Force[1] = min(Tank.Tread_Force[1] + 0.1f, 1.0f);
				} else if (Event.key.key == SDLK_D) {
					Tank.Tread_Force[1] = max(Tank.Tread_Force[1] - 0.1f, -1.0f);
				}
				if (Event.key.key == SDLK_W) {
					if (Tank.Firing <= 0) {
						Tank.Firing = 1.0f;
					}
				}
			}
		}
	}
}