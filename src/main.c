#include <stdio.h>
#include "smarthome.h"

int main() {
    struct Sneackers cros[N];
    FillArray(N, cros);

    printf("\nДо\n");
    PrintArr(N, cros);

    SortByPrice(N, cros);

    printf("\nПОСЛЕ\n");
    PrintArr(N, cros);

    return 0;
}