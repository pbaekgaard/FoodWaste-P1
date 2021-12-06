#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "app.h"


date currentDate;

int main(void) {
    /*Declaration and initialization of fridgeContent array, 
    used to store the ingredients inside the users fridge*/
    ingredients *fridgeContent = (ingredients *) calloc(getFridgeSize(), sizeof(ingredients));
    
    /*Checks if it was possible to allocate the appropriate memory for the fridgeContent array*/
    if(fridgeContent == NULL) {
        printf("Couldn't allocate memory!!");
        exit(EXIT_FAILURE);
    }
    /*Global variable*/ 
    currentDate = makeDayToday(); 

    /*Read the data from the DATABASE*/
    getFridgeContents(fridgeContent);

    /*Update the expiration dates for ingredients that are opened*/
    updateExpDates(fridgeContent);

    /*Sort the fridgeContent array in order of expiration date*/
    sortContent(fridgeContent);

    /*RUN MAIN MENU*/
    mainMenu(fridgeContent);

    /*EXIT FAILURE in case the program escapes mainMenu*/
    return EXIT_FAILURE;
}

/*Updates all expiration dates, if they have been opened*/
void updateExpDates (ingredients *fridgeContent){
    /*Declaration of variables used for, for loops*/
    int i, j;

    /*Declaration of temporary date variable to contain the opened expiration date*/
    date tempOpenExp;

    /*For loop that runs through every element of the fridgeContent*/ 
    for ( i = 0; i < fridgeSize; i++){ 
        /*Checks if expiration date is known*/
        if((fridgeContent[i].expirationDate.day != UNKNOWN && fridgeContent[i].expirationDate.month != UNKNOWN && fridgeContent[i].expirationDate.year != UNKNOWN)) {
            /*Checks if product is open*/
            if(fridgeContent[i].open.opened == TRUE){
                tempOpenExp = fridgeContent[i].open.isopen.openDate;
                /*Calculates new expirationdate. Adds how long the product is fresh after opening to the date opened*/
                for ( j = 0; j < fridgeContent[i].open.isopen.daysAfterOpen; j++){
                    tomorrow(&tempOpenExp); 
                }
                /*If the original exp date is larger than the updated exp date after opening. Updates the exp date*/
                if(dateComparatorenator(fridgeContent[i].expirationDate, tempOpenExp) == 1 || dateComparatorenator(fridgeContent[i].expirationDate, tempOpenExp) == 0){
                    fridgeContent[i].expirationDate.year = tempOpenExp.year;
                    fridgeContent[i].expirationDate.month = tempOpenExp.month;
                    fridgeContent[i].expirationDate.day = tempOpenExp.day;
                }
            }
        }    
    } 
}

/*Main Menu function to print a user-friendly menu for the user*/
void mainMenu(ingredients *fridgeContent) {
    /*Declaration of choice character variable*/
    char choice;

    /*Declaration and initialization for the flag control for the while loop*/
    int run = TRUE;

    /*While loop, which prints out the menu, and asks user for input*/
    while(run) {
        /*Clears the terminal / screen*/
        clearScreen();
        /*Prints the menu text for the terminal*/
        printf("Welcome to SmartFrAPP\n---------------------\n");
        printf("     %d/%d/%d\n\n", currentDate.year, currentDate.month, currentDate.day);
        printf("1 - Fridge Contents\n");
        printf("2 - Recipes\n");
        printf("---------------------\n");
        printf("Q - Quit\n");
        printf("F - ENTER FUTURE\n");
        printf("G - Go to date\n");
        printf("---------------------\n\n");

        /*Executing the printNotifications function*/
        printNotifications(fridgeContent);
        
        /*Scan for user-input*/
        scanf(" %c", &choice);
        
        /*Flush the input stream*/
        flushInput();
        

        /*Ends while loop when user enters valid input*/
        if(choice == '1' || choice == '2' || choice == 'Q' || choice == 'q' || 
           choice == 'F' || choice == 'f' || choice == 'G' || choice == 'g') {
            run = FALSE;

        }
    }
    /*Switch case to run the corresponding functions in relation to the user input*/
    switch(choice) {
        case '1':
            contents(fridgeContent);
            break;
        case '2':
            recipeMenu(fridgeContent);
            break;
        case 'Q': case 'q':
            /*Free the allocated memory from the fridgeContent array*/
            free(fridgeContent);
            /*Clear screen and exit the application*/
            clearScreen();
            exit(EXIT_SUCCESS);
            break;
        /*Advance the date by one day and execute the mainMenu function again*/
        case 'F': case 'f':
            tomorrow(&currentDate);
            mainMenu(fridgeContent);
            break;
        /*Go to a specific date*/
        case 'G': case 'g':
            skipToDate();
            mainMenu(fridgeContent);
            break;
    }
}

/*Sorts the items the in fridge using qsort from stdlib.h*/
void sortContent(ingredients *fridgeContent) {
    qsort(fridgeContent, fridgeSize, sizeof(fridgeContent[0]), contentCompare);
}

