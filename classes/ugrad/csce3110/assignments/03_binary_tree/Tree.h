// Tree.h
// Template Tree class definition.
#ifndef TREE_H
#define TREE_H

#include <iostream>
using std::cout;
using std::endl;

#include <new>
#include "Treenode.h"

// Tree class-template definition
template< typename NODETYPE > class Tree
{
public:
   Tree(); // constructor
   void insertNode( const NODETYPE & );
   void removeNode( const NODETYPE & );
   void preOrderTraversal() const;
   void inOrderTraversal() const;
   void postOrderTraversal() const;
   Tree< NODETYPE > ** findNode( const NODETYPE & );
private:
   TreeNode< NODETYPE > *rootPtr;

   // utility functions
   void insertNodeHelper( TreeNode< NODETYPE > *, TreeNode< NODETYPE > **, const NODETYPE & );
   void removeNodeHelper( TreeNode< NODETYPE > **, const NODETYPE & );
   void preOrderHelper( TreeNode< NODETYPE > * ) const;
   void inOrderHelper( TreeNode< NODETYPE > * ) const;
   void postOrderHelper( TreeNode< NODETYPE > * ) const;
}; // end class Tree

// constructor
template< typename NODETYPE >
Tree< NODETYPE >::Tree() 
{ 
   rootPtr = 0; // indicate tree is initially empty
} // end Tree constructor

// insert node in Tree
template< typename NODETYPE >
void Tree< NODETYPE >::insertNode( const NODETYPE &value )
{ 
   insertNodeHelper( 0, &rootPtr, value );
} // end function insertNode

// utility function called by insertNode; receives a pointer
// to a pointer so that the function can modify pointer's value
template< typename NODETYPE >
void Tree< NODETYPE >::insertNodeHelper( 
   TreeNode< NODETYPE > *parent, TreeNode< NODETYPE > **ptr, const NODETYPE &value )
{
   cout << "Insert " << parent << " " << *ptr << " " << value << endl;
   // subtree is empty; create new TreeNode containing value
   if ( *ptr == 0 )  
      *ptr = new TreeNode< NODETYPE >( parent, value );
   else // subtree is not empty
   {
      // data to insert is less than data in current node
      if ( value < ( *ptr )->data )
         insertNodeHelper( *ptr, &( ( *ptr )->leftPtr ), value );
      else
      {
         // data to insert is greater than data in current node
         if ( value > ( *ptr )->data )
            insertNodeHelper( *ptr, &( ( *ptr )->rightPtr ), value );
         else // duplicate data value ignored
            cout << value << " dup" << endl;
      } // end else
   } // end else
} // end function insertNodeHelper

// remove node in Tree
template< typename NODETYPE >
void Tree< NODETYPE >::removeNode( const NODETYPE &value )
{
   removeNodeHelper( &rootPtr, value );
} // end function removeNode

template< typename NODETYPE >
Tree< NODETYPE >** Tree< NODETYPE >::findNode( const NODETYPE &value )
{
   TreeNode< NODETYPE > *ptr = *rootPtr;
   while ( ( *ptr )->data != value )
   {
      if ( value < ( *ptr )->data )
         *ptr = ( *ptr )->leftPtr;
      else if ( value > ( *ptr )->data )
         *ptr = ( *ptr )->rightPtr;
   }
   return *ptr;
}

