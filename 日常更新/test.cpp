#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>

int partition_leftpivot(int s[], int lo, int hi) {
    int pivot = s[lo];
    int i = lo;
    int j = hi + 1;

    while (1) {
        do {
            i++;
        } while (s[i] < pivot);

        do {
            j--;
        } while (s[j] > pivot);

        if (i >= j) {
            break;
        }

        // Swap s[i] and s[j]
        int temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }

    // Swap s[lo] and s[j]
    int temp = s[lo];
    s[lo] = s[j];
    s[j] = temp;

    return j;
}

int quickSort(int arr[], int low, int high) {
    int count = 0;

    if (low < high) {
        int partitionIndex = partition_leftpivot(arr, low, high);
        count++;

        count += quickSort(arr, low, partitionIndex);
        count += quickSort(arr, partitionIndex + 1, high);
    }

    return count;
}

int main() {
    int arr[] = { 5, 3, 7, -1, 0, 10, 4, 8, 11 };
    int n = sizeof(arr) / sizeof(arr[0]);

    int count = quickSort(arr, 0, n - 1);

    printf("调用分区函数的次数：%d\n", count);

    return 0;
}