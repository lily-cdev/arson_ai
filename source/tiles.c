#include <engine.h>

void Collide_Tiles() {
	for (int C1 = 0; C1 < AI_WIDTH * 4; C1++) {
		for (int C2 = 0; C2 < AI_HEIGHT * 4; C2++) {
			if (Engine.Tilemap[C1][C2].Collider) {
				SDL_FRect Target = { C1 * 4, C2 * 4, 4, 4 };
				Engine.Tilemap[C1][C2].Triggered = false;
				bool Left_Unshielded = C1 <= 0 || !Engine.Tilemap[C1 - 1][C2].Collider;
				bool Right_Unshielded = C1 >= (AI_WIDTH * 4) - 1 || !Engine.Tilemap[C1 + 1][C2].Collider;
				bool Up_Unshielded = C2 <= 0 || !Engine.Tilemap[C1][C2 - 1].Collider;
				bool Down_Unshielded = C2 >= (AI_HEIGHT * 4) - 1 || !Engine.Tilemap[C1][C2 + 1].Collider;
				if (!Left_Unshielded && !Right_Unshielded && !Up_Unshielded && !Down_Unshielded) {
					continue;
				}
				if (Collide_Tri(Target, Tank.Heat_Cone)) {
					Engine.Tilemap[C1][C2].Triggered = true;
				}
			}
		}
	}
}

void Draw_Debug() {
	if (!Core.Debug) {
		return;
	}
	SDL_FColor Carrier = { 204.0f / 255.0f, 163.0f / 255.0f, 185.0f / 255.0f, 0.5f };
	SDL_Vertex Vertices[3] = {
		{ .position = { Tank.Heat_Cone[0].X, Tank.Heat_Cone[0].Y }, .color = Carrier },
		{ .position = { Tank.Heat_Cone[1].X, Tank.Heat_Cone[1].Y }, .color = Carrier },
		{ .position = { Tank.Heat_Cone[2].X, Tank.Heat_Cone[2].Y }, .color = Carrier }
	};
	SDL_RenderGeometry(Core.Renderer, NULL, Vertices, 3, NULL, 0);
	SDL_SetRenderDrawColor(Core.Renderer, 204, 163, 185, 127);
	for (int C1 = 0; C1 < AI_WIDTH * 4; C1++) {
		for (int C2 = 0; C2 < AI_HEIGHT * 4; C2++) {
			if (Engine.Tilemap[C1][C2].Collider) {
				SDL_FRect Target = { C1 * 4, C2 * 4, 4, 4 };
				SDL_RenderFillRect(Core.Renderer, &Target);
			}
		}
	}
	SDL_SetRenderDrawColor(Core.Renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	for (int C1 = 0; C1 < AI_WIDTH * 4; C1++) {
		for (int C2 = 0; C2 < AI_HEIGHT * 4; C2++) {
			if (Engine.Tilemap[C1][C2].Triggered) {
				SDL_FRect Target = { C1 * 4, C2 * 4, 4, 4 };
				SDL_RenderFillRect(Core.Renderer, &Target);
			}
		}
	}
	for (int C1 = 0; C1 < 32; C1++) {
		if (Tank.Sensors.Sensors[C1].Depth > 0) {
			float Length = Tank.Sensors.Sensors[C1].Depth * AI_WIDTH * 4.0f;
			Point_f End = { Tank.Pos.X * 4.0f + cosf(Tank.Sensors.Sensors[C1].Angle) * Length,
				Tank.Pos.Y * 4.0f + sinf(Tank.Sensors.Sensors[C1].Angle) * Length };
			SDL_RenderLine(Core.Renderer, px(Tank.Pos.X), px(Tank.Pos.Y), End.X * 4.0f, End.Y * 4.0f);
		}
	}
	Clear_Renderer();
}

void Draw_Tiles() {
	for (int C1 = 0; C1 < AI_WIDTH * 4; C1++) {
		for (int C2 = 0; C2 < AI_HEIGHT * 4; C2++) {
			if (Engine.Tilemap[C1][C2].Collider) {
				if (Engine.Tilemap[C1][C2].Alight) {
					if (Engine.Tilemap[C1][C2].Time > 0) {
						Engine.Tilemap[C1][C2].Time -= 1.0f / Core.Framerate;
						float Ratio = Engine.Tilemap[C1][C2].Time / 7.0f;
						SDL_SetRenderDrawColor(Core.Renderer, 255, 127, 0, (uint8_t)(255 * (1.0f - Ratio)));
					} else {
						Engine.Tilemap[C1][C2].Heat = 0;
						Engine.Tilemap[C1][C2].Collider = false;
					}
				} else {
					if (Engine.Tilemap[C1][C2].Heat < 500) {
						continue;
					}
					float Overlay = min(1.0f, Engine.Tilemap[C1][C2].Heat / 3700.0f);
					SDL_SetRenderDrawColor(Core.Renderer, 255, 0, 0, (uint8_t)(Overlay * 127));
				}
				SDL_FRect Target = { C1 * 4, C2 * 4, 4, 4 };
				SDL_RenderFillRect(Core.Renderer, &Target);
			}
			if (!Engine.Tilemap[C1][C2].Collider && Engine.Tilemap[C1][C2].Alight) {
				SDL_SetRenderDrawColor(Core.Renderer, 32, 32, 32, SDL_ALPHA_OPAQUE);
				SDL_FRect Target = { C1 * 4, C2 * 4, 4, 4 };
				SDL_RenderFillRect(Core.Renderer, &Target);
			}
		}
	}
	Clear_Renderer();
}