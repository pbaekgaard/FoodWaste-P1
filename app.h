/* Include guard that prevents double declaration of any identifiers such as types, enums and static variables*/
#ifndef APP_H
#define APP_H

#define BUFFERMAX 1000
#define INSTRUCTIONLINELENGTH 256
#define TRUE 1
#define FALSE 0
#define UNKNOWN -1
#define VEGETARIANNUMBER 3
#define LOWCARBNUMBER 3
#define LOWCALORIENUMBER 3
#define HIGHPROTEINNUMBER 3
#define NUMBEROFRECIPES 10
#define TYPESOFRECIPES 5
#define MAXINGREDIENTS 50
#define RED "\033[31;1m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[33;1m"
#define WHITE "\x1B[0m"
#define PURPLE "\033[0;34m"

typedef enum recipeType {lowCarb = 2, lowCalorie, highProtein, vegetarian} recipeType;

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
    char name[25];    
    double weight;
    date expirationDate;
    open open;
    char ingredientType[20];
} ingredients;

typedef struct Recipes {
    char* name;
    ingredients fridgeIngredients[MAXINGREDIENTS];
    ingredients notFridgeIngredients[MAXINGREDIENTS];
    char *filename;
    char *recipeType;
} Recipes;

/* Global variables - Extern means declaration only. No memory is allocated but the compiler is aware that the variable exists */
extern date todayDate;
extern int fridgeSize;

/* Prototypes */
extern int getFridgeSize(void);
extern void getFridgeContents(ingredients *);
extern char *whiteSpaceDeprotect(char *);
extern void updateExpDates (ingredients *);
extern int dateComparator(date, date);
extern void sortContent(ingredients *);
extern int contentCompare(const void *, const void *);
extern void mainMenu(ingredients *);
extern void printNotifications(ingredients *);
extern void contents(ingredients *);
extern void printFridgeContents(ingredients *);
extern void printIngredient(ingredients *, int);
extern void printColour(ingredients *, int);
extern void printIngType(ingredients *, int);
extern void printWeight(ingredients *, int);
extern void printExpirationDate(ingredients *, int);
extern void printOpenedDate(ingredients *, int);
extern void addIngredient(ingredients *);
extern void newIngredientOpenedDate(ingredients *);
extern int isDateValid(date);
extern void newIngredientExpirationDate(ingredients *);
extern void editIngredient(ingredients *, int);
extern void changeName(ingredients *, int);
extern void changeType(ingredients *, int);
extern void changeWeight(ingredients *, int);
extern void deleteIngredient(ingredients *, int);
extern void changeDate(ingredients *, int);
extern void changeOpenedState(ingredients *, int);
extern void recipeMenu(ingredients *);
extern void makeRecipes(ingredients *, Recipes*, Recipes*, Recipes*, Recipes*, Recipes*);
extern void printRecipeTypes(Recipes*, Recipes*, Recipes*, Recipes*, Recipes*, ingredients *);
extern void colourForRecipeType(Recipes *, ingredients *, int);
extern int colourForRecipe(int, Recipes *, ingredients *);
extern int colourization(ingredients *, char *, double);
extern void recipeList(Recipes *, ingredients *, int);
extern void printRecipeList(Recipes *, ingredients *, int);
extern void openRecipe(Recipes , ingredients *);
extern void printInstructions(Recipes);
extern void searchIngredient(ingredients *);
extern void searchTypes(ingredients *);
extern void tomorrow(date *);
extern int leapYear(int);
extern void skipToDate();
extern void clearScreen(void);
extern void flushInput(void);
extern date makeDayToday();
#endif
