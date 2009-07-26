-- This file contains functions to work with Binary Search Trees unrelated to their contents.

module BinarySearch where

-- The data type definition.  A tree can consist of either a Tip or a node with a value and two tree.
data Tree a = Tip | Node a (Tree a) (Tree a) deriving (Show,Eq)

-- Returns a leaf node, makes writing leafs easier
leaf x = Node x Tip Tip

-- Some examples of structure in code
t1 = Node 10 Tip Tip
t2 = Node 17 (Node 12 (Node 5 Tip (leaf 8)) (leaf 15))
             (Node 115
                   (Node 32 (leaf 30) (Node 46 Tip (leaf 57)))
                   (leaf 163))

-- The size of the tree - the number of items in the tree
size Tip = 0
size (Node _ tl tr) = 1 + size tl + size tr

-- Converts a tree into a list.  This list, if converted back into a tree, will return the same tree
treeToList Tip = []
treeToList (Node x xl xr) = x : treeToList xl ++ treeToList xr

-- Converts a tree into a sorted list.  This list will not convert back into the same tree.
treeToListOrd Tip = []
treeToListOrd (Node x xl xr) = treeToListOrd xl ++ x : treeToListOrd xr

-- Visually outputs a tree (sideways) by indenting each next node and additional tab in, outputs the right side, then the node, then the left side
pict t = putStr (pic "" t)
         where pic ind Tip = ind ++ "."
               pic ind (Node x tl tr) = pic ('\t':ind) tr ++ "\n" ++
                                        ind ++ show x     ++ "\n" ++
                                        pic ('\t':ind) tl ++ "\n"

-- Finds farthest left item.  In a BST this is the smallest value
farLft (Node x Tip _) = x
farLft (Node x xl _) = farLft xl

-- Finds farthest right item.  In a BST this is the largest value
farRt (Node x _ Tip) = x
farRt (Node x _ xr) = farRt xr

-- Mirrors a tree, so every right and left node switch
mirror Tip = Tip
mirror (Node x xl xr) = (Node x (mirror xr) (mirror xl))

