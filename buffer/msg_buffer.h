/*
 * message.h
 *
 *  Created on: Sep 17, 2020
 *      Author: ning
 */

#ifndef TOOLS_BUFFER_MESSAGE_BUFFER_H_
#define TOOLS_BUFFER_MESSAGE_BUFFER_H_

/*
 * header
 */

/*
 * define
 */
#define		MSG_BUFFER_SIZE					32
#define 	MSG_BUFFER_MASK 				(MSG_BUFFER_SIZE-1)

#define		MSG_BUFFER_OVERFLOW_NO			0		//neglect new data when overflow
#define		MSG_BUFFER_OVERFLOW_YES			1		//overwrite old data when overflow

typedef void (*HAND)(void);

/*
 * message buffer
 */
typedef struct message_arg_t {
	int			dog;
	int 		cat;
	int			duck;
	int			chick;
	int			tiger;
	int			wolf;
	void		*handler;	//pure pointer or function pointer, no further memory management
} message_arg_t;

typedef struct message_t {
	int				message;		//message id
	int				sender;
	int				receiver;
	int				result;
	message_arg_t	arg_in;
	message_arg_t	arg_pass;
	int				arg_size;
	void			*arg;
	int				extra_size;
	void			*extra;
	unsigned int	sequence;
} message_t;

typedef struct message_buffer_t {
	message_t			buffer[MSG_BUFFER_SIZE];
	int					head;
	int					tail;
	int					init;
	int					overflow;
	unsigned int		sequence;
	pthread_rwlock_t	lock;		//derpecated, use the static one
} message_buffer_t;

/*
 * function
 */
int msg_buffer_pop(message_buffer_t *buff, message_t *data );
int msg_buffer_push(message_buffer_t *buff, message_t *data);
void msg_buffer_init(message_buffer_t *buff, int overflow);
void msg_buffer_init2(message_buffer_t *buff, int overflow, pthread_mutex_t *mutex);
void msg_buffer_release(message_buffer_t *buff);
void msg_buffer_release2(message_buffer_t *buff, pthread_mutex_t *mutex);
int msg_init(message_t *data);
int msg_free(message_t *data);
int msg_deep_copy(message_t *dest, message_t *source);
int msg_buffer_is_empty(message_buffer_t *buffer);
int msg_buffer_is_full(message_buffer_t *buffer);
int msg_buffer_num_items(message_buffer_t *buffer);
int msg_buffer_probe_item(message_buffer_t *buff, int n, message_t *msg);
int msg_buffer_probe_item_extra(message_buffer_t *buff, int n, int *id, void**arg);
int msg_buffer_swap(message_buffer_t *buff, int org, int dest);
int msg_is_system(int id);
int msg_is_response(int id);

#endif /* TOOLS_BUFFER_MESSAGE_BUFFER_H_ */
