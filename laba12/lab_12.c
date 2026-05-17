#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

struct Sneackers {
    char brand[50];
    char model[50];
    int size;
    float price;
};

#define N 10

void FillArray(int n, struct Sneackers cros[n]){
    char brand[N][30]={"Nike", "Adidas", "PUMA", "Hoka", "Reebok", "Premiata", "Asics", "ANTA", "New Balance", "Saucony"};
    char model[N][30]={"Air Zoom Pegasus", "Duramo RC2", "Fade Nitro V2 Running", "Rincon 3", "Adidas Samba OG", "Salomon XT-6", "Nike Air Force 1 Low", "Asics Gel-1130",  "Fresh Foam", "Kinvara"};

    srand(time(NULL));
    for(int i=0; i<N; i++){
        strcpy(cros[i].brand, brand[i]);
        strcpy(cros[i].model, model[i]);

        cros[i].size = 35+rand()%12;
        cros[i].price = 2000+rand()%18001;
    }
}

void PrintArr(int n, struct Sneackers data[]){
    printf(" КАТАЛОГ КРОССОВОК:  \n");
    printf("Бренд      Модель      Размер  Цена\n");

    for (int i=0; i<N; i++){
        printf("%d. %10s %10s %5d %5.1f\n", i+1, data[i].brand, data[i].model, data[i].size, data[i].price);
    }
}

int main(){
    struct Sneackers cros[N];
    FillArray(N, cros);
    PrintArr(N, cros);

    for(int i=0; i<N; i++){
        for(int j=0; j<N-1; j++){
            if (cros[j].price > cros[j+1].price){
                struct Sneackers buff = cros[j];
                cros[j]=cros[j+1];
                cros[j+1]=buff;
            }
        }
    }
    PrintArr(N, cros);
    return 0;
}
