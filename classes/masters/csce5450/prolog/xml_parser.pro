to_cat(File,Cat):-
  new_cat(Cat),
  parse_xml(File,DOC),
  build_cat(DOC,Cat).

build_cat(xml_doc(_Header,Body),Cat):- 
  Body=node(cat,_,[OX,MX]),
  OX=node(objects,_,OY),
  MX=node(morphisms,_,MY),
  build_cat_obs(OY,Cat),
  build_cat_ms(MY,Cat).

build_cat_obs(OY,Cat):-foreach(member(node(object,_,Os),OY),build_cat_ob(Os,Cat)).
build_cat_ms(MY,Cat):-foreach(member(node(morphism,_,Ms),MY),build_cat_m(Ms,Cat)).

build_cat_ob(Os,Cat):-map(extract_field,Os,[Node,Attr,Val]),set_prop(Cat,Node,Attr,Val).
build_cat_m(Ms,Cat):-map(extract_field,Ms,[From,To,Attr,Val]),set_morphism(Cat,From,To,Attr,Val).

extract_field(node(_Name,_,[data(Xs)]),X):-process_field(Xs,X).

process_field([],X):-!,X='$empty'.
process_field([D],X):-atom(D),!,X=D.
process_field([D],X):-number(D),!,X=D.
process_field(Ds,X):-make_cmd(Ds,X).

parse_xml(File):-
  parse_xml(File,Tree),
  show_xml(Tree).

parse_xml(File,Tree):-
  findall(T,xml_token_of(File,T),Ts),
  % println(parsing_xml_tokens_list_from(File)),
  doc(Tree,Ts,[]).

xml_token_of(File,T):-name(NL,[10]),token_of(File,T),T\==NL.
   
doc(xml_doc(H,E))-->headers(H),element(E).

headers([header(Is)|Hs])-->['<','?'],!,header_items(Is),headers(Hs).
headers([comment(Is)|Hs])-->['<','!'],!,comment_items(Is),headers(Hs).
headers([])-->[].

header_items([])-->['?','>'],!.
header_items([X|Xs])-->[X],header_items(Xs).

comment_items([])-->['>'],!.
comment_items([X|Xs])-->[X],comment_items(Xs).

atomize([Name|_],Name):-!.
atomize(Name,Name).

xml_filter(Ns,_Node,NewNode):-
  atomize(Ns,Name),
  call_ifdef(xml_ignore(Name),fail),
  !,
  NewNode=ignored(Name).
xml_filter(_Name,Node,Node).

element(Node)-->
  % start_tag(Name,Atts,End),
  ['<'],
  !,
  xml_name(Name),
  atts(Atts,End),
  maybe_items(End,Is,Name),
  {xml_filter(Name,node(Name,Atts,Is),Node)}.
element(data(Ds))-->
  data_bloc(As),
  {xml_filter_data(As,Ds)}.

xml_filter_data(Ds,Ts):-
  call_ifdef(xml_transform(Ds,Ts),fail),
  !.
xml_filter_data(Ds,Ds).

data_bloc([D|Ds])-->data_element(D),!,data_bloc(Ds).
data_bloc([])-->[].

data_element(X)-->[X],{ \+(member(X,['<','>'])) }.

maybe_items(end,[],_)-->[].
maybe_items(more,Is,Name)-->items(Is,Name).

end_tag(_)-->['/','>'].
end_tag(Name)-->['<','/'],xml_name(Name),['>'].

items([],Name)-->end_tag(Name),!.
items(NewIs,Name)-->element(I),items(Is,Name),{add_element(I,Is,NewIs)}.

add_element(I,Is,[I|Is]).

atts([],end)-->['/','>'],!.
atts([],more)-->['>'],!.
atts(NewXs,End)-->
  xml_name(A,['=']),
  ['='],
  one_att(B),
  atts(Xs,End),
  {xml_filter_att(A=B,Xs,NewXs)}.

xml_filter_att(Ns=_,As,As):-
  atomize(Ns,Name),
  call_ifdef(xml_ignore_att(Name),fail),
  !.
xml_filter_att(A,As,[A|As]).

one_att(B)-->['"'],!,xml_names(B,['"']),['"'].
one_att(B)-->xml_name(B,['/','>']).

xml_names([B|Bs],End)-->xml_name(B,End),!,xml_names(Bs,End).
xml_names([],_)-->[].

xml_name(Xs)-->xml_name(Xs,['/','>']).

xml_name(Xs,End)-->xml_name0(Ns,End),{unlistify(Ns,Xs)}.

% unlistify([[N]],Xs):-!,Xs=N.
unlistify([N],Xs):-!,Xs=N.
unlistify(Ns,Ns).

xml_name0([X,Op|Xs],End)-->
   name_element(X,End),
   [Op],
   {member(Op,[':','-','.'])},
   !,
   xml_name0(Xs,End).
xml_name0([X],End)-->
  name_element(X,End).

name_element(X,End)-->[X],{ \+(member(X,End)) }.

% display an XML tree

show_xml(xml_doc(Hs,E)):-
  println('xml_doc:'),
  nl,
  foreach(
    member(H,Hs),
    show_header(H)
  ),
  nl,
  show_xml(E,2).

show_header(H):-tab(1),println(H).

show_xml(node(Name,As,Is),Level):-
  Next is Level+1,
  show_name(Level,Name,As),
  show_items(Is,Next).
show_xml(data(D),Level):-
  tab(Level),println(D).

show_name(Level,Name,As):-
  L is Level+1,
  tab(Level),write(Name),write(':'),nl,
  foreach(
    member(A,As),
    show_att(L,A)
  ),
  nl.
  
show_att(L,Ks=Vs):-!,
  tab(L),
  write(Ks),write('='),nl,
  L1 is L+1,
  member(V,Vs),
    tab(L1),write(V),nl,
  fail
;true.
show_att(L,A):-
  tab(L),
  println(A).
    
show_items([],_).
show_items([I|Is],Level):-
  show_xml(I,Level),
  show_items(Is,Level).
