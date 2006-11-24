#include <iostream>

using namespace std;

typedef int pType;

struct BinaryNode {
   BinaryNode *parent;
   BinaryNode *leftChild;
   BinaryNode *rightChild;
   pType payload;
   BinaryNode(pType item = 0) 
   {
      parent = NULL;
      leftChild = NULL;
      rightChild = NULL;
      payload = item;
   }
};

void insert(BinaryNode *&root, BinaryNode *&parent, pType newPayload)
{
   if (root == NULL)
   {
      root = new BinaryNode(newPayload);
      if (parent != NULL)
         root->parent = parent;
      return;
   }
   else if (newPayload < root->payload)
   {
      insert(root->leftChild, root, newPayload);
   }
   else
   {
      insert(root->rightChild, root, newPayload);
   }
}

BinaryNode *find(BinaryNode *&root, pType payload)
{
   if (root->payload == payload)
   {
      return root;
   }
   else
   {
      if (root->payload < payload)
         find(root->rightChild, payload);
      else if (root->payload > payload)
         find(root->leftChild, payload);
   }
}

void remove(BinaryNode *&root)
{
   BinaryNode *temp = root;
   if (root->leftChild == NULL)
   {
      root = root->rightChild;
      delete temp;
   }
   else if (root->rightChild == NULL)
   {
      root = root->leftChild;
      delete temp;
   }
   else
   {
      temp = root->leftChild;
      while (temp->rightChild != NULL)
         temp = temp->rightChild;
      root->payload = temp->payload;
      remove(temp);
   }
}

void inOrderPrint(BinaryNode *root)
{
   if (root == NULL)
      cout << "This tree is empty" << endl;
   if (root->leftChild != NULL)
      inOrderPrint(root->leftChild);
   cout << root->payload << endl;
   if (root->rightChild != NULL)
      inOrderPrint(root->rightChild);
}

void preOrderPrint(BinaryNode *root)
{
   if (root != NULL)
      cout << root->payload << endl;
   else
      cout << "This tree is empty" << endl;
   if (root->leftChild != NULL)
      preOrderPrint(root->leftChild);
   if (root->rightChild != NULL)
      preOrderPrint(root->rightChild);
}

void postOrderPrint(BinaryNode *root)
{
   if (root == NULL)
      cout << "This tree is empty" << endl;
   if (root->leftChild != NULL)
      postOrderPrint(root->leftChild);
   if (root->rightChild != NULL)
      postOrderPrint(root->rightChild);
   cout << root->payload << endl;
}

void printLevel(BinaryNode *root)
{
   if (root != NULL)
   {
      cout << root->payload << " ";
      printLevel(root->leftChild);
      printLevel(root->rightChild);
   }
}

int main()
{
   BinaryNode *root;
   root = NULL;
   BinaryNode *parent;
   parent = NULL;
   
   for (int i = 0; i < 50; ++i)
   {
      if (i % 2 == 0)
         insert(root, parent, 50 - i);
      else 
         insert(root, parent, 50 + i);
   }
   cout << "****** INSERT" << endl;
   inOrderPrint(root);
   root = find(root, 50);
   remove( root );
   cout << "****** REMOVE" << endl;
   inOrderPrint(root);
   return 0;
}
