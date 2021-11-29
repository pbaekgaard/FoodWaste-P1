/* Include guard that prevents double declaration of any identifiers such as types, enums and static variables*/
#ifndef APP_H_
#define app

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

int fridgeSize;

int getFridgeSize(ingredients *);
void getFridgeContents(ingredients *);

#endif
