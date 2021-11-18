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

void mainMenu(void);
void clearScreen(void);

int main(void) {
    ingredients fridgeContent[FRIDGESIZE];
    mainMenu();

    return EXIT_SUCCESS;
}

void mainMenu(void) {
    char* choice;
    int run = 1;
    while(run) {
        clearScreen();
        printf("Welcome to SmartFrAPP\n---------------------\n");
        printf("1 - My Fridge Contents\n");
        printf("2 - Recipes\n");
        printf("Q - Quit\n");
        printf("---------------------\n");

        scanf(" %s", &choice);
        printf("Test");
        if ((choice[0] == '1' || choice[0] == '2' || choice[0] == 'Q' || choice[0] == 'q') &&  choice[1] == '\0') {
            run = 0;
        }
    }
    switch(choice[0]) {
        /*case '1':
            contents();
            break;
        case '2':
            recipes();
            break;*/
        case 'Q': case 'q':
            exit(0);
            break;
    }
}

void clearScreen(void) {
    system("@cls||clear");
}