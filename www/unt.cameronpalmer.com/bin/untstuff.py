"""
Helpers for doing stuff the "UNT Way" (i.e. weird)
"""

__author__ = "Tim Hatch"
__copyright__ = "Copyright 2005, Tim Hatch"
__version__ = "$Rev$"
__license__ = "BSD"


__valid_semesters = {"spring": 1, "summer": 3, "fall": 8}
__rev_semesters = dict([(v,k) for (k,v) in __valid_semesters.items()])

def semester(year, s):
    """
    Come up with the silly EIS code based on a human readable representation.
    You can specify either a string or an int for year, though int would be
    better.
    
    >>> semester(2005, "spring")
    '1051'
    >>> semester(2006, "Fall")
    '1068'
    >>> semester("2005", "Summer")
    '1053'
    """
    x = s.lower().strip()
    assert(x in __valid_semesters)
    return "%03d%d" % (int(year) - 1900,  __valid_semesters[x])

def reverse_semester(code):
    """
    Comes up with a human-readable tuple given a silly EIS code.  Uses an int
    for year.
    
    >>> reverse_semester(1051)
    (2005, 'spring')
    >>> reverse_semester(1068)
    (2006, 'fall')
    """
    x = str(code)
    assert(len(x) == 4)
    year = int(x[:3]) + 1900
    sem = int(x[3])
    assert(sem in __rev_semesters)
    return (year, __rev_semesters[sem])

def normalize_semester(tup):
    """
    Given a semester tuple, make it into a more "standard" one, suitable for
    equality checks
    
    >>> normalize_semester((2005, "spring"))
    (2005, 'spring')
    >>> normalize_semester(("2005", "SPRIng"))
    (2005, 'spring')
    """
    return (int(tup[0]), str(tup[1]).lower().strip())
    
def _test():
    from doctest import testmod
    testmod()
    
if __name__ == "__main__":
    _test()
