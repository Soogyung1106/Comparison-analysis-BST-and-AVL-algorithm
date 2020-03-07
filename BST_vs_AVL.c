/* <BST와 AVL 비교 분석 프로그램 예상 결과> (난수 100개, 500개, 1000개, 1500개, 2000개....1만개)


********************************************************************
*								                                   *
*							BST vs AVL							   *
*																   *
********************************************************************


>> 생성할 난수 개수? x
>> 난수 x개를 100번 생성하여 평균 높이와 평균 비교 횟수를 분석하겠습니다.


.............................Loading.............................


					<난수 x개, 100번 test 결과>
			---------------------------------------------
			|					|	 BST	|	AVL		|
			--------------------------------------------
			|	평균 높이    	|	  n		|	 m		|
			--------------------------------------------
			|	평균 비교 횟수	|	  n		|	 m		|
			---------------------------------------------

*/

#include <stdio.h>
#include <stdlib.h> //malloc 함수 
#include <time.h>  //난수 생성 함수 
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



//BST 관련 함수들 
struct bstNode* newnode(int n);
struct bstNode* bstInsert(struct bstNode* root, int data);
int bstGetMax(int x, int y);
int bstGetHeight(struct bstNode* root);
struct bstNode* bstSearch(struct bstNode* node, int data);

//AVL 관련 함수들
int avlGetMax(int a, int b);
int avlHeight(struct avlNode* node);
int Balance(struct avlNode* node);
struct avlNode* LeftRotate(struct avlNode* z);
struct avlNode* RightRotate(struct avlNode* z);
struct avlNode* FindMin(struct avlNode* node);
struct avlNode* avlInsert(struct avlNode* root, int data);
struct avlNode* avlSearch(struct avlNode* node, int data);


//main 함수 안의 큰 줄기 함수들 
void printTitle(); //프로그램이 시작하면서 헤더를 출력하는 함수 
int setInput(); //생성할 난수 개수를 사용자에게 입력받는 함수 
void showTableResult(int); //최종 평균 측정값 분석 결과 표로 보여주는 함수  
int createRandomNumber(int, int*); //난수 생성하는 함수
void analyze100times(int n, int* arr, int(*pbst_arr)[100], int(*pavl_arr)[100]); //BST알고리즘과 AVL 알고리즘을 100번 돌려 분석하는 함수 
void calculateAverage(int(*pbst_arr)[100], int(*pavl_arr)[100]);// 배열을 아예 넘겨주기 


//전역변수 
int comparingCount = 0; //비교횟수 측정
//최종 분석한 데이터: 높이, 비교횟수  
double BST_avg_height; double BST_avg_comparing_time;
double AVL_avg_height; double AVL_avg_comparing_time;

int main() {

	srand((unsigned int)time(NULL)); //<주의> 난수 생성 함수에서 반드시 한 번만 선언해주도록 한다!

	//평균값을 구하기 위해 100번의 데이터를 담을 배열 -> 1행은 계산한 높이를 저장, 2행은 계산한 비교횟수를 저장
	int bst_arr_for_average[2][100]; int avl_arr_for_average[2][100]; //평균값을 구하기 위해 매번 계산한 높이, 비교횟수를 저장할 2차원 배열  


	int* pbst_arr = bst_arr_for_average; int* pavl_arr = avl_arr_for_average; //함수의 인자로 전달해주기 위해, 배열의 주소 저장

	//(1) 제목 출력 
	printTitle();


	//(2) 생성할 난수 개수 입력 
	int n = setInput();
	int* arr = (int*)malloc(sizeof(int) * n); //사용자가 입력한 n개에 대해 동적할당 


	//(3) 100번 BST 알고리즘과 AVL 알고리즘을 돌려서 평균 높이와 평균 비교 횟수 분석 
	analyze100times(n, arr, pbst_arr, pavl_arr);
	

	//(4) 평균 계산 -> error 
	calculateAverage(pbst_arr, pavl_arr);


	//(5) 표로 100번 구한 평균값 보여주기 
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
	printf(">> 생성할 난수 개수? ");
	scanf_s("%d", &n);

	printf(">> 난수 %d개를 100번 생성하여 평균 높이와 평균 비교 횟수를 분석하겠습니다.\n", n);
	printf("\n");
	printf("\n................................Loading................................\n\n\n");

	return n;

}


void showTableResult(int n)
{


	printf("\n");
	printf("                   <난수 %d개, 100번 test 결과>\n", n);
	printf("            ------------------------------------------------------\n");
	printf("            |                   |      BST     |     AVL         |\n");
	printf("            -----------------------------------------------------\n");
	printf("            |   평균 높이       |     %.2f       |     %.2f      |\n", BST_avg_height, AVL_avg_height);
	printf("            ------------------------------------------------------\n");
	printf("            |   평균 비교 횟수  |     %.2f       |     %.2f      |\n", BST_avg_comparing_time, AVL_avg_comparing_time);
	printf("            -----------------------------------------------------\n");


}

