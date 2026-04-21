#include <town.h>
Dir Directions[4] = { D_Left, D_Up, D_Right, D_Down };

void Generate_Path(Point Start, Dir Direction);

void Generate_Path(Point Start, Dir Direction) {
	Tick_State();
	int Cap = (Core.State % 16) + 10;
	for (int C1 = 0; C1 < Cap; C1++) {
		Tick_State();
		switch (Direction) {
		case D_Left:
			Start.X--;
			break;
		case D_Up:
			Start.Y--;
			break;
		case D_Right:
			Start.X++;
			break;
		case D_Down:
			Start.Y++;
			break;
		}
		if (Start.X < 0 || Start.X >= AI_WIDTH || Start.Y < 0 || Start.Y >= AI_HEIGHT) {
			return;
		}
		if (Town.Map[pt(Start)] == B_None) {
			Town.Map[pt(Start)] = B_Path;
			Tick_State();
			bool Branching = ((Core.State % 17) == 0 && C1 > 3);
			if ((Core.State & 3) == 0 && C1 == Cap - 1) {
				Branching = true;
			}
			if (Branching) {
				Dir New_Direction = Direction + 1;
				Tick_State();
				if ((Core.State & 1) == 0) {
					New_Direction = Direction - 1;
				}
				Generate_Path(Start, New_Direction);
				if (C1 == Cap - 1) {
					Generate_Path(Start, (New_Direction + 2) & 3);
					return;
				}
			}
		}
	}
}

void Connect_Paths() {
	for (int C1 = 0; C1 < AI_WIDTH; C1++) {
		for (int C2 = 0; C2 < AI_HEIGHT; C2++) {
			if (Town.Map[C1][C2] == B_Path) {
				if (C1 > 1 && Town.Map[C1 - 2][C2] == B_Path && Town.Map[C1 - 1][C2] == B_None) {
					Town.Map[C1 - 1][C2] = B_Path;
				}
				if (C2 > 1 && Town.Map[C1][C2 - 2] == B_Path && Town.Map[C1][C2 - 1] == B_None) {
					Town.Map[C1][C2 - 1] = B_Path;
				}
				if (C1 < AI_WIDTH - 2 && Town.Map[C1 + 2][C2] == B_Path && Town.Map[C1 + 1][C2] == B_None) {
					Town.Map[C1 + 1][C2] = B_Path;
				}
				if (C2 < AI_HEIGHT - 2 && Town.Map[C1][C2 + 2] == B_Path && Town.Map[C1][C2 + 1] == B_None) {
					Town.Map[C1][C2 + 1] = B_Path;
				}
			}
		}
	}
}

void Generate_Treerings() {
	bool Chunking[AI_WIDTH][AI_HEIGHT] = { };
	for (int C1 = 1; C1 < AI_WIDTH - 1; C1++) {
		for (int C2 = 1; C2 < AI_WIDTH - 1; C2++) {
			Chunking[C1][C2] = true;
			bool Breaking = false;
			for (int C3 = -1; C3 <= 1 && !Breaking; C3++) {
				for (int C4 = -1; C4 <= 1 && !Breaking; C4++) {
					if (Town.Map[C1 + C3][C2 + C4] != B_Path) {
						Chunking[C1][C2] = false;
						Breaking = true;
					}
				}
			}
		}
	}
	for (int C1 = 1; C1 < AI_WIDTH - 1; C1++) {
		for (int C2 = 1; C2 < AI_HEIGHT - 1; C2++) {
			if (Chunking[C1][C2]) {
				Town.Map[C1][C2] = B_Tree;
			}
		}
	}
}

bool Check_Rect(Point Pos, Point Size) {
	for (int C1 = 0; C1 < Size.X; C1++) {
		for (int C2 = 0; C2 < Size.Y; C2++) {
			if (Town.Map[Pos.X + C1][Pos.Y + C2] != B_None) {
				return false;
			}
		}
	}
	return true;
}

void Set_Rect(Point Pos, Point Size) {
	for (int C1 = 0; C1 < Size.X; C1++) {
		for (int C2 = 0; C2 < Size.Y; C2++) {
			Town.Map[Pos.X + C1][Pos.Y + C2] = B_Invalid;
		}
	}
}

bool Check_Line(Point Pos, int Length, Dir Direction, Buildings Target) {
	for (int C1 = 0; C1 < Length; C1++) {
		if (Town.Map[pt(Pos)] != Target) {
			return false;
		}
		switch (Direction) {
		case D_Left:
		case D_Right:
			Pos.Y++;
			break;
		case D_Up:
		case D_Down:
			Pos.X++;
			break;
		}
	}
	return true;
}

