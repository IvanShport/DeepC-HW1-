#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#define SIZE_OF_ROW 32
#define SIZE_OF_TEXT 16

char* input_row(char* row_pointer, size_t size);
size_t choice_rows(char** text, size_t size_text, char** fin_text);
char** realloc_for_text(char** text, size_t* size_of_text, const size_t size_of_row);
char** free_not_used_mem(char** text, size_t* size_of_text, const size_t new_size);
char** create_text(size_t size_of_text, size_t size_of_row);
char** input_text(char** text, size_t* size_of_row, size_t* size_of_text);
void output_result(char** result_text, const size_t size_of_result);
void free_text(char** text, size_t size);

int main() {
    size_t size_of_row = SIZE_OF_ROW;
    size_t size_of_text = SIZE_OF_TEXT;

    char** text = create_text(size_of_text, size_of_row);
    if (text == NULL) return 0;

    text = input_text(text, &size_of_row, &size_of_text);
    if (text == NULL) return 0;

    char** fin_text = create_text(size_of_text, 0);
    if (fin_text == NULL) {
        free_text(text, size_of_text);
        return 0;
    }

    size_t size_of_fin_text = choice_rows(text, size_of_text, fin_text);

    if (size_of_fin_text == size_of_text) {
        free_text(text, size_of_text);
        free(fin_text);
        printf("[error]");
        return 0;
    }

    output_result(fin_text, size_of_fin_text);

    free_text(text, size_of_text);
    free_text(fin_text, size_of_fin_text);

    return 0;
}

char** create_text(size_t size_of_text, size_t size_of_row) {
    char** text = (char**)malloc(sizeof(char*) * size_of_text);
    if (text == NULL) {
        printf("[error]");
        return NULL;
    }
    if (size_of_row == 0) return text;

    for (size_t i = 0; i < size_of_text; i++) {
        text[i] = (char*)malloc(sizeof(char) * size_of_row);

        if (text[i] == NULL) {
            printf("[error]");
            free_text(text, i);
            return NULL;
        }
    }
    return text;
}

char** input_text(char** text, size_t* size_of_row, size_t* size_of_text) {
    size_t i = 0;
    for (;; i++) {
        text[i] = input_row(text[i], *size_of_row);

        if (text[i] == NULL) {
            printf("[error]");
            free_text(text, i);
            return NULL;
        }

        if (i >= *size_of_text - 1) {
            char** temp_text = realloc_for_text(text, size_of_text, *size_of_row);

            if (temp_text == NULL) {
                printf("[error]");
                for (size_t j = 0; j <= i; j++) {
                    free(text[j]);
                }
                free(text);
                return NULL;
            }

            text = temp_text;

        }
        if (*text[i] == '\0') break;
    }

    char** temp_text = free_not_used_mem(text, size_of_text, i + 1);

    if (temp_text == NULL) {
        printf("[error]");
        free_text(text, *size_of_text);
        return NULL;
    }

    text = temp_text;

    return text;
}

char* input_row(char* row_pointer, size_t size) {
    ptrdiff_t cur_size = 1;
    size_t empty_size = size;

    char* end_row = row_pointer;

    row_pointer[0] = '\0';
    while (fgets(end_row, empty_size, stdin)) {
        if (end_row[strlen(end_row) - 1] == '\n') {
            end_row[strlen(end_row) - 1] = '\0';
            break;
        }

        empty_size = size;
        size *= 2;
        char* temp_row = (char*)realloc(row_pointer, size * sizeof(char));
        if (temp_row == NULL) return NULL;
        row_pointer = temp_row;
        end_row = row_pointer + (empty_size - cur_size);
        cur_size++;
    }

    char* temp_row = (char*)realloc(row_pointer, (strlen(row_pointer) + 1) * sizeof(char));
    if (temp_row == NULL) {
        free(row_pointer);
        return NULL;
    }
    row_pointer = temp_row;

    return row_pointer;
}

char** realloc_for_text(char** text, size_t* size_of_text, const size_t size_of_row) {
    *size_of_text *= 2;

    char** temp_text = (char**)realloc(text, *size_of_text * sizeof(char*));
    if (temp_text == NULL) return NULL;
    text = temp_text;

    for (size_t i = *size_of_text / 2; i < *size_of_text; i++) {
        text[i] = (char*)malloc(sizeof(char) * size_of_row);
        if (text[i] == NULL) return NULL;
    }

    return text;
}

char** free_not_used_mem(char** text, size_t* size_of_text, const size_t new_size) {
    for (size_t j = new_size; j < *size_of_text; j++) {
        free(text[j]);
    }
    *size_of_text = new_size;
    char** temp_text = (char**)realloc(text, *size_of_text * sizeof(char*));
    if (temp_text == NULL) return NULL;
    text = temp_text;
    return text;
}

size_t choice_rows(char** text, size_t size_text, char** fin_text) {
    size_t counter_of_bracket = 0;
    size_t counter_of_row = 0;

    for (size_t i = 0; *text[i] != '\0'; i++) {
        for (size_t j = 0; j < strlen(text[i]); j++) {
            if(text[i][j] == '(') counter_of_bracket++;
            if(text[i][j] == ')') counter_of_bracket--;
        }

        if (counter_of_bracket == 0) {
            char* temp_row = (char*)malloc(sizeof(char) * (strlen(text[i]) + 1));
            if (temp_row == NULL) {
                for (size_t k = 0; k < counter_of_row; k++) {
                    free(fin_text[k]);
                }
                return size_text;
            }
            fin_text[counter_of_row] = temp_row;

            memcpy(fin_text[counter_of_row], text[i], strlen(text[i]) + 1);
            counter_of_row++;
        }

        counter_of_bracket = 0;
    }

    return counter_of_row;
}

void output_result(char** result_text, const size_t size_of_result) {
    for (size_t i = 0; i < size_of_result; i++) {
        printf("%s\n", result_text[i]);
    }
}

void free_text(char** text, size_t size) {
    for (size_t i = 0; i < size; i++) {
        free(text[i]);
    }
    free(text);
}