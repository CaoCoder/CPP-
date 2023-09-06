#include <stdio.h>
#include <string.h>

typedef struct {
    const char* trading_name;
} Entry;
int count = 0;
int my_strncmp(const char* str1, const char* str2, size_t n) {
    count++;
    return strncmp(str1, str2, n);
}

int binary_search(Entry* arr, int low, int high, const char* target) {
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int comparison = my_strncmp(arr[mid].trading_name, target, strlen(target));

        if (comparison == 0) {
            // 找到目标字符串
            return mid;
        }
        else if (comparison < 0) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    // 未找到目标字符串
    return -1;
}

int main() {
    Entry entries[] = {
        {"Arbory Bar and Eatery"},
        {"Court House Hotel"},
        {"Cupanda Kitchen"},
        {"Darac Grill & Bar"},
        {"Domino's Pizza"},
        {"Doughboys Doughnuts"},
        {"Dragon Hot Pot"},
        {"El Rincon"},
        {"Little Bean Blue"},
        {"Mankeumi"},
        {"Princess Theatre"},
        {"Shujinko Ramen"},
        {"Shuxiang Hot Pot"},
        {"St. Charly"},
        {"Subway"},
        {"Tank Fish And Chippery"},
        {"The Basement Cafe"},
        {"The Journal Canteen"},
        {"The League Of Honest Coffee"},
        {"Troika Bar"}
    };

    const char* target = "Doughboys Doughnuts";
    int num_entries = sizeof(entries) / sizeof(entries[0]);
    binary_search(entries, 0, num_entries - 1, target);

    
    printf("需要查找%d次\n", count);
  

    return 0;
}