#include <sublinker.h>

bool Check_Inside(SDL_FRect A, Point_f B) {
	return (B.X > A.x) && (B.X < A.x + A.w) && (B.Y > A.y) && (B.Y < A.y + A.h);
}

bool Collide_Subline(Point_f A, Point_f B, Point_f C, Point_f D) {
	float Sub_A = ((D.X - C.X) * (A.Y - C.Y) - (D.Y - C.Y) * (A.X - C.X)) / ((D.Y - C.Y) * (B.X - A.X) -
		(D.X - C.X) * (B.Y - A.Y));
	float Sub_B = ((B.X - A.X) * (A.Y - C.Y) - (B.Y - A.Y) * (A.X - C.X)) / ((D.Y - C.Y) * (B.X - A.X) -
		(D.X - C.X) * (B.Y - A.Y));
	return (Sub_A >= 0 && Sub_A <= 1) && (Sub_B >= 0 && Sub_B <= 1);
}

bool Collide_Line(SDL_FRect A, Point_f B, Point_f C) {
	bool Checks[4] = {
		Collide_Subline(B, C, (Point_f){ A.x, A.y }, (Point_f){ A.x + A.w, A.y }),
		Collide_Subline(B, C, (Point_f){ A.x + A.w, A.y }, (Point_f){ A.x + A.w, A.y + A.h }),
		Collide_Subline(B, C, (Point_f){ A.x + A.w, A.y + A.h }, (Point_f){ A.x, A.y + A.h }),
		Collide_Subline(B, C, (Point_f){ A.x, A.y + A.h }, (Point_f){ A.x, A.y })
	};
	return Checks[0] || Checks[1] || Checks[2] || Checks[3];	
}

bool Inside_Tri(Point_f A, Point_f B, Point_f C, Point_f D) {
	float Og_Area = fabsf(((B.X - A.X) * (C.Y - A.Y)) - ((C.X - A.X) * (B.Y - A.Y)));
	float Areas[3] = {
		fabsf(((A.X - D.X) * (B.Y - D.Y)) - ((B.X - D.X) * (A.Y - D.Y))),
		fabsf(((B.X - D.X) * (C.Y - D.Y)) - ((C.X - D.X) * (B.Y - D.Y))),
		fabsf(((C.X - D.X) * (A.Y - D.Y)) - ((A.X - D.X) * (C.Y - D.Y)))
	};
	float Sum = Areas[0] + Areas[1] + Areas[2];
	float Epsilon = 0.01f;
	return (fabsf(Sum - Og_Area) < Epsilon);
}

bool Collide_Tri(SDL_FRect A, Point_f B[3]) {
	for (int C1 = 0; C1 < 3; C1++) {
		int Forward = (C1 + 1) % 3;
		if (Check_Inside(A, B[C1])) {
			return true;
		}
		if (Collide_Line(A, B[C1], B[Forward])) {
			return true;
		}
	}
	for (int C1 = 0; C1 < 4; C1++) {
		Point_f Corner;
		switch (C1) {
		case 0:
			Corner = (Point_f){ A.x, A.y };
			break;
		case 1:
			Corner = (Point_f){ A.x + A.w, A.y };
			break;
		case 2:
			Corner = (Point_f){ A.x + A.w, A.y + A.h };
			break;
		case 3:
			Corner = (Point_f){ A.x, A.y + A.h };
			break;
		}
		if (Inside_Tri(B[0], B[1], B[2], Corner)) {
			return true;
		}
	}
	return false;
}