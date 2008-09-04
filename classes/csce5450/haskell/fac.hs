module Main where

import System( getArgs )

factorial 0 = 1
factorial n = n * factorial (n - 1)

main :: IO ()
main = do
	x <- getArgs >>= readIO . head
	print (factorial x)
	return ()
