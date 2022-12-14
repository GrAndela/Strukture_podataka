#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CANNOT_ALLOCATE_MEMORY_ERROR (-1)
#define ROOT_DIRECTORY (1)
#define MAX_NAME (128)

struct _Directory;
typedef struct _Directory* Position;
typedef struct _Directory {
	char name[MAX_NAME];
	Position sibling;
	Position child;
} Directory;

struct _stackElement;
typedef struct _stackElement* stackPosition;
typedef struct _stackElement {
	Position parent;
	stackPosition next;
} stackElement;

Position AllocateMemoryForDirectory();
int CreateNewDirectory(Position c,stackPosition current);
stackPosition AllocateMemoryForStackElement();
int SortedInput(Position c, stackPosition current, Position newDir);
stackPosition ChangeDirectory(Position c, stackPosition current, stackPosition stackHead);
int Push(stackPosition stackHead, stackPosition stackElement);
stackPosition ExitDirectory(Position c, stackPosition current, stackPosition stackHead);
stackPosition Pop(Position c, stackPosition stackHead);
int PrintAllSubdirectories(stackPosition current);
int Commands();

int main() {
	Position C = AllocateMemoryForDirectory();
	stackPosition currentDir = AllocateMemoryForStackElement();
	stackPosition stackHead = AllocateMemoryForStackElement();
	int option = 0;

	strcpy(C->name, "C:");

	currentDir->parent = C;

	Push(stackHead, currentDir);

	while (option != 5) {
		Commands();
		printf("Choose one option:\n");
		scanf("%d", &option);

		switch (option) {
		case 1:
			CreateNewDirectory(C, currentDir);
			break;
		case 2:
			currentDir = ChangeDirectory(C, currentDir, stackHead);
			break;
		case 3:
			currentDir = ExitDirectory(C, currentDir, stackHead);
			break;
		case 4:
			PrintAllSubdirectories(currentDir);
			break;
		case 5:
			printf("Exit!\n");
			break;
		default:
			printf("You chose wrong option,try again!\n");
		}
	}

	return EXIT_SUCCESS;
}

Position AllocateMemoryForDirectory() {
	Position newDir = NULL;
	newDir = (Position)malloc(sizeof(Directory));

	if (!newDir) {
		printf("Cannot allocate memory!\n");
		return CANNOT_ALLOCATE_MEMORY_ERROR;
	}

	strcpy(newDir->name, " ");
	newDir->sibling = NULL;
	newDir->child = NULL;

	return newDir;
}

int CreateNewDirectory(Position c,stackPosition current) {
	Position newDir = AllocateMemoryForDirectory();
	char name[MAX_NAME] = { 0 };

	printf("Enter the name of the new directory:\n");
	scanf("%s", name);

	strcpy(newDir->name, name);
	SortedInput(c, current, newDir);

	return EXIT_SUCCESS;
}

stackPosition AllocateMemoryForStackElement() {
	stackPosition newStackElem = NULL;
	newStackElem = (stackPosition)malloc(sizeof(stackElement));

	if (!newStackElem) {
		printf("Cannot allocate memory!\n");
		return CANNOT_ALLOCATE_MEMORY_ERROR;
	}

	newStackElem->next = NULL;
	newStackElem->parent = NULL;

	return newStackElem;
}

int SortedInput(Position c, stackPosition current, Position newDir) {
	Position temp = current->parent;

	if (temp->child == NULL) {
		newDir->child = temp->child;
		temp->child = newDir;
	}
	else {
		temp = temp->child;
		while (temp->sibling != NULL && strcmp(newDir->name, temp->sibling->name) > 0)
			temp = temp->sibling;

		if (temp->sibling == NULL || strcmp(temp->sibling->name, newDir->name) != 0) {
			newDir->sibling = temp->sibling;
			temp->sibling = newDir;
		}
	}

	return EXIT_SUCCESS;
}

stackPosition ChangeDirectory(Position c, stackPosition current, stackPosition stackHead) {
	char name[MAX_NAME] = { 0 };
	Position temp = current->parent->child;

	printf("Enter the name of the directory:\n");
	scanf("%s", name);

	while (temp->sibling != NULL && strcmp(temp->name, name) != 0)
		temp = temp->sibling;

	if (strcmp(temp->name, name) == 0) {
		current->parent = temp;
		printf("Current directory is %s\n", current->parent->name);
		Push(stackHead, current);
	}
	else {
		printf("Directory with that name does not exist!\n");
	}

	return current;
}

int Push(stackPosition stackHead, stackPosition stackElement) {
	stackPosition newStackElem = AllocateMemoryForStackElement();

	newStackElem->parent = stackElement->parent;

	newStackElem->next = stackHead->next;
	stackHead->next = newStackElem;

	return EXIT_SUCCESS;
}

stackPosition ExitDirectory(Position c, stackPosition current, stackPosition stackHead) {
	current = Pop(c, stackHead);

	printf("Current directory is %s\n", current->parent->name);

	return current;
}

stackPosition Pop(Position c, stackPosition stackHead) {
	stackPosition delElement = NULL;
	stackPosition newCurrent = NULL;

	if (stackHead->next->parent == c) {
		printf("Cannot exit the root directory!\n");
		return ROOT_DIRECTORY;
	}
	else {
		delElement = stackHead->next;
		newCurrent = delElement->next;
		stackHead->next = delElement->next;
		free(delElement);
	}

	return newCurrent;
}

int PrintAllSubdirectories(stackPosition current) {
	Position temp = current->parent->child;

	while (temp->sibling != NULL) {
		printf("-%s\n", temp->name);
		temp = temp->sibling;
	}

	printf("-%s\n", temp->name);

	return EXIT_SUCCESS;
}

int Commands() {
	printf("1 - md(make new directory)\n");
	printf("2 - cd dir(change directory)\n");
	printf("3 - cd ..(exit directory)\n");
	printf("4 - dir(list all subdirectories)\n");
	printf("5 - exit\n");
	
	return EXIT_SUCCESS;
}

