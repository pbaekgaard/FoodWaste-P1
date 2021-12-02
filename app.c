#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "app.h"


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
            clearScreen();
            exit(EXIT_SUCCESS);
            break;
        case 'F': case 'f':
            tomorrow(&todayDate);
            mainMenu(fridgeContent);
            break;
    }
}

/* Sorts the items the in fridge using qsort from stdlib.h */
void sortContent(ingredients *fridgeContent) {
    qsort(fridgeContent, fridgeSize, sizeof(fridgeContent[0]), contentCompare);
}

/* Compare function used in qsort. Compares expiration dates */
int contentCompare(const void *content1, const void *content2) {
    date dateContent1 = ((ingredients *)content1)->expirationDate;
    date dateContent2 = ((ingredients *)content2)->expirationDate;

    /* Returns 1 if dateContent1 expires after dateContent2 */
    if((dateContent1.year > dateContent2.year) ||
       (dateContent1.year == dateContent2.year && dateContent1.month > dateContent2.month) ||
       (dateContent1.year == dateContent2.year && dateContent1.month == dateContent2.month &&
       dateContent1.day > dateContent2.day)) {
        return 1;
    }
    /* Returns 0 if dateContent1 and dateContent 2 expire on the same day */
    else if(dateContent1.year == dateContent2.year && dateContent1.month == dateContent2.month &&
            dateContent1.day == dateContent2.day) {
        return 0;
    }
    /* Returns -1 if dateContent1 expires before dateContent2 */
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
    printFridgeContents(fridgeContent);

    printf("\nWhich ingredient do you want to change? (press 'S' to search, 'N' to add an ingredient, or 'R' to return):\n");

    do{
        /* If fridge is empty */
        if(fridgeSize == 0) {
            clearScreen();
            printf("Your fridge is empty\n");
            printf("Press 'R' to return or 'N' to add an ingredient:\n");
        }
        /* Makes sure the user inputs a valid number */
        else if(ingredientNumber <= 0 || ingredientNumber > fridgeSize) {
            clearScreen();
            printFridgeContents(fridgeContent);
            printf("\nPlease enter a valid ingredient number, type 'S' to search,'N' to add an ingredient, or 'R' to return:\n");

        }
        scanf(" %s", choice);
        ingredientNumber = atoi(choice);
        /* Return to main menu if user presses 'R' */
        if(strcmp(choice, "r") == 0 || strcmp(choice, "R") == 0){
            mainMenu(fridgeContent);
        }
        else if(strcmp(choice, "n") == 0 || strcmp(choice, "N") == 0){
            addIngredient(fridgeContent);
        }
        else if(strcmp(choice, "s") == 0 || strcmp(choice, "S") == 0){
            search(fridgeContent, &ingredientNumber);
        }
    } while (ingredientNumber <= 0 || ingredientNumber > fridgeSize);
    editIngredient(fridgeContent, ingredientNumber - 1);
}

void printFridgeContents(ingredients *fridgeContent) {
    int itemNumber;
    printf("Your fridge contains\n");
    for(itemNumber = 0; itemNumber < fridgeSize; itemNumber++) {
        printIngredient(fridgeContent, itemNumber);
    }
}

void search(ingredients *fridgeContent, int *ingredientNumber) {
    char choice[2];

    clearScreen();
    printFridgeContents(fridgeContent);
    searchIngredient(fridgeContent);
    do{
        printf("\nWhich ingredient do you want to change? (press 'S' to search, 'N' to add an ingredient, or 'R' to return):\n");
        scanf(" %s", choice);
        *ingredientNumber = atoi(choice);
        /* Return to main menu if user presses 'R' */
        if(strcmp(choice, "r") == 0 || strcmp(choice, "R") == 0){
            mainMenu(fridgeContent);
        }
        else if(strcmp(choice, "s") == 0 || strcmp(choice, "S") == 0){
            search(fridgeContent, ingredientNumber);
        }
        else if(strcmp(choice, "n") == 0 || strcmp(choice, "N") == 0){
            addIngredient(fridgeContent);       
        }
    } while(*ingredientNumber <= 0 || *ingredientNumber > fridgeSize);
}

