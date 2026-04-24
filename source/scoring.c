#include <ai.h>

float Score_Performance() {
    float Total = 0;
    float Divisor = 0;
    float Burned = 0;
    for (int C1 = 0; C1 < AI_WIDTH * 4; C1++) {
        for (int C2 = 0; C2 < AI_HEIGHT * 4; C2++) {
            if (Engine.Tilemap[C1][C2].Flammable) {
                Divisor++;
                if (!Engine.Tilemap[C1][C2].Collider) {
                    Burned++;
                }
            }
        }
    }
    float Lethality = (Burned / Divisor) * 100.0f;
    float Efficiency = 0;
    float Preservation = (1.0f - (Tank.Health / 200.0f)) * 350.0f;
    Total = Lethality + Efficiency - Preservation;
    printf("\n\n===SCORE===\nTOTAL: %.2f\nLethality: %.2f\nEfficiency: %.2f\nSelf-Preservation: %.2f\n",
        Total, Lethality, Efficiency, -Preservation);
    return Total;
}