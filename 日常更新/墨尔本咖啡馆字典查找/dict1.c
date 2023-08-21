// dict1.c
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict1.h"

CafeDictionary* createCafeDictionary(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("无法打开文件");
        return NULL;
    }

    CafeDictionary* dict = (CafeDictionary*)malloc(sizeof(CafeDictionary));
    if (dict == NULL) {
        perror("无法分配内存");
        fclose(file);
        return NULL;
    }
    dict->head = NULL;

    char line[512];
    fgets(line, sizeof(line), file); // 跳过标题行

    while (fgets(line, sizeof(line), file) != NULL) {
        CafeInfo* cafe = (CafeInfo*)malloc(sizeof(CafeInfo));
        if (cafe == NULL) {
            perror("无法分配内存");
            fclose(file);
            freeCafeDictionary(dict);
            return NULL;
        }

        char* token = strtok(line, ",");
        if (token == NULL) {
            perror("无法解析CSV字段");
            fclose(file);
            free(cafe);
            freeCafeDictionary(dict);
            return NULL;
        }
        cafe->census_year = atoi(token);

        token = strtok(NULL, ",");
        cafe->block_id = atoi(token);
        token = strtok(NULL, ",");
        cafe->property_id = atoi(token);
        // 以此类推，解析其他字段
        // 请根据需要继续解析其他字段
        token = strtok(NULL, ",");
        cafe->base_property_id = atoi(token);

        token = strtok(NULL, ",");
        strncpy(cafe->building_address, token, sizeof(cafe->building_address) - 1);
        cafe->building_address[sizeof(cafe->building_address) - 1] = '\0';

        token = strtok(NULL, ",");
        strncpy(cafe->clue_small_area, token, sizeof(cafe->clue_small_area) - 1);
        cafe->clue_small_area[sizeof(cafe->clue_small_area) - 1] = '\0';

        token = strtok(NULL, ",");
        strncpy(cafe->business_address, token, sizeof(cafe->business_address) - 1);
        cafe->business_address[sizeof(cafe->business_address) - 1] = '\0';

        token = strtok(NULL, ",");
        strncpy(cafe->trading_name, token, sizeof(cafe->trading_name) - 1);
        cafe->trading_name[sizeof(cafe->trading_name) - 1] = '\0';

        token = strtok(NULL, ",");
        cafe->industry_code = atoi(token);

        token = strtok(NULL, ",");
        strncpy(cafe->industry_description, token, sizeof(cafe->industry_description) - 1);
        cafe->industry_description[sizeof(cafe->industry_description) - 1] = '\0';

        token = strtok(NULL, ",");
        strncpy(cafe->seating_type, token, sizeof(cafe->seating_type) - 1);
        cafe->seating_type[sizeof(cafe->seating_type) - 1] = '\0';

        token = strtok(NULL, ",");
        cafe->number_of_seats = atoi(token);

        token = strtok(NULL, ",");
        cafe->longitude = atof(token);

        token = strtok(NULL, ",");
        cafe->latitude = atof(token);

        cafe->next = dict->head;
        dict->head = cafe;

        
    }

    fclose(file);
    return dict;
}

// void searchCafeAndPrint(CafeDictionary *dict, char *searchKey, FILE *outputFile) {
//     // 实现代码以在咖啡馆字典中搜索与searchKey匹配的记录
//     // 将匹配的记录打印到outputFile
// }
void searchCafeAndPrint(CafeDictionary* dict, char* searchKey, FILE* outputFile) {
    if (dict == NULL || searchKey == NULL || outputFile == NULL) {
        perror("参数错误");
        return;
    }

    CafeInfo* current = dict->head; // 当前咖啡馆信息节点
    int matchCount = 0; // 记录匹配的数量

    // 遍历整个咖啡馆字典
    while (current != NULL) {
        // 判断搜索关键字是否存在于咖啡馆的 trading_name 中
        if (strstr(current->trading_name, searchKey) != NULL) {
            // 找到匹配的记录，将咖啡馆信息写入到输出文件中
            // fprintf(outputFile, "census_year: %d\nblock_id: %d\nproperty_id: %d\nbase_property_id: %d\nbuilding_address: %s\nclue_small_area: %s\nbusiness_address: %s\ntrading_name: %s\nindustry_code: %d\nindustry_description: %s\nseating_type: %s\nnumber_of_seats: %d\nlongitude: %.5lf\nlatitude: %.5lf\n\n",
            //     current->census_year, current->block_id, current->property_id, current->base_property_id,
            //     current->building_address, current->clue_small_area, current->business_address,
            //     current->trading_name, current->industry_code, current->industry_description,
            //     current->seating_type, current->number_of_seats, current->longitude, current->latitude);
            fprintf(outputFile, "census_year: %d\nblock_id: %d\nproperty_id: %d\nbase_property_id: %d\nbuilding_address: %s\nclue_small_area: %s\nbusiness_address: %s\ntrading_name: %s\nindustry_code: %d\nindustry_description: %s\nseating_type: %s\nnumber_of_seats: %d\nlongitude: %.5lf\nlatitude: %.5lf\n\n",
        current->census_year, current->block_id, current->property_id, current->base_property_id,
        current->building_address, current->clue_small_area, current->business_address,
        current->trading_name, current->industry_code, current->industry_description,
        current->seating_type, current->number_of_seats, current->longitude, current->latitude);

            matchCount++; // 增加匹配数量
        }

        current = current->next; // 移动到下一个咖啡馆信息节点
    }
    printf("%s --> %d\n", searchKey, matchCount); // 在标准输出中显示搜索关键字和匹配数量
    if (matchCount == 0) {
        printf("%s -->NOTFOUND\n",searchKey); // 输出未找到匹配记录的标识
    }
    
    fflush(outputFile);
}

void freeCafeDictionary(CafeDictionary* dict) {
    if (dict == NULL) {
        return; // 不需要释放空字典
    }

    CafeInfo* current = dict->head;
    while (current != NULL) {
        CafeInfo* temp = current;
        current = current->next;
        free(temp); // 释放咖啡馆信息节点内存
    }

    free(dict); // 释放字典结构内存
}


