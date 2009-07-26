-- higher order functions

map1 _ []=[]
map1 f (x:xs) = (f x) : (map1 f xs)

map2 _ [] []=[]
map2 f (x:xs) (y:ys) = (f x y) : (map2 f xs ys)

sum_ init [] = init
sum_ init (x:xs) = (x + (sum_ init xs))

product_ init [] = init
product_ init (x:xs) = (x * (product_ init xs))

fold_ _ init [] = init
fold_ f init (x:xs) = (f x (fold_ f init xs))

fold1_ f (x:xs) = fold_ f x xs
 
factorial n = fold_ (*) 1 [1..n]

