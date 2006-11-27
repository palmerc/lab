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

BinaryNode* insert(BinaryNode *&root, BinaryNode *parent, pType newPayload)
{
   if (parent == NULL)
      cout << newPayload << " parent is NULL" << endl;
   if (root == NULL)
   {
      root = new BinaryNode(newPayload);
      return root;
   }
   else if (newPayload < root->payload)
   {
      insert(root->leftChild, parent, newPayload);
   }
   else
   {
      insert(root->rightChild, parent, newPayload);
   }
}

BinaryNode *find(BinaryNode *root, pType payload)
{
   cout << "found " << root->payload << endl;
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
   cout << "removing " << root->payload << endl;
   BinaryNode *temp = root;
   // CASES
   // No children
   // Right Child
   // Left Child
   // Left and Right Child
   if (root->leftChild == NULL && root->rightChild == NULL)
   {
      if (root->parent->leftChild == root)
         root->parent->leftChild = NULL;
      else
         root->parent->rightChild = NULL;
      delete root;
   }
   else if (root->leftChild == NULL)
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
      temp->parent->rightChild = NULL;
      temp->parent = root->parent;
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
   int elements[7] = { 50, 40, 46, 65, 67, 54, 33 };
   BinaryNode *root, *temp;
   root = NULL;
   temp = NULL;
   BinaryNode *parent;
   parent = NULL;
   
   cout << "****** Insertion" << endl;
   for (int i = 0; i < 7; ++i)
   {
      cout << elements[i] << endl;
      parent = insert(root, parent, elements[i]);
   }
   cout << "****** Pre Order" << endl;
   preOrderPrint(root);   
   cout << "****** In Order" << endl;
   inOrderPrint(root);
   cout << "****** Post Order" << endl;
   postOrderPrint(root);
   temp = find(root, 50);
   remove( temp );
   cout << " 50 Removed " << endl;
   temp = find(root, 33);
   cout << "Weird " << temp->payload;
   remove( temp );
   cout << " 33 Removed " << endl;
   cout << "****** REMOVE" << endl;
   preOrderPrint(root);
   return 0;
}
