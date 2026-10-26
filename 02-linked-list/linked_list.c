/*
 * Exercises 2 and 3: singly linked list of people
 *
 * Each element holds a first name, a last name and a birth year. The list
 * uses a head (sentinel) node and no global variables. Operations:
 *   - insert at the front / at the end
 *   - find by last name, delete by last name
 *   - insert after / before a given element
 *   - sort by last name (bubble sort that swaps nodes, not data)
 *   - write the list to a file / read the list from a file
 *   - free all elements
 *
 * Usage: ./linked_list   (interactive menu, reads from standard input)
 */
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 32
#define FILE_NAME_LEN 128

struct _person;
typedef struct _person* Position;

typedef struct _person {
    char firstName[NAME_LEN];
    char lastName[NAME_LEN];
    int birthYear;
    Position next;
} Person;

Position createPerson(void);
void insertFront(Position head);
void insertBack(Position head);
Position findByLastName(Position first, const char* lastName);
Position findPrevious(Position head, const char* lastName);
void printList(Position first);
void deleteByLastName(Position head, const char* lastName);
void insertAfter(Position head, const char* lastName);
void insertBefore(Position head, const char* lastName);
void sortByLastName(Position head);
int writeToFile(Position first, const char* fileName);
int readFromFile(Position head, const char* fileName);
void deleteAll(Position head);
int compareIgnoreCase(const char* a, const char* b);
void readWord(const char* prompt, char* buffer);

