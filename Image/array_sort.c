#include <stdio.h>

/* Min_array */

/*
 * array_min_index(array, len): returns the index of the min value of array
 */
size_t array_min_index(int array[], size_t t, size_t j)
{
    size_t sm_index = t;
    int smallest = array[t];

    for (size_t i = t; i < j; i++)
    {
        if (smallest > array[i])
        {
            smallest = array[i];
            sm_index = i;
        }
    }
    return sm_index;
}
/* Selection sort */

/*
 * array_select_sort(array, len): sort array using select sort
 */
void array_select_sort(int array[], size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        int min = array_min_index(array, i, len);
        int i_val = array[i];
        int j_val = array[min];
        array[min] = i_val;
        array[i] = j_val;
    }
}