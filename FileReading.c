#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct date {
    int year, month, day;
};

struct ing {
    char name[20];
    double weight;
    struct date expirationDate;
    struct date openedDate;
};

int main(void) {
    int i = 0, j = 0;
    struct ing test[100];

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
        fscanf(readFile, " %s %lf %d %d %d %d %d %d", &test[i].name, &test[i].weight, &test[i].expirationDate.year, &test[i].expirationDate.month, &test[i].expirationDate.day,
                                            &test[i].openedDate.year, &test[i].openedDate.month, &test[i].openedDate.day);
        i++;
    }

    if(test->openedDate.year == 0 && test->openedDate.month == 0 && test->openedDate.day == 0) {
        for(; j < i; j++){
            printf("%s %.2lf %d %d %d\n", &test[j].name, test[j].weight, test[j].expirationDate.year, test[j].expirationDate.month, test[j].expirationDate.day);
        }
    }
    else {
        /* Prints the elements of each struct in the array of structs */
        for(; j < i; j++){
            printf("%s %.2lf %d %d %d %d %d %d\n", &test[j].name, test[j].weight, test[j].expirationDate.year, test[j].expirationDate.month, test[j].expirationDate.day,
                                                   test[j].openedDate.year, test[j].openedDate.month, test[j].openedDate.day);
        }
    }
    
    /* Closes file */
    fclose(readFile);
    
    return 0;
}