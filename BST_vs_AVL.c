/* <BST�� AVL �� �м� ���α׷� ���� ���> (���� 100��, 500��, 1000��, 1500��, 2000��....1����)


********************************************************************
*								                                   *
*							BST vs AVL							   *
*																   *
********************************************************************


>> ������ ���� ����? x
>> ���� x���� 100�� �����Ͽ� ��� ���̿� ��� �� Ƚ���� �м��ϰڽ��ϴ�.


.............................Loading.............................


					<���� x��, 100�� test ���>
			---------------------------------------------
			|					|	 BST	|	AVL		|
			--------------------------------------------
			|	��� ����    	|	  n		|	 m		|
			--------------------------------------------
			|	��� �� Ƚ��	|	  n		|	 m		|
			---------------------------------------------

*/

#include <stdio.h>
#include <stdlib.h> //malloc �Լ� 
#include <time.h>  //���� ���� �Լ� 
#include "math.h"


struct bstNode {
	int data;
	struct bstNode* left;
	struct bstNode* right;
};

struct avlNode
{
	int data;
	int height;
	struct avlNode* left;
	struct avlNode* right;
};

//Insertion and deletion in AVL Tree
struct avlNode* NewNode(int data)
{
	struct avlNode* temp = (struct avlNode*)malloc(sizeof(struct avlNode));
	temp->data = data;
	temp->left = NULL;
	temp->right = NULL;
	temp->height = 0;
	return temp;
}



//BST ���� �Լ��� 
struct bstNode* newnode(int n);
struct bstNode* bstInsert(struct bstNode* root, int data);
int bstGetMax(int x, int y);
int bstGetHeight(struct bstNode* root);
struct bstNode* bstSearch(struct bstNode* node, int data);

//AVL ���� �Լ���
int avlGetMax(int a, int b);
int avlHeight(struct avlNode* node);
int Balance(struct avlNode* node);
struct avlNode* LeftRotate(struct avlNode* z);
struct avlNode* RightRotate(struct avlNode* z);
struct avlNode* FindMin(struct avlNode* node);
struct avlNode* avlInsert(struct avlNode* root, int data);
struct avlNode* avlSearch(struct avlNode* node, int data);


//main �Լ� ���� ū �ٱ� �Լ��� 
void printTitle(); //���α׷��� �����ϸ鼭 ����� ����ϴ� �Լ� 
int setInput(); //������ ���� ������ ����ڿ��� �Է¹޴� �Լ� 
void showTableResult(int); //���� ��� ������ �м� ��� ǥ�� �����ִ� �Լ�  
int createRandomNumber(int, int*); //���� �����ϴ� �Լ�
void analyze100times(int n, int* arr, int(*pbst_arr)[100], int(*pavl_arr)[100]); //BST�˰������ AVL �˰������� 100�� ���� �м��ϴ� �Լ� 
void calculateAverage(int(*pbst_arr)[100], int(*pavl_arr)[100]);// �迭�� �ƿ� �Ѱ��ֱ� 


//�������� 
int comparingCount = 0; //��Ƚ�� ����
//���� �м��� ������: ����, ��Ƚ��  
double BST_avg_height; double BST_avg_comparing_time;
double AVL_avg_height; double AVL_avg_comparing_time;

int main() {

	srand((unsigned int)time(NULL)); //<����> ���� ���� �Լ����� �ݵ�� �� ���� �������ֵ��� �Ѵ�!

	//��հ��� ���ϱ� ���� 100���� �����͸� ���� �迭 -> 1���� ����� ���̸� ����, 2���� ����� ��Ƚ���� ����
	int bst_arr_for_average[2][100]; int avl_arr_for_average[2][100]; //��հ��� ���ϱ� ���� �Ź� ����� ����, ��Ƚ���� ������ 2���� �迭  


	int* pbst_arr = bst_arr_for_average; int* pavl_arr = avl_arr_for_average; //�Լ��� ���ڷ� �������ֱ� ����, �迭�� �ּ� ����

	//(1) ���� ��� 
	printTitle();


	//(2) ������ ���� ���� �Է� 
	int n = setInput();
	int* arr = (int*)malloc(sizeof(int) * n); //����ڰ� �Է��� n���� ���� �����Ҵ� 


	//(3) 100�� BST �˰������ AVL �˰������� ������ ��� ���̿� ��� �� Ƚ�� �м� 
	analyze100times(n, arr, pbst_arr, pavl_arr);
	

	//(4) ��� ��� -> error 
	calculateAverage(pbst_arr, pavl_arr);


	//(5) ǥ�� 100�� ���� ��հ� �����ֱ� 
	showTableResult(n);


	free(arr);

}

void printTitle()
{
	printf("********************************************************************\n");
	printf("*                                                                  *\n");
	printf("*                          BST vs AVL                              *\n");
	printf("*                                                                  *\n");
	printf("********************************************************************\n");
	printf("\n");

}

