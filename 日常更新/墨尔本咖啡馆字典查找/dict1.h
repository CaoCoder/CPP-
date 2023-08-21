// dict1.h
#pragma once
#include <stdio.h>
#include <stdlib.h>
#ifndef DICT1_H
#define DICT1_H

// 定义每个记录的数据结构
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

// 定义字典的数据结构
typedef struct CafeDictionary {
    CafeInfo* head;
} CafeDictionary;

CafeDictionary* createCafeDictionary(char* filename);
void searchCafeAndPrint(CafeDictionary* dict, char* searchKey, FILE* outputFile);
void freeCafeDictionary(CafeDictionary* dict);

#endif
