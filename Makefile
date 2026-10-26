CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic -O2

PROGRAMS = 01-student-scores/student_scores \
           02-linked-list/linked_list \
           03-sorted-list-from-file/memberships

all: $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Runs every program on its sample input
run: all
	./01-student-scores/student_scores 01-student-scores/students.txt
	./03-sorted-list-from-file/memberships 03-sorted-list-from-file/memberships.txt 2
	cd 02-linked-list && printf '10\npeople.txt\n8\n0\n' | ./linked_list

clean:
	rm -f $(PROGRAMS) $(addsuffix .exe,$(PROGRAMS))

.PHONY: all run clean
