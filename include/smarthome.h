#ifndef SMARTHOME_H
#define SMARTHOME_H

#define N 10

struct Sneackers {
    char brand[50]; 
    char model[50];    
    int size; 
    float price;      
};


void FillArray(int n, struct Sneackers cros[N]);   
void PrintArr(int n, struct Sneackers data[]);
void SortByPrice(int n, struct Sneackers cros[N]); 

#endif