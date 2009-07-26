% axiom + nonterminals

s-->ng,v.

ng-->pn.
ng-->art,cn.

% terminals

v-->[walks].
v-->[flies].

pn-->['Joe'].
pn-->['Mary'].

art-->[a].
art-->[the].

cn-->[dog].
cn-->[cat].
cn-->[pig].
cn-->[bird].

go:- s(Sent,[]),write(Sent),nl,fail.
go:- write(done),nl.


