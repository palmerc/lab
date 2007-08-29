Direction decide(Position& me, Position& theGoal, unsigned long North, unsigned long East, unsigned long South, unsigned long West) {
    unsigned long cur_x = me.getx();
    unsigned long cur_y = me.gety();
    unsigned long goal_x = theGoal.getx();
    unsigned long goal_y = theGoal.gety();
    unsigned long dir_x, dir_y;
    // Which directions are toward the goal?
    if (goal_x > cur_x)
        dir_x = East;
    else if (goal_x < cur_x)
        dir_x = West;
    else
        dir_x = 0;
    
    if (goal_y > cur_y)
        dir_y = South;
    else if (goal_y < cur_y)
        dir_y = North;
    else
        dir_y = 0;
        
    // Go toward X if less than or equal to Y and not zero
    if ( (dir_x < dir_y) && (dir_x != 0) ) {
        if ( East == dir_x ) {
            return EAST;
        } else if ( West == dir_x ) {
            return WEST;
        }
    // Got to Y if less than X and not zero
    } else if ( (dir_y < dir_x) && (dir_y != 0) ) {
        if ( North == dir_y ) {
            return NORTH;
        } else if ( South == dir_y ) {
            return SOUTH;
        }
    // if one is zero and the other isn't go that way
    } else if ( (dir_x == 0 && dir_y > 0) || (dir_y == 0 && dir_x > 0) ) {
        if (dir_x > 0) {
            if ( East == dir_x ) {
                return EAST;
            } else if ( West == dir_x ) {
                return WEST;
            }
        }
        if (dir_y > 0) {
            if ( North == dir_y ) {
                return NORTH;
            } else if ( South == dir_y ) {
                return SOUTH;
            }
        }
    } else if ( (goal_x - cur_x) == 1 )
        return EAST;
    else if ( (cur_x - goal_x) == 1 )
        return WEST;
    else if ( (cur_y - goal_y) == 1 )
        return NORTH;
    else if ( (goal_y - cur_y) == 1 )
        return SOUTH;
}
