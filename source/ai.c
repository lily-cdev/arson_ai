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
            Matrix[C1][C2] += Gaussian(0.1f);
            Matrix[C1][C2] = fminf(fmaxf(Matrix[C1][C2], -1.0f), 1.0f);
            printf("%f\n", Matrix[C1][C2]);
        }
    }
}

void Read_Network(bool Mutating) {
    //tmp
    memset(Hidden1_Matrix, 0, sizeof(Hidden1_Matrix));
    memset(Hidden2_Matrix, 0, sizeof(Hidden2_Matrix));
    memset(Hidden3_Matrix, 0, sizeof(Hidden3_Matrix));
    memset(Output_Matrix, 0, sizeof(Output_Matrix));
    //endtmp
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

void Handle_Output() {
    if (Core.Manual) {
        return;
    }
    Tank.Tread_Force[0] = 0;
    Tank.Tread_Force[1] = 0;
    float ip = 0; //tmp

    if (fabsf(ip) > EPSILON && Tank.Firing < 1.0f) {
        Tank.Firing = 1.0f;
    }
    //4 chars, last output indicates whether to print
}