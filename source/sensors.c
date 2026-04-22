#include <sublinker.h>

void Handle_Sensors() {
	#define clamp(Victim) min(max((Victim), -1.0f), 1.0f)
	memset(&Tank.Sensors, 0, sizeof(Tank.Sensors));
	Tank.Sensors.Temperature = clamp(Core.Temperature * 0.01f);
	Tank.Sensors.Wind_Angle = (Core.Wind_Angle - Tank.Angle) / M_PI;
	while (Tank.Sensors.Wind_Angle < -1.0f) {
		Tank.Sensors.Wind_Angle += 2.0f;
	}
	while (Tank.Sensors.Wind_Angle > 1.0f) {
		Tank.Sensors.Wind_Angle -= 2.0f;
	}
	Tank.Sensors.Firing = clamp(Tank.Firing * 2.0f - 1.0f);
	Tank.Sensors.Track1_Force = clamp(Tank.Tread_Force[0]);
	Tank.Sensors.Track2_Force = clamp(Tank.Tread_Force[1]);
	Tank.Sensors.Wind = clamp((Core.Wind * 2.0f) - 1.0f);
	Tank.Sensors.Bias = 1.0f;
	Tank.Sensors.Health = (Tank.Health - 100.0f) * 0.01f;
	Tank.Sensors.Fuel = (Tank.Fuel - 100.0f) * 0.01f;
	for (int C1 = 0; C1 < AI_SENSORS; C1++) {
		float Angle = ((M_PI / (360 / AI_SENSORS)) * C1) + Tank.Angle;
		Point_f Initial = { Tank.Pos.X * 4.0f, Tank.Pos.Y * 4.0f };
		Tank.Sensors.Sensors[C1].Depth = -1.0f;
		Tank.Sensors.Sensors[C1].Material = Engine.Material_Map[M_None];
		Tank.Sensors.Sensors[C1].Heat = -1.0f;
		while (Initial.X >= 0 && Initial.X < AI_WIDTH * 4 && Initial.Y >= 0 && Initial.Y < AI_HEIGHT * 4) {
			Initial.X += cosf(Angle) * 0.25f;
			Initial.Y += sinf(Angle) * 0.25f;
			Point Pos = { (int)floorf(Initial.X), (int)floorf(Initial.Y) };
			bool Matched = false;
			if (Pos.X < 0 || Pos.X >= AI_WIDTH * 4 || Pos.Y < 0 || Pos.Y >= AI_HEIGHT * 4) {
				Initial.X = min(max(Initial.X, 0.0f), AI_WIDTH * 4.0f);
				Initial.Y = min(max(Initial.Y, 0.0f), AI_HEIGHT * 4.0f);
				Matched = true;
			}
			if (Engine.Tilemap[pt(Pos)].Collider || Matched) {
				Tank.Sensors.Sensors[C1].Depth = sqrtf(sqr(Initial.X - (Tank.Pos.X * 4.0f)) + sqr(Initial.Y -
					(Tank.Pos.Y * 4.0f))) / (AI_WIDTH * 4.0f);
				Tank.Sensors.Sensors[C1].Angle = Angle;
				if (!Matched) {
					Tank.Sensors.Sensors[C1].Material = Engine.Tilemap[pt(Pos)].Material;
					Tank.Sensors.Sensors[C1].Heat = Engine.Tilemap[pt(Pos)].Heat * 0.01f;
				}
				break;
			}
		}
	}
	if (SDL_GetTicks() % 100 != 0) {
		return;
	}
	#ifdef __unix__
		system("clear");
	#elif defined(_WIN32) || defined(WIN32)
		system("cls");
	#endif
	printf("TMP: %.2f | WND: %.2f @ %.2f | Firing: %.2f | TRDS: %.2f & %.2f | BIAS: %.2f | HP: %.2f | FUEL: "
		"%.2f\n", Tank.Sensors.Temperature, Tank.Sensors.Wind, Tank.Sensors.Wind_Angle, Tank.Sensors.Firing,
		Tank.Sensors.Track1_Force, Tank.Sensors.Track2_Force, Tank.Sensors.Bias, Tank.Sensors.Health,
		Tank.Sensors.Fuel);
	for (int C1 = 0; C1 < AI_SENSORS; C1++) {
		printf("RCST%i TMP: %.2f | ", C1, Tank.Sensors.Sensors[C1].Heat);
		printf("RCST%i DST: %.2f | ", C1, Tank.Sensors.Sensors[C1].Depth);
		printf("RCST%i ANG: %.2f | ", C1, Tank.Sensors.Sensors[C1].Angle);
		printf("RCST%i MTL: %.2f\n", C1, Tank.Sensors.Sensors[C1].Material);
	}
	#undef clamp
}