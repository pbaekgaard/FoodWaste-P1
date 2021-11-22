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
    int i = 0, j;
    struct ing fridgeContent[100];

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
        fscanf(readFile, " %s %lf %d %d %d %d %d %d", &fridgeContent[i].name, &fridgeContent[i].weight, &fridgeContent[i].expirationDate.year, &fridgeContent[i].expirationDate.month, &fridgeContent[i].expirationDate.day,
                                            &fridgeContent[i].openedDate.year, &fridgeContent[i].openedDate.month, &fridgeContent[i].openedDate.day);
        i++;
    }
    /* Prints the elements of each struct in the array of structs */

    for(j = 0; j < i; j++){
        if(fridgeContent[j].openedDate.year == 0 && fridgeContent[j].openedDate.month == 0 && fridgeContent[j].openedDate.day == 0){
            printf("\n#%d %s %.2lfg >>> Expires on: %d %d %d / Not opened.\n", j+1, &fridgeContent[j].name, fridgeContent[j].weight, fridgeContent[j].expirationDate.year, fridgeContent[j].expirationDate.month, fridgeContent[j].expirationDate.day);
        }
        else {
            printf("\n#%d %s %.2lfg >>> Expires on: %d %d %d  >>> Opened on: %d %d %d\n", j+1, &fridgeContent[j].name, fridgeContent[j].weight, fridgeContent[j].expirationDate.year, fridgeContent[j].expirationDate.month, fridgeContent[j].expirationDate.day,
                                                   fridgeContent[j].openedDate.year, fridgeContent[j].openedDate.month, fridgeContent[j].openedDate.day);
        
        }
    }
    
    /* Closes file */
    fclose(readFile);
    
    return 0;
}