/*Compare function used in qsort. Compares expiration dates*/
int contentCompare(const void *content1, const void *content2) {
    date dateContent1 = ((ingredients *)content1)->expirationDate;
    date dateContent2 = ((ingredients *)content2)->expirationDate;

    /*Returns 1 if dateContent1 expires after dateContent2*/
    if((dateContent1.year > dateContent2.year) ||
       (dateContent1.year == dateContent2.year && dateContent1.month > dateContent2.month) ||
       (dateContent1.year == dateContent2.year && dateContent1.month == dateContent2.month &&
       dateContent1.day > dateContent2.day)) {
        return 1;
    }
    /*Returns 0 if dateContent1 and dateContent 2 expire on the same day*/
    else if(dateContent1.year == dateContent2.year && dateContent1.month == dateContent2.month &&
            dateContent1.day == dateContent2.day) {
        return 0;
    }
    /*Returns -1 if dateContent1 expires before dateContent2*/
    else
        return -1;
}

/*Notification function for the main menu, to print out "Soon to expire" and Expired ingredients*/
void printNotifications(ingredients *fridgeContent){
    int i = 0;
    /*PRINT SOON TO EXPIRE TITLE*/
    for(i = 0; i < fridgeSize; i++) {
        if(dateComparatorenator(fridgeContent[i].expirationDate, currentDate) == 0) {
            printf("###########################\n");            
            printf("         EXPIRING         \n");
            printf("---------------------------\n");
            break;
        }
    }

    /*For loop which goes through the fridgeContent array
      and prints out the ingredients that are close to their expiration date*/
    for (i = 0; i < fridgeSize; i++) {
        if(dateComparatorenator(fridgeContent[i].expirationDate, currentDate) == 0) {
            printf(YELLOW);
            printf("%s IS EXPIRING\n", fridgeContent[i].name);
            printf(WHITE);
        } 
    }
    printf("\n\n###########################\n");

    /*PRINT EXPIRED TITLE*/
    for(i = 0; i < fridgeSize; i++) {
        if(!(fridgeContent[i].expirationDate.day == UNKNOWN || fridgeContent[i].expirationDate.month == UNKNOWN || fridgeContent[i].expirationDate.year == UNKNOWN) 
            && dateComparatorenator(fridgeContent[i].expirationDate, currentDate) == -1) {
            printf("         EXPIRED\n");
            printf("---------------------------\n");
            break;
        }
    }
    /*For loop which goes through the fridgeContent array
      and prints out the ingredients that have expired*/    
    for (i = 0; i < fridgeSize; i++) {
        if(!(fridgeContent[i].expirationDate.day == UNKNOWN || fridgeContent[i].expirationDate.month == UNKNOWN || fridgeContent[i].expirationDate.year == UNKNOWN) 
            && dateComparatorenator(fridgeContent[i].expirationDate, currentDate) == -1) {
            printf(RED);
            printf("%s HAS EXPIRED\n", fridgeContent[i].name);
            printf(WHITE);
        }
    }
}

/*Function to get the present date*/
date makeDayToday(){
    /*Declaration of a temporary date variable*/
    date tempDate;
    /*Declaration and initialization of today, of type time_t and which value of time(NULL)
    which is the number of seconds since 1970/01/01 till now*/
    time_t today = time(NULL);

    /*Declaration and initialization of a struct tm called tm, 
    which takes the value of today and converts them to localtime
    and insert them to the tm struct*/
    struct tm tm = *localtime(&today);

    /*Assigns the year, month and day to the tempDate struct
    and adds 1900 to years and 1 month to match 2021 and the correct month*/
    tempDate.year = (tm.tm_year + 1900);
    tempDate.month = (tm.tm_mon + 1);
    tempDate.day = (tm.tm_mday);

    /*Return the date*/
    return tempDate;
}


/*Function to get the date of tomorrow*/
void tomorrow(date *date){
    /*Switch statement for month*/
    switch(date->month){
        /*Checks if the present month is 1, 3, 5, 7, 8, 10 or 12*/

        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            /*If the present day is less than 31,
              the day can be counted up by one*/
            if(date->day < 31){
                (date->day)++;
              /*If the present day is 31 and the month is 12
                Set the day and month to 1 and count the year up by 1*/
            } else if(date->day == 31 && date->month == 12){
                date->day = 1;
                date->month = 1;
                (date->year)++;
                /*Else set the day to one and count the month up by 1*/
            } else{
                date->day = 1;
                (date->month)++;
            }
            break;
        /*Check if the month is 4, 6, 9 or 11*/
        case 4: case 6: case 9: case 11:
            /*If the day is less than 30, count the day up by 1*/
            if(date->day < 30){
                (date->day)++;
              /*Else, set the day to 1 and count the month up by 1*/
            } else{
                date->day = 1;
                (date->month)++;
            }
            break;
        /*Check if the month is 2*/
        case 2:
            /*Check if it is leap year*/
            if(leapYear(date->year)){
                /*If the day is less tha 29, count the day up by 1*/
                if(date->day < 29){
                    (date->day)++;
                  /*Else set the day to 1 and count month up by 1*/
                } else{
                    date->day = 1;
                    (date->month)++;
                }
            }
            /*If it is not leap year*/
            else{
                /*If the day is less than 28, count day up by 1*/
                if(date->day < 28){
                    (date->day)++;
                  /*Else set the day to one, and count month up by 1*/
                } else{
                    date->day = 1;
                    (date->month)++;
                }
            }
            break;
    }
}

