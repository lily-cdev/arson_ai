#include <ai.h>

typedef struct {
    int Dest;
    float Weight;
} Link;

typedef struct {
    int ID;
    int Layer;
    float Sum;//zero-init upon load
    float Output;//zero-init upon load
    Link* Links;
} Node;

void Read_Network() {
    
}

void Handle_Output() {
    if (Core.Manual) {
        return;
    }
    Tank.Tread_Force[0] = 0;
    Tank.Tread_Force[1] = 0;
    int ip = 0; //tmp

    if (fabsf(ip) > EPSILON && Tank.Firing < 1.0f) {
        Tank.Firing = 1.0f;
    }
}