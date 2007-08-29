Direction decide(Position& me, Position& theGoal, unsigned long North, unsigned long East, unsigned long South, unsigned long West) {
    unsigned long cur_x = me.getx();
    unsigned long cur_y = me.gety();
    unsigned long goal_x = theGoal.getx();
    unsigned long goal_y = theGoal.gety();
    
    // The easiest solution is move toward the goal each time without concern about
    // distance. Of course this assumes that there are no zeros on the playing field.
    if (goal_x > cur_x)
        return EAST;
    if (goal_x < cur_x)
        return WEST;
    if (goal_y > cur_y)
        return SOUTH;
    if (goal_y < cur_y)
        return NORTH;
}