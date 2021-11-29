/* Include guard that prevents double declaration of any identifiers such as types, enums and static variables*/
#ifndef _APP_H
#define _APP_H

#define NUMBEROFRECIPES 5
#define INSTRUCTIONLINELENGTH 256
#define TRUE 1
#define FALSE 0
#define UNKNOWN -1
#define MAXINGREDIENTS 100
#define RED "\033[31;1m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[33;1m"
#define WHITE "\x1B[0m"
#define PURPLE "\033[0;34m"

typedef struct date {
    int year, month, day;
} date;

typedef struct isopen {
    date openDate;
    int daysAfterOpen;
} isopen;

typedef struct open {
    int opened;
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

date todayDate;
int fridgeSize;

/* Prototypes */
int getFridgeSize(ingredients *);
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
void editIngredient(ingredients*, int);
void changeName(ingredients*, int);
void changeWeight(ingredients*, int);
void changeDate(ingredients*, int);
void printColour(ingredients *, int);
void printWeight(ingredients *, int );
void printExpirationDate(ingredients *, int);
void printOpenedDate(ingredients *, int);
void addIngredient(ingredients *);
void newIngredientExpirationDate(ingredients *);
void newIngredientOpenedDate(ingredients *);
void recipeMenu(ingredients*);
int colourization(ingredients *, char *, double);
void printRecipeList(Recipes*, ingredients *);
int dateComparatorenator(date, date);
void printDate(ingredients *, int);
void returnMenu(char *, ingredients *);
void clearScreen(void);
void flushInput(void);
void openRecipe(Recipes , ingredients *);
void printInstructions(Recipes);

#endif