int setInput()
{
	int n;
	printf(">> ������ ���� ����? ");
	scanf_s("%d", &n);

	printf(">> ���� %d���� 100�� �����Ͽ� ��� ���̿� ��� �� Ƚ���� �м��ϰڽ��ϴ�.\n", n);
	printf("\n");
	printf("\n................................Loading................................\n\n\n");

	return n;

}


void showTableResult(int n)
{


	printf("\n");
	printf("                   <���� %d��, 100�� test ���>\n", n);
	printf("            ------------------------------------------------------\n");
	printf("            |                   |      BST     |     AVL         |\n");
	printf("            -----------------------------------------------------\n");
	printf("            |   ��� ����       |     %.2f       |     %.2f      |\n", BST_avg_height, AVL_avg_height);
	printf("            ------------------------------------------------------\n");
	printf("            |   ��� �� Ƚ��  |     %.2f       |     %.2f      |\n", BST_avg_comparing_time, AVL_avg_comparing_time);
	printf("            -----------------------------------------------------\n");


}

int createRandomNumber(int n, int* arr)  //�ߺ����� �ʰ� x�� ���� 
{
	//int a[15]; //�����Ҵ����� n���� �޸𸮸� �� ������ ���� 
	int i, j;
	int bFound;

	for (i = 0; i < n; ++i) {

		while (1) {

			// �������ϳ�����
			arr[i] = rand() % n + 1;

			// �̹��ִ°�����Ȯ���ϱ������÷��׼���
			bFound = 0;

			// ���������ִ���Ȯ���Ѵ�
			for (j = 0; j < i; ++j) {

				// ��������������
				if (arr[j] == arr[i]) {
					bFound = 1;
					break;
				}

			}

			// �������̾�����while��Ż��
			if (!bFound)
				break;

		}

	}


	return 0;
}

void calculateAverage(int (*pbst_arr)[100], int (*pavl_arr)[100])  //���� ũ�Ⱑ 100�� �迭�� ����Ű�� ������
{
	//���� �м��� ������: ����, ��Ƚ��  
	//��� ��� 
	for (int i = 0; i < 100; i++) {
		BST_avg_height += pbst_arr[0][i];
		BST_avg_comparing_time += pbst_arr[1][i];
		AVL_avg_height += pavl_arr[0][i];
		AVL_avg_comparing_time += pavl_arr[1][i];


	}

	BST_avg_height /= 100.0; BST_avg_comparing_time /= 100.0;
	AVL_avg_height /= 100.0; AVL_avg_comparing_time /= 100.0;

	//return;
}

void analyze100times(int n, int* arr, int (*pbst_arr)[100], int (*pavl_arr)[100])
{


	for (int i = 0; i < 100; i++) { //100���� n���� �ߺ����� �ʴ� ���� ����

		//���� 100�� �ߺ����� �ʰ� ���� -> �Է°��� x����� 1���� x ������ �� �ߺ����� �ʰ� ����  
		createRandomNumber(n, arr);

		// �˻��� �����͸� �Ź� ������ ����
		int data_to_search = rand() % n + 1;


		/*BST �м�*/

		struct bstNode* bst_root;
		bst_root = NULL;

		//������ ������ bst�� �����ϱ�
		for (int i = 0; i < n; i++) {
			bst_root = bstInsert(bst_root, arr[i]);
		}

		//Ʈ�� ���� ����ϱ� 
		pbst_arr[0][i] = bstGetHeight(bst_root); // 1���� ����� Ʈ�� ���̸� ���� 
		

		//��Ƚ�� ����ϱ� 
		struct bstNode* bst_node;
		bst_node = bstSearch(bst_root, data_to_search); //������ ������ ã�� -> comparingCount �� ��ȭ 
		pbst_arr[1][i] = comparingCount;
		comparingCount = 0; //0���� �ٽ� �ʱ�ȭ -> ���� test�� ����


		/* AVL �м� */

		struct avlNode* avl_root;
		avl_root = NULL;

		//������ ������ bst�� �����ϱ�
		for (int i = 0; i < n; i++) {
			avl_root = avlInsert(avl_root, arr[i]);
		}

		//Ʈ�� ���� ����ϱ� 
		pavl_arr[0][i] = avlHeight(avl_root);

		//��Ƚ�� ����ϱ�
		struct avlNode* avl_node;
		avl_node = avlSearch(avl_root, data_to_search); //������ ������ ã�� -> comparingCount �� ��ȭ 
		pavl_arr[1][i] = comparingCount;
		comparingCount = 0; //0���� �ٽ� �ʱ�ȭ -> ���� test�� ����



	}



}

/*
bst ���� �Լ���
*/


