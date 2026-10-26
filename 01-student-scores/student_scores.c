/*
 * Exercise 1: student scores from a file
 *
 * Counts the lines in the input file, dynamically allocates an array of
 * student structures, reads all records and prints every student's absolute
 * and relative score (relative = points / max points * 100).
 *
 * Input file: one student per line: <first name> <last name> <points>
 * Usage: ./student_scores [file]   (asks for the file name if none is given)
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 32
#define FILE_NAME_LEN 128
#define LINE_LEN 512

typedef struct {
    char firstName[NAME_LEN];
    char lastName[NAME_LEN];
    int points;
} Student;

int countLines(const char* fileName);
int readStudents(const char* fileName, Student* students, int count);
int maxPoints(const Student* students, int count);
void printStudents(const Student* students, int count, int max);

int main(int argc, char* argv[]) {
    char fileName[FILE_NAME_LEN] = "";
    Student* students = NULL;
    int count = 0;

    if (argc > 1) {
        strncpy(fileName, argv[1], FILE_NAME_LEN - 1);
    } else {
        printf("Enter the file name: ");
        if (scanf("%127s", fileName) != 1) return 1;
    }

    count = countLines(fileName);
    if (count < 0) {
        printf("The file '%s' could not be opened.\n", fileName);
        return 1;
    }
    if (count == 0) {
        printf("The file is empty.\n");
        return 1;
    }

    students = (Student*)malloc(sizeof(Student) * count);
    if (students == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    count = readStudents(fileName, students, count);
    if (count <= 0) {
        printf("No valid records were read.\n");
        free(students);
        return 1;
    }

    printStudents(students, count, maxPoints(students, count));

    free(students);
    return 0;
}

/* Returns the number of non-empty lines, or -1 if the file cannot be opened. */
int countLines(const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    char buffer[LINE_LEN];
    int counter = 0;

    if (fp == NULL) return -1;

    while (fgets(buffer, LINE_LEN, fp) != NULL) {
        if (strspn(buffer, " \t\r\n") < strlen(buffer)) counter++;  /* line has some content */
    }

    fclose(fp);
    return counter;
}

/* Reads up to `count` records and returns how many were read. */
int readStudents(const char* fileName, Student* students, int count) {
    FILE* fp = fopen(fileName, "r");
    int i = 0;

    if (fp == NULL) return -1;

    while (i < count &&
           fscanf(fp, " %31s %31s %d", students[i].firstName, students[i].lastName, &students[i].points) == 3) {
        i++;
    }

    fclose(fp);
    return i;
}

int maxPoints(const Student* students, int count) {
    int max = students[0].points;
    for (int i = 1; i < count; i++) {
        if (students[i].points > max) max = students[i].points;
    }
    return max;
}

void printStudents(const Student* students, int count, int max) {
    printf("%-15s %-15s %10s %10s\n", "First name", "Last name", "Points", "Relative");
    for (int i = 0; i < count; i++) {
        double relative = max > 0 ? (double)students[i].points / max * 100.0 : 0.0;
        printf("%-15s %-15s %10d %9.2f%%\n",
               students[i].firstName, students[i].lastName, students[i].points, relative);
    }
}
