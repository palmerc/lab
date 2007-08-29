sum([], 0).  
sum([First | Rest], Total) :-
	sum(Rest, TotalOfRest),
        Total is First + TotalOfRest.
list_length([], 0).
list_length([_ | Tail], Count) :-
	list_length(Tail, Counter),
	Count is Counter + 1.
mean([], 0) .
mean(Tail, Mean) :-
	sum(Tail, Sum),
	list_length(Tail, Count),
	Mean is Sum / Count.
