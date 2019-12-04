#include<stdio.h> 
#include<stdlib.h> 
#include <time.h>

#define MAX 1000000

int all_keys[MAX] = {};
int position_counte, med, rotations, height_avltree, height_rbtree;
clock_t tStart, tStop, tCurrent;
FILE *pont_arq; // create pointer variable for file

//============ AVL TREE ============

// An AVL tree node 
struct Node 
{ 
	int key; 
	struct Node *left; 
	struct Node *right; 
	int height; 
}; 

// A utility function to get the height of the tree 
int height(struct Node *N) 
{ 
	if (N == NULL) 
		return 0; 
	return N->height; 
} 

// A utility function to get maximum of two integers 
int max(int a, int b) 
{ 
	return (a > b)? a : b; 
} 

/* Helper function that allocates a new node with the given key and 
	NULL left and right pointers. */
struct Node* newNode(int key) 
{ 
	struct Node* node = (struct Node*) 
						malloc(sizeof(struct Node)); 
	node->key = key; 
	node->left = NULL; 
	node->right = NULL; 
	node->height = 1; // new node is initially added at leaf 
	return(node); 
} 

// A utility function to right rotate subtree rooted with y 
// See the diagram given above. 
struct Node *rightRotate(struct Node *y) 
{ 
	rotations = rotations + 1;
	struct Node *x = y->left; 
	struct Node *T2 = x->right; 

	// Perform rotation 
	x->right = y; 
	y->left = T2; 

	// Update heights 
	y->height = max(height(y->left), height(y->right))+1; 
	x->height = max(height(x->left), height(x->right))+1; 

	// Return new root 
	return x; 
} 

// A utility function to left rotate subtree rooted with x 
// See the diagram given above. 
struct Node *leftRotate(struct Node *x) 
{ 
	rotations = rotations + 1;
	struct Node *y = x->right; 
	struct Node *T2 = y->left; 

	// Perform rotation 
	y->left = x; 
	x->right = T2; 

	// Update heights 
	x->height = max(height(x->left), height(x->right))+1; 
	y->height = max(height(y->left), height(y->right))+1; 

	// Return new root 
	return y; 
} 

// Get Balance factor of node N 
int getBalance(struct Node *N) 
{ 
	if (N == NULL) 
		return 0; 
	return height(N->left) - height(N->right); 
} 

// Recursive function to insert a key in the subtree rooted 
// with node and returns the new root of the subtree. 
struct Node* insertAVL(struct Node* node, int key) 
{ 
	printf("teste insert \n");
	/* 1. Perform the normal BST insertion */
	if (node == NULL) 
		return(newNode(key)); 

	if (key < node->key) 
		node->left = insertAVL(node->left, key); 
	else if (key > node->key) 
		node->right = insertAVL(node->right, key); 
	else // Equal keys are not allowed in BST 
		return node; 

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height(node->left), 
						height(node->right)); 

	/* 3. Get the balance factor of this ancestor 
		node to check whether this node became 
		unbalanced */
	int balance = getBalance(node); 

	// If this node becomes unbalanced, then 
	// there are 4 cases 

	// Left Left Case 
	if (balance > 1 && key < node->left->key) 
		return rightRotate(node); 

	// Right Right Case 
	if (balance < -1 && key > node->right->key) 
		return leftRotate(node); 

	// Left Right Case 
	if (balance > 1 && key > node->left->key) 
	{ 
		node->left = leftRotate(node->left); 
		return rightRotate(node); 
	} 

	// Right Left Case 
	if (balance < -1 && key < node->right->key) 
	{ 
		node->right = rightRotate(node->right); 
		return leftRotate(node); 
	} 

	/* return the (unchanged) node pointer */
	return node; 
} 

// A utility function to print preorder traversal of the tree. 
void preOrderAVL(struct Node *root) 
{ 

	if(root != NULL) 
	{ 
		printf("%d ", root->key); 
		preOrderAVL(root->left); 
		preOrderAVL(root->right); 
	} 
} 

