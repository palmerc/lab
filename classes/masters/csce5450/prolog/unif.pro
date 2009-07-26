/*

1 ?- X=a.
X = a.

2 ?- X=a,X=b.
fail.

3 ?- X=Y.
X = Y.

4 ?- X=Y,Y=b.
X = b,
Y = b.

5 ?- X=Y,Y=Z,Z=c.
X = c,
Y = c,
Z = c.


10 ?- f(1)=f(2).
fail.

11 ?- f(1)=f(Y).
Y = 1.

12 ?- f(a,b)=f(X,X).
fail.

13 ?- [X|Xs]=[1,2,3]
|    .
X = 1,
Xs = [2, 3].

14 ?- [X,Y|Xs]=[1,2,3,4]
|    .
X = 1,
Y = 2,
Xs = [3, 4].
15 ?- [X,X|Xs]=[1,2,3,4].
fail.

16 ?- f(X,g(X,Y),Y)=f(a,Z,b).
X = a,
Y = b,
Z = g(a, b).

dereferencing a variable: follow pointers

22 ?- likes(mary,What).
What = wine.

23 ?- likes(mary,What)=likes(joe,beer).
fail.

24 ?- likes(mary,What)=likes(mary,wine).
What = wine.

try to unify query and facts in db
*/

likes(joe,beer).
likes(mary,wine).
likes(paul,water).
likes(mary,water).

/*

unification algorithm:

try to unify all parts - if one fails, fail

propagate variable equality and bindings
after dereferencing variables


*/
