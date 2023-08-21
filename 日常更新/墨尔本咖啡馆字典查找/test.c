#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict1.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("用法：%s <阶段> <数据文件> <输出文件>\n", argv[0]);
        return 1;
    }

    int stage = atoi(argv[1]);

    if (stage != 1) {
        printf("无效的阶段值\n");
        return 1;
    }

    char* datafile = argv[2];
    char* outputfile = argv[3];
  
  
    // 从数据文件创建咖啡馆字典
    CafeDictionary* dict = createCafeDictionary(datafile);

    if (dict == NULL) {
        printf("无法创建咖啡馆字典\n");
        return 1;
    }

    FILE* outfile = fopen(outputfile, "w");
    if (outfile == NULL) {
        perror("无法打开输出文件");
        freeCafeDictionary(dict);
        return 1;
    }
  
    // 根据用户输入搜索并打印咖啡馆记录
    char searchKey[129];
    while (fgets(searchKey, sizeof(searchKey), stdin) != NULL) {
        // 移除换行符
        searchKey[strcspn(searchKey, "\n")] = '\0';
        searchCafeAndPrint(dict, searchKey,outfile);
    }

    fclose(outfile);
    // 释放为咖啡馆字典分配的内存
    freeCafeDictionary(dict);

    return 0;
}
