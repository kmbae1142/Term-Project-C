#pragma warning (disable:6031)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 1024

int* p = NULL;
int max_row = -1;
int max_col = -1;
int mul_mem = 1;

void init() {

    p = (int*)malloc(sizeof(int) * PAGE_SIZE);
    if (p == NULL) {
        printf("메모리 할당 오류, 다시 시도하세요.\n");
        exit(1);
    }
    memset(p, 0, sizeof(int) * PAGE_SIZE);

}

void resize() {

    mul_mem++;
    int *temp = (int*)realloc(p, sizeof(int) * PAGE_SIZE * mul_mem);
    if (temp != NULL) {
        p = temp;
    }
    else {
        while (temp == NULL) {
            temp = (int*)realloc(p, sizeof(int) * PAGE_SIZE * mul_mem);
        }
    }
    memset(p + sizeof(int) * PAGE_SIZE * (mul_mem - 1), 0, sizeof(int) * PAGE_SIZE);

}

void create_row() {

    int row_num;
    scanf("%d", &row_num);
    while (getchar() != '\n');
    int size = (max_row + 1) * (max_col + 1);

    if (row_num < 0) {
        return;
    }
    else if (row_num <= max_row) {
        if (size + max_col >= PAGE_SIZE * mul_mem) {
            resize();
        }

        int start = row_num * (max_col + 1);
        for (int i = size + max_col; i >= start; i--) {
            p[i] = p[i - (max_col + 1)];
        }
        for (int i = start; i <= start + max_col; i++) {
            p[i] = 0;
        }

        max_row++;
    }
    else {
        if (size + (row_num - max_row) * max_col >= PAGE_SIZE * mul_mem) {
            resize();
        }

        int new_row = (row_num - max_row) * (max_col + 1);
        for (int i = size; i < size + new_row; i++) {
            p[i] = 0;
        }

        max_row = row_num;
    }


}

void create_col() {

    int col_num;
    scanf("%d", &col_num);
    while (getchar() != '\n');
    int size = (max_row + 1) * (max_col + 1);

    if (col_num < 0) {
        return;
    }
    else if (col_num <= max_col) {
        if (size + max_row >= PAGE_SIZE * mul_mem) {
            resize();
        }

        int cur_index = col_num;

        for (int i = 0; i < max_row + 1; i++) {
            for (int j = size; j >= cur_index; j--) {
                p[j + 1] = p[j];
            }

            p[cur_index] = 0;
            cur_index += (max_col + 2);
            size++;
        }

        max_col++;
    }
    else {
        int diff = col_num - max_col;
        int index = max_col;

        if (size + diff * max_row >= PAGE_SIZE * mul_mem) {
            resize();
        }

        for (int n = 0; n < max_row; n++) {
            for (int i = size; i > index; i--) {
                p[i + diff] = p[i];
            }

            for (int i = index + 1; i < index + 1 + diff; i++) {
                p[i] = 0;
            }

            size += diff;
            index += diff + (max_col + 1);
        }

        for (int i = size; i < size + diff; i++) {
            p[i] = 0;
        }
        max_col += diff;
    }

}

long sum() {

    long sum = 0;
    int total = (max_row + 1) * (max_col + 1);

    for (int i = 0; i < total; i++) {
        sum += p[i];
    }

    return sum;

}

double avg() {

    if (max_row > -1 && max_col > -1) {
        return (double)sum() / ((max_row + 1) * (max_col + 1));
    }
    else {
        return 0.0;
    }

}

void update_cell() {

    int row, col, data;
    scanf("%d %d %d", &row, &col, &data);

    while (getchar() != '\n');
    if (row > max_row || col > max_col) {
        return;
    }
    else {
        p[row * (max_col + 1) + col] = data;
    }

}

void delete_row() {

    int row_num;
    scanf("%d", &row_num);
    while (getchar() != '\n');
    int size = (max_row + 1) * (max_col + 1);

    if (row_num < 0 || row_num > max_row) {
        return;
    }
    else if (row_num <= max_row) {

        int start = row_num * (max_col + 1);

        for (int i = start; i < size - (max_col + 1); i++) {
            p[i] = p[i + max_col + 1];
        }

        max_row--;

    }

}

