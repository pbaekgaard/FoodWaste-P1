#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUMBEROFRECIPES 5
#define INSTRUCTIONLINELENGTH 256
#define FRIDGESIZE 25
#define TRUE 1
#define FALSE 0

typedef struct date {
    int year, month, day;
} date;

typedef struct isopen {
    date openDate;
    int daysAfterOpen;
} isopen;

typedef struct open {
    int unopened;
    isopen isopen;
} open;

typedef struct ingredients {
    char name[20];    
    double weight;
    date expirationDate;
    open open;
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
void updateExpDates (ingredients *);
void mainMenu(ingredients *);
void sortContent(ingredients *);
int contentCompare(const void *, const void *);
void printNotifications(ingredients *);
date makeDayToday();
void tomorrow(date *);
int leapYear(int);
void contents(ingredients *);
void printFridgeContents(ingredients *);
void recipeMenu(ingredients*);
int colourization(ingredients *, char *, double);
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
    updateExpDates(fridgeContent);
    mainMenu(fridgeContent);

    return EXIT_SUCCESS;
}

void getFridgeContents(ingredients *fridgeContent) {
    int i = 0;
    /* Pointer to a File */
    FILE *readFile;

    /* Name of file */
    char *filename = "db/fridge/ingredients.txt";

    /* Open and read file */
    readFile = fopen(filename, "r");

    /* If file doesn't open it gives Error message */
    if(readFile == NULL) {
        printf("Error");
    }
    
    /* Scans file into the structs name and integer into the structs weight until end of file */
    while(!feof(readFile)){
        fscanf(readFile, " %s %lf %d %d %d %d", fridgeContent[i].name, &fridgeContent[i].weight, &fridgeContent[i].expirationDate.year, &fridgeContent[i].expirationDate.month, &fridgeContent[i].expirationDate.day, &fridgeContent[i].open.unopened);
    if (fridgeContent[i].open.unopened == FALSE){
         fscanf(readFile, " %d %d %d %d", &fridgeContent[i].open.isopen.openDate.year, &fridgeContent[i].open.isopen.openDate.month, &fridgeContent[i].open.isopen.openDate.day, &fridgeContent[i].open.isopen.daysAfterOpen);
    }       
    else{
        fscanf(readFile, " %d", &fridgeContent[i].open.isopen.daysAfterOpen);
    }
        i++;
    }
    /* Closes file */
    fclose(readFile);
}

/* updates all expiration dates, if they have been opened*/
void updateExpDates (ingredients *fridgeContent){
    int i, j;
    date openExp;
    /*for loop that runs through every element of the fridgeContent*/ 
    for ( i = 0; i < FRIDGESIZE; i++){    
        /* checks if product is open*/
        if (fridgeContent[i].open.unopened == FALSE){
            openExp = fridgeContent[i].open.isopen.openDate;
            /* calculates new expirationdate. Adds how long the product is fresh after opening to the date opened*/
            for ( j = 0; j < fridgeContent[i].open.isopen.daysAfterOpen; j++){
                tomorrow(&openExp); 
            }
            /*If the original exp date is larger than the updated exp date after opening. Updates the exp date.*/
            if (dateComparatorenator(fridgeContent[i].expirationDate, openExp) == 1 || dateComparatorenator(fridgeContent[i].expirationDate, openExp) == 0){
                fridgeContent[i].expirationDate.year = openExp.year;
                fridgeContent[i].expirationDate.month = openExp.month;
                fridgeContent[i].expirationDate.day = openExp.day;
            }
        }
    } 
}

