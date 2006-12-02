#include <iostream>

using namespace std;

typedef int pType;

class BinaryTree
{
   public:
      BinaryTree();
      insert(ptype);
   
   private:
      BinaryNode *root;
   
      struct BinaryNode 
      {
         BinaryNode *parent;
         BinaryNode *leftChild;
         BinaryNode *rightChild;
         pType data;
            
         BinaryNode(pType item = 0) 
         {
            parent = NULL;
            leftChild = NULL;
            rightChild = NULL;
            data = item;
         }
      }
};

void BinaryTree::insert(pType newData)
{
   BinaryNode *x = root;
   y = NULL;
   while (x != NULL)
   {
      y = x;
      if (newData < x->data)
         x = x->left;
      else
         x = x->right;
   }
   z->parent = y;
   if (y == NULL)
      root = new BinaryNode(newData);
   else if (newData < y->data)
      y->left = z;
   else
      y->right = z;      
}

BinaryNode *find(BinaryNode *&root, pType data)
{
   if (root->data == data)
   {
      return root;
   }
   else
   {
      if (root->data < data)
         find(root->rightChild, data);
      else if (root->data > data)
         find(root->leftChild, data);
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
      root->data = temp->data;
      remove(temp);
   }
}

void inOrderPrint(BinaryNode *root)
{
   if (root == NULL)
      cout << "This tree is empty" << endl;
   if (root->leftChild != NULL)
      inOrderPrint(root->leftChild);
   cout << root->data << endl;
   if (root->rightChild != NULL)
      inOrderPrint(root->rightChild);
}

void preOrderPrint(BinaryNode *root)
{
   if (root != NULL)
      cout << root->data << endl;
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
   cout << root->data << endl;
}

void printLevel(BinaryNode *root)
{
   if (root != NULL)
   {
      cout << root->data << " ";
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
