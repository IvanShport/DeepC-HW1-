#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#define START_SIZE_OF_ROW 32
#define START_SIZE_OF_TEXT 2
#define BUFFER_FOR_INPUT 64
#define START_MEM_FOR_INPUT 1024

char* input_row(char* row_pointer);
size_t choice_rows(char** text, size_t size_of_text, char*** fin_text);
char** realloc_for_text(char** text, size_t* size_of_text, size_t size_of_row);
char** free_not_used_mem(char** text, size_t* size_of_text, size_t new_size);
char** create_text(size_t size_of_text, size_t size_of_row);
char** find_valid_rows(char** text, size_t size_of_text, size_t *size_of_fin_text);
char** input_text(char** text, size_t size_of_row, size_t* size_of_text);
char** init_start_text(size_t* size_of_text);
void output_result(const char** result_text, size_t size_of_result);
void free_text(char** text, size_t size);

int main() {
    size_t size_of_text = START_SIZE_OF_TEXT;
    size_t size_of_fin_text = START_SIZE_OF_TEXT;

    char** text = init_start_text(&size_of_text);
    if (text == NULL) {
        printf("[error]");
        return 0;
    }

    char** fin_text = find_valid_rows(text, size_of_text, &size_of_fin_text);
    if (fin_text == NULL) {
        printf("[error]");
        free_text(text, size_of_text);
        return 0;
    }

    output_result((const char**)fin_text, size_of_fin_text);

    free(fin_text);
    free_text(text, size_of_text);

    return 0;
}

char** init_start_text(size_t* size_of_text) {
    size_t size_of_row = START_SIZE_OF_ROW;

    char** text = create_text(*size_of_text, size_of_row);
    if (text == NULL)
        return NULL;

    char** temp_text = input_text(text, size_of_row, size_of_text);
    if (temp_text == NULL) {
        free_text(text, *size_of_text);
        return 0;
    }

    return temp_text;
}

char** create_text(size_t size_of_text, size_t size_of_row) {
    char** text = (char**)malloc(sizeof(char*) * size_of_text);
    if (text == NULL) {
        return NULL;
    }

    if (size_of_row == 0)
        return text;

    for (size_t i = 0; i < size_of_text; i++) {
        text[i] = (char*)malloc(sizeof(char) * size_of_row);

        if (text[i] == NULL) {
            free_text(text, i);
            return NULL;
        }
    }
    return text;
}

char** input_text(char** text, const size_t size_of_row, size_t* size_of_text) {
    size_t i = 0;
    for (;; i++) {

        char* temp_row = input_row(text[i]);
        if (temp_row == NULL)
            return NULL;

        text[i] = temp_row;

        if (i >= *size_of_text - 1) {
            char** temp_text = realloc_for_text(text, size_of_text, size_of_row);
            if (temp_text == NULL)
                return NULL;

            text = temp_text;

        }

        if (*text[i] == '\0') break;
    }

    char** temp_text = free_not_used_mem(text, size_of_text, i + 1);
    if (temp_text == NULL)
        return NULL;

    text = temp_text;

    return text;
}

char* input_row(char* row_pointer) {
    ptrdiff_t cur_size = 0;
    size_t empty_size = BUFFER_FOR_INPUT;
    size_t filled_size = 0;
    size_t row_size = START_MEM_FOR_INPUT;

    char end_row[empty_size];
    char* temp_row = (char*)realloc(row_pointer, row_size * sizeof(char));
    if (temp_row == NULL) return NULL;
    row_pointer = temp_row;

    row_pointer[0] = '\0';
    while (fgets(end_row, empty_size, stdin)) {
        if (end_row[strlen(end_row) - 1] == '\n') {
            end_row[strlen(end_row) - 1] = '\0';
            memcpy(row_pointer + filled_size - cur_size, end_row, strlen(end_row) + 1);
            break;
        }

        if (filled_size >= row_size) {
            row_size *= 2;
            temp_row = (char*)realloc(row_pointer, row_size * sizeof(char));
            if (temp_row == NULL)
                return NULL;
            row_pointer = temp_row;
        }

        memcpy(row_pointer + filled_size - cur_size, end_row, empty_size);
        filled_size += empty_size;
        cur_size++;
    }

    temp_row = (char*)realloc(row_pointer, (strlen(row_pointer) + 1) * sizeof(char));
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
    if (temp_text == NULL)
        return NULL;
    text = temp_text;

    if (size_of_row == 0)
        return text;

    for (size_t i = *size_of_text / 2; i < *size_of_text; i++) {
        text[i] = (char*)malloc(sizeof(char) * size_of_row);

        if (text[i] == NULL) {
            *size_of_text = i;
            return NULL;
        }
    }

    return text;
}

char** free_not_used_mem(char** text, size_t* size_of_text, const size_t new_size) {
    for (size_t j = new_size; j < *size_of_text; j++) {
        free(text[j]);
    }

    *size_of_text = new_size;
    char** temp_text = (char**)realloc(text, *size_of_text * sizeof(char*));
    if (temp_text == NULL)
        return NULL;

    text = temp_text;
    return text;
}

char** find_valid_rows(char** text, const size_t size_of_text, size_t *size_of_fin_text) {
    char** fin_text = create_text(*size_of_fin_text, 0);
    if (fin_text == NULL)
        return NULL;

    *size_of_fin_text = choice_rows(text, size_of_text, &fin_text);
    if (*size_of_fin_text == size_of_text) {
        free(fin_text);
        return NULL;
    }

    return fin_text;
}

size_t choice_rows(char** text, const size_t size_of_text, char*** fin_text) {
    size_t counter_of_bracket = 0;
    size_t counter_of_row = 0;
    size_t start_size_of_fin_text = START_SIZE_OF_TEXT;

    for (size_t i = 0; *text[i] != '\0'; i++) {
        for (size_t j = 0; j < strlen(text[i]); j++) {
            if (text[i][j] == '(') counter_of_bracket++;
            if (text[i][j] == ')') counter_of_bracket--;
        }

        if (counter_of_bracket == 0) {
            *(*fin_text + counter_of_row) = text[i];

            if (counter_of_row >= start_size_of_fin_text - 1) {
                char** temp_text = realloc_for_text(*fin_text, &start_size_of_fin_text, 0);
                if (temp_text == NULL)
                    return size_of_text;

                *fin_text = temp_text;
            }
            counter_of_row++;
        }
        counter_of_bracket = 0;
    }
    return counter_of_row;
}

void output_result(const char** result_text, const size_t size_of_result) {
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