gender_of('Bart',male).
gender_of('Maggie',female).
gender_of('Lisa',female).
gender_of('Marge',female).
gender_of('Homer',male).
gender_of('Abraham',male).
gender_of('Ned',male).
gender_of('Rodd',male).
gender_of('Todd',male).

parent_of('Bart','Homer').
parent_of('Lisa','Homer').
parent_of('Maggie','Homer').
parent_of('Bart','Marge').
parent_of('Lisa','Marge').
parent_of('Maggie','Marge').
parent_of('Homer','Abraham').
parent_of('Ned','Abraham').
parent_of('Rodd','Ned').
parent_of('Todd','Ned').
parent_of('Abraham','Adam').

male(X):-gender_of(X,male).

female(X):-gender_of(X,female).

person(X):-gender_of(X,_).

orphan(X):-person(X), \+(parent_of(X,_)).

father_of(Child,Father):-parent_of(Child,Father),male(Father).

mother_of(Child,Mother):-parent_of(Child,Mother),female(Mother).

child_of(Parent,Child):-parent_of(Child,Parent).

sibling_of(X,Y):-parent_of(X,Z),parent_of(Y,Z),\+(X==Y).

brother_of(X,Y):-sibling_of(X,Y),gender_of(Y,male).

sister_of(X,Y):-sibling_of(X,Y),gender_of(Y,female).

grandparent_of(X,Y):-parent_of(X,Z),parent_of(Z,Y).

% cousin_of(X,Y):-grandparent_of(X,Z),grandparent_of(Y,Z),\+(X==Y),\+(sibling_of(X,Y)).

cousin_of(X,Y):-parent_of(X,P),sibling_of(P,AU),child_of(AU,Y).


uncle_of(X,Y):-parent_of(X,Parent),brother_of(Parent,Y).

aunt_of(X,Y):-parent_of(X,Parent),sister_of(Parent,Y).

ancestor_of(Child,Ancestor):-
  parent_of(Child,Ancestor).
ancestor_of(Child,Ancestor):-
  parent_of(Child,Parent),
  ancestor_of(Parent,Ancestor).



%  end

/*
[debug] 15 ?- trace,orphan(X).
   Call: (8) orphan(_G464) ?
   Call: (9) person(_G464) ?    Call: (10) gender_of(_G464, _L244) ?
   Exit: (10) gender_of('Bart', male) ?    Exit: (9) person('Bart') ?
   Call: (9) parent_of('Bart', _L268) ?    Exit: (9) parent_of('Bart', 'Homer') ?
   Redo: (10) gender_of(_G464, _L244) ?    Exit: (10) gender_of('Maggie', female) ?
   Exit: (9) person('Maggie') ?    Call: (9) parent_of('Maggie', _L268) ?
   Exit: (9) parent_of('Maggie', 'Homer') ?    Redo: (10) gender_of(_G464, _L244) ?
   Exit: (10) gender_of('Lisa', female) ?    Exit: (9) person('Lisa') ?
   Call: (9) parent_of('Lisa', _L268) ?    Exit: (9) parent_of('Lisa', 'Homer') ?
   Redo: (10) gender_of(_G464, _L244) ?    Exit: (10) gender_of('Marge', female) ?
   Exit: (9) person('Marge') ?    Call: (9) parent_of('Marge', _L268) ?
   Fail: (9) parent_of('Marge', _L268) ?    Exit: (8) orphan('Marge') ?
X = 'Marge'

*/