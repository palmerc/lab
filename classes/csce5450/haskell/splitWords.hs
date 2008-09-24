module Test where
import IO
import List

type Word = String
type Line = String

splitWords :: Line -> [Word]
splitWords [] = []
splitWords line
	= takeWhile isLetter line : (splitWords . dropWhile (not.isLetter) .
		dropWhile isLetter) line where
			isLetter ch
				= ('a' <= ch) && (ch <= 'z')
				|| ('A' <= ch) && (ch <= 'Z')
				|| ('0' <= ch) && (ch <= '9')

