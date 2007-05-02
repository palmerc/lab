size([], 0).
size([First | Rest], Total) :-
	size(Rest, Count),
	Total is Count+Count.