#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE (1024)
#define MAX_NAME (128)

#define CANT_OPEN_FILE_ERROR (-1)
#define WRONG_FILE_FORMAT_ERROR (-2)
#define CANT_ALLOCATE_MEMORY_ERROR (-3)

struct _Element;
typedef struct _Element* Position;
typedef struct _Element {
	int coefficient;
	int exponent;
	Position next;
} Element;

int EnterFilename(char* filename);
int ReadFile(char* filename, Position head1, Position head2);
int ParseStringIntoList(Position head, char* buffer);
Position CreateNewElement(int coefficient, int exponent);
int InsertSorted(Position head, Position newElement);
int MergeAfter(Position position, Position newElement);
int AddAfter(Position position, Position newElement);
int DeleteAfter(Position previous);
int CreateAndAddAfter(int coefficient, int exponent, Position position);
int PrintPol(char* name, Position first);
int AddPol1(Position resultHead, Position head1, Position head2);
int AddPol2(Position resultHead, Position head1, Position head2);
int MultiplyPol(Position resultHead, Position head1, Position head2);
int DeleteAll(Position head);


int main() {
	Element head1 = { .coefficient = 0,.exponent = 0,.next = NULL };
	Element head2 = { .coefficient = 0,.exponent = 0,.next = NULL };
	Element resultAdd = { .coefficient = 0,.exponent = 0,.next = NULL };
	Element resultMult = { .coefficient = 0,.exponent = 0,.next = NULL };
	char filename[MAX_NAME] = { 0 };

	EnterFilename(filename);

	if (ReadFile(filename, &head1, &head2) == EXIT_SUCCESS) {
		PrintPol("Pol 1", head1.next);
		PrintPol("Pol 2", head2.next);

		AddPol1(&resultAdd, &head1, &head2);
		MultiplyPol(&resultMult, &head1, &head2);

		PrintPol("Pol. add", resultAdd.next);
		PrintPol("Pol. mult.", resultMult.next);

		DeleteAll(&head1);
		DeleteAll(&head2);
		DeleteAll(&resultAdd);
		DeleteAll(&resultMult);

	}


	return EXIT_SUCCESS;
}


int EnterFilename(char* filename) {

	printf("Enter the name of the file:\n");
	scanf("%s", filename);

	return EXIT_SUCCESS;
}

int ReadFile(char* filename, Position head1, Position head2) {
	FILE* file = NULL;
	char buffer[MAX_LINE] = { 0 };
	int status = EXIT_SUCCESS;

	file = fopen(filename, "r");
	if (!file) {
		printf("Cannot open the file!\n");
		return CANT_OPEN_FILE_ERROR;
	}

	fgets(buffer, MAX_LINE, file);
	status = ParseStringIntoList(head1, buffer);

	if (status != EXIT_SUCCESS)
		return EXIT_FAILURE;

	fgets(buffer, MAX_LINE, file);
	status = ParseStringIntoList(head2, buffer);

	if (status != EXIT_SUCCESS)
		return EXIT_FAILURE;

	fclose(file);

	return EXIT_SUCCESS;
}

int ParseStringIntoList(Position head, char* buffer) {
	char* currentbuffer = buffer;
	int coefficient = 0;
	int exponent = 0;
	int numBytes = 0;
	int status = 0;
	Position newElement = NULL;

	while (strlen(currentbuffer) > 0) {

		status = sscanf(currentbuffer, "%d %d %n", &coefficient, &exponent, &numBytes);
		if (status != 2) {
			printf("Wrong format of the file!\n");
			return WRONG_FILE_FORMAT_ERROR;
		}

		newElement = CreateNewElement(coefficient, exponent);
		if (!newElement) {
			printf("Cannot allocate memory!\n");
			return CANT_ALLOCATE_MEMORY_ERROR;
		}

		InsertSorted(head, newElement);

		currentbuffer += numBytes;
	}

	return EXIT_SUCCESS;
}

Position CreateNewElement(int coefficient, int exponent) {
	Position newElement = NULL;

	newElement = (Position)malloc(sizeof(Element));
	if (!newElement) {
		printf("Cannot allocate memory!\n");
		return CANT_ALLOCATE_MEMORY_ERROR;
	}

	newElement->coefficient = coefficient;
	newElement->exponent = exponent;
	newElement->next = NULL;

	return newElement;
}

int InsertSorted(Position head, Position newElement) {
	Position temp = head;

	while (temp->next != NULL && temp->next->exponent < newElement->exponent)
		temp = temp->next;

	MergeAfter(temp, newElement);

	return EXIT_SUCCESS;
}

