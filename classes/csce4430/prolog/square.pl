square_4([], []).  
square_4([First | Rest], [FirstSquared | SquareRest]) :-
        FirstSquared is First * First,
        square_4(Rest, SquareRest).