/*
 * Exam practice: sorted linked list built from a file
 *
 * Reads gym memberships (user ID, first name, last name, month, year, price)
 * from a file and inserts each one into a linked list kept sorted by year and
 * month. Then it picks a random month and year, deletes all memberships from
 * that period and prints the list before and after.
 *
 * Input file: <user id> <first name> <last name> <month> <year> <price> per line
 * Usage: ./memberships [file] [seed]   (asks for the file name if none is given)
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FIELD_LEN 32
#define FILE_NAME_LEN 128
#define FIRST_YEAR 2002
#define LAST_YEAR 2005

struct _membership;
typedef struct _membership* Position;

typedef struct _membership {
    char userId[FIELD_LEN];
    char firstName[FIELD_LEN];
    char lastName[FIELD_LEN];
    int month;
    int year;
    int price;
    Position next;
} Membership;

int readSortedFromFile(Position head, const char* fileName);
void insertSorted(Position head, Position q);
void printList(Position first);
int deletePeriod(Position head, int month, int year);
void deleteAll(Position head);

int main(int argc, char* argv[]) {
    Membership head = { "", "", "", -1, -1, -1, NULL };
    char fileName[FILE_NAME_LEN] = "";

    if (argc > 1) {
        strncpy(fileName, argv[1], FILE_NAME_LEN - 1);
    } else {
        printf("Enter the file name: ");
        if (scanf("%127s", fileName) != 1) return 1;
    }
    srand(argc > 2 ? (unsigned)atoi(argv[2]) : (unsigned)time(NULL));

    if (readSortedFromFile(&head, fileName) < 0) {
        printf("The file '%s' could not be opened.\n", fileName);
        return 1;
    }

    printf("Sorted by year and month:\n");
    printList(head.next);

    int month = rand() % 12 + 1;                                  /* 1-12 */
    int year = FIRST_YEAR + rand() % (LAST_YEAR - FIRST_YEAR + 1); /* FIRST_YEAR-LAST_YEAR */
    int removed = deletePeriod(&head, month, year);
    printf("\nRandom period %02d/%d: removed %d membership(s)\n", month, year, removed);
    printList(head.next);

    deleteAll(&head);
    return 0;
}

int readSortedFromFile(Position head, const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    Membership tmp;
    int count = 0;

    if (fp == NULL) return -1;

    while (fscanf(fp, " %31s %31s %31s %d %d %d", tmp.userId, tmp.firstName, tmp.lastName,
                  &tmp.month, &tmp.year, &tmp.price) == 6) {
        Position q = (Position)malloc(sizeof(Membership));
        if (q == NULL) {
            printf("Memory allocation failed.\n");
            break;
        }
        *q = tmp;
        insertSorted(head, q);
        count++;
    }

    fclose(fp);
    return count;
}

/* Inserts q after all elements with an earlier (year, month). */
void insertSorted(Position head, Position q) {
    Position prev = head;
    while (prev->next != NULL &&
           (prev->next->year < q->year ||
            (prev->next->year == q->year && prev->next->month <= q->month))) {
        prev = prev->next;
    }
    q->next = prev->next;
    prev->next = q;
}

void printList(Position first) {
    if (first == NULL) {
        printf("(empty)\n");
        return;
    }
    while (first != NULL) {
        printf("%-8s %-10s %-10s %02d/%d %5d\n",
               first->userId, first->firstName, first->lastName, first->month, first->year, first->price);
        first = first->next;
    }
}

/* Deletes every membership from the given month and year; returns how many were deleted. */
int deletePeriod(Position head, int month, int year) {
    int removed = 0;
    while (head->next != NULL) {
        if (head->next->year == year && head->next->month == month) {
            Position temp = head->next;
            head->next = temp->next;
            free(temp);
            removed++;
        } else {
            head = head->next;
        }
    }
    return removed;
}

void deleteAll(Position head) {
    while (head->next != NULL) {
        Position temp = head->next;
        head->next = temp->next;
        free(temp);
    }
}
