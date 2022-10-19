#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE (1024)
#define MAX_NAME (128)
#define FILE_DIDNT_OPEN_ERROR (-1)
#define MAX_BUFFER_SIZE (128)
#define MEMORY_DIDNT_ALLOCATE_ERROR (-2)

struct _Student;
typedef struct _Student {
	char name[MAX_NAME];
	char surname[MAX_NAME];
	int points;
} Student;

int CountStudentsFromFile(char* filename);
Student* AllocateMemoryAndReadFromFile(char* filename, int numStudents);
int Print(Student* students, int numStudents,double maxPoints);

int main() {
	Student* students= NULL;
	char* filename = "studenti.txt";
	int numStudents = 0;
	double maxPoints = 0;

	numStudents = CountStudentsFromFile(filename);
	if (numStudents < 0) {
		return 0;
	}

	students = AllocateMemoryAndReadFromFile(filename, numStudents);
	if (!students) {
		printf("Error while allocating the memory!\n");
		return MEMORY_DIDNT_ALLOCATE_ERROR;
	}

	for (int i = 0; i < numStudents; i++) {
		if (maxPoints < students[i].points) {
			maxPoints = students[i].points;
		}
	}

	Print(students, numStudents, maxPoints);


	return EXIT_SUCCESS;
}

int CountStudentsFromFile(char* filename) {
	FILE* file = NULL;
	char buffer[MAX_BUFFER_SIZE] = { 0 };
	int count = 0;

	file = fopen(filename, "r");

	if (!file) {
		printf("Cannot open file!\n");
		return FILE_DIDNT_OPEN_ERROR;
	}

	while (!feof(file)) {
		fgets(buffer, MAX_LINE, file);
		if (strcmp(buffer, "\n") != 0) {
			count++;
		}
	}

	fclose(file);

	return count;
}

Student* AllocateMemoryAndReadFromFile(char* filename, int numStudents) {
	FILE* file = NULL;
	Student* students = NULL;
	int counter = 0;

	students = (Student*)malloc(sizeof(Student) * numStudents);

	if (!students) {
		printf("Cannot allocate memory!\n");
		return MEMORY_DIDNT_ALLOCATE_ERROR;
	}

	file = fopen(filename, "r");

	if (!file) {
		printf("Cannot open file!\n");
		return FILE_DIDNT_OPEN_ERROR;
	}

	while (!feof(file)) {
		fscanf(file, "%s %s %d", students[counter].name, students[counter].surname, &students[counter].points);
		counter++;
	}

	fclose(file);

	return students;
}

int Print(Student* students, int numStudents,double maxPoints) {
	printf("List of students:\n");

	for (int i = 0; i < numStudents; i++) {
		printf("Name:%s\n", students[i].name);
		printf("Surname:%s\n", students[i].surname);
		printf("Points:%d\n", students[i].points);
		printf("Relative points:%lf\n\n", students[i].points / maxPoints * 100);
	}

	return EXIT_SUCCESS;
}