void delete_col() {

    int col_num;
    scanf("%d", &col_num);
    while (getchar() != '\n');
    int size = (max_row + 1) * (max_col + 1);

    if (col_num < 0 || col_num > max_col) {
        return;
    }
    else if (col_num <= max_col) {
        int cur_index = col_num;
        for (int i = 1; i <= max_row + 1; i++) {
            for (int j = cur_index; j < size - 1; j++) {
                p[j] = p[j + 1];
            }
            cur_index += max_col;
            size--;
        }

        max_col--;
    }

}

void save() {

    char file_name[100];
    scanf("%s", file_name);

    FILE* fp = fopen(file_name, "w");
    if (fp == NULL) return;

    int total = (max_row + 1) * (max_col + 1);
    for (int i = 0; i < total; i++) {
        if (i % (max_col + 1) == max_col) {
            fprintf(fp, "%d\n", p[i]);
        }
        else {
            fprintf(fp, "%d,", p[i]);
        }
    }

    fclose(fp);
    while (getchar() != '\n');

}

void create_obj() {

    printf("create new (r)ow | (c)olumn\n");
    printf("input your command: \n");

    char c;
    scanf("%c", &c);

    while (getchar() != '\n');
    switch (c)
    {
    case 'r':
        printf("input row num: ");
        create_row();
        break;
    case 'c':
        printf("input column num: ");
        create_col();
        break;
    default:
        printf("wrong command...\n");
    }

}

void functions() {

    printf("apply function (a)verage | (s)um\n");
    printf("input your command: \n");

    char c;
    scanf("%c", &c);

    while (getchar() != '\n');
    switch (c)
    {
    case 'a':
        printf("average: %f\n", avg());
        break;
    case 's':
        printf("sum: %ld", sum());
        break;
    default:
        printf("wrong command...\n");
    }

}

void delete_obj() {
    printf("delete (r)ow | (c)olumn\n");
    printf("input your command: \n");
    char c;
    scanf("%c", &c);
    while (getchar() != '\n');
    switch (c)
    {
    case 'r':
        printf("input row num: ");
        delete_row();
        break;
    case 'c':
        printf("input column num: ");
        delete_col();
        break;
    default:
        printf("wrong command...\n");
    }
}

void print_obj() {
    if (max_row == -1 || max_col == -1) return;
    printf("           ");
    for (int i = 0; i < max_col + 1; i++) {
        printf(" %11d", i);
    }
    printf("\n           ");
    for (int i = 0; i < max_col + 1; i++) {
        printf("------------");
    }
    printf("-\n");
    for (int i = 0; i < max_row + 1; i++) {
        printf("%11d", i);
        for (int j = 0; j < max_col + 1; j++) {
            printf("|%11d", *(p + ((max_col + 1) * i) + j));
        }
        printf("|\n");
    }
    printf("           ");
    for (int i = 0; i < max_col + 1; i++) {
        printf("------------");
    }
    printf("-\n");
}

void print_intro() {
    printf("#################################################\n");
    printf("#\t\t2025 CP3 mini project\t\t#\n");
    printf("#################################################\n");
    printf("\n");
    printf("(c)reate | (f)unction | (u)pdate | (d)elete | (s)ave | (p)rint | (q)uit\n");
    printf("input your command: \n");
}

int main() {
    int loop = 1;
    init();
    while (loop) {
        print_intro();

        char c;
        scanf("%c", &c);
        while (getchar() != '\n');

        switch (c)
        {
        case 'c':
            create_obj();
            break;
        case 'f':
            functions();
            break;
        case 'u':
            printf("input row and column num & data: ");
            update_cell();
            break;
        case 'd':
            delete_obj();
            break;
        case 's':
            printf("input filename: ");
            save();
            break;
        case 'q':
            loop = 0;
            break;
        case 'p':
            print_obj();
            break;
        default:
            printf("wrong command...\n");
        }
        printf("\n\n\n\n");
    }

    free(p);
    return 0;
}
