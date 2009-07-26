a(X):-b(X),c(X).

b(1).
b(2).
b(3).

c(2).
c(3).
c(4).

ite(Cond,Then,_Else):-call(Cond),!,call(Then).
ite(_Cond,_Then,Else):-call(Else).

not_(X):-ite(X,fail,true).

not__(X):-call(X),!,fail.
not__(_X).

once_(X):-call(X),!.


ex(X,Y):-
  ( X>5->Y=ok
  ; X>2->Y=maybe
  ; Y=sorry
  ).

