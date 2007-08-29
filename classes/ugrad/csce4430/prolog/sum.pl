sum([], 0).  
sum([First | Rest], Total) :-
	sum(Rest, TotalOfRest),
        Total is First + TotalOfRest.