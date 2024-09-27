#ifndef _RAND_SELECT_H
#define _RAND_SELECT_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

__attribute__((constructor))
void init() {
    srand((unsigned)time(0));
    printf("[SUCCEED] init random seed\n");
}

// 从小到大排序
int _greater(const void* p1, const void* p2) {
    const int* p1int = (const int32_t*)p1;
    const int* p2int = (const int32_t*)p2;
    return *p1int - *p2int;
}

void randSelect(const char* file_name, int32_t n) {

    // 统计文件行数
    char buff[1024];
    int32_t count = 0;
    FILE* fp = fopen(file_name, "r");
    while (fscanf(fp, "%[^\n]", buff) != EOF) {
        count += 1;
        fgetc(fp);
    }
    if (n >= count) {
        printf("[ERROR] randSelect input invalid\n");
        exit(1);
    }

    // 将文件内容读入到程序中
    char** cont;
    cont = (char**)malloc(count * sizeof(char*));
    for (int32_t i = 0; i < count; i++) {
        cont[i] = (char*)malloc(1024 * sizeof(char));
    }
    fseek(fp, 0, SEEK_SET);
    for (int32_t i = 0; i < count; i++) {
        fscanf(fp, "%[^\n]", cont[i]);
        fgetc(fp);
    }
    fclose(fp);

    // 记录选取行下标
    int32_t rand_arr[n];
    for (int32_t i = 0; i < n; i++) {
        rand_arr[i] = rand() % (count - 1) + 1;
    }

    // 将下标从小到大排序
    qsort(rand_arr, n, sizeof(int32_t), _greater);

    // 输出到目标文件中
    fp = fopen("test_seg", "w");
    fprintf(fp, "version 1\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%s\n", cont[rand_arr[i]]);
    }
    fclose(fp);

    return ;
}

#endif
