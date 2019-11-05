#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Character {
    char c;
    int freq;
} Character;

typedef struct Node {
    Character entry;
    struct Node *next;
} Node;

Node *search(char c, Node *list) {
    Node *p = list;
    while (p != NULL) {
        if (p->entry.c == c)
            return p;
        p = p->next;
    }
    return NULL;
}

void add(Node *list, char c, int *size) {
    Node *node = search(c, list);
    if (node != NULL)
        node->entry.freq += 1;
    else {
        Node *new = (Node *) malloc(sizeof(Node));
        new->entry.c = c;
        new->entry.freq = 1;
        
        Node *p = list;
        while (p->next != NULL)
            p = p->next;

        p->next = new;
        new->next = NULL;
        *size += 1;
    }
}

void swap(Character *a, Character *b) {
    Character tmp = *a;
    *a = *b;
    *b = tmp;
}

void quicksort(Character *chars, int min, int max) {
    Character pivot = chars[(min + max) / 2];
    int i = min, j = max;
    while (i <= j) {
        while (chars[i].freq < pivot.freq)
            i += 1;
        while (chars[j].freq > pivot.freq)
            j -= 1;
        if (i <= j) {
            swap(chars + i, chars + j);
            i += 1;
            j -= 1;
        }
    }

    if (min < j)
        quicksort(chars, min, j);
    if (i < max)
        quicksort(chars, i, max);
}

void fillarray(Character *array, Node *list) {
    Node *p = list;
    int i = 0;
    while (p != NULL) {
        array[i] = p->entry;
        p = p->next;
        i += 1;
    }
}

bool check_duplicates(Character *chars, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (chars[i].c == chars[j].c)
                return true;
        }
    }
    return false;
}

Character *read(char *filename) {
    Node *list = (Node *) malloc(sizeof(Character));
    list->next = NULL;
    int size = 0;

    FILE *f = fopen(filename, "r");
    char c;
    while ((c = fgetc(f)) != EOF) {
        if (size == 0) {
            list->entry.c = c;
            list->entry.freq = 1;
            size += 1;
        }
        else {
            add(list, c, &size);
        }
    }

    Character *chars;
    chars = (Character *) malloc(size * sizeof(Character));
    fillarray(chars, list);
    printf("%s\n\n", check_duplicates(chars, size) ? "Duplicates found!" : "No duplicates found");
    free(list);
    quicksort(chars, 0, size - 1);
    return chars;
}

void printchars(Character *chars) {
    for (int i = 0; chars[i].c != '\0'; i++) {
        if (chars[i].c == '\n')
            printf("\\n:\t");
        else
            printf("%c:\t", chars[i].c);
        printf("%d\n", chars[i].freq);
    }
}

int main() {
    Character *chars;
    chars = read("test.txt");
    printchars(chars);
    free(chars);
    return 0;
}