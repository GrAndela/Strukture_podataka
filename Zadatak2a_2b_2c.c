#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME (128)
#define CANT_ALLOCATE_MEMORY_ERROR (-1)
#define EMPTY_LIST_ERROR (-2)
#define WRONG_CHOICE_ERROR (-3)


struct _Student;
typedef struct _Student* Position;
typedef struct _Student {
	char name[MAX_NAME];
	char surname[MAX_NAME];
	int birthYear;
	Position next;
} Student;

int Prepend(Position head, char* name, char* surname, int birthYear);
int Print(Position head);
int Append(Position head, char* name, char* surname, int birthYear);
Position FindLast(Position head);
int AddAfter(Position PositionInList, Position newElement);

int main() {
	Position head = NULL;
	char name[MAX_NAME] = { 0 };
	char surname[MAX_NAME] = { 0 };
	int birthYear = 0;
	int choice = 1;
	int option = 0;

	head = (Position)malloc(sizeof(Student));
	if (!head) {
		printf("Cannot allocate memory!\n");
		return CANT_ALLOCATE_MEMORY_ERROR;
	}

	strcpy(head->name, " ");
	strcpy(head->surname, " ");
	head->birthYear = 0;
	head->next = NULL;

	while (choice != 0) {
		printf("\nAdd new person at the beggining of the list -> 1\n");
		printf("\nPrint list -> 2\n");
		printf("\nAdd new person at the end of the list -> 3\n");
		printf("\nYour choice:\n");
		scanf("%d", &option);


		if (option == 1) {
			printf("Write name,surname and birth year of the new person:\n");
			scanf("%s %s %d", name, surname, &birthYear);
			Prepend(head, name, surname, birthYear);
		}
		else if (option == 2) {
			Print(head->next);
		}
		else if (option == 3) {
			printf("Write name,surname and birth year of the new person:\n");
			scanf("%s %s %d", name, surname, &birthYear);
			Append(head, name, surname, birthYear);
		}
		else {
			printf("You chose wrong option!\n");
			return WRONG_CHOICE_ERROR;
		}
	}


	return EXIT_SUCCESS;
}

int Prepend(Position head, char* name, char* surname, int birthYear) {
	Position newElement = NULL;

	newElement = (Position)malloc(sizeof(Student));
	if (!newElement) {
		printf("Cannot allocate memory!\n");
		return CANT_ALLOCATE_MEMORY_ERROR;
	}

	strcpy(newElement->name, name);
	strcpy(newElement->surname, surname);
	newElement->birthYear = birthYear;
	newElement->next = NULL;
	
	newElement->next = head->next;
	head->next = newElement;

	return EXIT_SUCCESS;
}

int Print(Position head) {
	Position temp = NULL;
	temp = head;

	if (temp == NULL) {
		printf("List is empty!\n");
		return EMPTY_LIST_ERROR;
	}

	while (temp != NULL) {
		printf("Name: %s\n", temp->name);
		printf("Surname: %s\n", temp->surname);
		printf("Birth year: %d\n", temp->birthYear);

		temp = temp->next;
	}

	return EXIT_SUCCESS;
}

int Append(Position head, char* name, char* surname, int birthYear) {
	Position newElement = NULL;
	Position lastElement = NULL;

	newElement = (Position)malloc(sizeof(Student));
	if (!newElement) {
		printf("Cannot allocate memory!\n");
		return CANT_ALLOCATE_MEMORY_ERROR;
	}

	strcpy(newElement->name, name);
	strcpy(newElement->surname, surname);
	newElement->birthYear = birthYear;
	newElement->next = NULL;

	lastElement = FindLast(head);
	AddAfter(lastElement, newElement);

	return EXIT_SUCCESS;
}

Position FindLast(Position head) {
	Position temp = NULL;
	temp = head;

	while (temp->next != NULL) {
		temp = temp->next;
	}

	return temp;
}

int AddAfter(Position PositionInList, Position newElement) {
	newElement->next = PositionInList->next;
	PositionInList->next = newElement;

	return EXIT_SUCCESS;
}

