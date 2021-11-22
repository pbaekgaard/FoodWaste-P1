#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define FRIDGESIZE 2
#define INSTRUCTIONLINELENGTH 256
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
    ingredients ingredients[50];
    char *filename;
} Recipes;

/* Prototypes */
void mainMenu(ingredients *);
void contents(ingredients *);
void printFridgeContents(ingredients *);
void recipeMenu(ingredients*);
void printRecipeList(Recipes*, int);
void printDate(ingredients *, int);
void returnMenu(char *, ingredients *);
void clearScreen(void);
void flushInput(void);
void openRecipe(Recipes , ingredients *);
void printInstructions(Recipes);



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
            recipeMenu(fridgeContent);
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

void printDate(ingredients *fridgeContent, int itemNumber) {
    printf("%d/%d/%d", fridgeContent[itemNumber].expirationDate.year, fridgeContent[itemNumber].expirationDate.month, fridgeContent[itemNumber].expirationDate.day);
}

void returnMenu(char *menu, ingredients *fridgeContent) {
    char choice;
  
    printf("\n\nR - Return to %s            Q - Quit\n", menu);
    do {
        scanf(" %c", &choice);
        flushInput();
        if(choice == 'R' || choice == 'r'){
           if (strcmp(menu, "Main menu") == 0){
                mainMenu(fridgeContent);
            }
            else if (strcmp(menu, "Recipes") == 0){
                recipeMenu(fridgeContent);
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
    /*Declaration of different recipes*/
    Recipes pizza =  {"Pizza",

                     {{"Strong bread flour", 300}, {"Instant yeast", 3.1},
                     {"Salt", 5.69}, {"Olive Oil", 13.69}, {"Passata", 95.1},
                     {"Dried Basil", 2}, {"Garlic", 4}, {"Mozzarella", 125},
                     {"Parmesan", 10}, {"Cherry tomatoes", 85}},

                     "recipes/pizza/instructions.txt"};
    
    Recipes lasagne = {"Lasagne",

                      {{"Onion", 200}, {"Garlic", 24},
                      {"Ground beef", 400}, {"Oregano", 2}, {"Thyme", 1},
                      {"Carrots", 260}, {"Celery", 300}, {"Squash", 280},
                      {"Tomato pure", 55}, {"Chopped tomatoes", 800},
                      {"Red wine or Vegetable broth", 100}, {"Olive oil", 30},
                      {"Butter", 28.35}, {"Wheat flour", 15}, {"Milk", 300},
                      {"Nutmeg", 1}, {"Mozzarella", 250}, {"Lasagne Plates", 200}},

                      "recipes/lasagne/instructions.txt"};
    
    Recipes burningLove = {"Burning Love",

                          {{"Potatoes", 600}, {"Milk", 100},
                          {"Butter", 25}, {"salt", 2}, {"Bacon", 200},
                          {"Onion", 200}, {"Pickled Beetroots", 100}},

                          "recipes/burninglove/instructions.txt"};
    
    Recipes meatLoaf = {"Meat Loaf",

                       {{"Chopped lambmeat", 500}, {"Onion", 200},
                       {"Garlic", 12}, {"Milk", 100}, {"Cream", 47.5},
                       {"Egg", 100}, {"Oats", 90}, {"Thyme", 18}, {"Rosemary", 6},
                       {"Dried Tomatoes", 20}, {"Black Olives", 50}, {"Feta", 75},
                       {"Baby Potatoes", 800}, {"Oliveoil", 15}, {"Butter", 14},
                       {"Salt & pepper", 1}},

                       "recipes/meatloaf/instructions.txt"};  

    Recipes ricePudding = {"Rice Pudding", {{"Porridge Rice", 484.38}, {"Water", 100},
                      {"Milk", 2000}, {"Salt", 3}, {"Cinnamon", 10},
                      {"Sugar", 16}, {"Butter", 20}}, "recipes/ricepudding/instructions.txt"};

    Recipes recipeList[5]; 
    int numberOfRecipes;
    numberOfRecipes = sizeof(recipeList)/sizeof(recipeList[0]);
    recipeList[0] = pizza;
    recipeList[1] = lasagne;
    recipeList[2] = burningLove;
    recipeList[3] = meatLoaf;
    recipeList[4] = ricePudding;


    clearScreen();
    printRecipeList(recipeList, numberOfRecipes);
    printf("\nWhich recipe do you want to see? (press R to return to the main menu):\n");

    do{
        /*Makes sure the user inputs a valid number*/
        if (recipeNumber == 0 || recipeNumber > (sizeof(recipeList)/(sizeof(recipeList[0])))) {
            printRecipeList(recipeList, numberOfRecipes);
            printf("\nPlease enter a valid Recipe Number. Or type 'R' to Return:\n");
        }
        scanf("%s", choice);
        recipeNumber = atoi(choice);
        /*Return to fridgeContent if user presses 'R'*/
        if (strcmp(choice, "r") == 0 || strcmp(choice, "R") == 0){
            mainMenu(fridgeContent);
        }

    } while (recipeNumber == 0 || recipeNumber > (sizeof(recipeList)/(sizeof(recipeList[0]))));
    
    openRecipe(recipeList[recipeNumber  - 1], fridgeContent);
}

void printRecipeList(Recipes* recipeList, int numberOfRecipes) {
    int i;
    clearScreen();
    printf("This is a list of the recipes in your cookbook\n");
    /*Print the list of recipes*/
    for(i = 1; i <= numberOfRecipes; i++){
        printf("%d. %s\n", i, recipeList[i - 1].name);
    }
}

void openRecipe(Recipes recipe, ingredients *fridgeContent){
    int i;
    int j;
    clearScreen();
    printf("  -------------------------------------\n");
    printf("    %s recipe for 4. people\n", recipe.name);
    printf("  -------------------------------------\n");
    printf("              INGREDIENTS\n");
    printf("  -------------------------------------\n");
    /*Prints each ingredient for the chosen recipe*/
    for(i = 0 ; i < sizeof(recipe.ingredients) / sizeof(recipe.ingredients[0]) ; i++){
        if (recipe.ingredients[i].name != NULL){
            printf("    %s:", recipe.ingredients[i].name);
            for (j = 0; j < 15 - strlen(recipe.ingredients[i].name); j++) {
                printf(" ");
            }
            printf("%.2fg\n\n", recipe.ingredients[i].weight);
        }
    }
    printf("  -------------------------------------\n");
    printf("              INSTRUCTIONS\n");
    printf("  -------------------------------------\n");
    printInstructions(recipe);
    returnMenu("Recipes", fridgeContent);
}

void printInstructions(Recipes recipe) {
    /*Assigns the opened instruction file to point variable fp*/
    FILE *fp = fopen(recipe.filename, "r");
    char buffer[INSTRUCTIONLINELENGTH];

    /*If instructions file doesnt exist. give error message.*/
    if (fp == NULL) {
        printf("ERROR: Couldn't find instructions");
    }
    /*For each line in the instructions file, print the line*/
    while (fgets(buffer, INSTRUCTIONLINELENGTH, fp)){
        printf("%s", buffer);
    }
    /*Closes the instruction file*/
    fclose(fp);

}

void clearScreen(void) {
    system("@cls||clear");
}

void flushInput(void) {
    char flush;
    while((flush = getchar()) != '\n');
}


