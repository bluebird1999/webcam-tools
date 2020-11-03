/*
 * log.h
 *
 *  Created on: Aug 15, 2020
 *      Author: ning
 */

#ifndef TOOLS_LOG_H_
#define TOOLS_LOG_H_

/*
 * header
 */

/*
 * define
 */
#define log_debug(S,...)      printf("\n[%s-%s-%d]"S"\n",__FILE__,__func__,__LINE__, ##__VA_ARGS__)
#define log_info(S,...)      printf("\n[%s-%s-%d]"S"\n",__FILE__,__func__,__LINE__, ##__VA_ARGS__)
#define log_err(S,...)       printf("\n[%s-%s-%d]"S"\n",__FILE__,__func__,__LINE__, ##__VA_ARGS__)
#define log_warning(S,...)     printf("\n[%s-%s-%d]"S"\n",__FILE__,__func__,__LINE__, ##__VA_ARGS__)

#define log_qcy(level, S, ...)	log_new(__FILE__, __LINE__, level, S, ## __VA_ARGS__)

/*
 * structure
 */

/*
 * function
 */
int log_new(char* file, int line, int level, const char* format, ...);


#endif /* TOOLS_LOG_H_ */
