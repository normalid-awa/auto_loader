#include "vars.h"

int32_t max = 0;
int32_t current = 0;

int32_t get_var_max()
{
    return max;
}
void set_var_max(int32_t value)
{
    max = value;
}
int32_t get_var_current()
{
    return current;
}
void set_var_current(int32_t value)
{
    current = value;
}