void Place_Huts() {
	#define STRUCTS 8
	const int Widths[STRUCTS] = {
		6,
		9,
		5,
		7,
		4,
		4,
		5,
		5
	};
	const int Heights[STRUCTS] = {
		9,
		6,
		7,
		5,
		4,
		4,
		5,
		5
	};
	const Buildings Constructs[STRUCTS] = {
		B_Barn,
		B_Barn,
		B_LHut,
		B_LHut,
		B_Hut,
		B_Hut,
		B_Clayhut,
		B_Clayhut
	};
	const int Chances[STRUCTS] = {
		9,
		9,
		2,
		2,
		3,
		3,
		5,
		5
	};
	//function pointers w/tile updating
	void (*Placements[STRUCTS])(Point Pos) = {
		Set_BarnV,
		Set_BarnH,
		Set_LHutV,
		Set_LHutH,
		Set_Hut,
		Set_Hut,
		Set_Clayhut,
		Set_Clayhut
	};
	const int Height = 0;
	for (int C1 = 0; C1 < STRUCTS; C1++) {
		for (int C2 = 0; C2 < AI_WIDTH - Widths[C1]; C2++) {
			for (int C3 = 0; C3 < AI_HEIGHT - Heights[C1]; C3++) {
				if (Check_Rect((Point){ C2, C3 }, (Point){ Widths[C1], Heights[C1] })) {
					Tick_State();
					if (Core.State % Chances[C1] != 0) {
						continue;
					}
					#define PARAM(Victim, OX, OY, L) (Point){ C2 + (OX), C3 + (OY) }, L, Victim, B_Path
					if (Check_Line(PARAM(D_Left, -1, 0, Heights[C1])) && evn(C1)) {
						Town.Rotations[C2][C3] = D_Left;
					} else if (Check_Line(PARAM(D_Up, 0, -1, Widths[C1])) && !evn(C1)) {
						Town.Rotations[C2][C3] = D_Up;
					} else if (Check_Line(PARAM(D_Right, Widths[C1], 0, Heights[C1])) && evn(C1)) {
						Town.Rotations[C2][C3] = D_Right;
					} else if (Check_Line(PARAM(D_Down, 0, Heights[C1], Widths[C1])) && !evn(C1)) {
						Town.Rotations[C2][C3] = D_Down;
					} else {
						continue;
					}
					#undef PARAM
					Set_Rect((Point){ C2, C3 }, (Point){ Widths[C1], Heights[C1] });
					Town.Map[C2][C3] = Constructs[C1];
					if (Placements[C1]) {
						Placements[C1]((Point){ C2, C3 });
					}
				}
			}
		}
	}
	#undef STRUCTS
}

void Generate_Map() {
	Tick_State();
	Core.Wind = (Core.State % 2000) * 0.0005f;
	Tick_State();
	Core.Wind_Angle = (Core.State % 360) * (M_PI / 180.0f);
	memset(Town.Map, 0, sizeof(Town.Map));
	memset(Engine.Tilemap, 0, sizeof(Tile) * AI_WIDTH * AI_HEIGHT * 16);
	memset(Engine.Flamemap, 0, sizeof(float) * AI_WIDTH * AI_HEIGHT * 16);
	for (int C1 = 0; C1 < AI_WIDTH * 4; C1++) {
		for (int C2 = 0; C2 < AI_HEIGHT * 4; C2++) {
			Engine.Tilemap[C1][C2].Heat = Core.Temperature;
		}
	}
	Tick_State();
	Engine.Center.X = (Core.State % (AI_WIDTH - 24)) + 12;
	Tick_State();
	Engine.Center.Y = Core.State % ((AI_HEIGHT - 16)) + 8;
	Town.Map[pt(Engine.Center)] = B_Well;
	for (int C1 = 0; C1 < 4; C1++) {
		Generate_Path(Engine.Center, Directions[C1]);
	}
	Connect_Paths();
	Generate_Treerings();
	for (int C1 = -1; C1 <= 1; C1++) {
		for (int C2 = -1; C2 <= 1; C2++) {
			if (C1 != 0 || C2 != 0) {
				Town.Map[Engine.Center.X + C1][Engine.Center.Y + C2] = B_Path;
			}
		}
	}
	Place_Huts();
	Tick_State();
	int Tree_Count = (Core.State % 10) + 1;
	for (int C1 = 0; C1 < Tree_Count; C1++) {
		Tick_State();
		Point Pos = {
			.X = Core.State % AI_WIDTH
		};
		Tick_State();
		Pos.Y = Core.State % AI_HEIGHT;
		if (Town.Map[pt(Pos)] == B_None) {
			Town.Map[pt(Pos)] = B_Tree;
		}
	}
}