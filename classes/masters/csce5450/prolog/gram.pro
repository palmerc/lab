% axiom + nonterminals

:-op(900,xfx,(=>)).

s(sent=>(NG,V))-->ng(NG),v(V).

ng(ngroup=>PN)-->pn(PN).
ng(ngroup=>(Art,CN))-->art(Art),cn(CN).

% terminals

v(verb=walk:p3s)-->[walks].
v(verb=fly:p3s)-->[flies].

pn(pn=>'Joe')-->['Joe'].
pn(pn=>'Mary')-->['Mary'].

art(art=>a)-->[a].
art(art=>the)-->[the].

cn(cn=>dogg)-->[dog].
cn(cn=>cat)-->[cat].

go:- s(Sem,Sent,[]),write(Sent=>Sem),nl,fail.
go:- write(done),nl.


