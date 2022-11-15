#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME (128)

#define CANT_ALLOCATE_MEMORY_ERROR (-1)
#define NOT_VALID_POSTFIX_ERROR (-2)
#define PERFORMING_OPERATION_ERROR (-3)
#define DIVIDING_WITH_ZERO_ERROR (-4)
#define WRONG_OPERATION_ERROR (-5)
#define CANT_OPEN_FILE_ERROR (-6)


struct _Element;
typedef struct _Element* Position;
typedef struct _Element {
	int number;
	Position next;
} Element;

Position CreateElement(int number);
int AddAfter(Position position, Position newElement);
int Push(Position head, int number);
int DeleteAfter(Position position);
int Pop(int* result, Position head);
int PerformOperations(Position head, char operation);
int CalculatePostfixFromFile(int* result, char* filename);

int main() {
	char filename[MAX_NAME] = { 0 };
	int result = 0;

	printf("Enter the file name:\n");
	scanf("%s", filename);

	if (CalculatePostfixFromFile(&result, filename) == EXIT_SUCCESS) {
		printf("The postfix result is: %d\n", result);
		return EXIT_SUCCESS;
	}
	else
		return EXIT_FAILURE;

}

Position CreateElement(int number) {
	Position newElement = NULL;

	newElement = (Position)malloc(sizeof(Element));
	if (!newElement) {
		printf("Cannot allocate memory!\n");
		return CANT_ALLOCATE_MEMORY_ERROR;
	}

	newElement->number = number;
	newElement->next = NULL;

	return newElement;
}

int AddAfter(Position position, Position newElement) {
	newElement->next = position->next;
	position->next = newElement;

	return EXIT_SUCCESS;
}

int Push(Position head, int number) {
	Position newElement = NULL;

	newElement = CreateElement(number);

	AddAfter(head, newElement);

	return EXIT_SUCCESS;
}

int DeleteAfter(Position position) {
	Position temp = position->next;

	if (temp) {
		return EXIT_SUCCESS;
	}

	position->next = temp->next;
	free(temp);

	return EXIT_SUCCESS;
}

int Pop(int* result, Position head) {
	Position first = head->next;

	if (!first) {
		printf("Postfix is not valid!Check your file!\n");
		return NOT_VALID_POSTFIX_ERROR;
	}

	*result = first->number;
	DeleteAfter(head);

	return EXIT_SUCCESS;
}

int PerformOperations(Position head, char operation) {
	int operand1 = 0;
	int operand2 = 0;
	int status1 = 0;
	int status2 = 0;
	int result = 0;

	status2 = Pop(&operand2, head);

	if (status2 != EXIT_SUCCESS) {
		return PERFORMING_OPERATION_ERROR;
	}

	status1 = Pop(&operand1, head);

	if (status1 != EXIT_SUCCESS) {
		return PERFORMING_OPERATION_ERROR;
	}

	switch (operation) {
	case '+':
		result = operand1 + operand2;
		break;
	case '-':
		result = operand1 - operand2;
		break;
	case '*':
		result = operand1 * operand2;
		break;
	case '/':
		if (operand2 == 0) {
			printf("Cannot divide with 0!\n");
			return DIVIDING_WITH_ZERO_ERROR;
		}
		result = operand1 / operand2;
		break;
	default:
		printf("Error!\n");
		return WRONG_OPERATION_ERROR;
	}

	return EXIT_SUCCESS;
}

int CalculatePostfixFromFile(int* result, char* filename) {
	FILE* file = NULL;
	int fileLength = 0;
	char* buffer = NULL;
	char* currentBuffer = NULL;
	int numBytes = 0;
	char operation = 0;
	int number = 0, status = 0;
	Element head = { .number = 0,.next = NULL };

	file = fopen(filename, "r");
	if (!file) {
		printf("Cannot open the file!\n");
		return CANT_OPEN_FILE_ERROR;
	}

	fseek(file, 0, SEEK_END);
	fileLength = ftell(file);

	buffer = (char*)calloc(fileLength + 1, sizeof(char));
	if (!buffer) {
		printf("Cannot allocate memory!\n");
		return CANT_ALLOCATE_MEMORY_ERROR;
	}

	rewind(file);
	fread(buffer, sizeof(char), fileLength, file);
	printf("|%s|\n", buffer);

	fclose(file);

	currentBuffer = buffer;

	while (strlen(currentBuffer) > 0)
	{
		status = sscanf(currentBuffer, " %d %n", &number, &numBytes);

		if (status == 1)
		{
			Push(&head, number);
			currentBuffer += numBytes;
		}
		else
		{
			sscanf(currentBuffer, " %c %n", &operation, &numBytes);
			status = PerformOperations(&head, operation);

			if (status != EXIT_SUCCESS)
			{
				free(buffer);

				while (head.next != NULL)
					DeleteAfter(&head);

				return -1;
			}
			currentBuffer += numBytes;
		}
	}
	free(buffer);

	if (head.next->next)
	{
		printf("Postfix not valid! Please check your file.\n");
		return EXIT_FAILURE;
	}

	Pop(result, &head);

	return EXIT_SUCCESS;
}

