#include <engine.h>

typedef struct {
	bool Allocated;
	float Length;
	Point_f Pos;
} Wind_Line;

Wind_Line Wind_Lines[32] = { };
int Wind_Cursor = 0;

void Apply_Forces() {
	float Step = 1.0f / Core.Framerate;
	float Vel_Max = (5.0f / 18.0f) * Tank.Max_Speed;
	float Tread_Vel[2] = {
		Tank.Tread_Force[0] * Vel_Max,
		Tank.Tread_Force[1] * Vel_Max
	};
	float Roll = Core.Roll_Coefficient * 9.81f;
	float Velocity = (Tread_Vel[0] + Tread_Vel[1]) * 0.5f;
	Velocity -= min(sgn(Velocity) * Roll, fabsf(Velocity));
	float Omega = (Tread_Vel[1] - Tread_Vel[0]) / (Tank.Tread_Radius * 2);
	Tank.Angle += Omega * Step;
	Tank.Pos.X += cosf(Tank.Angle) * Velocity * Step;
	Tank.Pos.Y += sinf(Tank.Angle) * Velocity * Step;
	while (Tank.Angle > M_PI) {
		Tank.Angle -= M_PI * 2;
	}
	while (Tank.Angle <= -M_PI) {
		Tank.Angle += M_PI * 2;
	}
}

void Fire_Flamethrower() {
	float Lines[2];
	float Angles[2];
	float Spread = M_PI / 12.0f;
	float Range = 5.0f;
	for (int C1 = 0; C1 < 2; C1++) {
		float Side = (C1 * 2) - 1;
		float Subangle = (Side * Spread) + Tank.Angle;
		Angles[C1] = Subangle + (Core.Wind * cosf(Subangle - Core.Wind_Angle) * 0.5f);
        Lines[C1] = Range - (Core.Wind * cosf(Angles[C1] - Core.Wind_Angle) * 3.5f);
	}
	Point_f Pos = {
		.X = Tank.Pos.X + 0.5f,
		.Y = Tank.Pos.Y + 0.5f
	};
	Tank.Heat_Cone[0] = (Point_f){ px(Pos.X), px(Pos.Y) };
	Tank.Heat_Cone[1] = (Point_f){ px(Pos.X + cosf(Angles[0]) * Lines[0]), px(Pos.Y + sinf(Angles[0]) *
		Lines[0]) };
	Tank.Heat_Cone[2] = (Point_f){ px(Pos.X + cosf(Angles[1]) * Lines[1]), px(Pos.Y + sinf(Angles[1]) *
		Lines[1]) };
}

void Draw_Wind() {
	Core.Wind_Tick += 1.0f / Core.Framerate;
	if (Core.Wind_Tick > 1.0f) {
		Core.Wind_Tick = 0.0f;
		Tick_State();
		bool Is_Bottom = (Core.Wind_Angle > (M_PI * 0.25f) && Core.Wind_Angle < (M_PI * 0.75f));
		Wind_Lines[Wind_Cursor] = (Wind_Line){
			.Length = ((Core.State % 50) + 105) * px(Core.Wind),
			.Pos = {
				.X = (Core.State % px(AI_WIDTH * 3)) - px(AI_WIDTH * 1.5f)
			}
		};
		Wind_Lines[Wind_Cursor].Pos.Y = (Is_Bottom) ? px(AI_HEIGHT) + Wind_Lines[Wind_Cursor].Length : 0 -
			Wind_Lines[Wind_Cursor].Length;
		Wind_Lines[Wind_Cursor].Allocated = true;
		(Wind_Cursor >= 32) ? Wind_Cursor = 0 : Wind_Cursor++;
	}
	for (int C1 = 0; C1 < 32; C1++) {
		if (!Wind_Lines[C1].Allocated) {
			continue;
		}
		Wind_Lines[C1].Pos.Y -= sinf(Core.Wind_Angle) * 7.5f;
		Wind_Lines[C1].Pos.X += cosf(Core.Wind_Angle) * 7.5f;
		Point_f End = {
			.X = Wind_Lines[C1].Pos.X + cosf(Core.Wind_Angle) * Wind_Lines[C1].Length,
			.Y = Wind_Lines[C1].Pos.Y - sinf(Core.Wind_Angle) * Wind_Lines[C1].Length
		};
		Set_Renderer(210, 210, 210);
		SDL_RenderLine(Core.Renderer, Wind_Lines[C1].Pos.X, Wind_Lines[C1].Pos.Y, End.X, End.Y);
		Clear_Renderer();
	}
}