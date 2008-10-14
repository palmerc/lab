a(X):-b(X),!,c(X).
a(5).

b(1).
b(2).
b(3).

c(2).
c(3).
c(4).

/*
how cut works:

if you reach it, no more choices to the left of it (comit to bindings\
and fail of not of them matches

this means no other clauses are tried in the same predicate

*/