#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define FRIDGESIZE 2
#define NUMBEROFRECIPES 7

typedef struct date {
    int year, month, day;
} date;


typedef struct ingredients {
    char* name;    
    double weight;
    date expirationDate;
    date openedDate;
    int daysToExpire;
} ingredients;

typedef struct Recipes {
    char* name;
    ingredients ingredients[11];
    char* instructions;
} Recipes;

/* Prototypes */
void mainMenu(ingredients *);
void contents(ingredients *);
void printFridgeContents(ingredients *);
void recipes(ingredients *fridgeContent);
void printRecipeList(ingredients *);
void printDate(ingredients *, int);
void returnMenu(char *, ingredients *);
void clearScreen(void);
void flushInput(void);
void openRecipe(char, Recipes, ingredients *);


int main(void) {
    ingredients fridgeContent[FRIDGESIZE];
    fridgeContent[0].name = "Tomato";
    fridgeContent[0].expirationDate.year = 2021;
    fridgeContent[0].expirationDate.month = 11;
    fridgeContent[0].expirationDate.day = 23;
    fridgeContent[0].weight = 1000;

    fridgeContent[1].name = "Milk";
    fridgeContent[1].expirationDate.year = 2021;
    fridgeContent[1].expirationDate.month = 11;
    fridgeContent[1].expirationDate.day = 25;
    fridgeContent[1].weight = 1000;

    mainMenu(fridgeContent);

    return EXIT_SUCCESS;
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
            recipes(fridgeContent);
            break;
        case 'Q': case 'q':
            exit(0);
            break;
    }
}

void contents(ingredients *fridgeContent) {

    clearScreen();
    printf("Your fridge contains\n");
    printFridgeContents(fridgeContent);
    returnMenu("Main Menu", fridgeContent);
    
}

void printFridgeContents(ingredients *fridgeContent) {
    int itemNumber, i;

    for(itemNumber = 0 ; itemNumber < FRIDGESIZE ; itemNumber++) {
        printf("%s", fridgeContent[itemNumber].name);
        for(i = 0 ; i < 20 - strlen(fridgeContent[itemNumber].name) ; i++) {
            printf(" ");
        }
        printf("Expiration date: ");
        printDate(fridgeContent, itemNumber);

        printf("   %.2f g\n", fridgeContent[itemNumber].weight);
    }
}

void recipes(ingredients *fridgeContent) {
    printf("This is a list of the recipes in your cookbook\n");
    printRecipeList(fridgeContent);
}

void printDate(ingredients *fridgeContent, int itemNumber) {
    printf("%d/%d/%d", fridgeContent[itemNumber].expirationDate.year, fridgeContent[itemNumber].expirationDate.month, fridgeContent[itemNumber].expirationDate.day);
}

void returnMenu(char *menu, ingredients *fridgeContent) {
    char choice;
  
    printf("\nR - Return to %s            Q - Quit\n", menu);
    do {
        scanf(" %c", &choice);
        flushInput();
        if(choice == 'R' || choice == 'r'){
           if (strcmp(menu, "Main menu") == 0){
                mainMenu(fridgeContent);
            }
            else if (strcmp(menu, "Recipes") == 0){
                recipes(fridgeContent);
            }
        }
        else if(choice == 'Q' || choice == 'q') {
            exit(0);
        }
    } while(!(choice == 'R' || choice == 'r' || choice == 'Q' || choice == 'q'));
}


void printRecipeList(ingredients *fridgeContent) {
    int recipeNumber;

    Recipes pizzaDough =  {"Pizza", {{"Yeast", 25}, {"Water", 250}, {"Olive oil", 60}, {"Wheat flour", 500}}, "Make pizza"};
    Recipes lasagne = {"Lasagne", {{"Onion", 200}, {"Garlic", 24}, {"Ground beef", 400}, {"Oregano", 2}, {"Carrots", 260}, {"Celery", 300}, {"Squash", 280}, {"Tomato pure", 55}, {"Chopped tomatoes", 800}, {"Vegetable broth", 100}, {"Olive oil", 30}}, "Make lasagne"};

    Recipes recipeList[2]; 

    recipeList[0] = pizzaDough;
    recipeList[1] = lasagne;

    for(recipeNumber = 1; recipeNumber <= 2; recipeNumber++){
        printf("%d. %s\n", recipeNumber, recipeList[recipeNumber - 1]);
    }
    printf("\nWhich recipe do you want to see? (press R for main menu)");
    scanf("%d", &recipeNumber);
    if (recipeNumber == 82 || recipeNumber == 114){
          mainMenu(fridgeContent);
    }
    openRecipe(recipeNumber, recipeList[recipeNumber  - 1], fridgeContent);
}

void openRecipe(char recipeNumber, Recipes recipe, ingredients *fridgeContent){
    int i;
       
    switch (recipeNumber - 1)
    {
    case 0:
        printf("\n%s\n", recipe.name);
        for(i = 0 ; i < sizeof(recipe.ingredients) / sizeof(recipe.ingredients[0]) ; i++){
            if (recipe.ingredients[i].name != NULL){
                printf("%s\n%f\n\n", recipe.ingredients[i].name, recipe.ingredients[i].weight);
            }
        }
        break;
    
    case 1:
        printf("\n%s\n", recipe.name);
        for(i = 0 ; i < sizeof(recipe.ingredients) / sizeof(recipe.ingredients[0]) ; i++){
            if (recipe.ingredients[i].name != NULL){
                printf("%s\n%f\n\n", recipe.ingredients[i].name, recipe.ingredients[i].weight);
            }
        }
        break;
    default:
        break;
    }
    returnMenu("Recipes", fridgeContent);
}



void clearScreen(void) {
    system("@cls||clear");
}

void flushInput(void) {
    char flush;
    while((flush = getchar()) != '\n');
}


