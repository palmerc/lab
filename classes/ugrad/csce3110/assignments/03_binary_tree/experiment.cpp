#include <iostream>

using namespace std;

typedef int pType;

class BinaryTree
{
   public:
      BinaryTree(pType payload);
      ~BinaryTree();
       
   private:
      struct BinaryNode {
         BinaryNode *parent;
         BinaryNode *leftChild;
         BinaryNode *rightChild;
         pType payload;
         BinaryNode() : parent(0), payload(0), leftChild(0), rightChild(0) {}
         BinaryNode(BinaryNode _parent, pType _payload, BinaryNode _leftChild, BinaryNode _rightChild) :
            parent(_parent), payload(_payload), leftChild(_leftChild), rightChild(_rightChild) {}
      };
      BinaryNode *root;
      int height;
      int nodeCount;
};

BinaryTree::BinaryTree(pType item)
{
   root = new BinaryNode(item);
   height = 0;
   nodeCount = 0;
}

BinaryTree::~BinaryTree()
{

}

void binaryInsert(pType item)
{
   BinaryNode *cur = root;
   while (cur)
   if (item < cur->payload)
      cur = cur->leftchild;
   else if (item > cur->payload)
      cur = cur->rightchild;
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

int main()
{
   BinaryNode *root;
   root = NULL;
   
   for (int i = 0; i < 50; ++i)
      if (i % 2 == 0)
         insert(root, 50 - i);
      else 
         insert(root, 50 + i);
   printLevel(root);
   return 0;
}
