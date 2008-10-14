member_(X,[X|_]).
member_(X,[_|Xs]):-member_(X,Xs).

append_([],Xs,Xs).
append_([X|Xs],Ys,[X|Zs]):-append_(Xs,Ys,Zs).

maplist_(_,[],[]).
maplist_(F,[I|Is],[O|Os]):-call(F,I,O),maplist_(F,Is,Os).

foldl(F,Z,Xs,R):-foldl0(Xs,F,Z,R).
  
foldl0([],_,R, R).
foldl0([X|Xs],F,R1, R3):-call(F,R1,X,R2),foldl0(Xs,F,R2, R3).

foldr(F,Z,Xs,R):-foldr0(Xs,F,Z,R).
  
foldr0([],_,Z,Z).
foldr0([X|Xs],F,Z,R2):-foldr0(Xs,F,Z,R1),call(F,X,R1,R2).

sum_(Xs,R):-foldl(plus_,0,Xs,R).

prod_(Xs,R):-foldl(mult_,1,Xs,R).

plus_(X,Y,Z):-Z is X+Y.

mult_(X,Y,Z):-Z is X*Y.
