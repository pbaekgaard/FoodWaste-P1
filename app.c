#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FRIDGESIZE 120

typedef struct date {
    int year, month, day;
} date;


typedef struct ingredients {
    char* name;    
    date expirationDate;
    date openedDate;
    int daysToExpire;

    double weight;
} ingredients;

void clearScreen(void);

int main(void) {
    int run = 1;
    char *choice;
    ingredients fridgeContent[FRIDGESIZE];
    mainMenu(choice);

    return EXIT_SUCCESS;
}   
void mainMenu(void) {
    clearScreen();
    printf("Welcome to SmartFrAPP\n---------------------\n");
    printf("1 - My Fridge Contents\n");
    printf("2 - Recipes\n");
    printf("Q - Quit\n");
    printf("---------------------\n");
}

void clearScreen(void) {
    system("@cls||clear");
}