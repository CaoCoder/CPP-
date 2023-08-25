
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict1.h"
/* test.c
*
* This program creates a café dictionary from a data file, accepts user input search keywords,
* and prints the matched café records to an output file.
*
* To run the program:
* ./dict1 <Stage> <Data File> <Output File>
*
* Where:
*   <Stage>         Stage value, this program only accepts a stage value of 1
*   <Data File>     File name containing café data
*   <Output File>   Output file name to store matched café records
* 
*/


int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage：%s <Stage> <Data File> <Output File>\n", argv[0]);
        return 1;
    }

    int stage = atoi(argv[1]);

    if (stage != 1) {
        printf("Invalid stage value\n");
        return 1;
    }

    char* datafile = argv[2];
    char* outputfile = argv[3];
  
  
// Create a café dictionary from the data file
    CafeDictionary* dict = createCafeDictionary(datafile);

    if (dict == NULL) {
        printf("Unable to create café dictionary\n");
        return 1;
    }

    FILE* outfile = fopen(outputfile, "w");
    if (outfile == NULL) {
        perror("Unable to open output file");
        freeCafeDictionary(dict);
        return 1;
    }
  
    // Search and print café records based on user input
    char searchKey[129];
    while (fgets(searchKey, sizeof(searchKey), stdin) != NULL) {
       
        searchKey[strcspn(searchKey, "\n")] = '\0';
        searchCafeAndPrint(dict, searchKey,outfile);
    }

    fclose(outfile);
    // Free memory allocated for the café dictionary
    freeCafeDictionary(dict);

    return 0;
}
