#ifndef _HEXDUMP_H
#define _HEXDUMP_H

#include <stddef.h>

/*
 * Prints a buffer as a hexdump to stderr. For debugging purposes.
 * 
 * \param buffer A pointer to the data to be printed
 * \param len The length in bytes of the data to be printed
 */
void hexdump(const void *buffer, ssize_t len);

/*
 * Formats data in a buffer as a hexdump. The function returns a pointer to
 * an allocated string, containing the formatted hexdump.
 * 
 * \param buffer A pointer to the data to be printed
 * \param len The length in bytes of the data to be printed
 */
char * hexdump_str(const void *buffer, ssize_t len);

#endif
