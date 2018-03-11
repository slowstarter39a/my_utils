/*
 * =====================================================================================
 *
 *       Filename:  ring_buffer.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/10/2018 10:40:27 PM
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

//#include <iostream>
#include <string.h>
#include "ring_buffer.h"
#include <stdlib.h>


U32 ring_buff_init(ring_buff_t *rb, int size)
{
	if(rb != NULL && size > 0)
	{
		rb->buf_ptr = (byte*)malloc(size);
		memset(rb->buf_ptr, 0x0, size);
		rb->wr_ptr = rb->rd_ptr = 0;
		rb->size = size;
        return 1;
	}
	return 0;
}

U32 ring_buff_free(ring_buff_t *rb)
{
	if(rb)
	{
		free(rb->buf_ptr);
		rb->buf_ptr = NULL;
		rb->wr_ptr = rb->rd_ptr = 0;
		rb->size = 0;
        return 1;
	}
	return 0;
}

BOOL ring_buff_is_empty(ring_buff_t *rb)
{
	if(rb)
	{
		return (rb->wr_ptr == rb->rd_ptr);
	}
	return 0;
}

BOOL ring_buff_is_full(ring_buff_t *rb)
{
	if(rb && rb->size > 0)
	{
		return (((rb->wr_ptr + 1) % rb->size) == rb->rd_ptr);
	}
	return 0;
}

U32 ring_buff_get_size(ring_buff_t *rb)
{
	if(rb)
	{
		return rb->size;
	}
	return 0;
}

U32 ring_buff_get_capacity(ring_buff_t *rb)
{
	if(rb)
	{
		return rb->size - 1;
	}
	return 0;
}

U32 ring_buff_get_free_space(ring_buff_t *rb)
{
	if(rb)
	{
		if(rb->wr_ptr >= rb->rd_ptr)
		{
			return (ring_buff_get_capacity(rb) - (rb->wr_ptr - rb->rd_ptr));
		}
		else
		{
			return (rb->rd_ptr - rb->wr_ptr - 1);
		}
	}
	return 0;
}

U32 ring_buff_get_used_space(ring_buff_t *rb)
{
	if(rb)
	{
		return (ring_buff_get_capacity(rb) - ring_buff_get_free_space(rb));
	}
	return 0;
}


U32 ring_buff_get_right_used_space(ring_buff_t *rb)
{
	if(rb)
	{
		if(rb->wr_ptr >= rb->rd_ptr)
		{
			return (rb->wr_ptr - rb->rd_ptr);
		}
		else
		{
			return (rb->size - rb->rd_ptr);
		}
	}
	return 0;

}

U32 ring_buff_get_right_free_space(ring_buff_t *rb)
{
	if(rb)
	{
		if(rb->wr_ptr >= rb->rd_ptr)
		{
			return rb->size - rb->wr_ptr;
		}

		else
		{
			return rb->rd_ptr - rb->wr_ptr - 1;
		}
	}
	return 0;

}

U32 ring_buff_get_data(ring_buff_t *rb, byte *to, int count)
{
	U32 data_count = ring_buff_get_used_space(rb);
	U32 right_data_count = ring_buff_get_right_used_space(rb);

	if(rb && to)
	{
		if(count >= data_count)
		{
			count = data_count;
		}
		if(count > right_data_count)
		{
			memcpy(to, &rb->buf_ptr[rb->rd_ptr], right_data_count);
			ring_buff_move_rd_ptr(rb, right_data_count);

			memcpy(to + right_data_count, &rb->buf_ptr[rb->rd_ptr], count - right_data_count);
			ring_buff_move_rd_ptr(rb, count - right_data_count);
		}
		else
		{
			memcpy(to, &rb->buf_ptr[rb->rd_ptr], count);
			ring_buff_move_rd_ptr(rb, count);
		}

	}

	return count;
}

U32 ring_buff_put_data(ring_buff_t *rb, byte *from, int count)
{
	U32 free_space= ring_buff_get_free_space(rb);
	U32 right_free_space= ring_buff_get_right_free_space(rb);

	if(rb && from)
	{
		if(count >= free_space)
		{
			count = free_space;
		}
		if(count > right_free_space)
		{
			memcpy(&rb->buf_ptr[rb->wr_ptr], from, right_free_space);
			ring_buff_move_wr_ptr(rb, right_free_space);

			memcpy(&rb->buf_ptr[rb->wr_ptr], from + right_free_space, count - right_free_space);
			ring_buff_move_wr_ptr(rb, count - right_free_space);
		}
		else
		{
			memcpy(&rb->buf_ptr[rb->wr_ptr], from, count);
			ring_buff_move_wr_ptr(rb, count);
		}
	}
	return count;
}

U32 ring_buff_move_wr_ptr(ring_buff_t *rb, int count)
{
	rb->wr_ptr = ((rb->wr_ptr + count + rb->size) % rb->size);
	return rb->wr_ptr;
}

U32 ring_buff_move_rd_ptr(ring_buff_t *rb, int count)
{
	rb->rd_ptr = ((rb->rd_ptr + count + rb->size) % rb->size);
	return rb->rd_ptr;
}

U32 ring_buff_get_wr_ptr(ring_buff_t *rb)
{
	return rb->wr_ptr;
}

U32 ring_buff_get_rd_ptr(ring_buff_t *rb)
{
	return rb->rd_ptr;
}

U32 ring_buff_get_wr_addr(ring_buff_t *rb)
{
	return rb->buf_ptr[rb->wr_ptr];
}

U32 ring_buff_get_rd_addr(ring_buff_t *rb)
{
	return rb->buf_ptr[rb->rd_ptr];
}
