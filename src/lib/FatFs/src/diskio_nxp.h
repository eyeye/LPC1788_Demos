/*
 * diskio_nxp.h
 *
 *  Created on: 2012-6-14
 *      Author: YangZhiyong
 */

#ifndef DISKIO_NXP_H_
#define DISKIO_NXP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_extbus.h"


#define SDRAM_DISK_ADDRESS          (SDRAM_BASE_ADDR)
#define SDRAM_DISK_SIZE             (3*1024*1024)
#define SDRAM_DISK_SECTOR_SIZE      512



typedef struct tagCARDCONFIG
{
    uint32_t        SectorSize;    /* size (in byte) of each sector, fixed to 512bytes */
    uint32_t        SectorCount;     /* total sector number */
    uint32_t        BlockSize;     /* erase block size in unit of sector */
    uint32_t        CardAddress;    /* Card Address */
    uint32_t        OCR;            /* OCR */
    en_Mci_CardType CardType;       /* Card Type */
    st_Mci_CardId   CardID;         /* CID */
    uint8_t         CSD[16];        /* CSD */
} CARDCONFIG;



extern CARDCONFIG CardConfig;

Bool mci_read_configuration (void);

#ifdef __cplusplus
}
#endif

#endif /* DISKIO_NXP_H_ */
