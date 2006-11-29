// Treenode.h
// Template TreeNode class definition.
#ifndef TREENODE_H
#define TREENODE_H

// forward declaration of class Tree
template< typename NODETYPE > class Tree;  

// TreeNode class-template definition
template< typename NODETYPE >
class TreeNode 
{
   friend class Tree< NODETYPE >;
public:
   // constructor
   TreeNode( TreeNode< NODETYPE > **parent, const NODETYPE &d )
   :     parentPtr( parent ), // pointer to left subtree
         leftPtr( 0 ), // pointer to left subtree
         data( d ), // tree node data
         rightPtr( 0 ) // pointer to right substree
   { 
      // empty body 
   } // end TreeNode constructor

   // return copy of node's data
   NODETYPE getData() const 
   { 
      return data; 
   } // end getData function
private:
   TreeNode< NODETYPE > *parentPtr; // pointer to left subtree
   TreeNode< NODETYPE > *leftPtr; // pointer to left subtree
   NODETYPE data;
   TreeNode< NODETYPE > *rightPtr; // pointer to right subtree
}; // end class TreeNode

#endif
