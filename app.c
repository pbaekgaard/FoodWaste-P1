#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "app.h"

/* Definition of variable */
date todayDate;

int main(void) {
    ingredients *fridgeContent = (ingredients *) calloc(1, sizeof(ingredients));
    if(fridgeContent == NULL) {
        printf("Couldn't allocate memory!!");
        exit(EXIT_FAILURE);
    }
    fridgeContent = (ingredients *) realloc(fridgeContent, sizeof(ingredients) * getFridgeSize(fridgeContent));
    if(fridgeContent == NULL) {
        printf("Couldn't re-allocate memory");
        exit(EXIT_FAILURE);
    }

    todayDate = makeDayToday(); /*Global variable*/
    getFridgeContents(fridgeContent);
    updateExpDates(fridgeContent);
    sortContent(fridgeContent);
    mainMenu(fridgeContent);

    return EXIT_FAILURE;
}

/* updates all expiration dates, if they have been opened*/
void updateExpDates (ingredients *fridgeContent){
    int i, j;
    date openExp;
    /*for loop that runs through every element of the fridgeContent*/ 
    for ( i = 0; i < fridgeSize; i++){ 
        /* Checks if expiration date is known */
        if((fridgeContent[i].expirationDate.day != UNKNOWN && fridgeContent[i].expirationDate.month != UNKNOWN && fridgeContent[i].expirationDate.year != UNKNOWN)) {
            /* checks if product is open*/
            if(fridgeContent[i].open.opened == TRUE){
                openExp = fridgeContent[i].open.isopen.openDate;
                /* calculates new expirationdate. Adds how long the product is fresh after opening to the date opened*/
                for ( j = 0; j < fridgeContent[i].open.isopen.daysAfterOpen; j++){
                    tomorrow(&openExp); 
                }
                /*If the original exp date is larger than the updated exp date after opening. Updates the exp date.*/
                if(dateComparatorenator(fridgeContent[i].expirationDate, openExp) == 1 || dateComparatorenator(fridgeContent[i].expirationDate, openExp) == 0){
                    fridgeContent[i].expirationDate.year = openExp.year;
                    fridgeContent[i].expirationDate.month = openExp.month;
                    fridgeContent[i].expirationDate.day = openExp.day;
                }
            }
        }    
    } 
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
        printf("---------------------\n\n");
        printNotifications(fridgeContent);
        scanf(" %c", &choice);
        flushInput();
        
        if(choice == '1' || choice == '2' || choice == 'Q' || choice == 'q' || choice == 'F' || choice == 'f') {
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
            free(fridgeContent);
            exit(EXIT_SUCCESS);
            break;
        case 'F': case 'f':
            tomorrow(&todayDate);
            mainMenu(fridgeContent);
            break;
    }
}