// utility function called by removeNode; receives a pointer
// to a pointer so that the function can modify pointer's value
template< typename NODETYPE >
void Tree< NODETYPE >::removeNodeHelper( 
   TreeNode< NODETYPE > **ptr, const NODETYPE &value )
{
   if ( value < ( *ptr )->data )
      removeNodeHelper( &( ( *ptr )->leftPtr ), value );
   else if ( value > ( *ptr )->data )
      removeNodeHelper( &( ( *ptr )->rightPtr ), value );
   else if ( ( *ptr )->leftPtr != 0 && ( *ptr )->rightPtr != 0 ) // Two children
   {
      if ( ( *ptr )->rightPtr->leftPtr != 0 ) // In order successor
      {  
         cout << "This" << endl;
         ( *ptr )->rightPtr->leftPtr->parentPtr = ( *ptr )->parentPtr;
         // Connect with my new parent
         // If I am my parent's right child
         if ( ( *ptr )->parentPtr->rightPtr == *ptr)
            ( *ptr )->parentPtr->rightPtr = ( *ptr )->rightPtr->leftPtr;
         else // If I am my parent's left child
            ( *ptr )->parentPtr->leftPtr = ( *ptr )->rightPtr->leftPtr;
         // Change the in order successors parent
         
         //delete *ptr;
      }
      else
      {
         cout << "Promote the right one" << endl;
         // Fix the parent ptr of my replacement
         //cout << ( *ptr )->leftPtr->data << endl;
         ( *ptr )->rightPtr->leftPtr = ( *ptr )->leftPtr;
         
         // Fix the parent ptr to me
         if ( ( *ptr )->parentPtr != 0 ) // If you aren't root
         {
            ( *ptr )->rightPtr->parentPtr = ( *ptr )->parentPtr;
            if ( ( *ptr )->parentPtr->rightPtr == *ptr)
               ( *ptr )->parentPtr->rightPtr = ( *ptr )->rightPtr;
            else // If I am my parent's left child
               ( *ptr )->parentPtr->leftPtr = ( *ptr )->rightPtr;
         }
      }
   }
   else if ( ( *ptr )->leftPtr != 0 ) // if it has only a left child
   {
      ( *ptr )->leftPtr->parentPtr = ( *ptr )->parentPtr;
      // I am connected to the right of the parent
      if ( ( *ptr )->parentPtr->rightPtr == *ptr)
         ( *ptr )->parentPtr->rightPtr = ( *ptr )->leftPtr;
      else // If I am my parent's left child
         ( *ptr )->parentPtr->leftPtr = ( *ptr )->leftPtr;
      // Assign the left child my parent
      //delete *ptr;
   }
   else if ( ( *ptr )->rightPtr != 0 ) // if it has only a right child
   {
      //cout << "Delete with Right child only " << ( *ptr )->parentPtr << endl;
      ( *ptr )->rightPtr->parentPtr = ( *ptr )->parentPtr;
      //cout << "double checking " << ( *ptr )->rightPtr->parentPtr;
      // I am connected to the right of the parent
      //cout << ( *ptr )->parentPtr->rightPtr << "==" << *ptr << endl;
      //cout << ( *ptr )->parentPtr->leftPtr << "==" << *ptr << endl;
      if ( ( *ptr )->parentPtr->rightPtr == *ptr)
      {
         //cout << "tick1" << endl;
         // Assign the right child to the parents right side
         ( *ptr )->parentPtr->rightPtr = ( *ptr )->rightPtr;
      }
      else // If I am my parent's left child
      {
         //cout << "tick2" << endl;
         ( *ptr )->parentPtr->leftPtr = ( *ptr )->rightPtr;
      }
      // Assign the right child my parent
      //cout << "tick3" << endl;

      //cout << "tick4" << endl;
      //delete *ptr;
   }
   else // No children - Tested
   {
      //cout << "No Children" << endl;
      //cout << ( *ptr )->parentPtr->rightPtr << "==" << *ptr << endl;
      //cout << ( *ptr )->parentPtr->leftPtr << "==" << *ptr << endl;
      // I am connected to the right of the parent
      //cout << "This old value" << ( *ptr )->parentPtr->data << endl;
      if ( ( *ptr )->parentPtr->rightPtr == *ptr)
         ( *ptr )->parentPtr->rightPtr = 0;
      else // If I am my parent's left child
         ( *ptr )->parentPtr->leftPtr = 0;
      delete *ptr;
   }
} // end function removeNodeHelper


// begin preorder traversal of Tree
template< typename NODETYPE > 
void Tree< NODETYPE >::preOrderTraversal() const
{ 
   preOrderHelper( rootPtr ); 
} // end function preOrderTraversal

// utility function to perform preorder traversal of Tree
template< typename NODETYPE >
void Tree< NODETYPE >::preOrderHelper( TreeNode< NODETYPE > *ptr ) const
{
   if ( ptr != 0 ) 
   {
      cout << ptr->data << ' '; // process node          
      preOrderHelper( ptr->leftPtr ); // traverse left subtree 
      preOrderHelper( ptr->rightPtr ); // traverse right subtree
   } // end if
} // end function preOrderHelper

// begin inorder traversal of Tree
template< typename NODETYPE >
void Tree< NODETYPE >::inOrderTraversal() const
{ 
   inOrderHelper( rootPtr ); 
} // end function inOrderTraversal

// utility function to perform inorder traversal of Tree
template< typename NODETYPE >
void Tree< NODETYPE >::inOrderHelper( TreeNode< NODETYPE > *ptr ) const
{
   if ( ptr != 0 ) 
   {
      inOrderHelper( ptr->leftPtr ); // traverse left subtree  
      cout << ptr->data << ' '; // process node                
      inOrderHelper( ptr->rightPtr ); // traverse right subtree
   } // end if
} // end function inOrderHelper

// begin postorder traversal of Tree
template< typename NODETYPE >
void Tree< NODETYPE >::postOrderTraversal() const
{ 
   postOrderHelper( rootPtr ); 
} // end function postOrderTraversal

// utility function to perform postorder traversal of Tree
template< typename NODETYPE >
void Tree< NODETYPE >::postOrderHelper( 
   TreeNode< NODETYPE > *ptr ) const
{
   if ( ptr != 0 ) 
   {
      postOrderHelper( ptr->leftPtr ); // traverse left subtree  
      postOrderHelper( ptr->rightPtr ); // traverse right subtree
      cout << ptr->data << ' '; // process node                  
   } // end if
} // end function postOrderHelper

#endif
