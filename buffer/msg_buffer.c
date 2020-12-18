/*
 * message.c
 *
 *  Created on: Sep 17, 2020
 *      Author: ning
 */


/*
 * header
 */
//system header
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>
//program header
#include "../../manager/manager_interface.h"
//server header
#include "msg_buffer.h"
#include "../log.h"

/*
 * static
 */
//variable

//function;

//specific
/*
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
/*
 * helper
 */
static int msg_buffer_swap_item(message_buffer_t *buff, int org, int dest)
{
	message_t temp;
	//swap
	msg_init(&temp);
	msg_deep_copy( &temp, &(buff->buffer[org]) );
	msg_free(&(buff->buffer[org]));
	msg_deep_copy( &(buff->buffer[org]), &(buff->buffer[dest]));
	msg_free(&(buff->buffer[dest]));
	msg_deep_copy( &(buff->buffer[dest]), &temp );
	msg_free(&temp);
	return 0;
}

/*
 * interface
 */
int msg_buffer_is_empty(message_buffer_t *buffer)
{
  return (buffer->head == buffer->tail);
}

int msg_buffer_is_full(message_buffer_t *buffer)
{
  return ((buffer->head - buffer->tail) & MSG_BUFFER_MASK) == MSG_BUFFER_MASK;
}

int msg_buffer_num_items(message_buffer_t *buffer)
{
  return ((buffer->head - buffer->tail) & MSG_BUFFER_MASK);
}


