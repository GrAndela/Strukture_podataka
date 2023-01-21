#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME (128)
#define MAX_LINE (1024)
#define HASH_TABLE_SIZE (11)
#define CHARACTERS_NUMBER_TO_COUNT_FOR_HASH (5)

#define CANNOT_ALLOCATE_MEMORY_ERROR (-1)
#define CANNOT_OPEN_THE_FILE (-2)
#define FILE_ERROR (-3)

struct _TreeOfCities;
typedef struct _TreeOfCities* CityPosition;
typedef struct _TreeOfCities {
	char name[MAX_NAME];
	int population;
	CityPosition left;
	CityPosition right;
} TreeOfCities;

struct _ListOfStates;
typedef struct _ListOfStates* StatePosition;
typedef struct _ListOfStates {
	char name[MAX_NAME];
	StatePosition next;
	CityPosition city;
} ListOfStates;

struct _HashT;
typedef struct _HastT* HashPosition;
typedef struct _HashT {
	StatePosition headList;
} HashT;

StatePosition AllocateMemoryForState();
int ReadStateFile(StatePosition);
int SortedInputOfStates(StatePosition, StatePosition);
int ReadCityFile(StatePosition, char*);
CityPosition inputNodeInTree(char*, int, CityPosition);
CityPosition inputNode(char*, int);
int PrintStates(StatePosition);
int PrintCities(CityPosition);
int Search(StatePosition, char*, int);
int PrintCitiesWithPopulationAboveNumber(CityPosition, int);

int HashFunction(char* country_name);


int main()
{
	ListOfStates headElement[HASH_TABLE_SIZE] = { 0 };
	StatePosition head = &headElement;
	int i = 0;
	char selected_country[MAX_NAME] = { 0 };
	int selected_population = 0;
	StatePosition country = NULL;

	for (i = 0; i < HASH_TABLE_SIZE; i++) {
		strcpy(headElement[i].name, "");
		headElement[i].city = NULL;
		headElement->next = NULL;
	}

	ReadStateFile(head);
	PrintStates(head);

	
	/*printf("Enter a country and minimum population:\n");
	scanf("%s %d", selected_country, &selected_population);
	country = Search(head, selected_country, selected_population);
	
	if (country && country->city)
		PrintCitiesWithPopulationAboveNumber(country->city, selected_population);
	*/

	return EXIT_SUCCESS;
}

StatePosition AllocateMemoryForState()
{
	StatePosition newState = NULL;

	newState = (StatePosition)malloc(sizeof(ListOfStates));

	if (!newState)
	{
		printf("Cannot allocate memory!\n");
		return CANNOT_ALLOCATE_MEMORY_ERROR;
	}

	strcpy(newState->name, " ");
	newState->next = NULL;
	newState->city = NULL;

	return newState;
}

int ReadStateFile(StatePosition head)
{
	StatePosition newState = NULL;
	char buffer[MAX_LINE] = { 0 };
	char name[MAX_NAME] = { 0 };
	char fileName[MAX_NAME] = { 0 };
	char state_filename[MAX_NAME] = { 0 };
	int status = 0;
	FILE* file = NULL;


	printf("Enter the name of the state file:\n");
	scanf("%s", state_filename);
	file = fopen(state_filename, "r");

	if (!file)
	{
		printf("Cannot open the file!\n");
		return CANNOT_OPEN_THE_FILE;
	}

	while (!feof(file))
	{
		newState = AllocateMemoryForState();
		fscanf(file, "%s %s", name, fileName);
		strcpy(newState->name, name);
		SortedInputOfStates(head, newState);
		ReadCityFile(newState, fileName);
	}

	fclose(file);

	return EXIT_SUCCESS;
}

