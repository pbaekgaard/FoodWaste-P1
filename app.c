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

/*Global variable - Sorry. Just for testing changing days*/
date todayDate;

/* Prototypes */
void mainMenu(ingredients *);
date makeDayToday();
void tomorrow(date *);
int leapYear(int);
void contents(ingredients *);
void printFridgeContents(ingredients *);
void recipes(ingredients *fridgeContent);
void recipeMenu(ingredients*);
void printRecipeList(Recipes*);
void printDate(ingredients *, int);
void returnMenu(char *, ingredients *);
void clearScreen(void);
void flushInput(void);
void openRecipe(int, Recipes , ingredients *);


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

    /*Global variable*/
    todayDate = makeDayToday();

    mainMenu(fridgeContent);

    return EXIT_SUCCESS;
}

void mainMenu(ingredients *fridgeContent) {
    char choice;
    int run = 1;

    while(run) {
        clearScreen();
        printf("Welcome to SmartFrAPP\n---------------------\n");
        printf("     %d/%d/%d\n\n", todayDate.year, todayDate.month, todayDate.day);
        printf("1 - Fridge Contents\n");
        printf("2 - Recipes\n");
        printf("---------------------\n");
        printf("Q - Quit\n");
        printf("F - ENTER FUTURE\n");

        scanf(" %c", &choice);
        flushInput();
        
        if (choice == '1' || choice == '2' || choice == 'Q' || choice == 'q' || choice == 'F' || choice == 'f') {
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
        case 'F': case 'f':
            tomorrow(&todayDate);
            mainMenu(fridgeContent);
            break;
    }
}


date makeDayToday(){
    date tempDate;

    time_t today = time(NULL);
    struct tm tm = *localtime(&today);

    tempDate.year = (tm.tm_year + 1900);
    tempDate.month = (tm.tm_mon + 1);
    tempDate.day = (tm.tm_mday);

    return tempDate;
}

void tomorrow(date *d){
    switch(d->month){
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            if(d->day < 31){
                (d->day)++;
            } else if(d->day == 31 && d->month == 12){
                d->day = 1;
                d->month = 1;
                (d->year)++;
            } else{
                d->day = 1;
                (d->month)++;
            }
            break;

        case 4: case 6: case 9: case 11:
            if(d->day < 30){
                (d->day)++;
            } else{
                d->day = 1;
                (d->month)++;
            }
            break;
        case 2:
            if(leapYear(d->year)){
                if(d->day < 29){
                    (d->day)++;
                } else{
                    d->day = 1;
                    (d->month)++;
                }
            }
            else{
                if(d->day < 28){
                    (d->day)++;
                } else{
                    d->day = 1;
                    (d->month)++;
                }
            }
            break;
    }
}

int leapYear(int year){
  int result;

  if (year % 400 == 0) result = 1;
  else if (year % 100 == 0) result = 0;
  else if (year % 4 == 0) result = 1;
  else result = 0;

  return result;
}

void contents(ingredients *fridgeContent) {

    clearScreen();
    printf("Your fridge contains\n");
    printFridgeContents(fridgeContent);
    returnMenu("Main menu", fridgeContent);
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
    recipeMenu(fridgeContent);
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

void recipeMenu(ingredients *fridgeContent) {
    int recipeNumber = 1;
    char *choice = (char*) malloc(100*sizeof(char*));
    Recipes pizzaDough =  {"Pizza", {{"Yeast", 25}, {"Water", 250}, {"Olive oil", 60}, {"Wheat flour", 500}}, "Make pizza"};
    Recipes lasagne = {"Lasagne", {{"Onion", 200}, {"Garlic", 24}, {"Ground beef", 400}, {"Oregano", 2}, {"Carrots", 260}, {"Celery", 300}, {"Squash", 280}, {"Tomato pure", 55}, {"Chopped tomatoes", 800}, {"Vegetable broth", 100}, {"Olive oil", 30}}, "Make lasagne"};

    Recipes recipeList[2]; 

    recipeList[0] = pizzaDough;
    recipeList[1] = lasagne;

    clearScreen();
    printRecipeList(recipeList);
    printf("\nWhich recipe do you want to see? (press R to return to the main menu):\n");

    do{
        if (recipeNumber == 0 || recipeNumber > (sizeof(recipeList)/(sizeof(recipeList[0])))) {
            printRecipeList(recipeList);
            printf("\nPlease enter a valid Recipe Number. Or type 'R' to Return:\n");
        }
        scanf("%s", choice);
        recipeNumber = atoi(choice);
        if (strcmp(choice, "r") == 0 || strcmp(choice, "R") == 0){
            mainMenu(fridgeContent);
        }

    } while (recipeNumber == 0 || recipeNumber > (sizeof(recipeList)/(sizeof(recipeList[0]))));
    
    openRecipe(recipeNumber, recipeList[recipeNumber  - 1], fridgeContent);
}

void printRecipeList(Recipes* recipeList) {
    int i;
    clearScreen();
    printf("This is a list of the recipes in your cookbook\n");
    for(i = 1; i <= 2; i++){
        printf("%d. %s\n", i, recipeList[i - 1].name);
    }
}

void openRecipe(int recipeNumber, Recipes recipe, ingredients *fridgeContent){
    int i;
    clearScreen();
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