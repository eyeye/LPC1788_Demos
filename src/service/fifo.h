/*
 * fifo.h
 *
 *  Created on: 2012-9-14
 *      Author: YangZhiyong
 */

#ifndef FIFO_H_
#define FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


struct FIFO
{
    uint32_t read_idx;
    uint32_t write_idx;
    uint8_t* buffer;
    uint32_t size;
};


#define FIFO_DEFINE(name, size) \
                uint8_t name##buffer[size];\
                struct FIFO name = \
                {\
                    .read_idx = 0,\
                    .write_idx = 0,\
                    .buffer = name##buffer,\
                    .size = size \
                }


uint32_t FIFO_Put(struct FIFO* fifo,
                  void* buffer,
                  uint32_t length   );

uint32_t FIFO_Get(struct FIFO* fifo,
                  void* buffer,
                  uint32_t length   );


#ifdef __cplusplus
}
#endif

#endif /* FIFO_H_ */