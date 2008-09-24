import Data.Tree

data Term a = Atom a | Term a [Term a]
	deriving (Eq,Ord,Show,Read)

termToTree :: Term a -> Tree a
termToTree (Atom x)     = Node x []
termToTree (Term x xs)  = Node x (fmap termToTree xs)

treeToTerm :: Tree a -> Term a
treeToTerm (Node x []) = Atom x
treeToTerm (Node x xs) = Term x (fmap treeToTerm xs)

drawTerm :: (Show a) => Term a -> String
drawTerm = drawTree . fmap show . termToTree

printTerm :: (Show a) => Term a -> IO ()
printTerm = putStr . drawTerm

t2 = Term "term1" [Atom "atom1", Term "term2" [Atom "atom2", Atom "atom3"]]
