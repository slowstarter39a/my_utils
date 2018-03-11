/*
 * =====================================================================================
 *
 *       Filename:  ring_buffer.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/11/2018 01:06:44 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *        Company:
 *
 * =====================================================================================
 */

/* capacity = size -1  */
/* full => ((wr_ptr + 1) % size) == rd_ptr */
/*  empty => wr_ptr == rd_ptr */

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char byte;
typedef unsigned char U8;
typedef unsigned int U32;
typedef unsigned long long U64;
typedef unsigned char BOOL;

typedef struct
{
	byte *buf_ptr;
	U32 wr_ptr;
	U32 rd_ptr;
	U32 size;

}ring_buff_t;

U32 ring_buff_init(ring_buff_t *rb, int size) ;
BOOL ring_buff_is_empty(ring_buff_t *rb);
BOOL ring_buff_is_full(ring_buff_t *rb);
U32 ring_buff_get_size(ring_buff_t *rb);
U32 ring_buff_get_capacity(ring_buff_t *rb);
U32 ring_buff_get_free_space(ring_buff_t *rb);
U32 ring_buff_get_used_space(ring_buff_t *rb);
U32 ring_buff_get_right_used_space(ring_buff_t *rb);
U32 ring_buff_get_right_free_space(ring_buff_t *rb);
U32 ring_buff_get_data(ring_buff_t *rb, byte *to, int count);
U32 ring_buff_put_data(ring_buff_t *rb, byte *from, int count);
U32 ring_buff_move_wr_ptr(ring_buff_t *rb, int count) ;
U32 ring_buff_move_rd_ptr(ring_buff_t *rb, int count) ;
U32 ring_buff_get_wr_ptr(ring_buff_t *rb) ;
U32 ring_buff_get_rd_ptr(ring_buff_t *rb) ;
U32 ring_buff_get_wr_addr(ring_buff_t *rb) ;
U32 ring_buff_get_rd_addr(ring_buff_t *rb) ;

#ifdef __cplusplus
}
#endif
