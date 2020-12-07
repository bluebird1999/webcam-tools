#include "elr_mtx.h"

elr_counter_t elr_atomic_inc(elr_atomic_t* v)
{
	return __atomic_add_fetch( &v->counter, 1, __ATOMIC_SEQ_CST);
}

elr_counter_t elr_atomic_dec(elr_atomic_t* v)
{
	return __atomic_sub_fetch( &v->counter, 1, __ATOMIC_SEQ_CST);
}

/*
** ��ʼ�������壬����0��ʾ��ʼ��ʧ��
*/
int  elr_mtx_init(elr_mtx *mtx)
{
    if (pthread_mutex_init(&mtx->_cs, NULL) != 0){
            return 0;
    }
    return 1;
}

void elr_mtx_lock (elr_mtx *mtx)
{
	pthread_mutex_lock(&mtx->_cs);
}

void elr_mtx_unlock(elr_mtx *mtx)
{
	pthread_mutex_unlock(&mtx->_cs);
}

void elr_mtx_finalize(elr_mtx *mtx)
{
	pthread_mutex_destroy(&mtx->_cs);
}
