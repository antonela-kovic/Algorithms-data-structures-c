# Algorithms and Data Structures in C

Selected lab exercises from the *Algorithms and Data Structures* course (2nd year, Computer Science, FESB, University of Split): dynamic memory, structures, file I/O and singly linked lists in C, cleaned up and translated into English.

| Folder | Topic | Highlights |
|---|---|---|
| [`01-student-scores`](01-student-scores/student_scores.c) | Array of structures from a file | count lines, `malloc` an array of structs, read records, relative scores |
| [`02-linked-list`](02-linked-list/linked_list.c) | Singly linked list (exercises 2 and 3) | head node, insert front/back/after/before, find, delete, **bubble sort that relinks nodes**, save to and load from a file |
| [`03-sorted-list-from-file`](03-sorted-list-from-file/memberships.c) | Sorted linked list (exam practice) | sorted insertion by year and month, deleting all nodes that match a condition |

All programs use no global variables, check every `malloc` and `fopen`, and free all memory before exiting (checked with Valgrind).

## Build and run

Requires a C compiler (GCC, Clang or MSVC) and `make`.

```bash
make          # builds all three programs
make run      # runs them on the sample input files
make clean
```

Or compile a single program:

```bash
gcc -std=c11 -Wall -Wextra -o student_scores 01-student-scores/student_scores.c
./student_scores 01-student-scores/students.txt
```

### 1. Student scores

```
$ ./student_scores students.txt
First name      Last name           Points   Relative
Ana             Horvat                  42     84.00%
Maja            Babic                   50    100.00%
...
```

### 2. Linked list

An interactive menu that reads from standard input. Example session: load `people.txt` (option 10), sort it (8), exit (0):

```bash
cd 02-linked-list
printf '10\npeople.txt\n8\n0\n' | ./linked_list
```

The sort is a bubble sort that swaps the nodes themselves (pointer relinking) instead of copying the data, so it works for elements of any size.

### 3. Sorted list from a file

```bash
./memberships memberships.txt 2    # optional second argument: random seed
```

Each record is inserted at its sorted position (year, then month) while reading the file. A random month and year are then drawn and all memberships from that period are removed.

## License

MIT
