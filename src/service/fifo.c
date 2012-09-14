/*
 * fifo.c
 *
 *  Created on: 2012-9-14
 *      Author: YangZhiyong
 */


#include "fifo.h"

#include <string.h>


inline uint32_t empty_space(struct FIFO* fifo)
{
    return fifo->size - (fifo->write_idx - fifo->read_idx);
}


inline uint32_t fill_space(struct FIFO* fifo)
{
    return fifo->write_idx - fifo->read_idx;
}




uint32_t FIFO_Put(struct FIFO* fifo,
                  void* buffer,
                  uint32_t length   )
{
    uint32_t mask;
    uint32_t empty;
    uint32_t fill;
    uint32_t write_to_end;

    mask = fifo->size - 1;
    empty = empty_space(fifo);

    if( empty > length )
    {
        fill = length;
    }
    else
    {
        fill = empty;
    }

    write_to_end = fifo->size - (fifo->write_idx&mask);

    if( fill > write_to_end )
    {
        memcpy(fifo->buffer[fifo->write_idx&mask], buffer, fifo->size - (fifo->write_idx&mask));
    }
    else
    {

    }




}




uint32_t FIFO_Get(struct FIFO* fifo,
                  void* buffer,
                  uint32_t length   )
{

}