void searchIngredient(ingredients *fridgeContent) {
    char searchTerm[20], ingredientName[20];
    int i, j, hasFound = FALSE;

    printf("What would you like to search for?\n");
    scanf(" %s", searchTerm);

    /*Convert the searched term to all lowercase*/
    for(j = 0; j < strlen(searchTerm); j++) {
        searchTerm[j] = tolower(searchTerm[j]);
    }

    for(i = 0; i < fridgeSize; i++) {  

        strcpy(ingredientName, fridgeContent[i].name);

        /*Convert the ingredient name to all lowercase*/
        for(j = 0; j < strlen(ingredientName); j++) {
            ingredientName[j] = tolower(ingredientName[j]);
        }

        if(strstr(ingredientName, searchTerm) != NULL) {
            printIngredient(fridgeContent, i);
            hasFound = TRUE;
        }
    }
    if(hasFound == FALSE){
        clearScreen();
        printFridgeContents(fridgeContent);
        printf("No matches was found for %s.\n", searchTerm);
    }
}

void printIngredient(ingredients *fridgeContent, int itemNumber) {
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
    
    printIngType(fridgeContent, itemNumber);
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

void printIngType(ingredients *fridgeContent, int itemNumber) {
    int i;

    for (i = 0; i < 20 - strlen(fridgeContent[itemNumber].name); i++){
        printf(" ");
    }
    printf("%s", fridgeContent[itemNumber].ingredientType);
}

void printWeight(ingredients *fridgeContent, int itemNumber) {
    int i;
    
    if(fridgeContent[itemNumber].weight == UNKNOWN) {
        for (i = 0; i < 13 - strlen(fridgeContent[itemNumber].ingredientType); i++){
            printf(" ");
        }
    }
    else if(fridgeContent[itemNumber].weight < 10 && fridgeContent[itemNumber].weight > 0) {
        for(i = 0; i < 15 - strlen(fridgeContent[itemNumber].ingredientType); i++) {
            printf(" ");
        } 
    } 
    else if(fridgeContent[itemNumber].weight < 100 && fridgeContent[itemNumber].weight >= 10) {
        for(i = 0; i < 14 - strlen(fridgeContent[itemNumber].ingredientType); i++) {
            printf(" ");
        }           
    }
    else if(fridgeContent[itemNumber].weight < 1000 && fridgeContent[itemNumber].weight >= 100) {
        for(i = 0; i < 13 - strlen(fridgeContent[itemNumber].ingredientType); i++) {
            printf(" ");
        }           
    }
    else if(fridgeContent[itemNumber].weight < 10000 && fridgeContent[itemNumber].weight >= 1000) {
        for(i = 0; i < 12 - strlen(fridgeContent[itemNumber].ingredientType); i++) {
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
    while(fridgeContent[fridgeSize - 1].weight <= 0) {
        printf("Please type a valid weight: ");
        scanf(" %lf", &fridgeContent[fridgeSize - 1].weight);
    }

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
    
    updateExpDates(fridgeContent);
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
    flushInput();
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
    double tempWeight;
    printf("\nPlease type the new weight: ");
    scanf(" %lf", &tempWeight);
    flushInput();
    while(tempWeight < 0) {
        printf("\nPlease type a valid weight: ");
        scanf(" %lf", &tempWeight);
    }
    fridgeContent[ingredientNumber].weight = tempWeight;
    deleteIngredient(fridgeContent, ingredientNumber);
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
    char choice;

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

void deleteIngredient(ingredients *fridgeContent, int ingredientNumber) {
    ingredients temp;
    if(fridgeContent[ingredientNumber].weight == 0) {
        temp = fridgeContent[fridgeSize - 1];
        fridgeContent[fridgeSize - 1] = fridgeContent[ingredientNumber];
        fridgeContent[ingredientNumber] = temp;
        fridgeContent = (ingredients *) realloc(fridgeContent, sizeof(ingredients) * --fridgeSize);
        sortContent(fridgeContent);
        contents(fridgeContent);
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

void recipeMenu(ingredients *fridgeContent) {
    int recipeKind = 1;
    char choice[1];
    Recipes vegetarianRecipes[VEGETARIANNUMBER]; 
    Recipes lowCarbRecipes[LOWCARBNUMBER]; 
    Recipes lowCalorieRecipes[LOWCALORIENUMBER]; 
    Recipes highProteinRecipes[HIGHPROTEINNUMBER];
    Recipes uncategorizedRecipes[UNCATEGORIZEDNUMBER]; 
    Recipes allRecipes[NUMBEROFRECIPES];
    
    makeRecipes(fridgeContent, vegetarianRecipes, lowCarbRecipes, lowCalorieRecipes, highProteinRecipes, uncategorizedRecipes, allRecipes);

    clearScreen();
    /*printRecipeList(recipeList, fridgeContent);*/
    printRecipeTypes(vegetarianRecipes, lowCarbRecipes, lowCalorieRecipes, highProteinRecipes, allRecipes, fridgeContent);
    printf("\nWhich recipes do you want to see? (press R to return to the main menu):\n");

    do{
        /*Makes sure the user inputs a valid number*/
        if(recipeKind <= 0 || recipeKind > NUMBEROFRECIPES) {
            printRecipeTypes(vegetarianRecipes, lowCarbRecipes, lowCalorieRecipes, highProteinRecipes, allRecipes, fridgeContent);
            printf("\nPlease enter a valid Recipe Number. Or type 'R' to Return:\n");
        }
        scanf(" %s", choice);
        recipeKind = atoi(choice);
        /*Return to main menu if user presses 'R'*/
        if(strcmp(choice, "r") == 0 || strcmp(choice, "R") == 0){
            mainMenu(fridgeContent);
        }
    } while (recipeKind <= 0 || recipeKind > NUMBEROFRECIPES);

    switch (recipeKind){
    case vegetarian:
        RecipeList(vegetarianRecipes, fridgeContent, VEGETARIANNUMBER);
        break;
    case lowCarb:
        RecipeList(lowCarbRecipes, fridgeContent, LOWCARBNUMBER);
        break;
    case lowCalorie:
        RecipeList(lowCalorieRecipes, fridgeContent, LOWCALORIENUMBER);
        break;
    case highProtein:
        RecipeList(highProteinRecipes, fridgeContent, HIGHPROTEINNUMBER);
        break;
    default:
        RecipeList(allRecipes, fridgeContent, NUMBEROFRECIPES);
        break;
    }
}

void printRecipeTypes (Recipes* vegetarian, Recipes* lowCarb, Recipes* lowCalorie, Recipes* highProtein, Recipes* allRecipes, ingredients * fridgeContent){
    clearScreen();
    printf("Your recipe types:\n");
    printf("----------------------------\n");
    printf("1. All recipes\n");
    printf("----------------------------\n");
    colorForRecipeType (lowCarb, fridgeContent, LOWCARBNUMBER);
    printf("2. Low carb\n");
    colorForRecipeType (lowCalorie, fridgeContent, LOWCALORIENUMBER);
    printf("3. Low calorie\n");
    colorForRecipeType (highProtein, fridgeContent, HIGHPROTEINNUMBER);
    printf("4. High protein\n");
    colorForRecipeType (vegetarian, fridgeContent, VEGETARIANNUMBER);
    printf("5. Vegetarian\n");

    
    printf(WHITE);
}

void colorForRecipeType (Recipes *recipe, ingredients *fridgeContent, int numberOfRecipes){
    int i;
    for (i = 1; i <= numberOfRecipes; i++){
        if (colorForRecipe(i, recipe, fridgeContent) == 1){
            printf(GREEN);
            break;
        }
        else{
            printf(RED);
        }
    }
}
void RecipeList(Recipes *recipe, ingredients *fridgeContent, int numberOfRecipes){
    char choice[1];
    int recipeNumber = 1;
    clearScreen();
    printRecipeList(recipe, fridgeContent, numberOfRecipes);
    printf("\nWhich recipe do you want to see? (press R to return to recipes):\n");
    do{
        /*Makes sure the user inputs a valid number*/
        if(recipeNumber <= 0 || recipeNumber > numberOfRecipes) {
            clearScreen();
            printRecipeList(recipe, fridgeContent, numberOfRecipes);
            printf("\nPlease enter a valid Recipe Number. Or type 'R' to return to recipes:\n");
        }
        scanf(" %s", choice);
        recipeNumber = atoi(choice);
        /*Return to recipe menu if user presses 'R'*/
        if(strcmp(choice, "r") == 0 || strcmp(choice, "R") == 0){
            recipeMenu(fridgeContent);
        }
    } while (recipeNumber <= 0 || recipeNumber > numberOfRecipes);
    openRecipe(recipe[recipeNumber - 1], fridgeContent);
}

void printRecipeList(Recipes *recipe, ingredients *fridgeContent, int numberOfRecipes){
    int i;

    printf("This is a list of the recipes in your cookbook\n");
    /*Print the list of recipes of the given type*/
    for(i = 1; i <= numberOfRecipes; i++){
        colorForRecipe(i, recipe, fridgeContent);
        printf("%d. %s\n", i, recipe[i - 1].name);
    }
    printf(WHITE);
}

int colorForRecipe (int i, Recipes *recipe, ingredients *fridgeContent){
    int j, k, numberOfIngredients = 0, isGreen = TRUE;
    for (j = 0 ; j < MAXINGREDIENTS ; j++){
        if(strcmp(recipe[i - 1].fridgeIngredients[j].name, "\0") == 0){
            break;
        }
        else{
            numberOfIngredients++;
        } 
    }
    for(k = 0 ; k < (numberOfIngredients) ; k++){
        if(colourization(fridgeContent, recipe[i-1].fridgeIngredients[k].name, recipe[i-1].fridgeIngredients[k].weight) == 0){
            printf(RED);
            isGreen = FALSE;
            break;
        }
        printf(GREEN);  
    }
    return isGreen;
}

void openRecipe(Recipes recipe, ingredients *fridgeContent){
    int i;
    int j;
    char choice;
    clearScreen();
    printf("  -------------------------------------\n");
    printf("          Category: %s\n", recipe.recipeType);
    printf("    %s recipe for 4 people\n", recipe.name);
    printf("  -------------------------------------\n");
    printf("              INGREDIENTS\n");
    printf("  -------------------------------------\n");
    /*Prints each ingredient for the chosen recipe*/
    printf(WHITE);
    printf("Ingredients that are not in the fridge is assumed that you own: \n");

    for(i = 0 ; i < MAXINGREDIENTS ; i++){
        if(strcmp(recipe.notFridgeIngredients[i].name, "\0")){
            printf("    %s:", recipe.notFridgeIngredients[i].name);
            for (j = 0; j < 20 - strlen(recipe.notFridgeIngredients[i].name); j++){
                printf(" ");
            }
            printf("%.2fg\n\n", recipe.notFridgeIngredients[i].weight);
        }
    }
    
    
    printf("Ingredients that are in the fridge: \n");
    
    for(i = 0 ; i < MAXINGREDIENTS ; i++){
        if(strcmp(recipe.fridgeIngredients[i].name, "\0")){
            if(colourization(fridgeContent, recipe.fridgeIngredients[i].name, recipe.fridgeIngredients[i].weight) == 0){
                printf(RED);
            } else {
                printf(GREEN);
            }
            printf("    %s:", recipe.fridgeIngredients[i].name);
            for (j = 0; j < 20 - strlen(recipe.fridgeIngredients[i].name); j++){
                printf(" ");
            }
            printf("%.2fg\n\n", recipe.fridgeIngredients[i].weight);
        }
    }
    printf(WHITE);
    printf("  -------------------------------------\n");
    printf("              INSTRUCTIONS\n");
    printf("  -------------------------------------\n");
    printInstructions(recipe);
  
    printf("\n\nR - Return to Recipes\n");
    do {
        scanf(" %c", &choice);
        flushInput();
        if(choice == 'R' || choice == 'r'){
            recipeMenu(fridgeContent);    
        }
    } while(!(choice == 'R' || choice == 'r'));
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

void clearScreen(void) {
    system("@cls||clear");
}

void flushInput(void) {
    char flush;
    while((flush = getchar()) != '\n');
}
