#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ing {
    char name[20];
    int weight;
};

int main(void) {
    int i = 0, j;
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
        fscanf(readFile, "%s %d", &test[i].name, &test[i].weight);
        i++;
    }

    /* Prints the elements of each struct in the array of structs */
    for(j = 0; j < i; j++){
        printf("%s %d\n", &test[j].name, test[j].weight);
    }
    
    /* Closes file */
    fclose(readFile);
    
    return 0;
}