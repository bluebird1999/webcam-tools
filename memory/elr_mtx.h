#ifndef __ELR_MTX_H__
#define __ELR_MTX_H__

#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

typedef struct __elr_mtx
{
	pthread_mutex_t  _cs;/*!< the windows critical section object. */
}
elr_mtx;

/** platform independent atomic counter type. */
typedef struct {
    volatile int counter;
}atomic_t;

typedef atomic_t		elr_atomic_t;

/** platform independent counter integer type. */
typedef int            	elr_counter_t;

/** platform independent zero initial value of atomic counter type. */
#define   ELR_ATOMIC_ZERO     0

/*
** ԭ����������
*/
/*! \brief atomic increment operation.
 *  \param v pointer to a atomic counter type variable.
 *  \retval the integer value of v after increment.
 *
 *  make a increment of v by one atomicly.
 */
elr_counter_t elr_atomic_inc(elr_atomic_t* v);

/*
** ԭ���Լ�����
*/
/*! \brief atomic decrement operation.
 *  \param v pointer to a atomic counter type variable.
 *  \retval the integer value of v after decrement.
 *
 *  make a decrement of v by one atomicly.
 */
elr_counter_t elr_atomic_dec(elr_atomic_t* v);

/*
** ��ʼ�������壬����0��ʾ��ʼ��ʧ��
** �����ʼ��ʧ�ܾͲ���Ҫ�ٵ���elr_mtx_finalize
*/
/*! \brief initialize a mutex.
 *  \param mtx pointer to a mutex.
 *  \retval zero if failed.
 */
int  elr_mtx_init(elr_mtx *mtx);

/*! \brief waits on a mutex.
 *  \param mtx pointer to a mutex.
 */
void elr_mtx_lock (elr_mtx *mtx);

/*! \brief releases on a mutex.
 *  \param mtx pointer to a mutex.
 */
void elr_mtx_unlock(elr_mtx *mtx);

/*! \brief initialize a mutex.
 *  \param mtx pointer to a mutex.
 */
void elr_mtx_finalize(elr_mtx *mtx);

#endif
