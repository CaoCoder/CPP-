
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict1.h"
// 通用的CSV字段解析函数
void parseCSVField(char* dest, char* src) {
    if (src == NULL) {
        dest[0] = '\0';
        return;
    }

    if (src[0] == '"') {
        // 处理包含逗号的字段
        strncpy(dest, src + 1, 128 - 1);

        while (1) {
            src = strtok(NULL, ",");
            strncat(dest, ",", 128 - strlen(dest) - 1);
            strncat(dest, src, 128 - strlen(dest) - 1);
            if (src[strlen(src) - 1] == '"') {
                dest[strlen(dest) - 1] = '\0'; // 去掉最后一个逗号
                break;
            }
        }
    } else {
        // 处理普通字段
        strncpy(dest, src, 128 - 1);
    }
    dest[128 - 1] = '\0';
}


/**
* Create a Café Dictionary from a data file.
*
* This function reads data from a specified file and constructs a Café Dictionary
* containing information about cafés. Each line in the file represents a café's
* details in CSV format. The function parses the CSV fields and populates the
* dictionary with café records.
*
* param fname The name of the data file to read from.
* return A pointer to the newly created Café Dictionary, or NULL if an error occurs.
*/

CafeDictionary* createCafeDictionary(char* fname) {
    FILE* fl = fopen(fname, "r");
    if (fl == NULL) {
        perror("Unable to open the file");
        return NULL;
    }

    CafeDictionary* dict = (CafeDictionary*)malloc(sizeof(CafeDictionary));
    if (dict == NULL) {
        perror("Unable to allocate memory");
        fclose(fl);
        return NULL;
    }
    dict->head = NULL;

     CafeInfo* tail = NULL; // Track the tail of the list
    char line[512];
    fgets(line, sizeof(line), fl); //Skip the header row

    while (fgets(line, sizeof(line), fl) != NULL) {
        CafeInfo* cafe = (CafeInfo*)malloc(sizeof(CafeInfo));
        if (cafe == NULL) {
            perror("Unable to allocate memory");
            fclose(fl);
            freeCafeDictionary(dict);
            return NULL;
        }

        char* tk = strtok(line, ",");
        if (tk == NULL) {
            perror("Unable to parse CSV field");
            fclose(fl);
            free(cafe);
            freeCafeDictionary(dict);
            return NULL;
        }
        cafe->census_year = atoi(tk);

        tk = strtok(NULL, ",");
        cafe->block_id = atoi(tk);
        tk = strtok(NULL, ",");
        cafe->property_id = atoi(tk);

        tk = strtok(NULL, ",");
        cafe->base_property_id = atoi(tk);

        tk = strtok(NULL, ",");
        parseCSVField(cafe->building_address, tk);

        tk = strtok(NULL, ",");
        parseCSVField(cafe->clue_small_area, tk);

        tk = strtok(NULL, ",");
        parseCSVField(cafe->business_address, tk);

        tk = strtok(NULL, ",");
        parseCSVField(cafe->trading_name, tk);

        tk = strtok(NULL, ",");
        cafe->industry_code = atoi(tk);

        tk = strtok(NULL, ",");
        parseCSVField(cafe->industry_description, tk);

        tk = strtok(NULL, ",");
        parseCSVField(cafe->seating_type, tk);

        tk = strtok(NULL, ",");
        cafe->number_of_seats = atoi(tk);

        tk = strtok(NULL, ",");
        cafe->longitude = atof(tk);

        tk = strtok(NULL, ",");
        cafe->latitude = atof(tk);

            cafe->next = NULL; // Set the next pointer to NULL for the new node

        if (tail == NULL) {
            // If the list is empty, set both head and tail to the new node
            dict->head = cafe;
            tail = cafe;
        } else {
            // Otherwise, append the new node after the current tail and update tail
            tail->next = cafe;
            tail = cafe;
        }
        
    }

    fclose(fl);
    return dict;
}



// // Implement code to search for records matching searchKey in the cafe dictionary
// // and print the matching records to the outputFile

void searchCafeAndPrint(CafeDictionary* dict, char* searchKey, FILE* outputFile) {
    if (dict == NULL || searchKey == NULL || outputFile == NULL) {
        perror("Parameter error");
        return;
    }

    CafeInfo* current = dict->head; // Current café information node
    int matchCount = 0; // Number of matched records

    // Iterate through the entire café dictionary
    while (current != NULL) {
        if (strcmp(current->trading_name, searchKey) == 0) {
            if (matchCount == 0) {
                // Print the trading name only once
                fprintf(outputFile, "%s\n", current->trading_name);
            }

            // Print café information to the output file in the desired format
            fprintf(outputFile, "--> census_year: %d || block_id: %d || property_id: %d || base_property_id: %d || building_address: %s || clue_small_area: %s || business_address: %s || trading_name: %s || industry_code: %d || industry_description: %s || seating_type: %s || number_of_seats: %d || longitude: %.5lf || latitude: %.5lf ||\n",
                current->census_year, current->block_id, current->property_id, current->base_property_id,
                current->building_address, current->clue_small_area, current->business_address,
                current->trading_name, current->industry_code, current->industry_description,
                current->seating_type, current->number_of_seats, current->longitude, current->latitude);

            matchCount++;
        }

        current = current->next;
    }

    // Print the match count and "NOTFOUND" if no matches were found
    printf("%s --> %d\n", searchKey, matchCount);
    if (matchCount == 0) {
        printf("%s --> NOTFOUND\n", searchKey);
    }

    fflush(outputFile);
}


void freeCafeDictionary(CafeDictionary* dict) {
    if (dict == NULL) {
        return; 
    }

    CafeInfo* current = dict->head;
    while (current != NULL) {
        CafeInfo* temp = current;
        current = current->next;
        free(temp); // Free memory for café information nodes
    }

    free(dict); // Free memory for dictionary structure
}


