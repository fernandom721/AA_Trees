#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

struct node
{
    int key;
    struct node *left, *right;
};

struct node *newNode(int item)
{
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

void inorder(struct node *root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

struct node *insert(struct node *node, int key)
{ /* If the tree is empty, return a new node */
    if (node == NULL)
        return newNode(key); /* Otherwise, recur down the tree */
    if (key < node->key)
        node->left = insert(node->left, key);
    else
        node->right = insert(node->right, key); /* return the (unchanged) node pointer */
    return node;
}

struct node *minValueNode(struct node *node)
{
    struct node *current = node; /* loop down to find the leftmost leaf */
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

struct node *deleteNode(struct node *root, int key)
{
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);

    else if (key > root->key)
        root->right = deleteNode(root->right, key);

    else
    {
        if (root->left == NULL)
        {
            struct node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            struct node *temp = root->left;
            free(root);
            return temp;
        }

        struct node *temp = minValueNode(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}
//el padre en comun de 2 nodos
int ancestro(node *root, int a, int b)
{
    if (root->key < a && root->key < b)
    {
        return ancestro(root->right, a, b);
    }
    if (root->key > a && root->key > b)
    {
        return ancestro(root->left, a, b);
    }

    cout << "ancestro en comun: " << root->key << endl;

    return root->key;
}
//cant de nodos de diferencia de uno a otro

int suma(node *root, int a){
    
    if(root->key<a){
        return suma(root->right,a)+1;
    }
    if(root->key>a){
        return suma(root->left,a)+1;
    }
    if(root->key==a){
        return 1;
    }
}

//cant de nodos de diferencia de uno a otro
int aux(node *root, int a, int b)
{
    if (root != NULL)
    {
        if (root->key < a && root->key < b)
        {
            return aux(root->right, a, b);
        }
        if (root->key > a && root->key > b)
        {
            return aux(root->left, a, b);
        }
        cout << suma(root->left,a) + suma(root->right,b);
        return suma(root->left,a) + suma(root->right,b);
    }

    return -1;
}



int main()
{ /* Let us create following BST
    50
    / \
   30 70
  / \ / \
20 40 60 80 
/
10*/
    struct node *root = NULL;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 70);
    root = insert(root, 60);
    root = insert(root, 80);
    root = insert(root, 10);
    printf("Inorder traversal of the given tree \n");
    inorder(root);

    //ancestro(root, 10, 40);
    //aux(root,10,60);

    //deleteNode(root,30);
    //inorder(root);



    return 0;
}