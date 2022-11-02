#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME (128)
#define CANT_ALLOCATE_MEMORY_ERROR (-1)
#define EMPTY_LIST_ERROR (-2)
#define WRONG_CHOICE_ERROR (-3)
#define NO_PERSON_IN_LIST_ERROR (-4)
#define CANT_OPEN_FILE_ERROR (-5)


struct _Student;
typedef struct _Student* Position;
typedef struct _Student {
	char name[MAX_NAME];
	char surname[MAX_NAME];
	int birthYear;
	Position next;
} Student;

Position CreateNewStudent();
int Input(Position newElement);
int Prepend(Position head, char* name, char* surname, int birthYear);
int Print(Position head);
int Append(Position head, char* name, char* surname, int birthYear);
Position FindLast(Position head);
int AddAfter(Position head, Position previous);
Position FindElementBySurname(Position head, char* surname);
int DeleteElement(Position head, char* surname);
Position FindBefore(Position head, char* surname);
int AddBefore(Position head, Position nextElement);
int Sort(Position head);
int DeleteAll(Position head);


int main() {
	Position head = CreateNewStudent();
	char name[MAX_NAME] = { 0 };
	char surname[MAX_NAME] = { 0 };
	int birthYear = 0;
	int choice = 1;
	int option = 0;
	Position temp = NULL;
	char filename[MAX_NAME] = { 0 };
	Position wanted = NULL;


	while (choice != 0) {
		printf("\nAdd new person at the beggining of the list -> 1\n");
		printf("\nPrint list -> 2\n");
		printf("\nAdd new person at the end of the list -> 3\n");
		printf("\nFind person by surname -> 4\n");
		printf("\nDelete person from the list -> 5\n");
		printf("\nAdd person after another one -> 6\n");
		printf("\nAdd person before another one -> 7\n");
		printf("\nSort list -> 8\n");
		printf("\nExit -> 9\n");
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
		else if (option == 4) {
			printf("Write surname of the person you want to find:\n");
			scanf("%s", surname);
			temp = FindElementBySurname(head, surname);
			if (temp == NULL) {
				printf("That person is not in the list!\n");
				return NO_PERSON_IN_LIST_ERROR;
			}
			else {
				printf("Name: %s\n", temp->name);
				printf("Surname: %s\n", temp->surname);
				printf("Birth year: %d\n", temp->birthYear);
			}

		}
		else if (option == 5) {
			printf("Write surname of the person you want to delete:\n");
			scanf("%s", surname);
			DeleteElement(head, surname);
		}
		else if (option == 6) {
			printf("Enter the surname of the student you want to add person after:\n");
			scanf("%s", surname);
			wanted = FindElementBySurname(head, surname);
			AddAfter(head, wanted);
		}
		else if (option == 7) {
			printf("Enter the surname of the student you want to add person before:\n");
			scanf("%s", surname);
			wanted = FindElementBySurname(head, surname);
			AddBefore(head, wanted);
		}
		else if (option == 8) {
			Sort(head);
		}
		else if (option == 9) {
			printf("Goodbye!\n");
		}
	}

	DeleteAll(head);


	return EXIT_SUCCESS;
}

Position CreateNewStudent() {
	Position newElement = NULL;

	newElement = (Position)malloc(sizeof(Student));
	if (!newElement) {
		printf("Cannot allocate memory!\n");
		return CANT_ALLOCATE_MEMORY_ERROR;
	}

	strcpy(newElement->name, " ");
	strcpy(newElement->surname, " ");
	newElement->birthYear = 0;
	newElement->next = NULL;

	return newElement;
}

int Input(Position newElement) {
	char buffer[MAX_NAME] = { 0 };

	printf("Enter the name of the new student:\n");
	scanf("%s", buffer);
	strcpy(newElement->name, buffer);

	printf("Enter the surname of the new student:\n");
	scanf("%s", buffer);
	strcpy(newElement->surname, buffer);

	printf("Enter the birth year of the new student:\n");
	scanf("%d", &newElement->birthYear);

	newElement->next = NULL;

	return EXIT_SUCCESS;
}

int Prepend(Position head, char* name, char* surname, int birthYear) {
	Position newElement = CreateNewStudent();

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
	Position newElement = CreateNewStudent();
	Position lastElement = CreateNewStudent();

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

int AddAfter(Position head, Position previous) {
	Position newElement = CreateNewStudent();
	Position temp = head;

	while (temp != previous) {
		temp = temp->next;
	}

	Input(newElement);

	newElement->next = temp->next;
	temp->next = newElement;


	return EXIT_SUCCESS;
}

Position FindElementBySurname(Position head, char* surname) {
	Position temp = NULL;
	temp = head;

	while (temp != NULL) {
		if (strcmp(temp->surname, surname) == 0) {
			return temp;
		}

		temp = temp->next;
	}

	return EXIT_SUCCESS;
}

int DeleteElement(Position head, char* surname) {
	Position ElementBefore = NULL;
	Position ToDelete = NULL;

	ElementBefore = FindBefore(head, surname);
	if (ElementBefore == NULL) {
		printf("This person is not in the list!\n");
		return NO_PERSON_IN_LIST_ERROR;
	}
	else {
		ToDelete = ElementBefore->next;
		ElementBefore->next = ToDelete->next;
		free(ToDelete);
	}

	return EXIT_SUCCESS;
}

Position FindBefore(Position head, char* surname) {
	Position temp = NULL;
	temp = head;

	for (temp; temp->next; temp = temp->next) {
		if (!strcmp(temp->next->surname, surname))
			return temp;
	}

	return EXIT_SUCCESS;
}

int AddBefore(Position head, Position nextElement) {
	Position newElement = CreateNewStudent();
	Position temp = head;

	while (temp->next != nextElement) {
		temp = temp->next;
	}

	Input(newElement);

	newElement->next = temp->next;
	temp->next = newElement;

	return EXIT_SUCCESS;
}

int Sort(Position head) {
	Position p = head;
	Position q = NULL;
	Position pre_q = NULL;
	Position last = NULL;

	while (p->next != last)
	{
		pre_q = p;
		q = pre_q->next;

		while (q->next != last)
		{
			if (strcmp(q->surname, q->next->surname) > 0)
			{
				pre_q->next = q->next;
				q->next = q->next->next;
				pre_q->next->next = q;
				q = pre_q->next;
			}
			pre_q = q;
			q = q->next;
		}
		last = q;
	}

	return EXIT_SUCCESS;
}


int DeleteAll(Position head) {
	while (head->next != NULL) {
		DeleteElement(head->next, head->next->surname);
		head = head->next;
	}

	return EXIT_SUCCESS;
}