/*Function to check if the present date is a leap year*/
int leapYear(int year){
  int result;

  if(year % 4 == 0) result = 1;
  else result = 0;

  return result;
}

/*Function for setting the date to a specific day*/
void skipToDate() {
    currentDate.day = 0;
    printf("What date do you want to skip to? (yyyy/mm/dd)\n");
    scanf("%d/%d/%d", &currentDate.year, &currentDate.month, &currentDate.day);

    while(currentDate.day > 31 ||
          ((currentDate.month == 4 || currentDate.month == 6 || currentDate.month == 9 || currentDate.month == 11) && currentDate.day > 30) ||
          (leapYear(currentDate.year) == 1 && currentDate.month == 2 && currentDate.day > 28) ||
          (currentDate.month == 2 && currentDate.day > 29) || currentDate.month < 1 || currentDate.month > 12 || currentDate.day < 1){
        
        flushInput();
        printf("Please type a valid date!\n(yyyy/mm/dd): ");
        scanf(" %d/%d/%d", &currentDate.year, &currentDate.month, &currentDate.day);
    }
}

/*Function for Fridge Contents screen*/
void contents(ingredients *fridgeContent) {
    int ingredientNumber = 1, searchOption;
    char choice[2];

    clearScreen();
    /*Print the the content of the fridge*/
    printFridgeContents(fridgeContent);

    printf("\nWhich ingredient do you want to change? (press 'S' to search, 'T' to search food by type, 'N' to add an ingredient, or 'R' to return):\n");

    do{
        /*Check if the fridge is empty*/
        if(fridgeSize == 0) {
            clearScreen();
            printf("Your fridge is empty\n");
            printf("Press 'R' to return or 'N' to add an ingredient:\n");
        }
        /*Makes sure the user inputs a valid number*/
        else if(ingredientNumber <= 0 || ingredientNumber > fridgeSize) {
            clearScreen();
            printFridgeContents(fridgeContent);
            printf("\nPlease enter a valid ingredient number! (press 'S' to search, 'T' to search food by type, 'N' to add an ingredient, or 'R' to return):\n");
        }
        scanf(" %s", choice);
        ingredientNumber = atoi(choice);
        /*Return to main menu if user presses 'R'*/
        if(strcmp(choice, "r") == 0 || strcmp(choice, "R") == 0){
            mainMenu(fridgeContent);
        }
        /*Add new ingredient if the user presses 'N'*/
        else if(strcmp(choice, "n") == 0 || strcmp(choice, "N") == 0){
            addIngredient(fridgeContent);
        }
        /*Search if the user presses 'S'*/
        else if(strcmp(choice, "s") == 0 || strcmp(choice, "S") == 0){
            searchOption = 1;
            search(fridgeContent, &ingredientNumber, searchOption);
        }
        else if(strcmp(choice, "t") == 0 || strcmp(choice, "T") == 0){
            searchOption = 0;
            search(fridgeContent, &ingredientNumber, searchOption);       
        }
    } while (ingredientNumber <= 0 || ingredientNumber > fridgeSize);
    /*Edit the ingredient the user decides, if the user inputs a number*/
    editIngredient(fridgeContent, ingredientNumber - 1);
}

/*Function for printing out each ingredient in the fridgeContent array*/
void printFridgeContents(ingredients *fridgeContent) {
    int itemNumber;
    printf("Your fridge contains\n");
    for(itemNumber = 0; itemNumber < fridgeSize; itemNumber++) {
        printIngredient(fridgeContent, itemNumber);
    }
}


/*Function for search menu*/
void search(ingredients *fridgeContent, int *ingredientNumber, int searchOption) {
    char choice[2];

    /*Run the correct search function*/
    if(searchOption == 1){
        clearScreen();
        printFridgeContents(fridgeContent);
        searchIngredient(fridgeContent);
    } else if(searchOption == 0){
        clearScreen();
        printFridgeContents(fridgeContent);
        searchTypes(fridgeContent);
    }
    printf("\nWhich ingredient do you want to change? (press 'S' to search, 'T' to search food by type, 'N' to add an ingredient, or 'R' to return):\n");
    do{
        scanf(" %s", choice);
        *ingredientNumber = atoi(choice);
        /*Return to main menu if user presses 'R'*/
        if(strcmp(choice, "r") == 0 || strcmp(choice, "R") == 0){
            mainMenu(fridgeContent);
        }
        /*Add new ingredient if the user presses 'N'*/
        else if(strcmp(choice, "n") == 0 || strcmp(choice, "N") == 0){
            addIngredient(fridgeContent);
        }
        /*Search if the user presses 'S'*/
        else if(strcmp(choice, "s") == 0 || strcmp(choice, "S") == 0){
            searchOption = 1;
            search(fridgeContent, ingredientNumber, searchOption);
        }
        else if(strcmp(choice, "t") == 0 || strcmp(choice, "T") == 0){
            searchOption = 0;
            search(fridgeContent, ingredientNumber, searchOption);       
        }
        else if(*ingredientNumber <= 0 || *ingredientNumber > fridgeSize) {
            clearScreen();
            printFridgeContents(fridgeContent);
            printf("\nPlease enter a valid ingredient number! (press 'S' to search, 'T' to search food by type, 'N' to add an ingredient, or 'R' to return):\n");
        }
    } while(*ingredientNumber <= 0 || *ingredientNumber > fridgeSize);
}

