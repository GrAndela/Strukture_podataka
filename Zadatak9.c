#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CANNOT_ALLOCATE_MEMORY_ERROR (-1)
#define CANNOT_OPEN_FILE_ERROR (-2)
#define MAX_NAME (128)
#define MIN (10)
#define MAX (90)


struct _Node;
typedef struct _Node* PositionN;
typedef struct _Node {
	int number;
	PositionN left;
	PositionN right;
} Node;

struct _List;
typedef struct _List* PositionL;
typedef struct _List {
	int number;
	PositionL next;
} List;

PositionN CreateTree(int);
int PrintInorder(PositionN);
int PrintInorderInList(PositionL, PositionN);
PositionN Insert(int, PositionN);
int Replace(PositionN);
int RandomTree(PositionN);
int FindRandomNum(int, int);
int WriteInFile(PositionN, char*);
PositionL CreateList();
int AddLastToList(int, PositionL);
int DeleteList(PositionL);


int main() {
	srand(time(NULL));
	int length = 0, i = 0, num = 0;
	int array[] = { 2,5,7,8,11,1,4,2,3,7 };
	PositionN root = NULL;
	char filename[MAX_NAME] = { 0 };

	length = sizeof(array) / sizeof(int);

	for (i = 0; i < length; i++) {
		root = Insert(array[i], root);
	}

	printf("Original tree:\n");
	PrintInorder(root);
	printf("\n");

	printf("Enter the name of the file:\n");
	scanf("%s", filename);
	WriteInFile(root, filename);

	printf("Tree after summing up:\n");
	Replace(root);
	PrintInorder(root);
	printf("\n");

	WriteInFile(root, filename);

	return EXIT_SUCCESS;
}

PositionN CreateTree(int number) {
	PositionN q = NULL;
	q = (PositionN)malloc(sizeof(Node));

	if (!q) {
		printf("Cannot allocate memory!\n");
		return CANNOT_ALLOCATE_MEMORY_ERROR;
	}

	q->number = number;
	q->left = NULL;
	q->right = NULL;

	return q;
}

int PrintInorder(PositionN p) {
	if (p == NULL) {
		return NULL;
	}
	else {
		PrintInorder(p->left);
		printf("%d\t", p->number);
		PrintInorder(p->right);
	}

	return EXIT_SUCCESS;
}

int PrintInorderInList(PositionL head, PositionN p) {
	if (p == NULL) {
		return EXIT_SUCCESS;
	}
	else {
		PrintInorderInList(head, p->left);
		AddLastToList(p->number, head);
		PrintInorderInList(head, p->right);
	}

	return EXIT_SUCCESS;
}

PositionN Insert(int number, PositionN p) {
	if (p == NULL) {
		p = CreateTree(number);
	}
	else {
		if (number == p->number) {
			p->left = Insert(number, p->left);
		}
		else if (number < p->number) {
			p->right = Insert(number, p->right);
		}
		else if (number > p->number) {
			p->left = Insert(number, p->left);
		}
	}

	return p;
}

int Replace(PositionN p) {
	int temp = 0;
	
	if (p == NULL) {
		return NULL;
	}

	temp = p->number;
	p->number = Replace(p->left) + Replace(p->right);

	return p->number + temp;
}

int RandomTree(PositionN p) {
	if (p == NULL) {
		return NULL;
	}
	
	p->number = FindRandomNum(MIN, MAX);
	if (p->left) {
		RandomTree(p->left);
	}
	else if (p->right) {
		RandomTree(p->right);
	}

	return EXIT_SUCCESS;
}

int FindRandomNum(int min, int max) {
	return ((rand() % (max - min + 1)) + min);
}

int WriteInFile(PositionN p, char* filename) {
	PositionL head = NULL;
	head = CreateList();
	FILE* file = NULL;

	PrintInorderInList(head, p);

	file = fopen(filename, "w");
	if (!file) {
		printf("Cannot open the file!\n");
		return CANNOT_OPEN_FILE_ERROR;
	}

	while (head->next != NULL) {
		head = head->next;
		fprintf(file, "%d\t", head->number);
	}
	fprintf(file, "\n");

	fclose(file);
	DeleteList(head);

	return EXIT_SUCCESS;
}

PositionL CreateList() {
	PositionL q = NULL;
	q = (PositionL)malloc(sizeof(List));

	if (!q) {
		printf("Cannot aalocate memory!\n");
		return CANNOT_ALLOCATE_MEMORY_ERROR;
	}

	q->next = NULL;
	q->next = 0;

	return q;
}

int AddLastToList(int number, PositionL head) {
	PositionL q = NULL;
	q = CreateList();

	while (head->next != NULL) {
		head = head->next;
	}

	q->number = number;
	q->next = head->next;
	head->next = q;

	return EXIT_SUCCESS;
}

int DeleteList(PositionL head) {
	PositionL temp = NULL;

	while (head->next) {
		temp = head->next;
		head->next = temp->next;
		free(temp);
	}

	return EXIT_SUCCESS;
}