int createRandomNumber(int n, int* arr)  //중복되지 않고 x개 생성 
{
	//int a[15]; //동적할당으로 n개의 메모리를 힙 영역에 생성 
	int i, j;
	int bFound;

	for (i = 0; i < n; ++i) {

		while (1) {

			// 난수를하나생성
			arr[i] = rand() % n + 1;

			// 이미있는값인지확인하기위해플래그설정
			bFound = 0;

			// 같은값이있는지확인한다
			for (j = 0; j < i; ++j) {

				// 같은값이있으면
				if (arr[j] == arr[i]) {
					bFound = 1;
					break;
				}

			}

			// 같은값이없으면while문탈출
			if (!bFound)
				break;

		}

	}


	return 0;
}

void calculateAverage(int (*pbst_arr)[100], int (*pavl_arr)[100])  //가로 크기가 100인 배열을 가리키는 포인터
{
	//최종 분석한 데이터: 높이, 비교횟수  
	//평균 계산 
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


	for (int i = 0; i < 100; i++) { //100번을 n개의 중복되지 않는 난수 생성

		//난수 100개 중복되지 않게 생성 -> 입력값이 x개라면 1부터 x 사이의 수 중복되지 않고 생성  
		createRandomNumber(n, arr);

		// 검색할 데이터를 매번 난수로 생성
		int data_to_search = rand() % n + 1;


		/*BST 분석*/

		struct bstNode* bst_root;
		bst_root = NULL;

		//생성된 난수를 bst에 삽입하기
		for (int i = 0; i < n; i++) {
			bst_root = bstInsert(bst_root, arr[i]);
		}

		//트리 높이 계산하기 
		pbst_arr[0][i] = bstGetHeight(bst_root); // 1행은 계산한 트리 높이를 저장 
		

		//비교횟수 계산하기 
		struct bstNode* bst_node;
		bst_node = bstSearch(bst_root, data_to_search); //임의의 데이터 찾기 -> comparingCount 값 변화 
		pbst_arr[1][i] = comparingCount;
		comparingCount = 0; //0으로 다시 초기화 -> 다음 test를 위해


		/* AVL 분석 */

		struct avlNode* avl_root;
		avl_root = NULL;

		//생성된 난수를 bst에 삽입하기
		for (int i = 0; i < n; i++) {
			avl_root = avlInsert(avl_root, arr[i]);
		}

		//트리 높이 계산하기 
		pavl_arr[0][i] = avlHeight(avl_root);

		//비교횟수 계산하기
		struct avlNode* avl_node;
		avl_node = avlSearch(avl_root, data_to_search); //임의의 데이터 찾기 -> comparingCount 값 변화 
		pavl_arr[1][i] = comparingCount;
		comparingCount = 0; //0으로 다시 초기화 -> 다음 test를 위해



	}



}

/*
bst 관련 함수들
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



// 순환 버전
struct bstNode* bstSearch(struct bstNode* node, int data) //bst용 탐색 알고리즘  
{
	/* 찾고자 하는 값을 찾아봤는데 아예 없었을 경우
	혹은 애초에 node 자체가 NULL이었을 경우 */
	if (!node) //NULL일 경우
		return NULL;

	comparingCount++; //비교횟수 카운팅

	// 순환으로 탐색 중 원하는 값을 찾았을 때
	if (data == node->data)
		return node; // 해당 노드의 주소를 반환

	  // 찾는 값보다 루트키값이 작은 경우
	else if (data > node->data)
		return bstSearch(node->right, data); // 오른쪽 서브트리로 이동

	  // 찾는 값보다 루트키값이 큰 경우
	else
		return bstSearch(node->left, data); // 왼쪽 서브트리로 이동
}



/*
avl 관련 함수들
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



// 순환 버전
struct avlNode* avlSearch(struct avlNode* node, int data) //탐색 
{
	/* 찾고자 하는 값을 찾아봤는데 아예 없었을 경우
	혹은 애초에 node 자체가 NULL이었을 경우 */
	if (!node)
		return NULL;

	comparingCount++; //비교횟수 카운팅

	// 순환으로 탐색 중 원하는 값을 찾았을 때
	if (data == node->data)
		return node; // 해당 노드의 주소를 반환

	  // 찾는 값보다 루트키값이 작은 경우
	else if (data > node->data)
		avlSearch(node->right, data); // 오른쪽 서브트리로 이동

	  // 찾는 값보다 루트키값이 큰 경우
	else
		avlSearch(node->left, data); // 왼쪽 서브트리로 이동
}