/*Search function for searching the fridgeContent array*/
void searchIngredient(ingredients *fridgeContent) {
    /*Declaration of strings: searchTerm and ingredientName*/
    char searchTerm[20], ingredientName[20];
    int i, j, hasFound = FALSE;

    /*Prompt user for search term*/
    printf("What would you like to search for?\n");
    scanf(" %s", searchTerm);
    printf("\n");

    /*Convert the searched term to all lowercase*/
    for(j = 0; j < strlen(searchTerm); j++) {
        searchTerm[j] = tolower(searchTerm[j]);
    }

    /*Goes through the fridgeContent array and checks for matches*/
    for(i = 0; i < fridgeSize; i++) {
        /*Copy the current ingredient's name into the ingredientName variable*/  
        strcpy(ingredientName, fridgeContent[i].name);

        /*Convert the ingredient name to all lowercase*/
        for(j = 0; j < strlen(ingredientName); j++) {
            ingredientName[j] = tolower(ingredientName[j]);
        }

        /*Check if the searchTerm and ingredientName have similar names*/
        if(strstr(ingredientName, searchTerm) != NULL) {
            printIngredient(fridgeContent, i);
            hasFound = TRUE;
        }
    }
    /*If no matches was found, inform the user*/
    if(hasFound == FALSE){
        clearScreen();
        printFridgeContents(fridgeContent);
            printf("No matches was found for %s.\n", searchTerm);
    }
}

void searchTypes(ingredients *fridgeContent){
    int i, j, hasFound = FALSE;
    char searchType[20], ingredientType[20];

    printf("What food type would you like to search for?\n");
    scanf(" %s", searchType);
    printf("\n");

    for(i = 0 ; i < strlen(searchType) ; i++){
        searchType[i] = tolower(searchType[i]);
    }

    for(j = 0 ; j < fridgeSize ; j++){  

        strcpy(ingredientType, fridgeContent[j].ingredientType);

        /*Convert the ingredient name to all lowercase*/
        for(i = 0; i < strlen(ingredientType); i++) {
            ingredientType[i] = tolower(ingredientType[i]);
        }

        if(strcmp(searchType, ingredientType) == 0) {
            printIngredient(fridgeContent, j);
            hasFound = TRUE;
        }
    }

    if(hasFound == FALSE){
        clearScreen();
        printFridgeContents(fridgeContent);
        printf("No matches was found for %s.\n", searchType);
    }
}

/*Function for printing an ingredient*/
void printIngredient(ingredients *fridgeContent, int itemNumber) {
    /*Check which color the ingredient should be output as*/
    printColour(fridgeContent, itemNumber);
    
    /*Check if there are any data on the name of the ingredient*/    
    if(strcmp(fridgeContent[itemNumber].name, "-1") == 0) {
        strcpy(fridgeContent[itemNumber].name, "?????????");
    }
    /*Print the item number and name with a specific amount of spaces before the item number*/
    if (itemNumber + 1 < 10) {
        printf("  %d - %s", itemNumber + 1, fridgeContent[itemNumber].name);
    }
    else {
        printf(" %d - %s", itemNumber + 1, fridgeContent[itemNumber].name);
    }
    /*Print the Type, Weight, Expiration date, and Opened date of the ingredient*/
    printIngType(fridgeContent, itemNumber);
    printWeight(fridgeContent, itemNumber);
    printExpirationDate(fridgeContent, itemNumber);
    printOpenedDate(fridgeContent, itemNumber);

    /*Check if the ingredient has info on the amount of days it can last after being opened and print it*/
    if(!(fridgeContent[itemNumber].open.isopen.daysAfterOpen == UNKNOWN)){
        printf("Shelf time after opening is %d days\n", fridgeContent[itemNumber].open.isopen.daysAfterOpen);
    }
    else{
        printf("Shelf time after opening is UNKNOWN\n");
    }
    printf(WHITE);
}

/*Function for setting the correct printing color for the text in regards to ingredient data*/
void printColour(ingredients *fridgeContent, int itemNumber) {
    /*Set the color to PURPLE if the ingredient has missing information*/
    if(fridgeContent[itemNumber].expirationDate.day == UNKNOWN || fridgeContent[itemNumber].expirationDate.month == UNKNOWN ||
       fridgeContent[itemNumber].expirationDate.year == UNKNOWN || strcmp(fridgeContent[itemNumber].name, "-1") == 0 ||
       fridgeContent[itemNumber].weight == UNKNOWN || fridgeContent[itemNumber].open.opened == UNKNOWN ||
       (fridgeContent[itemNumber].open.opened == FALSE && (fridgeContent[itemNumber].open.isopen.openDate.day == UNKNOWN ||
       fridgeContent[itemNumber].open.isopen.openDate.month == UNKNOWN || fridgeContent[itemNumber].open.isopen.openDate.year == UNKNOWN ||
       fridgeContent[itemNumber].open.isopen.daysAfterOpen == UNKNOWN))) {
        printf(PURPLE);
    }
    /*Set the color to RED if the ingredient has expired*/
    else if(dateComparatorenator(fridgeContent[itemNumber].expirationDate, currentDate) == -1) {
        printf(RED);
    }
    /*Set the color to GREEN if the ingredient has not expired*/
    else if(dateComparatorenator(fridgeContent[itemNumber].expirationDate, currentDate) == 1) {
        printf(GREEN);
    }
    /*Set the color to YELLOW if the ingredient is close to its expiration date*/
    else {
        printf(YELLOW);
    }
}