void inOrderAVL(struct Node *root)
{
	if(root != NULL){
		inOrderAVL (root->left);
		//printf("%d ", root->key);
		//printf("\n ");
		all_keys[position_counte] = root->key;
		position_counte = position_counte+1;
		inOrderAVL(root->right);
	}
}

int AVLHeight(struct Node *root){
	return max(height(root->left), height(root->right));
}

//============ RED BLACK TREE ============

enum nodeColor
{
  RED,
  BLACK
};
struct rbNode
{
  int data, color;
  struct rbNode *link[2];
};

struct rbNode *rootRB = NULL;

struct rbNode *createNode(int data)
{
  struct rbNode *newnode;
  newnode = (struct rbNode *)malloc(sizeof(struct rbNode));
  newnode->data = data;
  newnode->color = RED;
  newnode->link[0] = newnode->link[1] = NULL;
  return newnode;
}
void insertRB(int data)
{
  struct rbNode *stack[98], *ptr, *newnode, *xPtr, *yPtr;
  int dir[98], ht = 0, index;
  ptr = rootRB;
  if (!rootRB)
  {
    rootRB = createNode(data);
    return;
  }
  stack[ht] = rootRB;
  dir[ht++] = 0;
  while (ptr != NULL)
  {
    if (ptr->data == data)
    {
      printf("Duplicates Not Allowed!!\n");
      return;
    }
    index = (data - ptr->data) > 0 ? 1 : 0;
    stack[ht] = ptr;
    ptr = ptr->link[index];
    dir[ht++] = index;
  }
  stack[ht - 1]->link[index] = newnode = createNode(data);
  while ((ht >= 3) && (stack[ht - 1]->color == RED))
  {
    if (dir[ht - 2] == 0)
    {
      yPtr = stack[ht - 2]->link[1];
      if (yPtr != NULL && yPtr->color == RED)
      {
        stack[ht - 2]->color = RED;
        stack[ht - 1]->color = yPtr->color = BLACK;
        ht = ht - 2;
      }
      else
      {
        if (dir[ht - 1] == 0)
        {
          yPtr = stack[ht - 1];
        }
        else
        {
          xPtr = stack[ht - 1];
          yPtr = xPtr->link[1];
          xPtr->link[1] = yPtr->link[0];
          yPtr->link[0] = xPtr;
          stack[ht - 2]->link[0] = yPtr;
          rotations = rotations +1;
        }
        xPtr = stack[ht - 2];
        xPtr->color = RED;
        yPtr->color = BLACK;
        xPtr->link[0] = yPtr->link[1];
        yPtr->link[1] = xPtr;
        rotations = rotations +1;
        if (xPtr == rootRB)
        {
          rootRB = yPtr;
        }
        else
        {
          stack[ht - 3]->link[dir[ht - 3]] = yPtr;
        }
        break;
      }
    }
    else
    {
      yPtr = stack[ht - 2]->link[0];
      if ((yPtr != NULL) && (yPtr->color == RED))
      {
        stack[ht - 2]->color = RED;
        stack[ht - 1]->color = yPtr->color = BLACK;
        ht = ht - 2;
      }
      else
      {
        if (dir[ht - 1] == 1)
        {
          yPtr = stack[ht - 1];
        }
        else
        {
          xPtr = stack[ht - 1];
          yPtr = xPtr->link[0];
          xPtr->link[0] = yPtr->link[1];
          yPtr->link[1] = xPtr;
          stack[ht - 2]->link[1] = yPtr;
          rotations = rotations +1;
        }
        xPtr = stack[ht - 2];
        yPtr->color = BLACK;
        xPtr->color = RED;
        xPtr->link[1] = yPtr->link[0];
        yPtr->link[0] = xPtr;
        rotations = rotations +1;
        if (xPtr == rootRB)
        {
          rootRB = yPtr;
        }
        else
        {
          stack[ht - 3]->link[dir[ht - 3]] = yPtr;
        }
        break;
      }
    }
  }
  rootRB->color = BLACK;
}
void deleteRB(int data)
{
  struct rbNode *stack[98], *ptr, *xPtr, *yPtr;
  struct rbNode *pPtr, *qPtr, *rPtr;
  int dir[98], ht = 0, diff, i;
  enum nodeColor color;
  if (!rootRB)
  {
    printf("Tree not available\n");
    return;
  }
  ptr = rootRB;
  while (ptr != NULL)
  {
    if ((data - ptr->data) == 0)
      break;
    diff = (data - ptr->data) > 0 ? 1 : 0;
    stack[ht] = ptr;
    dir[ht++] = diff;
    ptr = ptr->link[diff];
  }
  if (ptr->link[1] == NULL)
  {
    if ((ptr == rootRB) && (ptr->link[0] == NULL))
    {
      free(ptr);
      rootRB = NULL;
    }
    else if (ptr == rootRB)
    {
      rootRB = ptr->link[0];
      free(ptr);
    }
    else
    {
      stack[ht - 1]->link[dir[ht - 1]] = ptr->link[0];
    }
  }
  else
  {
    xPtr = ptr->link[1];
    if (xPtr->link[0] == NULL)
    {
      xPtr->link[0] = ptr->link[0];
      color = xPtr->color;
      xPtr->color = ptr->color;
      ptr->color = color;
      if (ptr == rootRB)
      {
        rootRB = xPtr;
      }
      else
      {
        stack[ht - 1]->link[dir[ht - 1]] = xPtr;
      }
      dir[ht] = 1;
      stack[ht++] = xPtr;
    }
    else
    {
      i = ht++;
      while (1)
      {
        dir[ht] = 0;
        stack[ht++] = xPtr;
        yPtr = xPtr->link[0];
        if (!yPtr->link[0])
          break;
        xPtr = yPtr;
      }
      dir[i] = 1;
      stack[i] = yPtr;
      if (i > 0)
        stack[i - 1]->link[dir[i - 1]] = yPtr;
      yPtr->link[0] = ptr->link[0];
      xPtr->link[0] = yPtr->link[1];
      yPtr->link[1] = ptr->link[1];
      if (ptr == rootRB)
      {
        rootRB = yPtr;
      }
      color = yPtr->color;
      yPtr->color = ptr->color;
      ptr->color = color;
    }
  }
  if (ht < 1)
    return;
  if (ptr->color == BLACK)
  {
    while (1)
    {
      pPtr = stack[ht - 1]->link[dir[ht - 1]];
      if (pPtr && pPtr->color == RED)
      {
        pPtr->color = BLACK;
        break;
      }
      if (ht < 2)
        break;
      if (dir[ht - 2] == 0)
      {
        rPtr = stack[ht - 1]->link[1];
        if (!rPtr)
          break;
        if (rPtr->color == RED)
        {
          stack[ht - 1]->color = RED;
          rPtr->color = BLACK;
          stack[ht - 1]->link[1] = rPtr->link[0];
          rPtr->link[0] = stack[ht - 1];
          if (stack[ht - 1] == rootRB)
          {
            rootRB = rPtr;
          }
          else
          {
            stack[ht - 2]->link[dir[ht - 2]] = rPtr;
          }
          dir[ht] = 0;
          stack[ht] = stack[ht - 1];
          stack[ht - 1] = rPtr;
          ht++;
          rPtr = stack[ht - 1]->link[1];
        }
        if ((!rPtr->link[0] || rPtr->link[0]->color == BLACK) &&
          (!rPtr->link[1] || rPtr->link[1]->color == BLACK))
        {
          rPtr->color = RED;
        }
        else
        {
          if (!rPtr->link[1] || rPtr->link[1]->color == BLACK)
          {
            qPtr = rPtr->link[0];
            rPtr->color = RED;
            qPtr->color = BLACK;
            rPtr->link[0] = qPtr->link[1];
            qPtr->link[1] = rPtr;
            rPtr = stack[ht - 1]->link[1] = qPtr;
          }
          rPtr->color = stack[ht - 1]->color;
          stack[ht - 1]->color = BLACK;
          rPtr->link[1]->color = BLACK;
          stack[ht - 1]->link[1] = rPtr->link[0];
          rPtr->link[0] = stack[ht - 1];
          if (stack[ht - 1] == rootRB)
          {
            rootRB = rPtr;
          }
          else
          {
            stack[ht - 2]->link[dir[ht - 2]] = rPtr;
          }
          break;
        }
      }
      else
      {
        rPtr = stack[ht - 1]->link[0];
        if (!rPtr)
          break;
        if (rPtr->color == RED)
        {
          stack[ht - 1]->color = RED;
          rPtr->color = BLACK;
          stack[ht - 1]->link[0] = rPtr->link[1];
          rPtr->link[1] = stack[ht - 1];
          if (stack[ht - 1] == rootRB)
          {
            rootRB = rPtr;
          }
          else
          {
            stack[ht - 2]->link[dir[ht - 2]] = rPtr;
          }
          dir[ht] = 1;
          stack[ht] = stack[ht - 1];
          stack[ht - 1] = rPtr;
          ht++;
          rPtr = stack[ht - 1]->link[0];
        }
        if ((!rPtr->link[0] || rPtr->link[0]->color == BLACK) &&
          (!rPtr->link[1] || rPtr->link[1]->color == BLACK))
        {
          rPtr->color = RED;
        }
        else
        {
          if (!rPtr->link[0] || rPtr->link[0]->color == BLACK)
          {
            qPtr = rPtr->link[1];
            rPtr->color = RED;
            qPtr->color = BLACK;
            rPtr->link[1] = qPtr->link[0];
            qPtr->link[0] = rPtr;
            rPtr = stack[ht - 1]->link[0] = qPtr;
          }
          rPtr->color = stack[ht - 1]->color;
          stack[ht - 1]->color = BLACK;
          rPtr->link[0]->color = BLACK;
          stack[ht - 1]->link[0] = rPtr->link[1];
          rPtr->link[1] = stack[ht - 1];
          if (stack[ht - 1] == rootRB)
          {
            rootRB = rPtr;
          }
          else
          {
            stack[ht - 2]->link[dir[ht - 2]] = rPtr;
          }
          break;
        }
      }
      ht--;
    }
  }
}
void inOrderRB(struct rbNode *node)
{
  	if (node)
	{
		inOrderRB(node->link[0]);
		//printf("%d ", node->data);
		//printf("\n ");
		all_keys[position_counte] = node->data;
		position_counte = position_counte+1;
		
		inOrderRB(node->link[1]);
	}
  return;
}

