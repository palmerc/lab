List[12,24,333,8,9].
sumlist([],0).
sumlist([H|T],N) :- sumlist(T,N1), N is N1+H.