struct bstNode* newnode(int n)
{
	struct bstNode* temp;
	temp = (struct bstNode*)malloc(sizeof(struct bstNode));
	temp->data = n;
	temp->left = temp->right = NULL;

	return temp;
}


struct bstNode* bstInsert(struct bstNode* root, int data)
{
	if (root == NULL)
		root = newnode(data);

	else if (data <= root->data)
		root->left = bstInsert(root->left, data);

	else
		root->right = bstInsert(root->right, data);

	return root;

}



int bstGetMax(int x, int y)
{
	return(x > y ? x : y);
}


int bstGetHeight(struct bstNode* root)
{
	if (root == NULL)
		return -1;

	return((bstGetMax(bstGetHeight(root->left), bstGetHeight(root->right))) + 1);
}



// ��ȯ ����
struct bstNode* bstSearch(struct bstNode* node, int data) //bst�� Ž�� �˰�����  
{
	/* ã���� �ϴ� ���� ã�ƺôµ� �ƿ� ������ ���
	Ȥ�� ���ʿ� node ��ü�� NULL�̾��� ��� */
	if (!node) //NULL�� ���
		return NULL;

	comparingCount++; //��Ƚ�� ī����

	// ��ȯ���� Ž�� �� ���ϴ� ���� ã���� ��
	if (data == node->data)
		return node; // �ش� ����� �ּҸ� ��ȯ

	  // ã�� ������ ��ƮŰ���� ���� ���
	else if (data > node->data)
		return bstSearch(node->right, data); // ������ ����Ʈ���� �̵�

	  // ã�� ������ ��ƮŰ���� ū ���
	else
		return bstSearch(node->left, data); // ���� ����Ʈ���� �̵�
}



/*
avl ���� �Լ���
*/

int avlGetMax(int a, int b)
{
	return (a > b) ? a : b;
}

int avlHeight(struct avlNode* node)
{
	if (node == NULL)
		return 0;

	return node->height;
}

int Balance(struct avlNode* node)
{
	if (node == NULL)
		return 0;

	return avlHeight(node->left) - avlHeight(node->right);
}

struct avlNode* LeftRotate(struct avlNode* z)
{
	struct avlNode* y = z->right;
	struct avlNode* t2 = y->left;

	y->left = z;
	z->right = t2;

	z->height = avlGetMax(avlHeight(z->left), avlHeight(z->right)) + 1;
	y->height = avlGetMax(avlHeight(y->left), avlHeight(y->right)) + 1;

	return y;
}


struct avlNode* RightRotate(struct avlNode* z)
{
	struct avlNode* y = z->left;
	struct avlNode* t3 = y->right;

	y->right = z;
	z->left = t3;

	z->height = avlGetMax(avlHeight(z->left), avlHeight(z->right)) + 1;
	y->height = avlGetMax(avlHeight(y->left), avlHeight(y->right)) + 1;

	return y;
}

struct avlNode* FindMin(struct avlNode* node)
{
	while (node->left != NULL)
		node = node->left;

	return node;
}


struct avlNode* avlInsert(struct avlNode* root, int data)
{
	if (root == NULL)
		return NewNode(data);

	if (data < root->data)
		root->left = avlInsert(root->left, data);

	else if (data > root->data)
		root->right = avlInsert(root->right, data);

	else
		return root;

	root->height = avlGetMax(avlHeight(root->left), avlHeight(root->right)) + 1;

	int balance = Balance(root);

	// Left Left
	if (balance > 1 && data < root->left->data)
		return RightRotate(root);

	// Right Right 
	if (balance < -1 && data > root->right->data)
		return LeftRotate(root);

	//Left Right
	if (balance > 1 && data > root->left->data)
	{
		root->left = LeftRotate(root->left);
		return RightRotate(root);
	}

	// Right Left
	if (balance < -1 && data < root->right->data)
	{
		root->right = RightRotate(root->right);
		return LeftRotate(root);
	}

	return root;
}



// ��ȯ ����
struct avlNode* avlSearch(struct avlNode* node, int data) //Ž�� 
{
	/* ã���� �ϴ� ���� ã�ƺôµ� �ƿ� ������ ���
	Ȥ�� ���ʿ� node ��ü�� NULL�̾��� ��� */
	if (!node)
		return NULL;

	comparingCount++; //��Ƚ�� ī����

	// ��ȯ���� Ž�� �� ���ϴ� ���� ã���� ��
	if (data == node->data)
		return node; // �ش� ����� �ּҸ� ��ȯ

	  // ã�� ������ ��ƮŰ���� ���� ���
	else if (data > node->data)
		avlSearch(node->right, data); // ������ ����Ʈ���� �̵�

	  // ã�� ������ ��ƮŰ���� ū ���
	else
		avlSearch(node->left, data); // ���� ����Ʈ���� �̵�
}