/*
 * h264.h
 *
 *  Created on: Oct 7, 2020
 *      Author: ning
 */

#ifndef TOOLS_H264_H_
#define TOOLS_H264_H_

/*
 * header
 */

/*
 * define
 */

/*
 * structure
 */
typedef struct nalu_unit_t
{
  int 	type;
  int 	size;
  unsigned char *data;
} nalu_unit_t;

/*
 * function
 */
int h264_is_iframe(char *p);
int h264_is_pframe(char *p);
int h264_read_nalu(unsigned char *pPack, unsigned int nPackLen, unsigned int offSet, nalu_unit_t *pnalu_unit);

#endif /* TOOLS_H264_H_ */
