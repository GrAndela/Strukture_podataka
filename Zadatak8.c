#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


struct treeNode;
typedef struct treeNode* TreeNodePtr;

typedef struct treeNode {
	int data;
	TreeNodePtr left;
	TreeNodePtr right;
}TreeNode;

TreeNodePtr Insert(TreeNodePtr root, int data);
TreeNodePtr CreateNewNode(int data);
int Preorder(TreeNodePtr root);
int Inorder(TreeNodePtr root);
int Postorder(TreeNodePtr root);
TreeNodePtr Delete(TreeNodePtr root, int data);
TreeNodePtr FindMin(TreeNodePtr root);
TreeNodePtr SearchElement(TreeNodePtr root, int data);

int main() {
	TreeNodePtr root = NULL;
	int choice = 1;
	int data = NULL;

	printf("----------MENI---------\n");
	printf("Tipka 1 - Unos elementa u stablo\n");
	printf("Tipka 2 - Ispis stabla - PREORDER\n");
	printf("Tipka 3 - Ispis stabla - INORDER\n");
	printf("Tipka 4 - Ispis stabla - POSTORDER\n");
	printf("Tipka 5 - Brisanje elementa iz stabla\n");
	printf("Tipka 6 - Trazenje elementa u stablu\n");
	printf("Tipka 0 - Prekid programa\n");

	while (choice != 0) {

		scanf(" %d", &choice);

		switch (choice)
		{
		case 0:
			printf("Program prekinut\n");
			break;

		case 1:
			printf("Koji element zelite unjeti u stablo?\n");
			scanf(" %d", &data);
			root = Insert(root, data);
			printf("Element uspjesno dodan u stablo\n");
			break;

		case 2:
			printf("PREORDER\n");
			Preorder(root);
			break;

		case 3:
			printf("INORDER\n");
			Inorder(root);
			break;

		case 4:
			printf("POSTORDER\n");
			Postorder(root);
			break;

		case 5:
			printf("Koji element zelite izbrisati?\n");
			scanf(" %d", &data);
			root = Delete(root, data);
			printf("Element je izbrisan iz stabla\n");
			break;

		case 6:
			printf("Koji element trazite?\n");
			scanf(" %d", &data);
			root = SearchElement(root, data);
			printf("Element uspjesno pronaden\n");
			break;
		}
	}



	return 0;
}

TreeNodePtr Insert(TreeNodePtr root, int data)
{
	if (root == NULL) { 		
		root = CreateNewNode(data);
	}
	else if (data < root->data) {
		root->left = Insert(root->left, data);
	}
	else if (data > root->data) {
		root->right = Insert(root->right, data);
	}

	return root;
}

TreeNodePtr CreateNewNode(int data)
{
	TreeNodePtr newNode = NULL;

	newNode = (TreeNodePtr)malloc(sizeof(TreeNode));

	if (!newNode) {
		perror("Can not allocate memory!\n");
		return -1;
	}

	newNode->data = data;
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

int Preorder(TreeNodePtr root)
{
	if (root == NULL)
		return NULL;

	printf("%d ", root->data);
	Preorder(root->left);
	Preorder(root->right);

	return EXIT_SUCCESS;
}

int Inorder(TreeNodePtr root)
{
	if (root == NULL)
		return NULL;

	Inorder(root->left);
	printf("%d ", root->data);
	Inorder(root->right);

	return EXIT_SUCCESS;
}

int Postorder(TreeNodePtr root)
{
	if (root == NULL)
		return NULL;

	Postorder(root->left);
	Postorder(root->right);
	printf("%d ", root->data);

	return EXIT_SUCCESS;
}

TreeNodePtr FindMin(TreeNodePtr root)
{
	while (root->left != NULL)
		root = root->left;

	return root;
}

TreeNodePtr Delete(TreeNodePtr root, int data)
{
	TreeNodePtr temp = NULL;

	if (root == NULL)
		return NULL;

	else if (data < root->data)
		root->left = Delete(root->left, data);

	else if (data > root->data)
		root->right = Delete(root->right, data);

	else
	{	//case 1: no child
		if (root->left == NULL && root->right == NULL) {
			free(root);
			root = NULL;
		}
		//case 2: one child
		else if (root->left == NULL) {
			temp = root;
			root = root->right; //zamjenjujemo ga sa djetetom
			free(temp);
		}
		else if (root->right == NULL) {
			temp = root;
			root = root->left; //zamjenjujemo ga sa djetetom
			free(temp);
		}
		//case 3: two children
		else {
			temp = FindMin(root->right); //ili FindMax(root->left)
			root->data = temp->data;
			root->right = Delete(root->right, temp->data);
		}

		return root;
	}

	//return root;	
}

TreeNodePtr SearchElement(TreeNodePtr root, int data)
{
	if (root == NULL) {
		printf("Element does not exist\n");
		return root;
	}

	else if (data < root->data)
		root = SearchElement(root->left, data);

	else if (data > root->data)
		root = SearchElement(root->right, data);

	return root;
}