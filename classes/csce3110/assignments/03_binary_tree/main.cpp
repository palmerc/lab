/*
Cameron L Palmer
Binary Search Tree

Instructions:
To compile: g++ -g -o main main.cpp
To run: ./main
*/
#include <iostream>
#include "BinaryTree.h"

using namespace std;

int main()
{
   BinaryTree intTree;
   
   cout << "Beginning insert operations" << endl;
   intTree.insert( 7 );
   intTree.insert( 10 );  
   intTree.insert( 13 );
   intTree.insert( 2 );
   intTree.insert( 3 );  
   intTree.insert( 20 );  
   intTree.insert( 12 );  
   intTree.insert( 11 );  
   intTree.insert( 1 );  
   intTree.insert( 5 );  
   intTree.insert( 4 );  
   intTree.insert( 6 );
   
   cout << "Preorder Output" << endl;
   intTree.preOrder();
   cout << "Inorder Output" << endl;
   intTree.inOrder();
   cout << "Postorder Output" << endl;
   intTree.postOrder();
   
   cout << "Beginning delete operations" << endl;
   intTree.remove( 1 );
   intTree.remove( 20 );
   intTree.remove( 12 );
   intTree.remove( 11 );
   intTree.remove( 10 );
   intTree.remove( 7 );
   intTree.remove( 2 );
   intTree.remove( 3 );
   intTree.remove( 4 );
   intTree.remove( 5 );
   intTree.remove( 6 );
   intTree.remove( 13 );

   cout << "Preorder Output" << endl;
   intTree.preOrder();
   cout << "Inorder Output" << endl;
   intTree.inOrder();
   cout << "Postorder Output" << endl;
   intTree.postOrder();
      
   return 0;
}