int RBHeight(struct rbNode *n) {
    if (n == NULL) return -1;
    return max(RBHeight(n->link[0]), RBHeight(n->link[1])) + 1;
}

//============  VALUES ============

void Generate_Values(){
	int choice = 0,rand_num = MAX,pos_vet = 0,tmp, i;
	//opening file with opening type w
	pont_arq = fopen("file_values.txt", "w");
	
	//testing if the file was actually created
	if(pont_arq == NULL)
	{
	  printf("Error opening file!");
	  return;
	}
    
    while( pos_vet < MAX - 1 )
    {
        // Choose a random position 
        choice = rand() % rand_num;
        // Switch to sequential position
        tmp = all_keys[pos_vet];
        all_keys[pos_vet] = all_keys[choice];
        all_keys[choice] = tmp;
        // Next position to be changed
        pos_vet++;
    }
    // Shows scrambled vector
    printf("--------------------------- \n");
    printf("|   scrambled numbers     | \n");
    printf("--------------------------- \n");
    for( i = 0; i< MAX; i++){
        fprintf(pont_arq, "%d", all_keys[i]);
    	fprintf(pont_arq, "%s", " \n");
	}
	
	//using fclose to close the file
	fclose(pont_arq);

	printf("Data successfully recorded!");

}

void Read_ValuesAVL(){
	int i;
	struct Node *root = NULL; 
	char Line[MAX];
	int *result;
	int values_int[MAX];
	// Open a TEXT file for READING
	pont_arq = fopen("file_values.txt", "rt");
	if (pont_arq == NULL)  // If there was an error opening
	{
		printf("File opening issues\n");
		return;
	}
	i = 1;
	tStart = clock();
	
	while (!feof(pont_arq))
	{
		// Read a line
		result = fgets(Line, 100, pont_arq);  // fgets reads up to 99 characters or up to \ n
		
		if (result){ // If it was possible to read
			printf("Line %d : %s",i,Line);
			//root = insertAVL(root, atoi(result));
			i++;
		}	
	}
	fclose(pont_arq);
	
	
	
	printf("---------------------------\n");
    printf("|     In-order AVL tree    |\n");
    printf("---------------------------\n");
	
	inOrderAVL(root);
	
	tStop = clock();
	
	tCurrent = ((tStop - tStart) / (CLOCKS_PER_SEC / 1000));
	
	height_avltree = AVLHeight(root);
}

