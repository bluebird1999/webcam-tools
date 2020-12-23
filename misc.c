/*
 * misc.c
 *
 *  Created on: Aug 27, 2020
 *      Author: ning
 */


/*
 * header
 */
//system header
#include <stdio.h>
#include <pthread.h>
#include <syscall.h>
#include <sys/prctl.h>
#include <sys/time.h>
#include <malloc.h>
//program header
#include "../manager/manager_interface.h"
//server header
#include "misc.h"
#include "log.h"
/*
 * static
 */
//variable

//function;



/*
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */

int misc_generate_random_id(void)
{
	struct timeval t1 = {0, 0};
    int value = 0;
    int max = 20000;
    int min = 10000;

    unsigned int srandVal;
    gettimeofday(&t1, NULL);
    srandVal = t1.tv_sec + t1.tv_usec;
	srand( srandVal );

	//[10000, 20000]
	value = rand() % (max + 1 - min) + min;

	return value;
}

void misc_set_thread_name(char *name)
{
    prctl(PR_SET_NAME, (unsigned long)name, 0,0,0);
    pid_t tid;
    tid = syscall(SYS_gettid);
    log_qcy(DEBUG_INFO, "set pthread name:%d, %s, %s pid:%d tid:%d\n", __LINE__, __func__, name, getpid(), tid);

}

int misc_get_bit(int a, int bit)
{
	return ( a >> bit) & 0x01;
}

int misc_set_bit(int *a, int bit, int value)
{
	*a &= ~(0x1<<bit);
	*a |= value<<bit;
}

int misc_clear_bit(int *a, int bit)
{
	*a &= ~(0x1<<bit);
}

int misc_full_bit(int a, int num)
{
	if( a == ((1<<num) -1) )
		return 1;
	else
		return 0;
}


int misc_substr(char dst[], char src[], int start, int len)
{
    int i;
    for(i=0;i<len;i++) {
        dst[i]=src[start+i];
    }
    dst[i]='\0';
    return i;
}

int misc_mips_address_check(unsigned int address)
{
	if( address == 0)
		return 1;
/*	else if( (address | 0x3) )
		return 1;
		*/
	else if( (address > 0x80000000) )
		return 1;
	else if( (address < 0x1000) )
		return 1;
	else
		return 0;
}
