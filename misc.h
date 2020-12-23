/*
 * misc.h
 *
 *  Created on: Aug 27, 2020
 *      Author: ning
 */

#ifndef TOOLS_MISC_H_
#define TOOLS_MISC_H_

/*
 * header
 */

/*
 * define
 */

/*
 * structure
 */

/*
 * function
 */
int misc_generate_random_id(void);
void misc_set_thread_name(char *name);
int misc_get_bit(int a, int bit);
int misc_set_bit(int *a, int bit, int value);
int misc_clear_bit(int *a, int bit);
int misc_full_bit(int a, int num);
int misc_substr(char *dst, char *src, int start, int len);
int misc_mips_address_check(unsigned int address);

#endif /* TOOLS_MISC_H_ */