void Read_ValuesRB(){
	int i;
	//struct Node *rootRB = NULL; 
	char Line[MAX];
	int *result;
	int values_int[MAX];
	// Open a TEXT file for READING
	pont_arq = fopen("file_values.txt", "rt");
	if (pont_arq == NULL)  // If there was an error opening
	{
		printf("File opening issues\n");
		return;
	}
	i = 1;
	
	tStart = clock();
	while (!feof(pont_arq))
	{
		// Read a line
		result = fgets(Line, 100, pont_arq);  // fgets reads up to 99 characters or up to \ n
		
		if (result){ // If it was possible to read
			//printf("Line %d : %s",i,Line);
			insertRB(atoi(result));
			i++;
		}	
	}
	fclose(pont_arq);

	printf("--------------------------- \n");
    printf("|      In-order RB tree    |\n");
    printf("--------------------------- \n");
	
	inOrderRB(rootRB);
	
	tStop = clock();
	
	tCurrent = ((tStop - tStart) / (CLOCKS_PER_SEC / 1000));
	
	height_rbtree = RBHeight(rootRB);
}

void Med(int vet[], int qtd){
	float value_med;
	printf("---------------------------\n");
    printf("|           Median        |\n");
    printf("---------------------------\n");
	if((qtd%2)==0){
		value_med = ((float)vet[qtd/2-1] + (float)vet[(qtd/2)])/2;
	}else{
		value_med = vet[qtd/2];
	}
	printf("%f", value_med);
}