int MergeAfter(Position position, Position newElement) {
	int resultCoefficient = 0;

	if (position->next == NULL || position->next->exponent != newElement->exponent) {
		AddAfter(position, newElement);
	}
	else {
		resultCoefficient = position->next->coefficient + newElement->coefficient;
		if (resultCoefficient == 0) {
			DeleteAfter(position);
		}
		else {
			position->next->coefficient = resultCoefficient;
		}

		free(newElement);
	}

	return EXIT_SUCCESS;
}

int AddAfter(Position position, Position newElement) {
	newElement->next = position->next;
	position->next = newElement;

	return EXIT_SUCCESS;
}

int DeleteAfter(Position previous) {
	Position toDelete = NULL;

	toDelete = previous->next;
	previous->next = toDelete->next;
	free(toDelete);

	return EXIT_SUCCESS;
}

int CreateAndAddAfter(int coefficient, int exponent, Position position) {
	Position newElement = CreateNewElement(coefficient, exponent);
	if (!newElement) {
		printf("Error while allocating memory!\n");
		return CANT_ALLOCATE_MEMORY_ERROR;
	}

	AddAfter(position, newElement);
	position = position->next;

	return EXIT_SUCCESS;
}

int PrintPol(char* name, Position first) {
	printf("%s =", name);

	if (first) {
		if (first->exponent < 0) {
			if (first->coefficient == 1) {
				printf("x^(%d)", first->exponent);
			}
			else {
				printf("%dx^(%d)", first->coefficient, first->exponent);
			}
		}
		else {
			if (first->coefficient == 1) {
				printf("x^%d", first->exponent);
			}
			else {
				printf("%dx^%d", first->coefficient, first->exponent);
			}
		}

		first = first->next;
	}

	for (; first != NULL; first = first->next) {
		if (first->coefficient < 0) {
			if (first->exponent < 0) {
				printf("-%dx^(%d)", abs(first->coefficient), first->exponent);
			}
			else {
				printf("-%dx^%d", abs(first->coefficient), first->exponent);
			}
		}
		else {
			if (first->coefficient == 1) {
				printf("+x^%d", first->exponent);
			}
			else {
				printf("+%dx^%d", first->coefficient, first->exponent);
			}
		}
	}

	printf("\n");

	return EXIT_SUCCESS;
}

int AddPol1(Position resultHead, Position head1, Position head2) {
	Position p = head1->next;
	Position q = head2->next;
	Position k = resultHead;
	Position temp = NULL;

	while (p != NULL && q != NULL) {
		if (p->exponent == q->exponent) {
			CreateAndAddAfter(p->coefficient + q->coefficient, p->exponent, k);
			p = p->next;
			q = q->next;
		}
		else if (p->exponent < q->exponent) {
			CreateAndAddAfter(p->coefficient, p->exponent, k);
			p = p->next;
		}
		else {
			CreateAndAddAfter(q->coefficient, q->exponent, k);
			q = q->next;
		}
	}

	if (p == NULL) {
		temp = q;
	}
	else {
		temp = p;
	}

	while (temp != NULL) {
		CreateAndAddAfter(temp->coefficient, temp->exponent, k);
		temp = temp->next;
	}

	return EXIT_SUCCESS;
}

int AddPol2(Position resultHead, Position head1, Position head2) {
	Position p = NULL;
	Position q = NULL;
	Position newElement = NULL;

	for (p = head1->next; p != NULL; p = p->next) {
		newElement = CreateNewElement(p->coefficient, p->exponent);
		if (!newElement) {
			printf("Error while allocating memory!\n");
			return CANT_ALLOCATE_MEMORY_ERROR;
		}

		InsertSorted(resultHead, newElement);
	}

	for (q = head2->next; q != NULL; q = q->next) {
		newElement = CreateNewElement(q->coefficient, q->exponent);
		if (!newElement) {
			printf("Error while allocating memory!\n");
			return CANT_ALLOCATE_MEMORY_ERROR;
		}

		InsertSorted(resultHead, newElement);
	}

	return EXIT_SUCCESS;
}

int MultiplyPol(Position resultHead, Position head1, Position head2) {
	Position p = NULL;
	Position q = NULL;
	Position newElement = NULL;

	for (p = head1->next; p != NULL; p = p->next) {
		for (q = head2->next; q != NULL; q = q->next) {
    
			newElement = CreateNewElement(p->coefficient * q->coefficient, p->exponent + q->exponent);
			newElement = CreateNewElement(p->coefficient * q->coefficient, p->exponent * q->exponent);

			if (!newElement) {
				printf("Error while allocating memory!\n");
				return CANT_ALLOCATE_MEMORY_ERROR;
			}

			InsertSorted(resultHead, newElement);
		}
	}

	return EXIT_SUCCESS;
}

int DeleteAll(Position head) {
	Position temp = head;

	while (temp->next) {
		DeleteAfter(temp);
	}

	return EXIT_SUCCESS;
}