/*Prints the ingredient type*/
void printIngType(ingredients *fridgeContent, int itemNumber) {
    int i;

    /*Prints the appropriate amount of spaces in regards to the length of the name*/
    for (i = 0; i < 25 - strlen(fridgeContent[itemNumber].name); i++){
        printf(" ");
    }
    printf("%s", fridgeContent[itemNumber].ingredientType);
}

/*Prints the weight of the ingredient*/
void printWeight(ingredients *fridgeContent, int itemNumber) {
    int i;
    /*Prints the appropriate amount of spaces in regards to the weight of the ingredient
      and the stringlength of the type*/
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
    /*If the weight is valid, print the weight with 2 decimal places*/
    else if(fridgeContent[itemNumber].weight < 100000 && fridgeContent[itemNumber].weight >= 10000) {
        for(i = 0; i < 11 - strlen(fridgeContent[itemNumber].ingredientType); i++) {
            printf(" ");
        }           
    }    
    if(fridgeContent[itemNumber].weight > 0){
        printf("%.2f g", fridgeContent[itemNumber].weight);           
    }
    /*Print question marks if the weight is invalid or unknown*/
    else {
        printf("????????");
    }
}

/*Function for printing the expiration date*/
void printExpirationDate(ingredients *fridgeContent, int itemNumber) {
    printf("   Expiration date: ");
    /*Check if the expiration date is unknown*/
    if(!(fridgeContent[itemNumber].expirationDate.day == UNKNOWN || fridgeContent[itemNumber].expirationDate.month == UNKNOWN || fridgeContent[itemNumber].expirationDate.year == UNKNOWN)) {
        /*Print the date if the expiration date is known*/
        printf("%d/%d/%d", fridgeContent[itemNumber].expirationDate.year, fridgeContent[itemNumber].expirationDate.month, fridgeContent[itemNumber].expirationDate.day);
    }
    else printf("???\?/?\?/??");

    /*If the expiration date is known, print the appropriate amount of spaces in the regards to the month and day*/
    if(!(fridgeContent[itemNumber].expirationDate.day == UNKNOWN || fridgeContent[itemNumber].expirationDate.month == UNKNOWN || fridgeContent[itemNumber].expirationDate.year == UNKNOWN)) {
        if(fridgeContent[itemNumber].expirationDate.month < 10) {
           printf(" ");
        } 
        if(fridgeContent[itemNumber].expirationDate.day < 10) {
           printf(" ");
        }         
    }
}

/*Function for printing the opened date of an ingredient*/
void printOpenedDate(ingredients *fridgeContent, int itemNumber) {
    /*Check if the state of the ingredient being opened is known
      If FALSE, print UNOPENED
      If TRUE, print the date*/
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
            /*If the date is unknown, print question marks*/
            else
                printf("   Opened on: ???\?/?\?/??   ");
        }            
    }
    /*If the state of the ingredient is unknown, print N/A*/
    else {
        printf("   N/A                     ");
    }
}