void sortContent(ingredients *fridgeContent) {
    qsort(fridgeContent, fridgeSize, sizeof(fridgeContent[0]), contentCompare);
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
    for(i = 0; i < fridgeSize; i++) {
        if(dateComparatorenator(fridgeContent[i].expirationDate, todayDate) == 0) {
            printf("###########################\n");            
            printf("         EXPIRING         \n");
            printf("---------------------------\n");
            break;
        }
    }

    for (i = 0; i < fridgeSize; i++) {
        if(dateComparatorenator(fridgeContent[i].expirationDate, todayDate) == 0) {
            printf(YELLOW);
            printf("%s IS EXPIRING\n", fridgeContent[i].name);
            printf(WHITE);
        } 
    }
    printf("\n\n###########################\n");

    /*EXPIRED*/

    for(i = 0; i < fridgeSize; i++) {
        if(!(fridgeContent[i].expirationDate.day == UNKNOWN || fridgeContent[i].expirationDate.month == UNKNOWN || fridgeContent[i].expirationDate.year == UNKNOWN) 
            && dateComparatorenator(fridgeContent[i].expirationDate, todayDate) == -1) {
            printf("         EXPIRED\n");
            printf("---------------------------\n");
            break;
        }
    }
    for (i = 0; i < fridgeSize; i++) {
        if(!(fridgeContent[i].expirationDate.day == UNKNOWN || fridgeContent[i].expirationDate.month == UNKNOWN || fridgeContent[i].expirationDate.year == UNKNOWN) 
            && dateComparatorenator(fridgeContent[i].expirationDate, todayDate) == -1) {
            printf(RED);
            printf("%s HAS EXPIRED\n", fridgeContent[i].name);
            printf(WHITE);
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

  if(year % 400 == 0) result = 1;
  else if(year % 100 == 0) result = 0;
  else if(year % 4 == 0) result = 1;
  else result = 0;

  return result;
}

void contents(ingredients *fridgeContent) {
    int ingredientNumber = 1;
    char choice[2];
    clearScreen();
    printf("Your fridge contains\n");
    printFridgeContents(fridgeContent);

    printf("\nWhich ingredient do you want to change? (press 'R' to return or 'N' to add an ingredient):\n");

    do{
        /*Makes sure the user inputs a valid number*/
        if(ingredientNumber <= 0 || ingredientNumber > fridgeSize) {
            clearScreen();
            printf("Your fridge contains\n");
            printFridgeContents(fridgeContent);
            printf("\nPlease enter a valid ingredient number, type 'R' to Return, or 'N' to add an ingredient:\n");
        }
        scanf(" %s", choice);
        ingredientNumber = atoi(choice);
        /*Return to main menu if user presses 'R'*/
        if(strcmp(choice, "r") == 0 || strcmp(choice, "R") == 0){
            mainMenu(fridgeContent);
        }
        else if(strcmp(choice, "n") == 0 || strcmp(choice, "N") == 0){
            addIngredient(fridgeContent);
        }
    } while (ingredientNumber <= 0 || ingredientNumber > fridgeSize);
    editIngredient(fridgeContent, ingredientNumber - 1);
}

void printFridgeContents(ingredients *fridgeContent) {
    int itemNumber;

    for(itemNumber = 0; itemNumber < fridgeSize; itemNumber++) {
        printColour(fridgeContent, itemNumber);
        
        if(strcmp(fridgeContent[itemNumber].name, "-1") == 0) {
            strcpy(fridgeContent[itemNumber].name, "?????????");
        }
        if (itemNumber + 1 < 10) {
            printf("  %d - %s", itemNumber + 1, fridgeContent[itemNumber].name);
        }
        else {
            printf(" %d - %s", itemNumber + 1, fridgeContent[itemNumber].name);
        }

        printWeight(fridgeContent, itemNumber);
        printExpirationDate(fridgeContent, itemNumber);
        printOpenedDate(fridgeContent, itemNumber);

        if(!(fridgeContent[itemNumber].open.isopen.daysAfterOpen == UNKNOWN)){
            printf("Shelf time after opening is %d days\n", fridgeContent[itemNumber].open.isopen.daysAfterOpen);
        }
        else{
            printf("Shelf time after opening is UNKNOWN\n");
        }
        printf(WHITE);
    }
}

void printColour(ingredients *fridgeContent, int itemNumber) {
    if(fridgeContent[itemNumber].expirationDate.day == UNKNOWN || fridgeContent[itemNumber].expirationDate.month == UNKNOWN ||
       fridgeContent[itemNumber].expirationDate.year == UNKNOWN || strcmp(fridgeContent[itemNumber].name, "-1") == 0 ||
       fridgeContent[itemNumber].weight == UNKNOWN || fridgeContent[itemNumber].open.opened == UNKNOWN ||
       (fridgeContent[itemNumber].open.opened == FALSE && (fridgeContent[itemNumber].open.isopen.openDate.day == UNKNOWN ||
       fridgeContent[itemNumber].open.isopen.openDate.month == UNKNOWN || fridgeContent[itemNumber].open.isopen.openDate.year == UNKNOWN ||
       fridgeContent[itemNumber].open.isopen.daysAfterOpen == UNKNOWN))) {
        printf(PURPLE);
    }
    else if(dateComparatorenator(fridgeContent[itemNumber].expirationDate, todayDate) == -1) {
        printf(RED);
    }
    else if(dateComparatorenator(fridgeContent[itemNumber].expirationDate, todayDate) == 1) {
        printf(GREEN);
    }
    else {
        printf(YELLOW);
    }
}

void printWeight(ingredients *fridgeContent, int itemNumber) {
    int i;
    if(fridgeContent[itemNumber].weight == UNKNOWN) {
        for (i = 0; i < 23 - strlen(fridgeContent[itemNumber].name); i++){
            printf(" ");
        }
    }
    else if(fridgeContent[itemNumber].weight < 10 && fridgeContent[itemNumber].weight > 0) {
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

    if(fridgeContent[itemNumber].weight > 0){
        printf("%.2f g", fridgeContent[itemNumber].weight);           
    }
    else {
        printf("????????");
    }
}

void printExpirationDate(ingredients *fridgeContent, int itemNumber) {

    printf("   Expiration date: ");
    if(!(fridgeContent[itemNumber].expirationDate.day == UNKNOWN || fridgeContent[itemNumber].expirationDate.month == UNKNOWN || fridgeContent[itemNumber].expirationDate.year == UNKNOWN)) {
        printDate(fridgeContent, itemNumber);            
    }
    else printf("???\?/?\?/??");

    if(!(fridgeContent[itemNumber].expirationDate.day == UNKNOWN || fridgeContent[itemNumber].expirationDate.month == UNKNOWN || fridgeContent[itemNumber].expirationDate.year == UNKNOWN)) {
        if(fridgeContent[itemNumber].expirationDate.month < 10) {
           printf(" ");
        } 
        if(fridgeContent[itemNumber].expirationDate.day < 10) {
           printf(" ");
        }         
    }
}

void printOpenedDate(ingredients *fridgeContent, int itemNumber) {
    if(!(fridgeContent[itemNumber].open.opened == UNKNOWN)) {
        if(fridgeContent[itemNumber].open.opened == FALSE) {
            printf("   UNOPENED                ");
        }
        else {
            if(!(fridgeContent[itemNumber].open.isopen.openDate.day == UNKNOWN || fridgeContent[itemNumber].open.isopen.openDate.month == UNKNOWN || fridgeContent[itemNumber].open.isopen.openDate.year == UNKNOWN)) {
                printf("   Opened on: %d/%d/%d   ", fridgeContent[itemNumber].open.isopen.openDate.year, fridgeContent[itemNumber].open.isopen.openDate.month, fridgeContent[itemNumber].open.isopen.openDate.day);
                
                if(fridgeContent[itemNumber].open.isopen.openDate.month < 10) {
                    printf(" ");
                } 
                if(fridgeContent[itemNumber].open.isopen.openDate.day < 10) {
                    printf(" ");
                }
            }
            else
                printf("   Opened on: ???\?/?\?/??   ");
        }            
    }
    else {
        printf("   N/A                     ");
    }
}

void addIngredient(ingredients *fridgeContent) {
    char opened;
    fridgeContent = (ingredients *) realloc(fridgeContent, sizeof(ingredients) * ++fridgeSize);
    if(fridgeContent == NULL) {
        printf("Couldn't re-allocate memory");
        exit(EXIT_FAILURE);
    }

    clearScreen();
    printf("What is the name of the ingredient?\n");
    scanf(" %s", fridgeContent[fridgeSize - 1].name);
    flushInput();

    printf("What is the weight of the ingredient in grams?\n");
    scanf("%lf", &fridgeContent[fridgeSize - 1].weight);

    newIngredientExpirationDate(fridgeContent);
    
    printf("Is the ingredient opened? (y/n)\n");
    while(opened != 'y' && opened != 'Y' && opened != 'n' && opened != 'N') {
        scanf(" %c", &opened);
        flushInput();
    }
    if(opened == 'y' || opened == 'Y') {
        fridgeContent[fridgeSize - 1].open.opened = 1;
    }
    else if(opened == 'n' || opened == 'N') {
        fridgeContent[fridgeSize - 1].open.opened = 0;
    }

    if(fridgeContent[fridgeSize - 1].open.opened == 1) {
        newIngredientOpenedDate(fridgeContent);
    }
    printf("How many days can the ingredient last after being opened?\n");
    scanf("%d", &fridgeContent[fridgeSize - 1].open.isopen.daysAfterOpen);
    
    sortContent(fridgeContent);
    contents(fridgeContent);
}

void newIngredientExpirationDate(ingredients *fridgeContent) {
    int tempDay = 0, tempMonth = 0, tempYear = 0;

    printf("What is the expiration date of the ingredient? (yyyy/mm/dd)\n");
    scanf("%d/%d/%d", &tempYear, &tempMonth, &tempDay);

    while(tempDay > 31 ||
          ((tempMonth == 4 || tempMonth == 6 || tempMonth == 9 || tempMonth == 11) && tempDay > 30) ||
          (leapYear(tempYear) == 1 && tempMonth == 2 && tempDay > 28) ||
          (tempMonth == 2 && tempDay > 29) || tempMonth < 1 || tempMonth > 12 || tempDay < 1){
        
        flushInput();
        printf("Please type a valid date!\n(yyyy/mm/dd): ");
        scanf(" %d/%d/%d", &tempYear, &tempMonth, &tempDay);
    }
    fridgeContent[fridgeSize - 1].expirationDate.year = tempYear;
    fridgeContent[fridgeSize - 1].expirationDate.month = tempMonth;
    fridgeContent[fridgeSize - 1].expirationDate.day = tempDay;
}

void newIngredientOpenedDate(ingredients *fridgeContent) {
    int tempDay = 0, tempMonth = 0, tempYear = 0;

    printf("When was the ingredient opened? (yyyy/mm/dd)\n");
    scanf("%d/%d/%d", &tempYear, &tempMonth, &tempDay);

    while(tempDay > 31 ||
          ((tempMonth == 4 || tempMonth == 6 || tempMonth == 9 || tempMonth == 11) && tempDay > 30) ||
          (leapYear(tempYear) == 1 && tempMonth == 2 && tempDay > 28) ||
          (tempMonth == 2 && tempDay > 29) || tempMonth < 1 || tempMonth > 12 || tempDay < 1){
        
        flushInput();
        printf("Please type a valid date!\n(yyyy/mm/dd): ");
        scanf(" %d/%d/%d", &tempYear, &tempMonth, &tempDay);
    }
    fridgeContent[fridgeSize - 1].open.isopen.openDate.year = tempYear;
    fridgeContent[fridgeSize - 1].open.isopen.openDate.month = tempMonth;
    fridgeContent[fridgeSize - 1].open.isopen.openDate.day = tempDay;
}

void editIngredient(ingredients *fridgeContent, int ingredientNumber) {
    char choice;
    char* openStatus;
    clearScreen();
    if(fridgeContent[ingredientNumber].open.opened == 1) {
        openStatus = "IN THE STATE OF OPENEDNESS";
    }
    else if (fridgeContent[ingredientNumber].open.opened == 0){
        openStatus = "IN THE STATE OF NOT IN THE STATE OF OPENEDNESS";
    }
    printf("EDITING %s\n\n", fridgeContent[ingredientNumber].name);
    printf("1. Name            : %s\n", fridgeContent[ingredientNumber].name);
    printf("2. Weight          : %0.2f\n", fridgeContent[ingredientNumber].weight);
    printf("3. Expiration Date : %d/%d/%d\n", fridgeContent[ingredientNumber].expirationDate.year, fridgeContent[ingredientNumber].expirationDate.month, fridgeContent[ingredientNumber].expirationDate.day);
    printf("4. Opened Status   : %s\n", openStatus);
    printf("\n\n");
    printf("# - EDIT         R - RETURN\n");
    scanf(" %c", &choice);
    switch (choice) {
        case '1': 
            changeName(fridgeContent, ingredientNumber);
            break;
        case '2': 
            changeWeight(fridgeContent, ingredientNumber);
            break;
        case '3': 
            changeDate(fridgeContent, ingredientNumber);
            break;
        case '4': 
            changeOpenedState(fridgeContent, ingredientNumber);
            break;
        case 'r': case 'R':
            sortContent(fridgeContent);
            contents(fridgeContent);
        default:
            break;
    }
    editIngredient(fridgeContent, ingredientNumber);
}

void changeName(ingredients *fridgeContent, int ingredientNumber) {
    printf("\nPlease type the new name: ");
    scanf(" %s", fridgeContent[ingredientNumber].name);
    flushInput();
}

void changeWeight(ingredients *fridgeContent, int ingredientNumber) {
    printf("\nPlease type the new weight: ");
    scanf(" %lf", &fridgeContent[ingredientNumber].weight);
    flushInput();
}

void changeDate(ingredients *fridgeContent, int ingredientNumber) {
    int tempYear, tempMonth, tempDay;
    printf("\nPlease type the new date (yyyy/mm/dd): ");
    scanf(" %d/%d/%d", &tempYear, &tempMonth, &tempDay);
    while(tempDay > 31 ||
        (tempMonth <= 0 || tempDay <= 0) || ((tempMonth == 4 || tempMonth == 6 || tempMonth == 9 || tempMonth == 11) && tempDay > 30) ||
        (leapYear(tempYear) == 1 && tempMonth == 2 && tempDay > 29) ||
        (leapYear(tempYear) == 0 && tempMonth == 2 && tempDay > 28) || tempMonth < 1 || tempMonth > 12){
        
        flushInput();
        printf("Please type a valid date!\n(yyyy/mm/dd): ");
        scanf(" %d/%d/%d", &tempYear, &tempMonth, &tempDay);
    }
    fridgeContent[ingredientNumber].expirationDate.year = tempYear;
    fridgeContent[ingredientNumber].expirationDate.month = tempMonth;
    fridgeContent[ingredientNumber].expirationDate.day = tempDay;
    flushInput();
}

void changeOpenedState(ingredients *fridgeContent, int ingredientNumber) {
    printf("This will override the date. Are you sure? y/n: ");
    scanf(" %c", &choice);
    if(choice == 'y' || choice == 'Y') {
        if (fridgeContent[ingredientNumber].open.opened == 0){
            fridgeContent[ingredientNumber].open.opened = 1;
            fridgeContent[ingredientNumber].open.isopen.openDate.day = todayDate.day;
            fridgeContent[ingredientNumber].open.isopen.openDate.month = todayDate.month;
            fridgeContent[ingredientNumber].open.isopen.openDate.year = todayDate.year;
            updateExpDates(fridgeContent);
        }
        else if (fridgeContent[ingredientNumber].open.opened == 1){
            fridgeContent[ingredientNumber].open.opened = 0;
        } 
    }
    else if(choice == 'n' || choice == 'N'){
        printf("Nice bro, good choice! You're going places in your life.\n");
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
           if(strcmp(menu, "Main menu") == 0){
                mainMenu(fridgeContent);
            }
            else if(strcmp(menu, "Recipes") == 0){
                recipeMenu(fridgeContent);
            }
        }
        else if(choice == 'Q' || choice == 'q') {
            free(fridgeContent);
            exit(EXIT_SUCCESS);
        }
    } while(!(choice == 'R' || choice == 'r' || choice == 'Q' || choice == 'q'));
}

void recipeMenu(ingredients *fridgeContent) {
    int recipeNumber = 1;
    char choice[1];
    /*Declaration of different recipes*/
    Recipes pizza =  {"Pizza",

                     {{"Bread_flour", 300}, {"Instant_yeast", 3.1},
                     {"Salt", 5.69}, {"Olive_oil", 13.69}, {"Passata", 95.1},
                     {"Dried_basil", 2}, {"Garlic", 4}, {"Mozzarella", 125},
                     {"Parmesan", 10}, {"Cherry_tomatoes", 85}, {"Last_element"}},

                     "db/recipes/pizza/instructions.txt"};
    
    Recipes lasagne = {"Lasagne",

                      {{"Onion", 200}, {"Garlic", 24},
                      {"Ground_beef", 400}, {"Oregano", 2}, {"Thyme", 1},
                      {"Carrots", 260}, {"Celery", 300}, {"Squash", 280},
                      {"Tomato_puree", 55}, {"Chopped_tomatoes", 800},
                      {"Vegetable_broth", 100}, {"Olive_oil", 30},
                      {"Butter", 28.35}, {"Wheat_flour", 15}, {"Milk", 300},
                      {"Nutmeg", 1}, {"Mozzarella", 250}, {"Lasagne_Plates", 200}, {"Last_element"}},

                      "db/recipes/lasagne/instructions.txt"};
    
    Recipes burningLove = {"Burning Love",

                          {{"Potatoes", 600}, {"Milk", 100},
                          {"Butter", 25}, {"Salt", 2}, {"Bacon", 200},
                          {"Onion", 200}, {"Pickled_beetroots", 100}, {"Last_element"}},

                          "db/recipes/burninglove/instructions.txt"};
    
    Recipes meatLoaf = {"Meat Loaf",

                       {{"Chopped_lambmeat", 500}, {"Onion", 200},
                       {"Garlic", 12}, {"Milk", 100}, {"Cream", 47.5},
                       {"Egg", 100}, {"Oats", 90}, {"Thyme", 18}, {"Rosemary", 6},
                       {"Dried_tomatoes", 20}, {"Black_olives", 50}, {"Feta", 75},
                       {"Baby_potatoes", 800}, {"Oliveoil", 15}, {"Butter", 14},
                       {"Salt_&_pepper", 1}, {"Last_element"}},

                       "db/recipes/meatloaf/instructions.txt"};  

    Recipes ricePudding = {"Rice_pudding", {{"Porridge_rice", 484.38}, {"Water", 100},
                      {"Milk", 2000}, {"Salt", 3}, {"Cinnamon", 10},
                      {"Sugar", 16}, {"Butter", 20}, {"Last_element"}}, "db/recipes/ricepudding/instructions.txt"};

    Recipes recipeList[NUMBEROFRECIPES]; 
    recipeList[0] = pizza;
    recipeList[1] = lasagne;
    recipeList[2] = burningLove;
    recipeList[3] = meatLoaf;
    recipeList[4] = ricePudding;


    clearScreen();
    printRecipeList(recipeList, fridgeContent);
    printf("\nWhich recipe do you want to see? (press R to return to the main menu):\n");

    do{
        /*Makes sure the user inputs a valid number*/
        if(recipeNumber <= 0 || recipeNumber > NUMBEROFRECIPES) {
            printRecipeList(recipeList, fridgeContent);
            printf("\nPlease enter a valid Recipe Number. Or type 'R' to Return:\n");
        }
        scanf(" %s", choice);
        recipeNumber = atoi(choice);
        /*Return to main menu if user presses 'R'*/
        if(strcmp(choice, "r") == 0 || strcmp(choice, "R") == 0){
            mainMenu(fridgeContent);
        }

    } while (recipeNumber <= 0 || recipeNumber > NUMBEROFRECIPES);
    openRecipe(recipeList[recipeNumber  - 1], fridgeContent);
}

void printRecipeList(Recipes* recipeList, ingredients *fridgeContent) {
    int i, j, k, counter;
    clearScreen();
    printf("This is a list of the recipes in your cookbook\n");
    /*Print the list of recipes*/
    for(i = 1; i <= NUMBEROFRECIPES; i++){
        counter = 0;
        for (j = 0 ; j < MAXINGREDIENTS ; j++){
            if(strcmp(recipeList[i - 1].ingredients[j].name, "Last_element") == 0){
                break;
            }
            else{
                counter++;
            } 
        }
        
        for(k = 0 ; k < (counter) ; k++){
            if(colourization(fridgeContent, recipeList[i-1].ingredients[k].name, recipeList[i-1].ingredients[k].weight) == 0){
                printf(RED);
                break;
            }
            printf(GREEN);
                
        }
        printf("%d. %s\n", i, recipeList[i - 1].name);
    }
    printf(WHITE);
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
    for(i = 0 ; i < MAXINGREDIENTS ; i++){
        if(strcmp(recipe.ingredients[i].name, "\0")){
            if(colourization(fridgeContent, recipe.ingredients[i].name, recipe.ingredients[i].weight) == 0){
                printf(RED);
            } else {
                printf(GREEN);
            }
            if(strcmp(recipe.ingredients[i].name, "Last_element")){
                printf("    %s:", recipe.ingredients[i].name);
                for (j = 0; j < 20 - strlen(recipe.ingredients[i].name); j++) {
                    printf(" ");
            }
                printf("%.2fg\n\n", recipe.ingredients[i].weight);
            }
        }
    }
    printf(WHITE);
    printf("  -------------------------------------\n");
    printf("              INSTRUCTIONS\n");
    printf("  -------------------------------------\n");
    printInstructions(recipe);
    returnMenu("Recipes", fridgeContent);
}

int colourization(ingredients *fridgeContent, char *ingredientName, double neededWeight){
    int i;
    for(i = 0; i < fridgeSize; i++){
        if(strcmp(ingredientName, fridgeContent[i].name) == 0) {
            if(fridgeContent[i].weight < neededWeight){
                return 0;
            }
            if(dateComparatorenator(fridgeContent[i].expirationDate, todayDate) == -1) {
                return 0;
            }
            return 1;
        }
    }   
    return 0;
} 

void printInstructions(Recipes recipe) {
    FILE *fp = fopen(recipe.filename, "r");
    char buffer[INSTRUCTIONLINELENGTH];

    if(fp == NULL) {
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