//============  MAIN ============

int main() 
{ 
	int op = 1;
	int i;
	
	for(i=0; i<MAX; i++)
        all_keys[i]=i+1;
	
	do{
		printf("\n Exit(1) AVL(2) RB(3) \n");
		scanf("%d",&op);
		if(op==2){
			do{
				printf("\n Back(0) Read(1) Generate(2) Median (3) Rotations (4) Height (5)\n");
				scanf("%d",&op);
				
			  	if(op==2){
					Generate_Values();
				}else if(op==1){
					Read_ValuesAVL();
					printf("Tempo: %d", tCurrent);
					printf("\n");
				}else if(op==3){
					Med(all_keys,MAX);
				}else if(op==4){
					printf("Rotations: %d",rotations);
					printf("\n");
					rotations = 0;
				}else if(op==5){
					printf("Height: %d",height_avltree);
				}
				
				
			}while(op!=0);
		}
		if(op==3){
			do{
				printf("\n Back(0) Read(1) Generate(2) Median (3) Rotations (4) Height (5)\n");
				scanf("%d",&op);
				
			  	if(op==2){
					Generate_Values();
				}else if(op==1){
					Read_ValuesRB();
					printf("Tempo: %d", tCurrent);
					printf("\n");
				}else if(op==3){
					Med(all_keys,MAX);
				}else if(op==4){
					printf("Rotations: %d",rotations);
					printf("\n");
					rotations = 0;
				}else if(op==5){
					printf("Height: %d",height_rbtree);
				}
			}while(op!=0);
		}	
	}while(op!=1);

}
