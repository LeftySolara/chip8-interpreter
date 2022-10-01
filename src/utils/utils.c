/**
 * @file utils.c
 * @author Julianne Adams <julianne@julianneadams.info>
 * @brief Utility functions
 * @version 0.1
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "utils.h"

/**
 * @brief Check if the system is big or little endian.
 *
 * @return int 1 if the system is little endian, or 0 if it is big endian.
 */
int is_little_endian()
{
    int n = 1;
    return (*(char *)&n == 1);
}