/*Function for adding an ingredient to the fridgeContent array*/
void addIngredient(ingredients *fridgeContent) {
    char opened;
    /*Reallocate memory for an additional ingredient*/
    char weightTemp[10];
    fridgeContent = (ingredients *) realloc(fridgeContent, sizeof(ingredients) * ++fridgeSize);
    /*If reallocation has failed, print error message and exit with EXIT_FAILURE*/
    if(fridgeContent == NULL) {
        printf("ERROR: Couldn't re-allocate memory");
        exit(EXIT_FAILURE);
    }

    /*Clear screen and prompt user for the name, type, weight,
      expiration date, opened state, opened date and how long it can last when opened*/
    clearScreen();

    printf("What is the name of the ingredient? (Max 20 characters & use underscore for space)\n");
    scanf(" %20s", fridgeContent[fridgeSize - 1].name);
    flushInput();
    fridgeContent[fridgeSize - 1].name[0] = whiteSpaceDeprotect(toupper(fridgeContent[fridgeSize - 1].name[0]));

    printf("What type of food is it?\n");
    scanf("%s", fridgeContent[fridgeSize - 1].ingredientType);
    fridgeContent[fridgeSize - 1].ingredientType[0] = whiteSpaceDeprotect(toupper(fridgeContent[fridgeSize - 1].ingredientType[0]));
    flushInput();

    printf("What is the weight of the ingredient in grams?\n");

    scanf("%s", weightTemp);
    fridgeContent[fridgeSize - 1].weight = atof(weightTemp);
    while(!(fridgeContent[fridgeSize - 1].weight > 0 || fridgeContent[fridgeSize - 1].weight == -1)) {
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

    do {
        scanf("%d", &fridgeContent[fridgeSize - 1].open.isopen.daysAfterOpen);
        if (fridgeContent[fridgeSize -1].open.isopen.daysAfterOpen < 0) {
            printf("Please enter a valid number!\n");
        }
    } while(fridgeContent[fridgeSize -1].open.isopen.daysAfterOpen < 0);
    
    /*Update the expiration date of the fridgeContent array and sort it
      then return to the content menu*/
    updateExpDates(fridgeContent);
    sortContent(fridgeContent);
    contents(fridgeContent);
}

/*Function for setting the expiration date of a new ingredient*/
void newIngredientExpirationDate(ingredients *fridgeContent) {
    int newIngredient = fridgeSize - 1;
    date tempDate; 
    tempDate.day = 0;
    /*Prompt the user for the expiration date of the new ingredient*/
    printf("What is the expiration date of the ingredient? (yyyy/mm/dd)\n");
    scanf("%d/%d/%d", &tempDate.year, &tempDate.month, &tempDate.day);

    /*Validate the date*/
    while(tempDate.day > 31 ||
          ((tempDate.month == 4 || tempDate.month == 6 || tempDate.month == 9 || tempDate.month == 11) && tempDate.day > 30) ||
          (leapYear(tempDate.year) == 1 && tempDate.month == 2 && tempDate.day > 28) ||
          (tempDate.month == 2 && tempDate.day > 29) || tempDate.month < 1 || tempDate.month > 12 || tempDate.day < 1){
        
        flushInput();
        printf("Please type a valid date!\n(yyyy/mm/dd): ");
        scanf(" %d/%d/%d", &tempDate.year, &tempDate.month, &tempDate.day);
    }
    /*Assign the date to the ingredient in the fridgeContent array*/
    fridgeContent[newIngredient].expirationDate = tempDate;
}

/*Function for setting the opened date of a new ingredient*/
void newIngredientOpenedDate(ingredients *fridgeContent) {
    int newIngredient = fridgeSize - 1;
    date tempDate;
    tempDate.day = 0;

    printf("When was the ingredient opened? (yyyy/mm/dd)\n");
    scanf("%d/%d/%d", &tempDate.year, &tempDate.month, &tempDate.day);

    while(tempDate.day > 31 ||
          ((tempDate.month == 4 || tempDate.month == 6 || tempDate.month == 9 || tempDate.month == 11) && tempDate.day > 30) ||
          (leapYear(tempDate.year) == 1 && tempDate.month == 2 && tempDate.day > 28) ||
          (tempDate.month == 2 && tempDate.day > 29) || tempDate.month < 1 || tempDate.month > 12 || tempDate.day < 1){
        
        flushInput();
        printf("Please type a valid date!\n(yyyy/mm/dd): ");
        scanf(" %d/%d/%d", &tempDate.year, &tempDate.month, &tempDate.day);
    }
    fridgeContent[newIngredient].open.isopen.openDate = tempDate;
}

/*Function for editing an existing ingredient*/
void editIngredient(ingredients *fridgeContent, int ingredientNumber) {
    char choice;
    /*String to hold the status of the openedness of the ingredient*/
    char* openStatus;
    clearScreen();
    /*Check if the ingredient has been opened and assign the appropriate message for openStatus*/
    if(fridgeContent[ingredientNumber].open.opened == 1) {
        openStatus = "NOT IN THE STATE OF NOT IN THE STATE OF IN THE STATE OF OPENEDNESS";
    }
    else if (fridgeContent[ingredientNumber].open.opened == 0){
        openStatus = "IN THE STATE OF NOT IN THE STATE OF OPENEDNESS";
    }
    /*Print the text for the menu*/
    printf("EDITING %s\n\n", fridgeContent[ingredientNumber].name);
    printf("1. Name            : %s\n", fridgeContent[ingredientNumber].name);
    printf("2. Weight          : %0.2f\n", fridgeContent[ingredientNumber].weight);
    printf("3. Expiration Date : %d/%d/%d\n", fridgeContent[ingredientNumber].expirationDate.year, fridgeContent[ingredientNumber].expirationDate.month, fridgeContent[ingredientNumber].expirationDate.day);
    printf("4. Opened Status   : %s\n", openStatus);
    printf("\n\n");
    printf("# - EDIT         R - RETURN\n");
    scanf(" %c", &choice);
    flushInput();
    /*Switch statement, that redirects the user to the appropriate menus in regard of their choice*/
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
    /*Rerun the edit menu if the user has not been returned to the contents menu*/
    editIngredient(fridgeContent, ingredientNumber);
}

/*Function for changing the name of an ingredient*/
void changeName(ingredients *fridgeContent, int ingredientNumber) {
    printf("\nPlease type the new name: ");
    scanf(" %s", fridgeContent[ingredientNumber].name);
    flushInput();
}

/*Function for changing the weight of an ingredient*/
void changeWeight(ingredients *fridgeContent, int ingredientNumber) {
    double tempWeight;
    printf("\nPlease type the new weight: ");
    scanf(" %lf", &tempWeight);
    flushInput();
    /*INPUT VALIDATION*/
    while(tempWeight < 0) {
        printf("\nPlease type a valid weight: ");
        scanf(" %lf", &tempWeight);
    }
    fridgeContent[ingredientNumber].weight = tempWeight;
    /*Check if the weight is set to 0. If the weight is 0, delete the ingredient*/
    if(fridgeContent[ingredientNumber].weight == 0) {
        deleteIngredient(fridgeContent, ingredientNumber);
    }
}

/*Function for changing the date of an existing ingredient*/
void changeDate(ingredients *fridgeContent, int ingredientNumber) {
    date tempDate;
    tempDate.day = 0;
    printf("\nPlease type the new date (yyyy/mm/dd): ");
    scanf(" %d/%d/%d", &tempDate.year, &tempDate.month, &tempDate.day);
    flushInput();
    /*INPUT VALIDATION*/
    while(tempDate.day > 31 ||
          (tempDate.month <= 0 || tempDate.day <= 0) || ((tempDate.month == 4 || tempDate.month == 6 || tempDate.month == 9 || tempDate.month == 11) && tempDate.day > 30) ||
          (leapYear(tempDate.year) == 1 && tempDate.month == 2 && tempDate.day > 29) ||
          (leapYear(tempDate.year) == 0 && tempDate.month == 2 && tempDate.day > 28) || tempDate.month < 1 || tempDate.month > 12){
        flushInput();
        printf("Please type a valid date!\n(yyyy/mm/dd): ");
        scanf(" %d/%d/%d", &tempDate.year, &tempDate.month, &tempDate.day);
    }
    /*Set the tempDate as the expiration date*/
    fridgeContent[ingredientNumber].expirationDate = tempDate;
}

/*Function for changing the OPEN STATE of the ingredient*/
void changeOpenedState(ingredients *fridgeContent, int ingredientNumber) {
    char choice;

    /*WARN THE USER FOR POTENTIAL HARM TO THE DATE*/
    printf("This will override the date. Are you sure? y/n: ");
    scanf(" %c", &choice);
    if(choice == 'y' || choice == 'Y') {
        /*If the user accepts, change the state of the ingredient and update the openDate
          if the ingredient was previously unopened*/
        if (fridgeContent[ingredientNumber].open.opened == 0){
            fridgeContent[ingredientNumber].open.opened = 1;
            fridgeContent[ingredientNumber].open.isopen.openDate = currentDate;
            updateExpDates(fridgeContent);
        }
        else if (fridgeContent[ingredientNumber].open.opened == 1){
            fridgeContent[ingredientNumber].open.opened = 0;
        } 
    }
    /*Do nothing if the user declines*/
    else if(choice == 'n' || choice == 'N'){
        printf("Nice bro, good choice! You're going places in your life.\n");
    }
}

/*Function for deleting an existing ingredient*/
void deleteIngredient(ingredients *fridgeContent, int ingredientNumber) {
    /*Create a temporary ingredient with the values of the last element in the fridgeContent array*/
    ingredients temp = fridgeContent[fridgeSize - 1];
    /*Assign the chosen ingredient to the last position in the array*/
    fridgeContent[fridgeSize - 1] = fridgeContent[ingredientNumber];
    /*Assign the temporary ingredient to the position of the chosen ingredient*/
    fridgeContent[ingredientNumber] = temp;
    /*Reallocate the memory with one less ingredient*/
    fridgeContent = (ingredients *) realloc(fridgeContent, sizeof(ingredients) * --fridgeSize);
    /*Sort the fridgeContent array and return to the contents menu*/
    sortContent(fridgeContent);
    contents(fridgeContent);
}
/*Function for comparing dates*/
int dateComparatorenator(date expirationDate, date openedDate) {
    int i;
    /*Assign the soonToExpireDate to be the openedDate*/
    date soonToExpireDate = openedDate;

    /*Advance the soonToExpiredate by 2 days*/
    for(i = 1; i <= 2; i++) {
        tomorrow(&soonToExpireDate);
    }
    /*Check which date is greater than the other and return the respective value*/
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

void recipeMenu(ingredients *fridgeContent) {
    int recipeKind = 1;
    char choice[1];
    Recipes vegetarianRecipes[VEGETARIANNUMBER]; 
    Recipes lowCarbRecipes[LOWCARBNUMBER]; 
    Recipes lowCalorieRecipes[LOWCALORIENUMBER]; 
    Recipes highProteinRecipes[HIGHPROTEINNUMBER];
    Recipes allRecipes[NUMBEROFRECIPES];
    
    makeRecipes(fridgeContent, vegetarianRecipes, lowCarbRecipes, lowCalorieRecipes, highProteinRecipes, allRecipes);

    clearScreen();
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
    /*Switch statement for printing the different categories of recipes*/
    switch (recipeKind){
    case vegetarian:
        recipeList(vegetarianRecipes, fridgeContent, VEGETARIANNUMBER);
        break;
    case lowCarb:
        recipeList(lowCarbRecipes, fridgeContent, LOWCARBNUMBER);
        break;
    case lowCalorie:
        recipeList(lowCalorieRecipes, fridgeContent, LOWCALORIENUMBER);
        break;
    case highProtein:
        recipeList(highProteinRecipes, fridgeContent, HIGHPROTEINNUMBER);
        break;
    default:
        recipeList(allRecipes, fridgeContent, NUMBEROFRECIPES);
        break;
    }
}

/*Function for printing the recipe types menu*/
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

/*Function for coloring the different recipe types*/
void colorForRecipeType (Recipes *recipe, ingredients *fridgeContent, int numberOfRecipes){
    int i;
    for (i = 1; i <= numberOfRecipes; i++){
        /*Set the color of the recipe type to green
          if it contains atleast one recipe that can be made*/
        if (colorForRecipe(i, recipe, fridgeContent) == 1){
            printf(GREEN);
            break;
        }
        else{
            printf(RED);
        }
    }
}

/*Recipe Menu function*/
void recipeList(Recipes *recipe, ingredients *fridgeContent, int numberOfRecipes){
    char choice[1];
    int recipeNumber = 1;
    clearScreen();
    printRecipeList(recipe, fridgeContent, numberOfRecipes);
    printf("\nWhich recipe do you want to see? (press R to return to recipes):\n");
    do{
        /*INPUT VALIDATION*/
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

/*Function for printing the recipe list*/
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

/*Function for coloring a single recipe*/
int colorForRecipe (int i, Recipes *recipe, ingredients *fridgeContent){
    int j, k, numberOfIngredients = 0, isGreen = TRUE;
    /*Count number of ingredients in the recipe*/
    for (j = 0 ; j < MAXINGREDIENTS ; j++){
        if(strcmp(recipe[i - 1].fridgeIngredients[j].name, "\0") == 0){
            break;
        }
        else{
            numberOfIngredients++;
        } 
    }
    /*Set the color for the recipe*/
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

/*Function for opening a recipe*/
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
    /*Print the ingredients used for the recipe that are not in the fridge*/
    printf("Ingredients that are not in the fridge is assumed that you own: \n");
    for(i = 0 ; i < MAXINGREDIENTS ; i++){
        if(strcmp(recipe.notFridgeIngredients[i].name, "\0")){
            printf("    %s:", recipe.notFridgeIngredients[i].name);
            for (j = 0; j < 30 - strlen(recipe.notFridgeIngredients[i].name); j++){
                printf(" ");
            }
            printf("%.2fg\n\n", recipe.notFridgeIngredients[i].weight);
        }
    }
    
    /*Print the ingredients used for the recipe that are in the fridge*/
    printf("Ingredients that are in the fridge: \n");
    
    for(i = 0 ; i < MAXINGREDIENTS ; i++){
        if(strcmp(recipe.fridgeIngredients[i].name, "\0")){
            if(colourization(fridgeContent, recipe.fridgeIngredients[i].name, recipe.fridgeIngredients[i].weight) == 0){
                printf(RED);
            } else {
                printf(GREEN);
            }
            printf("    %s:", recipe.fridgeIngredients[i].name);
            for (j = 0; j < 30 - strlen(recipe.fridgeIngredients[i].name); j++){
                printf(" ");
            }
            printf("%.2fg\n\n", recipe.fridgeIngredients[i].weight);
        }
    }
    /*Print the instructions for the recipe*/
    printf(WHITE);
    printf("  -------------------------------------\n");
    printf("              INSTRUCTIONS\n");
    printf("  -------------------------------------\n");
    printInstructions(recipe);
    
    /*Ability to return to recipes*/
    printf("\n\nR - Return to Recipes\n");
    do {
        scanf(" %c", &choice);
        flushInput();
        if(choice == 'R' || choice == 'r'){
            recipeMenu(fridgeContent);    
        }
    } while(!(choice == 'R' || choice == 'r'));
}

/*Function for colorization of different ingredients in a recipe*/
int colourization(ingredients *fridgeContent, char *ingredientName, double neededWeight){
    int i;
    /*Go through the different ingredients of a recipe
      and see if there is enough in the fridge*/
    for(i = 0; i < fridgeSize; i++){
        if(strcmp(ingredientName, fridgeContent[i].name) == 0) {
            if(fridgeContent[i].weight < neededWeight){
                return 0;
            }
            if(dateComparatorenator(fridgeContent[i].expirationDate, currentDate) == -1) {
                return 0;
            }
            return 1;
        }
    }   
    return 0;
}

/*Function for clearing the terminal / screen*/
void clearScreen(void) {
    /*system("") prints what is inside the string into the system terminal of the user.
      Since systems don't have similar commands for clearing their respective terminal
      The two most common commands for clearing the terminal are used as an or statement
      So if the first one fails, the second one might execute*/
    system("@cls||clear");
}

/*Function for flushing the input stream*/
void flushInput(void) {
    char flush;
    /*While characters are in the input stream,
      assign them to flush as long as they are not a new line.
      Then the while loop breaks and the flush has finalized*/
    while((flush = getchar()) != '\n');
}
