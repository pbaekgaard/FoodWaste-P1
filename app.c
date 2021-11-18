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
    ingredients fridgeContent[FRIDGESIZE];
    mainMenu();

    return EXIT_SUCCESS;
}   
void mainMenu() {
    char* choice;
    int run = 1;
    while(run) {
        clearScreen();
        printf("Welcome to SmartFrAPP\n---------------------\n");
        printf("1 - My Fridge Contents\n");
        printf("2 - Recipes\n");
        printf("Q - Quit\n");
        printf("---------------------\n");

        scanf("%s", choice);

        if ((choice[0] == '1' || choice[0] == '2' || choice[0] == 'Q' || choice[0] == 'q') &&  choice[1] == '\0') {
            run = 0;
        }
        
    }
}

void clearScreen(void) {
    system("@cls||clear");
}