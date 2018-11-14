#ifndef SORTBYANGLE_H
#define SORTBYANGLE_H

#include "algorithms.h"

class sortbyangle
{
public:
    sortbyangle();

    bool operator()(vec_angle &t, vec_angle &t1)
    {
        if(fabs(t.a-t1.a) < 10e-6)
        {
            if(t.d < t1.d)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        else if(t.a < t1.a)
        {
            return 1;
        }

        else
        {
            return 0;
        }
    }
};

#endif // SORTBYANGLE_H