void mainMenu(ingredients *fridgeContent) {
    char choice;
    int run = 1;

    sortContent(fridgeContent);
    while(run) {
        clearScreen();
        printf("Welcome to SmartFrAPP\n---------------------\n");
        printf("     %d/%d/%d\n\n", todayDate.year, todayDate.month, todayDate.day);
        printf("1 - Fridge Contents\n");
        printf("2 - Recipes\n");
        printf("---------------------\n");
        printf("Q - Quit\n");
        printf("F - ENTER FUTURE\n");
        printf("---------------------\n\n");
        printNotifications(fridgeContent);
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

void sortContent(ingredients *fridgeContent) {
    qsort(fridgeContent, FRIDGESIZE, sizeof(fridgeContent[0]), contentCompare);
}

int contentCompare(const void *content1, const void *content2) {
    date dateContent1 = ((ingredients *)content1)->expirationDate;
    date dateContent2 = ((ingredients *)content2)->expirationDate;

    if((dateContent1.year > dateContent2.year) ||
       (dateContent1.year == dateContent2.year && dateContent1.month > dateContent2.month) ||
       (dateContent1.year == dateContent2.year && dateContent1.month == dateContent2.month && dateContent1.day > dateContent2.day)) {
        return 1;
    }
    else if(dateContent1.year == dateContent2.year && dateContent1.month == dateContent2.month && dateContent1.day == dateContent2.day) {
        return 0;
    }   
    else
        return -1;
}

void printNotifications(ingredients *fridgeContent){
    int i = 0;
    /*SOON TO EXPIRE*/
    for(i = 0; i < FRIDGESIZE; i++) {
        if (dateComparatorenator(fridgeContent[i].expirationDate, todayDate) == 0) {
            printf("###########################\n");            
            printf("         EXPIRING         \n");
            printf("---------------------------\n");
            break;
        }
    }

    for (i = 0; i < FRIDGESIZE; i++) {
        if (dateComparatorenator(fridgeContent[i].expirationDate, todayDate) == 0) {
            printf("\033[33;1m%s IS EXPIRING\n\x1B[0m", fridgeContent[i].name);
        } 
    }
    printf("\n\n###########################\n");

    /*EXPIRED*/

    for(i = 0; i < FRIDGESIZE; i++) {
        if (dateComparatorenator(fridgeContent[i].expirationDate, todayDate) == -1) {
            printf("         EXPIRED\n");
            printf("---------------------------\n");
            break;
        }
    }
    for (i = 0; i < FRIDGESIZE; i++) {
        if (dateComparatorenator(fridgeContent[i].expirationDate, todayDate) == -1) {
            printf("\033[31;1m%s HAS EXPIRED\n\x1B[0m", fridgeContent[i].name);
        }
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

void tomorrow(date *date){
    switch(date->month){
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            if(date->day < 31){
                (date->day)++;
            } else if(date->day == 31 && date->month == 12){
                date->day = 1;
                date->month = 1;
                (date->year)++;
            } else{
                date->day = 1;
                (date->month)++;
            }
            break;

        case 4: case 6: case 9: case 11:
            if(date->day < 30){
                (date->day)++;
            } else{
                date->day = 1;
                (date->month)++;
            }
            break;
        case 2:
            if(leapYear(date->year)){
                if(date->day < 29){
                    (date->day)++;
                } else{
                    date->day = 1;
                    (date->month)++;
                }
            }
            else{
                if(date->day < 28){
                    (date->day)++;
                } else{
                    date->day = 1;
                    (date->month)++;
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
        if(dateComparatorenator(fridgeContent[itemNumber].expirationDate, todayDate) == -1) {
            printf("\033[31;1m");
        }
        else if(dateComparatorenator(fridgeContent[itemNumber].expirationDate, todayDate) == 1) {
            printf("\033[0;32m");
        }
        else {
            printf("\033[33;1m");
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

        if(fridgeContent[itemNumber].open.unopened == TRUE) {
            printf("   UNOPENED\n");
        }
        else {
            printf("   Opened on: %d/%d/%d\n", fridgeContent[itemNumber].open.isopen.openDate.year, fridgeContent[itemNumber].open.isopen.openDate.month, fridgeContent[itemNumber].open.isopen.openDate.day);
        }
        printf("\x1B[0m");
    }
}

int dateComparatorenator(date expirationDate, date openedDate) {
    int i;
    date soonToExpireDate = openedDate;

    for(i = 1; i <= 2; i++) {
        tomorrow(&soonToExpireDate);
    }

    if((expirationDate.year > openedDate.year) ||
       (expirationDate.year == openedDate.year && expirationDate.month > openedDate.month) ||
       (expirationDate.year == openedDate.year && expirationDate.month == openedDate.month && expirationDate.day >= openedDate.day)) {
        if((expirationDate.year > soonToExpireDate.year) ||
           (expirationDate.year == soonToExpireDate.year && expirationDate.month > soonToExpireDate.month) ||
           (expirationDate.year == soonToExpireDate.year && expirationDate.month == soonToExpireDate.month && expirationDate.day > soonToExpireDate.day)) {
            return 1;
        }
        else {
            return 0;
        }
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

                     {{"Bread_flour", 300}, {"Instant_yeast", 3.1},
                     {"Salt", 5.69}, {"Olive_oil", 13.69}, {"Passata", 95.1},
                     {"Dried_basil", 2}, {"Garlic", 4}, {"Mozzarella", 125},
                     {"Parmesan", 10}, {"Cherry_tomatoes", 85}},

                     "db/recipes/pizza/instructions.txt"};
    
    Recipes lasagne = {"Lasagne",

                      {{"Onion", 200}, {"Garlic", 24},
                      {"Ground_beef", 400}, {"Oregano", 2}, {"Thyme", 1},
                      {"Carrots", 260}, {"Celery", 300}, {"Squash", 280},
                      {"Tomato_puree", 55}, {"Chopped_tomatoes", 800},
                      {"Vegetable_broth", 100}, {"Olive_oil", 30},
                      {"Butter", 28.35}, {"Wheat_flour", 15}, {"Milk", 300},
                      {"Nutmeg", 1}, {"Mozzarella", 250}, {"Lasagne_Plates", 200}},

                      "db/recipes/lasagne/instructions.txt"};
    
    Recipes burningLove = {"Burning Love",

                          {{"Potatoes", 600}, {"Milk", 100},
                          {"Butter", 25}, {"Salt", 2}, {"Bacon", 200},
                          {"Onion", 200}, {"Pickled_beetroots", 100}},

                          "db/recipes/burninglove/instructions.txt"};
    
    Recipes meatLoaf = {"Meat Loaf",

                       {{"Chopped_lambmeat", 500}, {"Onion", 200},
                       {"Garlic", 12}, {"Milk", 100}, {"Cream", 47.5},
                       {"Egg", 100}, {"Oats", 90}, {"Thyme", 18}, {"Rosemary", 6},
                       {"Dried_tomatoes", 20}, {"Black_olives", 50}, {"Feta", 75},
                       {"Baby_potatoes", 800}, {"Oliveoil", 15}, {"Butter", 14},
                       {"Salt_&_pepper", 1}},

                       "db/recipes/meatloaf/instructions.txt"};  

    Recipes ricePudding = {"Rice_pudding", {{"Porridge_rice", 484.38}, {"Water", 100},
                      {"Milk", 2000}, {"Salt", 3}, {"Cinnamon", 10},
                      {"Sugar", 16}, {"Butter", 20}}, "db/recipes/ricepudding/instructions.txt"};

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
    printf("    %s recipe for 4 people\n", recipe.name);
    printf("  -------------------------------------\n");
    printf("              INGREDIENTS\n");
    printf("  -------------------------------------\n");
    /*Prints each ingredient for the chosen recipe*/
    for(i = 0 ; i < sizeof(recipe.ingredients) / sizeof(recipe.ingredients[0]) ; i++){
        if (strcmp(recipe.ingredients[i].name, "\0")){
            if(colourization(fridgeContent, recipe.ingredients[i].name, recipe.ingredients[i].weight) == 0){
                printf("\033[31;1m");
            } else {
                printf("\033[0;32m");
            }
            printf("    %s:", recipe.ingredients[i].name);
            for (j = 0; j < 20 - strlen(recipe.ingredients[i].name); j++) {
                printf(" ");
            }
            printf("%.2fg\n\n", recipe.ingredients[i].weight);
        }
    }
    printf("\x1B[0m");
    printf("  -------------------------------------\n");
    printf("              INSTRUCTIONS\n");
    printf("  -------------------------------------\n");
    printInstructions(recipe);
    returnMenu("Recipes", fridgeContent);
}

int colourization(ingredients *fridgeContent, char *ingredientName, double neededWeight){
    int i;
    for(i = 0; i < FRIDGESIZE; i++){
        if(strcmp(ingredientName, fridgeContent[i].name) == 0) {
            if(fridgeContent[i].weight < neededWeight){
                return(0);
            }
            if(dateComparatorenator(fridgeContent[i].expirationDate, fridgeContent[i].open.isopen.openDate) == -1) {
                return(0);
            }
            return(1);
        }
    }   
    return(0);
} 
    /* |||| TO DO ||||*/
    /* - Lav en for loop til at sammenligne weight
       - Lav en for loop til at sammenligne dato 
       - Lav farverne */

    /* |||| FARVER |||| */ 
    /*for(itemNumber = 0; itemNumber < FRIDGESIZE; itemNumber++) {
    if(dateComparatorenator(fridgeContent[itemNumber].expirationDate, fridgeContent[itemNumber].openedDate) < 0) {
        printf("\033[31;1m");
    }
    else if(dateComparatorenator(fridgeContent[itemNumber].expirationDate, fridgeContent[itemNumber].openedDate) > 0) {
        printf("\033[0;32m");
    }
    else {
        printf("\033[33;1m");
    }    
}*/

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
