#include <stdio.h>
#include <stdlib.h>
#include "app.h"

int fridgeSize;

int getFridgeSize(ingredients *fridgeContent) {
    int numberOfLines = 0;
    /* Pointer to a FILE */
    FILE *fileptr;

    /* Name of file */
    char *fileName = "db/fridge/ingredients.txt", ch;
    
    /* Sets the size of the fridge to zero before getting the number of ingredients in the fridge */
    fridgeSize = 0;
    
    /* Open and read file */
    fileptr = fopen(fileName, "r");

    /* If file doesn't open it gives Error message and exits */
    if(fileptr == NULL) {
        printf("Error - File not found");
        exit(EXIT_FAILURE);
    }

    /* Read single character of file */
    ch = getc(fileptr);

    
    while(ch != EOF) {
        /* Count number of newlines in the file */
        if(ch == '\n') {
            numberOfLines++;
        }
        /* Read single character of file */
        ch = getc(fileptr);
    }
    /* The last ingredient is without a newline in the file */
    numberOfLines++;
    /* Close the file */
    fclose(fileptr);

    fridgeSize = numberOfLines;

    return fridgeSize;
}


void getFridgeContents(ingredients *fridgeContent) {
    int i = 0;
    /* Pointer to a FILE */
    FILE *readFile;

    /* Name of file */
    char *filename = "db/fridge/ingredients.txt";

    /* Open and read file */
    readFile = fopen(filename, "r");

    /* If file doesn't open it gives Error message and exits */
    if(readFile == NULL) {
        printf("Error - File not found");
        exit(EXIT_FAILURE);
    }

    /* Scans file into the structs name and integer into the structs weight until end of file */
    while(!feof(readFile)){
        fscanf(readFile, " %s %lf %d %d %d %d", fridgeContent[i].name, &fridgeContent[i].weight, &fridgeContent[i].expirationDate.year, &fridgeContent[i].expirationDate.month, &fridgeContent[i].expirationDate.day, &fridgeContent[i].open.opened);
        if(fridgeContent[i].open.opened == TRUE){
            fscanf(readFile, " %d %d %d %d", &fridgeContent[i].open.isopen.openDate.year, &fridgeContent[i].open.isopen.openDate.month, &fridgeContent[i].open.isopen.openDate.day, &fridgeContent[i].open.isopen.daysAfterOpen);
        }
        else{
            fscanf(readFile, " %d", &fridgeContent[i].open.isopen.daysAfterOpen);
        }
        fscanf(readFile, " %s", fridgeContent[i].ingredientType);
        i++;
    }
    /* Closes file */
    fclose(readFile);
}

void printInstructions(Recipes recipe) {
    FILE *fp = fopen(recipe.filename, "r");
    char buffer[INSTRUCTIONLINELENGTH];

    if(fp == NULL) {
        printf("ERROR: Couldn't find instructions");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, INSTRUCTIONLINELENGTH, fp)){
        printf("%s", buffer);
    }
    fclose(fp);
}
