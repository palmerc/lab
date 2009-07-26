data Tree a = Empty | Branch a (Tree a) (Tree a)
        deriving (Show, Eq)

tree1 = Branch 'a' (Branch 'b' (Branch 'd' Empty Empty)
                              (Branch 'e' Empty Empty))
                  (Branch 'c' Empty
                              (Branch 'f' (Branch 'g' Empty Empty)
                                        Empty))

tree4 = Branch 1 (Branch 2 Empty (Branch 4 Empty Empty))
                 (Branch 2 Empty Empty)

add :: Ord a => a -> Tree a -> Tree a
add x Empty            = Branch x Empty Empty
add x t@(Branch y l r) = case compare x y of
                            LT -> Branch y (add x l) r
                            GT -> Branch y l (add x r)
                            EQ -> t
 
construct xs = foldl (flip add) Empty xs

stringToTree :: (Monad m) => String -> m (Tree Char)
stringToTree "" = return Empty
stringToTree [x] = return $ Branch x Empty Empty
stringToTree str = tfs str >>= \ ("", t) -> return t
    where tfs a@(x:xs) | x == ',' || x == ')' = return (a, Empty)
          tfs (x:y:xs)
                | y == ',' || y == ')' = return (y:xs, Branch x Empty Empty)
                | y == '(' = do (',':xs', l) <- tfs xs
                                (')':xs'', r) <- tfs xs'
                                return $ (xs'', Branch x l r)
          tfs _ = fail "bad parse"

treeToString :: Tree Char -> String
treeToString Empty = ""
treeToString (Branch x Empty Empty) = [x]
treeToString (Branch x l r) =
        x : '(' : treeToString l ++ "," ++ treeToString r ++ ")"