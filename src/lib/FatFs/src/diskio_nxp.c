/***
* @file     fs_mci.c
* @purpose      Drivers for SD
* @version      1.0
* @date     23. February. 2012
* @author       NXP MCU SW Application Team
*---------------------------------------------------------------------
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "lpc177x_8x_mci.h"
#include "lpc177x_8x_gpdma.h"
#include "debug_frmwrk.h"
#include "bsp.h"
#include "diskio.h"

#include "diskio_nxp.h"
#include <string.h>

/* Disk Status */
static volatile DSTATUS Stat = STA_NOINIT;


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
DRESULT disk_ioctl (
                    BYTE drv,       /* Physical drive number (0) */
                    BYTE ctrl,      /* Control code */
                    void *buff      /* Buffer to send/receive control data */
                        )
{
    DRESULT res;
//    BYTE n, *ptr = buff;

    if (drv) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    res = RES_ERROR;

    switch (ctrl) {
    case CTRL_SYNC :        /* Make sure that no pending write process */

        break;

    case GET_SECTOR_COUNT : /* Get number of sectors on the disk (DWORD) */
        *(DWORD*)buff = SDRAM_DISK_SIZE/SDRAM_DISK_SECTOR_SIZE;
        res = RES_OK;
        break;

    case GET_SECTOR_SIZE :  /* Get R/W sector size (WORD) */
        *(WORD*)buff = SDRAM_DISK_SECTOR_SIZE;
        res = RES_OK;
        break;

    case GET_BLOCK_SIZE :   /* Get erase block size in unit of sector (DWORD) */
        *(DWORD*)buff = 1;
        res = RES_OK;
        break;

    case MMC_GET_TYPE :     /* Get card type flags (1 byte) */
        *(BYTE*)buff = MCI_SDHC_SDXC_CARD;
        res = RES_OK;
        break;

//    case MMC_GET_CSD :      /* Receive CSD as a data block (16 bytes) */
//        for (n=0;n<16;n++)
//            *(ptr+n) = CardConfig.CSD[n];
//        res = RES_OK;
//        break;
//
//    case MMC_GET_CID :      /* Receive CID as a data block (16 bytes) */
//        {
//            uint8_t* cid = (uint8_t*) &CardConfig.CardID;
//            for (n=0;n<sizeof(st_Mci_CardId);n++)
//                *(ptr+n) = cid[n];
//        }
//        res = RES_OK;
//        break;
//
//    case MMC_GET_SDSTAT :   /* Receive SD status as a data block (64 bytes) */
//        {
//            int32_t cardStatus;
//            if(MCI_GetCardStatus(&cardStatus) == MCI_FUNC_OK)
//            {
//                uint8_t* status = (uint8_t*)&cardStatus;
//                for (n=0;n<2;n++)
//                    *(ptr+n) = ((uint8_t*)status)[n];
//                res = RES_OK;
//            }
//        }
//        break;

    default:
        res = RES_PARERR;
    }

    return res;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
                   BYTE drv,            /* Physical drive number (0) */
                   BYTE *buff,          /* Pointer to the data buffer to store read data */
                   DWORD sector,        /* Start sector number (LBA) */
                   BYTE count           /* Sector count (1..255) */
                       )
{
    if (drv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    memcpy(buff,
           (void*)(SDRAM_DISK_ADDRESS + sector*SDRAM_DISK_SECTOR_SIZE),
           count*SDRAM_DISK_SECTOR_SIZE);

    return RES_OK;

}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
                     BYTE drv       /* Physical drive number (0) */
                         )
{
    if (drv) return STA_NOINIT;     /* Supports only single drive */

    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
#if _READONLY == 0
DRESULT disk_write (
                    BYTE drv,           /* Physical drive number (0) */
                    const BYTE *buff,   /* Pointer to the data to be written */
                    DWORD sector,       /* Start sector number (LBA) */
                    BYTE count          /* Sector count (1..255) */
                        )
{
    if (drv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    memcpy((void*)(SDRAM_DISK_ADDRESS + sector*SDRAM_DISK_SECTOR_SIZE),
           buff,
           count*SDRAM_DISK_SECTOR_SIZE );

    return RES_OK;

}
#endif /* _READONLY == 0 */



/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support a real time clock.          */
/* This is not required in read-only configuration.        */
DWORD get_fattime ()
{
    return 0;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

/**
* @}
*/

