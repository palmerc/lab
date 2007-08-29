#include<iostream>
typedef int pType;

using std::cout;
using std::endl;

class BinaryTree
{
   public:
      BinaryTree();
      ~BinaryTree();
      void insert(pType);
      void remove(pType);
      void preOrder();
      void inOrder();
      void postOrder();
   
   private: 
      struct BinaryNode 
      {
         BinaryNode* parent;
         BinaryNode* left;
         BinaryNode* right;
         pType data;
            
         BinaryNode(pType item = 0) 
         {
            parent = 0;
            left = 0;
            right = 0;
            data = item;
         }
      };
      
      BinaryNode *rootPtr;
      int nodeCount;
      
      void treeInsert(BinaryNode*);
      BinaryNode* treeDelete(BinaryNode*);
      BinaryNode* treeSuccessor(BinaryNode*);
      BinaryNode* treeMin(BinaryNode*);
      BinaryNode* treeMax(BinaryNode*);
      BinaryNode* treeSearch(BinaryNode *, pType);
      BinaryNode* iterTreeSearch(BinaryNode *, pType);
      void preOrderTreeWalk(BinaryNode*);
      void inOrderTreeWalk(BinaryNode*);
      void postOrderTreeWalk(BinaryNode*);
};

BinaryTree::BinaryTree()
{
   rootPtr = 0;
}
BinaryTree::~BinaryTree()
{
   while (nodeCount > 0)
      remove(rootPtr->data);
}
void BinaryTree::insert(pType data)
{
   BinaryNode* z = new BinaryNode(data);
   treeInsert(z);
   nodeCount++;
}
void BinaryTree::treeInsert(BinaryNode* z)
{
   BinaryNode* x = rootPtr;
   BinaryNode* y = 0;
   while (x != 0)
   {
      y = x;
      if (z->data < x->data)
         x = x->left;
      else
         x = x->right;
   }
   z->parent = y;
   if (y == 0)
      rootPtr = z;
   else if (z->data < y->data)
      y->left = z;
   else
      y->right = z;      
}
void BinaryTree::remove(pType k)
{
   BinaryNode* x = treeSearch(rootPtr, k);
   if (x != 0)
      x = treeDelete(x);
   delete x;
   nodeCount--;
}
BinaryTree::BinaryNode* BinaryTree::treeDelete(BinaryNode* z)
{
   BinaryNode* x = 0;
   BinaryNode* y = 0;
   if (z->left == 0 || z->right == 0)
      y = z;
   else
      y = treeSuccessor(z);
   
   if (y->left != 0)
      x = y->left;
   else
      x = y->right;
   
   if (x != 0)
      x->parent = y->parent;
   
   if (y->parent == 0)
      rootPtr = x;
   else if (y == y->parent->left)
      y->parent->left = x;
   else
      y->parent->right = x;
   
   if (y != z)
      z->data = y->data;
   return y;
}
BinaryTree::BinaryNode* BinaryTree::treeSuccessor(BinaryNode* x)
{
   if (x->right != 0)
      return treeMin(x->right);
   
   BinaryNode* y = x->parent;
   while (y != 0 and x == y->right)
   {
      x = y;
      y = y->parent;
   }
   return y;
}
BinaryTree::BinaryNode* BinaryTree::treeMin(BinaryNode* x)
{
   while (x->left != 0)
      x = x->left;
   return x;
}
BinaryTree::BinaryNode* BinaryTree::treeMax(BinaryNode* x)
{
   while (x->right != 0)
      x = x->right;
   return x;
}
BinaryTree::BinaryNode* BinaryTree::treeSearch(BinaryNode *x, pType k)
{
   if (x == 0 || k == x->data)
      return x;
   if (k < x->data)
      return treeSearch(x->left, k);
   else
      return treeSearch(x->right, k);
}
BinaryTree::BinaryNode* BinaryTree::iterTreeSearch(BinaryNode *x, pType k)
{
   while (x != 0 && k != x->data)
   {
      if (k < x->data)
         x = x->left;
      else
         x = x->right;
   }
   return x;
}
void BinaryTree::preOrder()
{
   preOrderTreeWalk(rootPtr);
}
void BinaryTree::preOrderTreeWalk(BinaryNode* x)
{
   if (x != 0)
   {
      cout << x->data << endl;
      preOrderTreeWalk(x->left);
      preOrderTreeWalk(x->right);
   }
}
void BinaryTree::inOrder()
{
   inOrderTreeWalk(rootPtr);
}
void BinaryTree::inOrderTreeWalk(BinaryNode* x)
{
   if (x != 0)
   {
      inOrderTreeWalk(x->left);
      cout << x->data << endl;
      inOrderTreeWalk(x->right);
   }
}
void BinaryTree::postOrder()
{
   postOrderTreeWalk(rootPtr);
}
void BinaryTree::postOrderTreeWalk(BinaryNode* x)
{
   if (x != 0)
   {
      postOrderTreeWalk(x->left);
      postOrderTreeWalk(x->right);
      cout << x->data << endl;
   }
}
