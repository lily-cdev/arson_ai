#include <ai.h>

float Score_Performance() {
    float Total = 0;
    float Lethality = 0.0f;
    for (int C1 = 0; C1 < AI_WIDTH * 4; C1++) {
        for (int C2 = 0; C2 < AI_HEIGHT * 4; C2++) {
            if (Engine.Tilemap[C1][C2].Flammable && !Engine.Tilemap[C1][C2].Collider) {
                Lethality += 1.5f;
            }
        }
    }
    float Efficiency = Tank.Max_Dist * 2.0f;
    float Preservation = (1.0f - (Tank.Health / 200.0f)) * 750.0f;
    Total = Lethality + Efficiency - Preservation;
    return Total;
}