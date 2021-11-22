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
void recipeMenu(ingredients*);
void printRecipeList(Recipes*);
void printDate(ingredients *, int);
void returnMenu(char *, ingredients *);
void clearScreen(void);
void flushInput(void);
void openRecipe(int, Recipes , ingredients *);



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