int SortedInputOfStates(StatePosition head, StatePosition newState)
{
	StatePosition temp = head;

	while (temp->next != NULL && strcmp(temp->next->name, newState->name) < 0)
	{
		temp = temp->next;
	}


	if (strcmp(temp->name, newState->name) != 0)
	{
		newState->next = temp->next;
		temp->next = newState;
	}
	else
	{
		free(newState);
	}

	return EXIT_SUCCESS;
}

int ReadCityFile(StatePosition newState, char* fileName)
{
	CityPosition root = NULL;
	char buffer[MAX_LINE] = { 0 };
	int status = 0;
	char name[MAX_NAME] = { 0 };
	int number = 0;

	FILE* file = fopen(fileName, "r");

	if (!file)
	{
		printf("Cannot open the file!\n");
		return CANNOT_OPEN_THE_FILE;
	}

	while (!feof(file))
	{
		fscanf(file, "%s %d", name, &number);
		root = inputNodeInTree(name, number, root);
	}

	fclose(file);

	newState->city = root;

	return EXIT_SUCCESS;
}

CityPosition inputNodeInTree(char* name, int number, CityPosition root)
{
	CityPosition newNode = NULL;
	CityPosition current = root;

	if (current == NULL)
	{
		newNode = inputNode(name, number);
		return newNode;
	}
	else if (current->population > 0)
	{
		current->left = inputNodeInTree(name, number, current->left);
	}
	else if (current->population < 0)
	{
		current->right = inputNodeInTree(name, number, current->right);
	}
	else
	{
		if (strcmp(current->name, name) > 0)
		{
			current->left = inputNodeInTree(name, number, current->left);
		}
		else if (strcmp(current->name, name) < 0)
		{
			current->right = inputNodeInTree(name, number, current->right);
		}
	}

	return current;
}

CityPosition inputNode(char* name, int number)
{
	CityPosition newNode = NULL;

	newNode = (CityPosition)malloc(sizeof(TreeOfCities));

	if (!newNode)
	{
		printf("Cannot allocate memory!\n");
		return CANNOT_ALLOCATE_MEMORY_ERROR;
	}

	strcpy(newNode->name, name);
	newNode->population = number;
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}


int PrintStates(StatePosition head)
{
	if (head == NULL)
	{
		printf("The list is empty!\n");
		return NULL;
	}

	while (head->next != NULL)
	{
		printf("%s\n", head->name);
		PrintCities(head->city);
		head = head->next;
	}

	printf("%s\n", head->name);
	PrintCities(head->city);

	return EXIT_SUCCESS;
}

int PrintCities(CityPosition root)
{
	if (root != NULL)
	{
		PrintCities(root->left);
		printf("%s %d\n", root->name, root->population);
		PrintCities(root->right);
	}

	return EXIT_SUCCESS;
}

int Search(StatePosition head, char* name, int number)
{
	StatePosition temp = head;

	while (temp->next != NULL && strcmp(temp->next->name, name) < 0)
	{
		temp = temp->next;
	}

	if (strcmp(temp->next->name, name) == 0)
	{
		PrintCitiesWithPopulationAboveNumber(temp->next->city, number);
	}
	else
	{
		printf("State with that name does not exist in the file!\n");
	}

	return EXIT_SUCCESS;
}

int PrintCitiesWithPopulationAboveNumber(CityPosition root, int number)
{
	if (root != NULL)
	{
		PrintCitiesWithPopulationAboveNumber(root->left, number);
		if (root->population > number)
			printf(" %s\n", root->name);
		PrintCitiesWithPopulationAboveNumber(root->right, number);
	}

	return EXIT_SUCCESS;
}

int HashFunction(char* country_name) {
	int result = 0, i=0, n=0;

	if (strlen(country_name) > CHARACTERS_NUMBER_TO_COUNT_FOR_HASH)
		n = CHARACTERS_NUMBER_TO_COUNT_FOR_HASH;
	else
		n = strlen(country_name);

	for (i = 0; i < n; i++) {
		result += country_name[i];
	}

	return (result % HASH_TABLE_SIZE);
}