int msg_deep_copy(message_t *dest, message_t *source)
{
	unsigned char *block;
	if( dest->arg_size > 0 && dest->arg != NULL) {
		#ifdef MEMORY_POOL
			elr_mpl_free(dest->arg);
		#else
			free(dest->arg);
		#endif
	}
	if( dest->extra_size > 0 && dest->extra != NULL) {
		#ifdef MEMORY_POOL
			elr_mpl_free(dest->extra);
		#else
			free(dest->extra);
		#endif
	}
	memcpy( dest, source, sizeof(message_t));
	if(  source->arg_size > 0 && source->arg != NULL) {
		#ifdef MEMORY_POOL
			block = elr_mpl_alloc( &_pool_);
			memset(block,0,source->arg_size);
		#else
			block = calloc( source->arg_size, 1);
		#endif
//		log_qcy(DEBUG_INFO, "+++++++++++++++++++new allocation here size = %d", source->arg_size);
		if( block == NULL ) {
			log_qcy(DEBUG_SERIOUS, "!!!!!!!!!!!!!!!!!!!!!!!!!1buffer memory allocation failed for arg_size=%d!!!!!!!!!!!!!!!!!!!!!!!!!!!!", source->arg_size );
			log_qcy(DEBUG_SERIOUS, "msg from %d and message = %x", source->sender, source->message );
			return -1;
		}
		//deep structure copy
		dest->arg = block;
		memcpy( dest->arg, source->arg, source->arg_size );
	}
	block = NULL;
	if(  source->extra_size > 0 && source->extra != NULL) {
//		log_qcy(DEBUG_INFO, "+++++++++++++++++++new allocation here size = %d", source->extra_size);
		#ifdef MEMORY_POOL
			block = elr_mpl_alloc( &_pool_);
			memset(block,0,source->extra_size);
		#else
			block = calloc( source->extra_size, 1);
		#endif
		if( block == NULL ) {
			log_qcy(DEBUG_SERIOUS, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!buffer memory allocation failed for extra_size=%d!!!!!!!!!!!!!!!!!!!!!!!!!!!!", source->extra_size );
			log_qcy(DEBUG_SERIOUS, "msg from %d and message = %x", source->sender, source->message );
			return -1;
		}
		//deep structure copy
		dest->extra = block;
		memcpy( dest->extra, source->extra, source->extra_size );
	}
	return 0;
}

void msg_buffer_init(message_buffer_t *buff, int overflow)
{
	buff->head = 0;
	buff->tail = 0;
	buff->sequence = 0;
	buff->overflow = overflow;
	pthread_rwlock_init(&buff->lock, NULL);
	for(int i=0;i<MSG_BUFFER_SIZE;i++) {
		if( buff->buffer[i].arg != NULL && buff->buffer[i].arg_size>0) {
			#ifdef MEMORY_POOL
				elr_mpl_free( buff->buffer[i].arg );
			#else
				free( buff->buffer[i].arg );
			#endif
			buff->buffer[i].arg = NULL;
			buff->buffer[i].arg_size = 0;
		}
		if( buff->buffer[i].extra != NULL && buff->buffer[i].extra_size>0) {
			#ifdef MEMORY_POOL
				elr_mpl_free( buff->buffer[i].extra );
			#else
				free( buff->buffer[i].extra );
			#endif
			buff->buffer[i].extra = NULL;
			buff->buffer[i].extra_size = 0;
		}
	}
	buff->init = 1;
}

void msg_buffer_init2(message_buffer_t *buff, int overflow, pthread_mutex_t *mutex)
{
	pthread_mutex_lock(mutex);
	msg_buffer_init(buff,overflow);
	pthread_mutex_unlock(mutex);
}


void msg_buffer_release(message_buffer_t *buff)
{
	int i;
	buff->head = 0;
	buff->tail = 0;
	buff->sequence = 0;
	pthread_rwlock_destroy(&buff->lock);
	for(i=0;i<MSG_BUFFER_SIZE;i++) {
		if( buff->buffer[i].arg != NULL && buff->buffer[i].arg_size>0) {
			#ifdef MEMORY_POOL
				elr_mpl_free( buff->buffer[i].arg );
			#else
				free( buff->buffer[i].arg );
			#endif
			buff->buffer[i].arg = NULL;
			buff->buffer[i].arg_size = 0;
		}
		if( buff->buffer[i].extra != NULL && buff->buffer[i].extra_size>0) {
			#ifdef MEMORY_POOL
				elr_mpl_free( buff->buffer[i].extra );
			#else
				free( buff->buffer[i].extra );
			#endif
			buff->buffer[i].extra = NULL;
			buff->buffer[i].extra_size = 0;
		}
	}
	buff->init = 0;
}

void msg_buffer_release2(message_buffer_t *buff, pthread_mutex_t *mutex)
{
	int i;
	pthread_mutex_lock(mutex);
	msg_buffer_release(buff);
	pthread_mutex_unlock(mutex);
}

int msg_buffer_probe_item(message_buffer_t *buff, int n, message_t *msg)
{
	int index;
	if( msg_buffer_is_empty(buff) ) {
		return 1;
	}
	index = ((buff->tail + n) & MSG_BUFFER_MASK);
	if(  index == buff->head ) {
		return 1;
	}
	memcpy(msg, &(buff->buffer[index]), sizeof(message_t));
	return 0;
}

int msg_buffer_probe_item_extra(message_buffer_t *buff, int n, int *id, void**arg)
{
	int index;
	if( msg_buffer_is_empty(buff) ) {
		return 1;
	}
	index = ((buff->tail + n) & MSG_BUFFER_MASK);
	if(  index == buff->head ) {
		return 1;
	}
	*id = buff->buffer[index].message;
	*arg = buff->buffer[index].arg_in.handler;
	return 0;
}

int msg_buffer_swap(message_buffer_t *buff, int org, int dest)
{
	int org_index, dest_index, rundown_index, last_index;
	message_t temp;
	if( org == dest) return 1;
	org_index = ((buff->tail + org) & MSG_BUFFER_MASK);
	dest_index = ((buff->tail + dest) & MSG_BUFFER_MASK);
	//swap from the queue top
	msg_buffer_swap_item(buff, org_index, dest_index);
	//search upward till the top is found
	last_index = dest_index;
	rundown_index = ( (last_index - 1) & MSG_BUFFER_MASK);
	while( rundown_index != org_index ) {
		msg_buffer_swap_item(buff, rundown_index, last_index);
		last_index = rundown_index;
		rundown_index = ( (last_index - 1) & MSG_BUFFER_MASK);
	}
	return 0;
}

int msg_buffer_pop(message_buffer_t *buff, message_t *data )
{
	if( msg_buffer_is_empty(buff) ) {
		return 1;
	}
	//copy message
	msg_deep_copy( data, &(buff->buffer[buff->tail]));
	// free the memory allocated in the push process
	if( buff->buffer[buff->tail].arg_size > 0 && buff->buffer[buff->tail].arg != NULL) {
		#ifdef MEMORY_POOL
			elr_mpl_free( buff->buffer[buff->tail].arg );
		#else
			free( buff->buffer[buff->tail].arg );
		#endif
		buff->buffer[buff->tail].arg = NULL;
		buff->buffer[buff->tail].arg_size = 0;
	}
	if( buff->buffer[buff->tail].extra_size > 0 && buff->buffer[buff->tail].extra != NULL) {
		#ifdef MEMORY_POOL
			elr_mpl_free( buff->buffer[buff->tail].extra );
		#else
			free( buff->buffer[buff->tail].extra );
		#endif
		buff->buffer[buff->tail].extra = NULL;
		buff->buffer[buff->tail].extra_size = 0;
	}
	//update tail
	buff->tail = ((buff->tail + 1) & MSG_BUFFER_MASK);
	return 0;
}

int msg_buffer_push(message_buffer_t *buff, message_t *data)
{
	if( data == NULL) {
		return -1;
	}
	//check and replace oldest item
	if( msg_buffer_is_full(buff) ) {
		if( buff->overflow == MSG_BUFFER_OVERFLOW_YES) {
			//free old data
			if( buff->buffer[buff->tail].arg != NULL && buff->buffer[buff->tail].arg_size>0) {
				#ifdef MEMORY_POOL
					elr_mpl_free( buff->buffer[buff->tail].arg );
				#else
					free( buff->buffer[buff->tail].arg );
				#endif
				buff->buffer[buff->tail].arg = NULL;
				buff->buffer[buff->tail].arg_size = 0;
			}
			if( buff->buffer[buff->tail].extra != NULL && buff->buffer[buff->tail].extra_size>0) {
				#ifdef MEMORY_POOL
					elr_mpl_free( buff->buffer[buff->tail].extra );
				#else
					free( buff->buffer[buff->tail].extra );
				#endif
				buff->buffer[buff->tail].extra = NULL;
				buff->buffer[buff->tail].extra_size = 0;
			}
			buff->tail = ((buff->tail + 1) & MSG_BUFFER_MASK);
		}
		else {
			return 1;
		}
	}
	buff->sequence++;
	//data copy
	msg_deep_copy( &(buff->buffer[buff->head]), data );
	buff->buffer[buff->head].sequence = buff->sequence;
	//renew head
	buff->head = ((buff->head + 1) & MSG_BUFFER_MASK);
	return 0;
}

int msg_init(message_t *data)
{
	memset(data, 0, sizeof(message_t));
}

int msg_free(message_t *data)
{
	if( data == NULL )
		return -1;
	if( data->arg!=NULL && data->arg_size>0 ) {
		#ifdef MEMORY_POOL
			elr_mpl_free(data->arg);
		#else
			free(data->arg);
		#endif
		data->arg = NULL;
		data->arg_size = 0;
	}
	if( data->extra!=NULL && data->extra_size>0) {
		#ifdef MEMORY_POOL
			elr_mpl_free(data->extra);
		#else
			free(data->extra);
		#endif
		data->extra = NULL;
		data->extra_size = 0;
	}
	return 0;
}

int msg_is_system(int id)
{
	if( (id & 0xFFF0) == 0 )
		return 1;
	else
		return 0;
}

int msg_is_response(int id)
{
	if( (id & 0xF000) )
		return 1;
	else
		return 0;
}
