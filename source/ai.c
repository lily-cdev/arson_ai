#include <ai.h>
#define INPUTS 73
#define LAYER1 64
#define LAYER2 32
#define LAYER3 16
#define OUTPUTS 8

float Hidden1_Matrix[LAYER1][INPUTS];
float Hidden2_Matrix[LAYER2][LAYER1];
float Hidden3_Matrix[LAYER3][LAYER2];
float Output_Matrix[OUTPUTS][LAYER3];

float Gaussian(float Sigma) {
	float Roots[2];
	for (int C1 = 0; C1 < 2; C1++) {
		Tick_State();
		Roots[C1] = (float)((Core.State + 1.0) / (UINT32_MAX + 1.0));
	}
	return sqrtf(-2.0f * logf(Roots[0])) * cosf(Roots[1] * M_PI * 2.0f) * Sigma;
}

void Randomize(int X, int Y, float Matrix[X][Y]) {
	for (int C1 = 0; C1 < X; C1++) {
		for (int C2 = 0; C2 < Y; C2++) {
			Tick_State();
			Matrix[C1][C2] += Gaussian(0.05f);
			Matrix[C1][C2] = fminf(fmaxf(Matrix[C1][C2], -1.0f), 1.0f);
		}
	}
}

void Save_Network(const int ID) {
	if (Core.Manual) {
		return;
	}
	char Path[64];
	snprintf(Path, sizeof(Path), "./nets/candidate_%i.net", ID);
	FILE* Net = fopen(Path, "wb");
	for (int C1 = 0; C1 < LAYER1; C1++) {
		fwrite(Hidden1_Matrix[C1], sizeof(float), INPUTS, Net);
	}
	for (int C1 = 0; C1 < LAYER2; C1++) {
		fwrite(Hidden2_Matrix[C1], sizeof(float), LAYER1, Net);
	}
	for (int C1 = 0; C1 < LAYER3; C1++) {
		fwrite(Hidden3_Matrix[C1], sizeof(float), LAYER2, Net);
	}
	for (int C1 = 0; C1 < OUTPUTS; C1++) {
		fwrite(Output_Matrix[C1], sizeof(float), LAYER3, Net);
	}
	fclose(Net);	
	
	snprintf(Path, sizeof(Path), "./nets/score_%i.net", ID);
	FILE* Score = fopen(Path, "wb");
	fwrite(&(float){ Score_Performance() }, sizeof(float), 1, Score);
	fclose(Score);
}

void Read_Network(const bool Mutating) {
	if (Core.Manual) {
		return;
	}
	FILE* Net = fopen("./nets/best.net", "rb");
	if (!Net) {
		puts("net not found!");
		exit(-1);
	}
	for (int C1 = 0; C1 < LAYER1; C1++) {
		fread(Hidden1_Matrix[C1], sizeof(float), INPUTS, Net);
	}
	for (int C1 = 0; C1 < LAYER2; C1++) {
		fread(Hidden2_Matrix[C1], sizeof(float), LAYER1, Net);
	}
	for (int C1 = 0; C1 < LAYER3; C1++) {
		fread(Hidden3_Matrix[C1], sizeof(float), LAYER2, Net);
	}
	for (int C1 = 0; C1 < OUTPUTS; C1++) {
		fread(Output_Matrix[C1], sizeof(float), LAYER3, Net);
	}
	fclose(Net);
	if (!Mutating) {
		return;
	}
	Randomize(LAYER1, INPUTS, Hidden1_Matrix);
	Randomize(LAYER2, LAYER1, Hidden2_Matrix);
	Randomize(LAYER3, LAYER2, Hidden3_Matrix);
	Randomize(OUTPUTS, LAYER3, Output_Matrix);
}

void Step(int X, int Y, float Applied[X], float Initial[Y], float Matrix[X][Y]) {
	for (int C1 = 0; C1 < X; C1++) {
		for (int C2 = 0; C2 < Y; C2++) {
			Applied[C1] += Matrix[C1][C2] * Initial[C2];
		}
		Applied[C1] = tanhf(Applied[C1]);
	}
}

void Forward_Pass(float Yield[OUTPUTS]) {
	memset(Yield, 0, sizeof(float) * OUTPUTS);
	float A0[INPUTS] = { };
	float A1[LAYER1] = { };
	float A2[LAYER2] = { };
	float A3[LAYER3] = { };
	A0[0] = Tank.Sensors.Bias;
	A0[1] = Tank.Sensors.Firing;
	A0[2] = Tank.Sensors.Track1_Force;
	A0[3] = Tank.Sensors.Track2_Force;
	A0[4] = Tank.Sensors.Wind;
	A0[5] = Tank.Sensors.Wind_Angle;
	A0[6] = Tank.Sensors.Health;
	A0[7] = Tank.Sensors.Fuel;
	A0[8] = Tank.Sensors.Temperature;
	for (int C1 = 0; C1 < 32; C1++) {
		A0[(C1 * 2) + 8] = Tank.Sensors.Sensors[C1].Depth;
		A0[(C1 * 2) + 9] = Tank.Sensors.Sensors[C1].Material;
	}
	Step(LAYER1, INPUTS, A1, A0, Hidden1_Matrix);
	Step(LAYER2, LAYER1, A2, A1, Hidden2_Matrix);
	Step(LAYER3, LAYER2, A3, A2, Hidden3_Matrix);
	Step(OUTPUTS, LAYER3, Yield, A3, Output_Matrix);
}

void Feed_Forward() {
	float Output[8];
	Forward_Pass(Output);
	for (int C1 = 0; C1 < 2; C1++) {
		Tank.Tread_Force[C1] = Output[C1];
	}
	if (Output[2] > 0.0f && Tank.Firing < 1.0f) {
		Tank.Firing = 1.0f;
	}
	if (Output[3] > 0.0f) {
		char Speech[5];
		for (int C1 = 0; C1 < 4; C1++) {
			Speech[C1] = (char)min(max(floor((Output[C1 + 4] + 1.0f) * 64.0f), 127), 0);
		}
		Speech[4] = '\0';
		puts(Speech);
	}
}