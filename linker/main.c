#include <stdlib.h>
#include <stdio.h>

int main() {
    system("seq 4 | xargs -P 4 -I {} ./yield/arson");
    printf("done!");
    //system("wait");
    return 0;
}