#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUMBEROFRECIPES 5
#define INSTRUCTIONLINELENGTH 256
#define FRIDGESIZE 25

typedef struct date {
    int year, month, day;
} date;

typedef struct ingredients {
    char name[20];    
    double weight;
    date expirationDate;
    date openedDate;
} ingredients;

typedef struct Recipes {
    char* name;
    ingredients ingredients[100];
    char *filename;
} Recipes;

/*Global variable - Sorry. Just for testing changing days*/
date todayDate;

/* Prototypes */
void getFridgeContents(ingredients *);
void mainMenu(ingredients *);
date makeDayToday();
void tomorrow(date *);
int leapYear(int);
void contents(ingredients *);
void printFridgeContents(ingredients *);
void recipeMenu(ingredients*);
void printRecipeList(Recipes*);
int dateComparatorenator(date, date);
void printDate(ingredients *, int);
void returnMenu(char *, ingredients *);
void clearScreen(void);
void flushInput(void);
void openRecipe(Recipes , ingredients *);
void printInstructions(Recipes);


int main(void) {
    ingredients fridgeContent[FRIDGESIZE];

    todayDate = makeDayToday(); /*Global variable*/
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
    
    /* Scans file into the structs name and integer into the structs weight until end of file */
    while(!feof(readFile)){
        fscanf(readFile, " %s %lf %d %d %d %d %d %d", fridgeContent[i].name, &fridgeContent[i].weight, &fridgeContent[i].expirationDate.year, &fridgeContent[i].expirationDate.month, &fridgeContent[i].expirationDate.day,
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
            recipeMenu(fridgeContent);
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

    for(itemNumber = 0; itemNumber < FRIDGESIZE; itemNumber++) {
        if(dateComparatorenator(fridgeContent[itemNumber].expirationDate, fridgeContent[itemNumber].openedDate) < 0) {
            printf("\033[0;31m");
        }
        else if(dateComparatorenator(fridgeContent[itemNumber].expirationDate, fridgeContent[itemNumber].openedDate) > 0) {
            printf("\x1B[32m");
        }
        else {
            printf("\033[31;1m");
        }
        printf(" %s", fridgeContent[itemNumber].name);
        if(fridgeContent[itemNumber].weight < 10) {
            for(i = 0; i < 25 - strlen(fridgeContent[itemNumber].name); i++) {
                printf(" ");
            } 
        } 
        else if(fridgeContent[itemNumber].weight < 100 && fridgeContent[itemNumber].weight >= 10) {
            for(i = 0; i < 24 - strlen(fridgeContent[itemNumber].name); i++) {
                printf(" ");
            }           
        }

        else if(fridgeContent[itemNumber].weight < 1000 && fridgeContent[itemNumber].weight >= 100) {
            for(i = 0; i < 23 - strlen(fridgeContent[itemNumber].name); i++) {
                printf(" ");
            }           
        }
        
        else if(fridgeContent[itemNumber].weight < 10000 && fridgeContent[itemNumber].weight >= 1000) {
            for(i = 0; i < 22 - strlen(fridgeContent[itemNumber].name); i++) {
                printf(" ");
            }           
        }

        printf("%.2f g", fridgeContent[itemNumber].weight);
        printf("   Expiration date: ");
        printDate(fridgeContent, itemNumber); 

        if(fridgeContent[itemNumber].expirationDate.month < 10) {
            printf(" ");
        } 
        if(fridgeContent[itemNumber].expirationDate.day < 10) {
            printf(" ");
        }

        if(fridgeContent[itemNumber].openedDate.day == 0) {
            printf("   UNOPENED\n");
        }
        else {
            printf("   Opened on: %d/%d/%d\n", fridgeContent[itemNumber].openedDate.year, fridgeContent[itemNumber].openedDate.month, fridgeContent[itemNumber].openedDate.day);
        }
        printf("\x1B[0m");
    }
}

int dateComparatorenator(date expirationDate, date openedDate) {
    if((expirationDate.year > todayDate.year) ||
       (expirationDate.year == todayDate.year && expirationDate.month > todayDate.month) ||
       (expirationDate.year == todayDate.year && expirationDate.month == todayDate.month && expirationDate.day > todayDate.day)) {
        return 1;
    }
    else if(expirationDate.year == todayDate.year && expirationDate.month == todayDate.month && expirationDate.day == todayDate.day) {
        return 0;
    }   
    else
        return -1;
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
                      {"Vegetable broth", 100}, {"Olive oil", 30},
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

    Recipes recipeList[NUMBEROFRECIPES]; 
    recipeList[0] = pizza;
    recipeList[1] = lasagne;
    recipeList[2] = burningLove;
    recipeList[3] = meatLoaf;
    recipeList[4] = ricePudding;


    clearScreen();
    printRecipeList(recipeList);
    printf("\nWhich recipe do you want to see? (press R to return to the main menu):\n");

    do{
        /*Makes sure the user inputs a valid number*/
        if (recipeNumber == 0 || recipeNumber > (sizeof(recipeList)/(sizeof(recipeList[0])))) {
            printRecipeList(recipeList);
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

void printRecipeList(Recipes* recipeList) {
    int i;
    clearScreen();
    printf("This is a list of the recipes in your cookbook\n");
    /*Print the list of recipes*/
    for(i = 1; i <= NUMBEROFRECIPES; i++){
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
        if (strcmp(recipe.ingredients[i].name, "\0")){
            printf("    %s:", recipe.ingredients[i].name);
            for (j = 0; j < 20 - strlen(recipe.ingredients[i].name); j++) {
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
    FILE *fp = fopen(recipe.filename, "r");
    char buffer[INSTRUCTIONLINELENGTH];

    if (fp == NULL) {
        printf("ERROR: Couldn't find instructions");
    }

    while (fgets(buffer, INSTRUCTIONLINELENGTH, fp)){
        printf("%s", buffer);
    }
    fclose(fp);

}

void clearScreen(void) {
    system("@cls||clear");
}

void flushInput(void) {
    char flush;
    while((flush = getchar()) != '\n');
}