int main(void) {
    Person head = { "", "", 0, NULL };
    char name[NAME_LEN];
    char fileName[FILE_NAME_LEN];
    int choice = -1;

    while (choice != 0) {
        printf("\n1  insert at the front\n");
        printf("2  insert at the end\n");
        printf("3  find by last name\n");
        printf("4  print the list\n");
        printf("5  delete by last name\n");
        printf("6  insert after an element\n");
        printf("7  insert before an element\n");
        printf("8  sort by last name\n");
        printf("9  write the list to a file\n");
        printf("10 read the list from a file\n");
        printf("0  exit\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1) break;  /* end of input */

        switch (choice) {
        case 1: insertFront(&head); break;
        case 2: insertBack(&head); break;
        case 3: {
            readWord("Last name: ", name);
            Position p = findByLastName(head.next, name);
            if (p) printf("Found: %s %s %d\n", p->firstName, p->lastName, p->birthYear);
            else printf("Not found.\n");
            break;
        }
        case 4: printList(head.next); break;
        case 5: readWord("Last name to delete: ", name); deleteByLastName(&head, name); break;
        case 6: readWord("Insert after (last name): ", name); insertAfter(&head, name); break;
        case 7: readWord("Insert before (last name): ", name); insertBefore(&head, name); break;
        case 8: sortByLastName(&head); printList(head.next); break;
        case 9:
            readWord("File name: ", fileName);
            if (writeToFile(head.next, fileName) != 0) printf("The file could not be opened.\n");
            break;
        case 10:
            readWord("File name: ", fileName);
            if (readFromFile(&head, fileName) < 0) printf("The file could not be opened.\n");
            break;
        case 0: break;
        default: printf("Invalid choice.\n");
        }
    }

    deleteAll(&head);
    return 0;
}

/* Allocates a new element and reads its data from the user. */
Position createPerson(void) {
    Position q = (Position)malloc(sizeof(Person));
    if (q == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    printf("First name, last name and birth year: ");
    if (scanf(" %31s %31s %d", q->firstName, q->lastName, &q->birthYear) != 3) {
        free(q);
        return NULL;
    }
    q->next = NULL;
    return q;
}

void insertFront(Position head) {
    Position q = createPerson();
    if (q == NULL) return;
    q->next = head->next;
    head->next = q;
}

void insertBack(Position head) {
    Position q = createPerson();
    if (q == NULL) return;
    while (head->next != NULL) head = head->next;  /* go to the last element */
    head->next = q;
}

Position findByLastName(Position first, const char* lastName) {
    while (first != NULL && compareIgnoreCase(first->lastName, lastName) != 0) first = first->next;
    return first;
}

/* Returns the element before the one with the given last name, or NULL. */
Position findPrevious(Position head, const char* lastName) {
    while (head->next != NULL && compareIgnoreCase(head->next->lastName, lastName) != 0) head = head->next;
    return head->next == NULL ? NULL : head;
}

void printList(Position first) {
    if (first == NULL) {
        printf("The list is empty.\n");
        return;
    }
    while (first != NULL) {
        printf("%s %s %d\n", first->firstName, first->lastName, first->birthYear);
        first = first->next;
    }
}

void deleteByLastName(Position head, const char* lastName) {
    Position prev = findPrevious(head, lastName);
    if (prev == NULL) {
        printf("Not found.\n");
        return;
    }
    Position temp = prev->next;
    prev->next = temp->next;
    printf("Deleted: %s %s %d\n", temp->firstName, temp->lastName, temp->birthYear);
    free(temp);
}

void insertAfter(Position head, const char* lastName) {
    Position p = findByLastName(head->next, lastName);
    if (p == NULL) {
        printf("Not found.\n");
        return;
    }
    Position q = createPerson();
    if (q == NULL) return;
    q->next = p->next;
    p->next = q;
}

void insertBefore(Position head, const char* lastName) {
    Position prev = findPrevious(head, lastName);
    if (prev == NULL) {
        printf("Not found.\n");
        return;
    }
    Position q = createPerson();
    if (q == NULL) return;
    q->next = prev->next;
    prev->next = q;
}

/*
 * Bubble sort by last name that relinks nodes instead of copying data.
 * After each pass the largest remaining element is at the end, and `end`
 * marks the start of the already sorted tail.
 */
void sortByLastName(Position head) {
    Position end = NULL;

    while (head->next != end) {
        Position before = head;          /* element before the pair */
        Position first = before->next;   /* first element of the pair */
        Position second = first->next;   /* second element of the pair */

        while (second != end) {
            if (compareIgnoreCase(first->lastName, second->lastName) > 0) {
                first->next = second->next;   /* swap the two nodes */
                before->next = second;
                second->next = first;
                before = second;
            } else {
                before = first;
                first = second;
            }
            second = first->next;
        }
        end = first;
    }
}

int writeToFile(Position first, const char* fileName) {
    FILE* fp = fopen(fileName, "w");
    if (fp == NULL) return -1;
    while (first != NULL) {
        fprintf(fp, "%s %s %d\n", first->firstName, first->lastName, first->birthYear);
        first = first->next;
    }
    fclose(fp);
    return 0;
}

/* Appends the people from the file to the end of the list. Returns the number read or -1. */
int readFromFile(Position head, const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    Person tmp;
    int count = 0;

    if (fp == NULL) return -1;
    while (head->next != NULL) head = head->next;

    while (fscanf(fp, " %31s %31s %d", tmp.firstName, tmp.lastName, &tmp.birthYear) == 3) {
        Position q = (Position)malloc(sizeof(Person));
        if (q == NULL) break;
        *q = tmp;
        q->next = NULL;
        head->next = q;
        head = q;
        count++;
    }
    fclose(fp);
    printf("Read %d people.\n", count);
    return count;
}

void deleteAll(Position head) {
    while (head->next != NULL) {
        Position temp = head->next;
        head->next = temp->next;
        free(temp);
    }
}

/* Case-insensitive string comparison (portable replacement for _strcmpi). */
int compareIgnoreCase(const char* a, const char* b) {
    while (*a && tolower((unsigned char)*a) == tolower((unsigned char)*b)) {
        a++;
        b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

void readWord(const char* prompt, char* buffer) {
    printf("%s", prompt);
    if (scanf(" %31s", buffer) != 1) buffer[0] = '\0';
}
