#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define FRIDGESIZE 25
#define NUMBEROFRECIPES 7

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

/* Prototypes */
void mainMenu(ingredients *);
void contents(ingredients *);
void printFridgeContents(ingredients *);
void recipes(void);
void printRecipeList(void);
void printDate(ingredients *, int);
void returnMenu(char *);
void clearScreen(void);
void flushInput(void);

int main(void) {
    ingredients fridgeContent[FRIDGESIZE];

    getFridgeContents(fridgeContent);
    
    mainMenu(fridgeContent);

    return EXIT_SUCCESS;
}

void getFridgeContents(ingredients *fridgeContent) {
    int i = 0;
    /* Pointer to a File */
    FILE *readFile;

    /* Name of file */
    char *filename = "ingredients.txt";

    /* Open and read file */
    readFile = fopen(filename, "r");

    /* If file doesn't open it gives Error message */
    if(readFile == NULL) {
        printf("Error");
    }
    
    /* Scans string into the structs name and integer into the structs weight until end of file */
    while(!feof(readFile)){
        fscanf(readFile, " %s %lf %d %d %d %d %d %d", &fridgeContent[i].name, &fridgeContent[i].weight, &fridgeContent[i].expirationDate.year, &fridgeContent[i].expirationDate.month, &fridgeContents[i].expirationDate.day,
                                            &fridgeContent[i].openedDate.year, &fridgeContent[i].openedDate.month, &fridgeContent[i].openedDate.day);
        i++;
    }
    /* Closes file */
    fclose(readFile);
}

void mainMenu(ingredients *fridgeContent) {
    char choice;
    int run = 1;

    while(run) {
        clearScreen();
        printf("Welcome to SmartFrAPP\n---------------------\n");
        printf("1 - My Fridge Contents\n");
        printf("2 - Recipes\n");
        printf("Q - Quit\n");
        printf("---------------------\n");

        scanf(" %c", &choice);
        flushInput();
        
        if (choice == '1' || choice == '2' || choice == 'Q' || choice == 'q') {
            run = 0;
        }

    }
    switch(choice) {
        case '1':
            contents(fridgeContent);
            break;
        case '2':
            recipes();
            break;
        case 'Q': case 'q':
            exit(0);
            break;
    }
}

void contents(ingredients *fridgeContent) {
    char choice;

    clearScreen();
    printf("Your fridge contains\n");
    printFridgeContents(fridgeContent);
    returnMenu("Main Menu");
    do {
        scanf(" %c", &choice);
        flushInput();
        if(choice == 'R' || choice == 'r'){
            mainMenu(fridgeContent);
        }
        else if(choice == 'Q' || choice == 'q') {
            exit(0);
        }
    } while(!(choice == 'R' || choice == 'r' || choice == 'Q' || choice == 'q'));
}

void printFridgeContents(ingredients *fridgeContent) {
    int itemNumber, i;

    for(itemNumber = 0; itemNumber < FRIDGESIZE; itemNumber++) {
        printf("%s", fridgeContent[itemNumber].name);
        for(i = 0; i < 20 - strlen(fridgeContent[itemNumber].name); i++) {
            printf(" ");
        }
        printf("Expiration date: ");
        printDate(fridgeContent, itemNumber);

        printf("   %.2lf g\n", fridgeContent[itemNumber].weight);
    }
}

void recipes(void) {
    printf("This is a list of the recipes in your cookbook\n");
    printRecipeList();
    returnMenu("Main Menu");
}

void printRecipeList(void) {
    int recipeNumber;
    char *recipeList[] = {"Pizza", "Lasagne", "Burger", "Spagetti carbonara", "Taco", "Beef Wellington", "Braendende kaerlighed"};

    for(recipeNumber = 1; recipeNumber <= NUMBEROFRECIPES; recipeNumber++){
        printf("%d. %s\n", recipeNumber, recipeList[recipeNumber - 1]);
    }
}

void printDate(ingredients *fridgeContent, int itemNumber) {
    printf("%d/%d/%d", fridgeContent[itemNumber].expirationDate.year, fridgeContent[itemNumber].expirationDate.month, fridgeContent[itemNumber].expirationDate.day);
}

void returnMenu(char *choice) {
    printf("\nR - Return to %s            Q - Quit\n", choice);
}

void clearScreen(void) {
    system("@cls||clear");
}

void flushInput(void) {
    char flush;
    while((flush = getchar()) != '\n');
}