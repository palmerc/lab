// Cameron Palmer
// Compile: g++ -o example example.cpp
// run: ./example

#include <iostream>
using std::cout;
using std::cin;
using std::fixed;

#include <iomanip>
using std::setprecision;

#include "Tree.h" // Tree class definition

int main()
{
   Tree< int > intTree; // create Tree of int values

   // insert 10 integers to intTree
   cout << "Inserting" << endl;
   intTree.insertNode(7);
   intTree.insertNode( 10 );  
   intTree.insertNode( 13 );
   intTree.insertNode( 2 );
   intTree.insertNode( 3 );  
   intTree.insertNode( 20 );  
   intTree.insertNode( 12 );  
   intTree.insertNode( 11 );  
   intTree.insertNode( 1 );  
   intTree.insertNode( 5 );  
   intTree.insertNode( 4 );  
   intTree.insertNode( 6 );  
   cout << "\nPreorder traversal\n";
   intTree.preOrderTraversal();
   cout << "\nInorder traversal\n";
   intTree.inOrderTraversal();
   cout << "\nPostorder traversal\n";
   intTree.postOrderTraversal();
   cout << endl;
   
   cout << intTree.findNode( 7 );
   
   cout << "Deleting" << endl;
   cout << "1 " << endl;
   intTree.removeNode( 1 );
   cout << "20 " << endl;
   intTree.removeNode( 20 );
   cout << "12 " << endl;
   intTree.removeNode( 12 );
   cout << "11 " << endl;
   intTree.removeNode( 11 );
   cout << "10 " << endl;
   intTree.removeNode( 10 );
   cout << "7 " << endl;
   intTree.removeNode( 7 );
   cout << "\nPreorder traversal\n";
   intTree.preOrderTraversal();
   cout << "\nInorder traversal\n";
   intTree.inOrderTraversal();
   cout << "\nPostorder traversal\n";
   intTree.postOrderTraversal();
   cout << endl;
   cout << "2 " << endl;
   intTree.removeNode( 2 );
   cout << "3 ";
   intTree.removeNode( 3 );
   cout << "4 ";
   intTree.removeNode( 4 );
   cout << "5 ";
   intTree.removeNode( 5 );
   cout << "6 ";
   intTree.removeNode( 6 );
   cout << "13";
   intTree.removeNode( 13 );
   cout << endl;
   
   cout << "\nPreorder traversal\n";
   intTree.preOrderTraversal();
   cout << "\nInorder traversal\n";
   intTree.inOrderTraversal();
   cout << "\nPostorder traversal\n";
   intTree.postOrderTraversal();
   cout << endl;
   return 0;
} // end main
