#include "gradient.h"
int comp(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}
path_point find_highest_point()
{
    int x = 5;
    int y = 5;
    int step_count = 0;
    float view[VIEW_SIZE][VIEW_SIZE];

    float step_factor = 0.2f; // multiply the first jump by this much to not fly past the range

    int prev_jump_x = 0; // store the previous amount we jumped in the x direction
    int prev_jump_y = 0; // store the previous amount we jumped in the x direction


    int short_jump = 0; //boolean for if the next jump will be long or short

    int prev_plateau = 0; // amount of the previous consecutive jumps contained plateaus

    while (1)
    {
        int outside_count = 0; // amount of -1 values
        int highest_is_plateau = 0; //boolean
        if (step_count > 500) break;
        float highest = 0;
        int highest_x = 0;
        int highest_y = 0;
        int detect_plateau = 0; // boolean
        generate_view(view, y, x);
        float x_weight = 0; // how much to jump in the x direction
        float y_weight = 0; // how much to jump in the y direction
        int outside_x = 0; // -1s based on x
        int outside_y = 0; // -1s based on y

        int plateau_count= 0;

        for (int i = 0; i < VIEW_SIZE;i++)
        {
            for (int j = 0; j<VIEW_SIZE; j++)
            {
                if (view[i][j] == view[i][j+1])
                {
                    plateau_count++;
                    if (view[i][j] == highest)
                    {
                        highest_is_plateau = 1;
                    }
                }
                if (plateau_count > 1)
                {
                    if (view[i][j] == highest)
                    {
                        detect_plateau = 1;
                    }
                }
                if (view[i][j] > highest)
                {
                    highest = view[i][j];
                    highest_x = j;
                    highest_y = i;
                    highest_is_plateau = 0;
                }

                if (i > VIEW_SIZE/2)
                {
                    y_weight += view[i][j];
                }

                if (j > VIEW_SIZE/2)
                {
                    x_weight += view[i][j];
                }
                if (i < VIEW_SIZE/2)
                {
                    y_weight -= view[i][j];
                }

                if (j < VIEW_SIZE/2)
                {
                    x_weight -= view[i][j];
                }
                if (view[i][j] == -1)
                {
                    outside_count++;
                    if (j < VIEW_SIZE/2)
                    {
                        outside_x--;
                    }
                    if (j > VIEW_SIZE/2)
                    {
                        outside_x++;
                    }
                    if (i < VIEW_SIZE/2)
                    {
                        outside_y--;
                    }
                    if (i > VIEW_SIZE/2)
                    {
                        outside_y++;
                    }
                }


            }
        }

        if (highest_x < VIEW_SIZE-1 && highest_y < VIEW_SIZE-1 && highest_x > 0 && highest_y > 0 && !highest_is_plateau)
        {
            x += highest_x - VIEW_SIZE/2;
            y += highest_y - VIEW_SIZE/2;


            if (declare_peak(x,y))
            {
                path_point ret = {x,y};
                return ret;
            }
        }


        if (!(highest_x < VIEW_SIZE-1 && highest_y < VIEW_SIZE-1 && highest_x > 0 && highest_y > 0) && plateau_count > 0)
        {
            detect_plateau = 1;
        }

        if (outside_count == VIEW_SIZE * VIEW_SIZE)
        {
            x += -prev_jump_x/2;
            y += -prev_jump_y/2;
            step_factor /= 2;
            step_count++;
            continue;
        }

        if (outside_count > 0)
        {
            x -= outside_x;
            y -= outside_y;
            step_count++;
            continue;
        }

        if (detect_plateau)
        {
            if (prev_jump_x > VIEW_SIZE/2)
            {
                x -= VIEW_SIZE/2;
            }
            if (prev_jump_x < -VIEW_SIZE/2)
            {
                x += VIEW_SIZE/2;
            }
            if (prev_jump_y > VIEW_SIZE/2)
            {
                y -= VIEW_SIZE/2;
                step_count++;
                continue;
            }
            if (prev_jump_y < -VIEW_SIZE/2)
            {
                y += VIEW_SIZE/2;
                step_count++;
                continue;
            }
            if (prev_jump_x == 0 && prev_jump_y == 0)
            {
                x += VIEW_SIZE/2;
                y += VIEW_SIZE/2;
                prev_jump_x = VIEW_SIZE/2;
                prev_jump_y = VIEW_SIZE/2;
            }else if (prev_jump_x < 0 && (prev_plateau == 0 || prev_jump_y == 0))
            {
                x -= VIEW_SIZE/2;
                prev_jump_x = -VIEW_SIZE/2;
            }if (prev_jump_y < 0)
            {
                y -= VIEW_SIZE/2;
                prev_jump_y = -VIEW_SIZE/2;
            }
            if (prev_jump_x > 0 && (prev_plateau == 0 || prev_jump_y == 0))
            {
                x += VIEW_SIZE/2;
                prev_jump_x = VIEW_SIZE/2;
            }if (prev_jump_y > 0)
            {
                y += VIEW_SIZE/2;
                prev_jump_y = VIEW_SIZE/2;
            }
            prev_plateau++;
            step_count++;
            continue;
        }



        if (short_jump)
        {
            if ((int)y_weight == 0)
            {
                y += 0;
            }
            if ((int)x_weight == 0)
            {
                x += 0;
            }
            if ((int)x_weight < 0)
            {
                x -= VIEW_SIZE / 2;
                prev_jump_x = -VIEW_SIZE/2;
            }
            if ((int)x_weight > 0)
            {
                x += VIEW_SIZE / 2;
                prev_jump_x = VIEW_SIZE/2;
            }
            if ((int)y_weight < 0)
            {
                y -= VIEW_SIZE / 2;
                prev_jump_y = -VIEW_SIZE/2;
            }
            if ((int)y_weight > 0)
            {
                y += VIEW_SIZE / 2;
                prev_jump_y = VIEW_SIZE/2;
            }

            step_count++;
            continue;
        }

        int x_step = step_factor * x_weight;
        int y_step = step_factor * y_weight;

        if (x_step > 95) x_step = 95;
        if (y_step > 95) y_step = 95;
        x += x_step;
        y += y_step;

        prev_jump_x = x_step;
        prev_jump_y = y_step;

        short_jump = 1;

        prev_plateau = 0;

        step_factor /= 2;

        step_count++;
    }
}