
#pragma once
#include <stdio.h>
#include <stdlib.h>


// Define the data structure for each record
typedef struct CafeInfo {
    int census_year;
    int block_id;
    int property_id;
    int base_property_id;
    char building_address[129];
    char clue_small_area[129];
    char business_address[129];
    char trading_name[129];
    int industry_code;
    char industry_description[129];
    char seating_type[129];
    int number_of_seats;
    double longitude;
    double latitude;
    struct CafeInfo* next;
} CafeInfo;

// Define the data structure of the dictionary
typedef struct CafeDictionary {
    CafeInfo* head;
} CafeDictionary;



// Create a café dictionary from a data file.
CafeDictionary* createCafeDictionary(char* fname);

// Search for and print matching café records based on a search key in the provided café dictionary to the specified output file.
void searchCafeAndPrint(CafeDictionary* dict, char* searchKey, FILE* outputFile);

// Free the memory allocated for the café dictionary and its contents.
void freeCafeDictionary(CafeDictionary* dict);

