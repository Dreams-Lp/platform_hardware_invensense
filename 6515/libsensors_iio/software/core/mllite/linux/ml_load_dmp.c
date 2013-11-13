/*
 $License:
    Copyright (C) 2012 InvenSense Corporation, All Rights Reserved.
 $
 */

/******************************************************************************
 *
 * $Id:$
 *
 *****************************************************************************/

/**
 * @defgroup ML_LOAD_DMP
 *
 * @{
 *      @file     ml_load_dmp.c
 *      @brief    functions for writing dmp firmware.
 */
#include <stdio.h>

#undef MPL_LOG_TAG
#define MPL_LOG_TAG "MPL-loaddmp"

#include "ml_load_dmp.h"
#include "log.h"
#include "mlos.h"

#define LOADDMP_LOG MPL_LOGI
#define LOADDMP_LOG MPL_LOGI

#define NUM_LOCAL_KEYS (sizeof(dmpTConfig)/sizeof(dmpTConfig[0]))
#define DMP_CODE_SIZE 2913

static const unsigned char dmpMemory[DMP_CODE_SIZE] = {
    /* bank # 0 */
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x01, 0x0b, 0x10, 0xe4, 0x00, 0x00, 0x00, 0x01, 0x00, 0x7d, 0x00, 0x0a, 
    0x06, 0xb8, 0x81, 0xa1, 0xe1, 0x89, 0xb2, 0x78, 0x37, 0xf8, 0x5a, 0x9c, 0xcf, 0xdd, 0x85, 0x3b, 
    0x00, 0x00, 0x00, 0x00, 0x1f, 0xdb, 0xe2, 0xb9, 0x3b, 0x7b, 0x11, 0x7b, 0x32, 0x57, 0x83, 0x7b, 
    0x00, 0x3e, 0x03, 0x30, 0x40, 0x00, 0x00, 0x00, 0x02, 0xca, 0xe3, 0x09, 0x3e, 0x80, 0x00, 0x00, 
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 
    0x00, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x18, 0x6e, 0x00, 0x00, 0x06, 0x92, 0x0a, 0x16, 0xc0, 0xdf, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x07, 
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x0b, 0x2b, 0x00, 0x00, 0x16, 0x57, 0x00, 0x00, 0x03, 0x59, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1d, 0xfa, 0x00, 0x02, 0x6c, 0x1d, 0x00, 0x00, 0x00, 0x00, 
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /* bank # 1 */
    0x01, 0x6c, 0x16, 0xc1, 0x00, 0x01, 0x9b, 0xdf, 0x00, 0x08, 0x17, 0x42, 0x00, 0x00, 0x13, 0x88, 
    0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x10, 0x72, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x20, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x58, 0x05, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0a, 0x00, 0x00, 0xff, 
    0x08, 0x00, 0x02, 0x00, 0x04, 0x00, 0x01, 0x00, 0x80, 0x00, 0x10, 0x00, 0x20, 0x00, 0x40, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb2, 0x6a, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x10, 0x00, 0x00, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf4, 0x00, 0x00, 0x10, 0x00, 
    /* bank # 2 */
    0x00, 0x28, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x0b, 0xeb, 0xc2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 
    0x00, 0x00, 0x00, 0x00, 0x02, 0x03, 0x18, 0x85, 0x04, 0x06, 0x31, 0x0a, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x04, 0x78, 0x68, 0xc0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0x05, 0x00, 0x05, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x1b, 0x00, 0x00, 0x03, 0x33, 0x33, 0x34, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0x05, 0x00, 0x05, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0x05, 0x00, 0x05, 
    0xff, 0xe5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /* bank # 3 */
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x24, 0x26, 0xd3, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x10, 0x00, 0x96, 0x00, 0x3c, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x0c, 0x0a, 0x4e, 0x68, 0xcd, 0xcf, 0x77, 0x09, 0x50, 0x16, 0x67, 0x59, 0xc6, 0x19, 0xce, 0x82, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x3c, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03, 0x18, 0x85, 0x00, 0x00, 0x40, 0x00, 
    0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x67, 0x7d, 0xdf, 0x7e, 0x72, 0x90, 0x2e, 0x55, 0x4c, 0xf6, 0xe6, 0x88, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0xb8, 0x51, 0xec, 0x01, 0x47, 0xae, 0x14, 

    /* bank # 4 */
    0xd8, 0xb0, 0xb5, 0xb9, 0xf3, 0xa6, 0xf8, 0xf9, 0xd1, 0xd9, 0x81, 0x96, 0xf8, 0xf7, 0x3e, 0xd8, 
    0xf3, 0xb1, 0x86, 0x96, 0xa3, 0x31, 0xd1, 0xda, 0xf1, 0xff, 0xd8, 0xf3, 0xb9, 0xab, 0xf8, 0xf9, 
    0xd1, 0xda, 0xf1, 0xb8, 0xa8, 0xdf, 0xdf, 0xdf, 0xf2, 0xb0, 0x81, 0xa8, 0xc1, 0xc9, 0xc3, 0xb0, 
    0xb6, 0xb9, 0x88, 0x90, 0xad, 0xf0, 0x2c, 0x50, 0x78, 0x91, 0x24, 0x48, 0x70, 0x3c, 0x92, 0x48, 
    0x70, 0xb1, 0xb5, 0x8e, 0x9d, 0xad, 0xf1, 0x2d, 0x55, 0x7d, 0xd8, 0xb1, 0xb5, 0x8d, 0x9d, 0xa0, 
    0xf0, 0x2c, 0x50, 0x78, 0xf5, 0x80, 0x94, 0x30, 0xf2, 0xab, 0xde, 0xd9, 0xf8, 0xf8, 0xd8, 0xa0, 
    0xcb, 0xf3, 0xde, 0xf1, 0x1e, 0x90, 0x52, 0xa4, 0xc2, 0xf1, 0xa0, 0x36, 0x94, 0x29, 0xf5, 0x30, 
    0xca, 0xf3, 0xde, 0xf1, 0x90, 0x04, 0x52, 0xa0, 0x36, 0xa0, 0x08, 0xad, 0x9d, 0x0e, 0x16, 0x1e, 
    0x84, 0x90, 0xbb, 0xaf, 0x02, 0xb3, 0xb7, 0x8f, 0x9f, 0xf0, 0x34, 0x18, 0xd0, 0xf1, 0x0d, 0xf5, 
    0x9d, 0x18, 0xf2, 0xd9, 0xb9, 0xab, 0xf8, 0xd8, 0xb3, 0xb7, 0xbb, 0x8e, 0x9e, 0xae, 0xf1, 0x32, 
    0xf5, 0x1b, 0xf1, 0xb4, 0xb8, 0xb0, 0x80, 0x97, 0xf1, 0xa9, 0xdf, 0xdf, 0xdf, 0xaa, 0xdf, 0xdf, 
    0xdf, 0xf2, 0xaa, 0xc5, 0xcd, 0xc7, 0xa9, 0x0c, 0xc9, 0x2c, 0x97, 0xf1, 0xa9, 0x89, 0x26, 0x46, 
    0x66, 0xb2, 0x89, 0x99, 0xa9, 0x2d, 0x55, 0x7d, 0xb0, 0x8a, 0xa8, 0x96, 0x36, 0x56, 0x76, 0xd8, 
    0xf1, 0xb0, 0x89, 0xb9, 0xa3, 0xc3, 0xc5, 0xc7, 0xb1, 0x81, 0xb4, 0x97, 0xa3, 0x11, 0xb5, 0x93, 
    0xa1, 0xf0, 0x24, 0x08, 0x44, 0x10, 0x64, 0x18, 0xf1, 0x93, 0x81, 0xa3, 0x2d, 0x55, 0x3d, 0xf2, 
    0xa6, 0xfa, 0xf9, 0xd1, 0xd9, 0xf8, 0xf1, 0x83, 0xa2, 0xc3, 0xc5, 0xc7, 0xd8, 0xf3, 0xa2, 0xde, 
    /* bank # 5 */
    0xf1, 0x82, 0x93, 0xa3, 0x2d, 0x55, 0x7d, 0x83, 0x95, 0xf5, 0xa3, 0x30, 0xd9, 0xf3, 0xa2, 0xf8, 
    0xd8, 0xf5, 0xa3, 0x50, 0xd9, 0xf3, 0xa2, 0xf8, 0xd8, 0xf5, 0xa3, 0x70, 0xd9, 0xf3, 0xa2, 0xf8, 
    0xd8, 0xf2, 0xb9, 0xa2, 0xf8, 0xf9, 0xd1, 0xd9, 0xa6, 0xde, 0xdf, 0xf4, 0x1c, 0xd8, 0xf2, 0xa6, 
    0xf8, 0xf9, 0xd1, 0xd9, 0xf4, 0x14, 0xd8, 0xf2, 0xf9, 0xd1, 0xd9, 0xf8, 0xf4, 0x1e, 0xd8, 0xf2, 
    0xf9, 0xd1, 0xd9, 0xf8, 0xf8, 0xf4, 0x14, 0xd8, 0xf4, 0x10, 0xd8, 0xf3, 0xa2, 0xf8, 0xf9, 0xd1, 
    0xda, 0xf2, 0xa6, 0xf8, 0xf1, 0xa5, 0xde, 0xd8, 0xf4, 0xa3, 0x14, 0x14, 0xd8, 0xf1, 0xa5, 0xf8, 
    0xa3, 0x85, 0x95, 0x09, 0xd9, 0xf1, 0xa5, 0xde, 0xf2, 0xa6, 0xf8, 0xdf, 0xd8, 0xf4, 0xa3, 0x09, 
    0xd8, 0xf3, 0xa2, 0xf8, 0xf9, 0xd1, 0xf4, 0xd9, 0x08, 0x17, 0xda, 0x42, 0xf2, 0xa2, 0xde, 0xf4, 
    0x0b, 0xd8, 0xf1, 0xa5, 0xf8, 0xa3, 0x85, 0x95, 0x19, 0xda, 0xf4, 0x05, 0xd8, 0xf2, 0xa6, 0xde, 
    0xdf, 0xd8, 0xf1, 0xb8, 0xaa, 0xb3, 0x8d, 0xb4, 0x98, 0x0d, 0x35, 0x5d, 0xb2, 0xb6, 0xba, 0xaf, 
    0x8c, 0x96, 0x19, 0x8f, 0x9f, 0xb8, 0xa8, 0x0e, 0x16, 0x1e, 0xb4, 0x9a, 0xaa, 0x88, 0x2c, 0x54, 
    0x7c, 0xd8, 0xf1, 0xb8, 0xb4, 0xb0, 0x97, 0x86, 0xa8, 0x31, 0x9b, 0x06, 0x99, 0x07, 0xab, 0x97, 
    0x28, 0x88, 0x9b, 0xf0, 0x0c, 0x20, 0x14, 0x40, 0xb0, 0xb4, 0xb8, 0xf0, 0xa8, 0x8a, 0x9a, 0x28, 
    0x50, 0x78, 0xb7, 0x9b, 0xa8, 0x29, 0x51, 0x79, 0x24, 0x70, 0x59, 0x44, 0x69, 0x38, 0x64, 0x48, 
    0x31, 0xf1, 0xbb, 0xab, 0x88, 0x00, 0x2c, 0x54, 0x7c, 0xf0, 0xb3, 0x8b, 0xb8, 0xa8, 0x04, 0x28, 
    0x50, 0x78, 0xf1, 0xb0, 0x88, 0xb4, 0x97, 0x26, 0xa8, 0x59, 0x98, 0xbb, 0xab, 0xb3, 0x8b, 0x02, 
    /* bank # 6 */
    0x26, 0x46, 0x66, 0xb0, 0xb8, 0xf0, 0x8a, 0x9c, 0xa8, 0x29, 0x51, 0x79, 0x8b, 0x29, 0x51, 0x8a, 
    0x24, 0x70, 0x59, 0x8b, 0x20, 0x58, 0x8a, 0x44, 0x69, 0x38, 0x8b, 0x39, 0x40, 0x8a, 0x64, 0x48, 
    0x31, 0x8b, 0x30, 0x49, 0x88, 0xf1, 0xac, 0x00, 0x2c, 0x54, 0x7c, 0xf0, 0x8c, 0xa8, 0x04, 0x28, 
    0x50, 0x78, 0xf1, 0x88, 0x97, 0x26, 0xa8, 0x59, 0x98, 0xac, 0x8c, 0x02, 0x26, 0x46, 0x66, 0xf0, 
    0x89, 0x9c, 0xa8, 0x29, 0x51, 0x79, 0x24, 0x70, 0x59, 0x44, 0x69, 0x38, 0x64, 0x48, 0x31, 0xa9, 
    0x88, 0x09, 0x20, 0x59, 0x70, 0xab, 0x11, 0x38, 0x40, 0x69, 0xa8, 0x19, 0x31, 0x48, 0x60, 0xb1, 
    0x96, 0x8e, 0xab, 0xf1, 0x06, 0x8a, 0x07, 0xb0, 0xa8, 0x82, 0x92, 0xf0, 0x04, 0x28, 0x51, 0x79, 
    0x1d, 0x30, 0x14, 0x38, 0xb1, 0x8d, 0xab, 0xd0, 0x98, 0x2c, 0x50, 0x50, 0x78, 0x78, 0x9b, 0xf1, 
    0x1a, 0xb0, 0xb5, 0xb9, 0x8b, 0x90, 0xa3, 0xf5, 0x78, 0xf2, 0xd9, 0xab, 0xf8, 0xd8, 0xab, 0xf9, 
    0xf9, 0xb4, 0xb8, 0xd9, 0xf1, 0x9b, 0xab, 0xd0, 0x7c, 0x78, 0x78, 0x78, 0x78, 0x78, 0xd8, 0xb4, 
    0xf0, 0x8a, 0x92, 0xa8, 0x29, 0x51, 0x79, 0x8b, 0x29, 0x51, 0x79, 0x8a, 0x24, 0x70, 0x59, 0x8b, 
    0x20, 0x58, 0x71, 0x8a, 0x44, 0x69, 0x38, 0x8b, 0x39, 0x40, 0x68, 0x8a, 0x64, 0x48, 0x31, 0x8b, 
    0x30, 0x49, 0x60, 0x88, 0xf1, 0xa2, 0x00, 0x2c, 0x54, 0x7c, 0xf0, 0x82, 0xa8, 0x04, 0x28, 0x50, 
    0x78, 0xf1, 0x88, 0x97, 0x26, 0xa8, 0x59, 0x98, 0xa2, 0x82, 0x02, 0x26, 0x46, 0x66, 0xf0, 0x89, 
    0x92, 0xa8, 0x29, 0x51, 0x79, 0x24, 0x70, 0x59, 0x44, 0x69, 0x38, 0x64, 0x48, 0x31, 0xb9, 0xaa, 
    0x88, 0x09, 0x20, 0x59, 0x70, 0xae, 0x11, 0x38, 0x40, 0x69, 0xb8, 0xa8, 0x19, 0x31, 0x48, 0x60, 
    /* bank # 7 */
    0x82, 0xa8, 0x3c, 0x41, 0x5c, 0x20, 0x7c, 0x00, 0xf1, 0x87, 0x98, 0x19, 0x86, 0xa8, 0x6e, 0x76, 
    0x7e, 0xa9, 0x99, 0x88, 0x2d, 0x55, 0x7d, 0xd8, 0xf1, 0xb3, 0x8b, 0xb4, 0x97, 0xbb, 0xab, 0xf8, 
    0xf9, 0xb9, 0xa3, 0xda, 0xc3, 0xc5, 0xc7, 0xd9, 0x2d, 0x4d, 0x6d, 0xd8, 0xb0, 0x8c, 0xb8, 0xac, 
    0xf8, 0xf9, 0xa8, 0xda, 0xc3, 0xc5, 0xc7, 0xd9, 0x2d, 0x4d, 0x6d, 0xd8, 0xb0, 0x82, 0xb8, 0xa2, 
    0xf8, 0xf9, 0xba, 0xab, 0xda, 0xc3, 0xc5, 0xc7, 0xd9, 0x2d, 0x4d, 0x6d, 0xd8, 0xf3, 0xb9, 0xac, 
    0xde, 0xd8, 0xb1, 0xb6, 0xb9, 0xf1, 0xa8, 0xf8, 0xf3, 0xb9, 0xaa, 0xfa, 0xf9, 0xd1, 0xda, 0xf2, 
    0x8a, 0xca, 0xf4, 0x0e, 0xf3, 0xb1, 0x87, 0xdd, 0xc2, 0xc2, 0xf1, 0x8c, 0xc4, 0xdc, 0xf3, 0xb9, 
    0xac, 0xf8, 0xd8, 0xf2, 0xb2, 0xb6, 0xba, 0xad, 0xfa, 0x8d, 0x9d, 0xab, 0x39, 0xd9, 0xad, 0xdf, 
    0xf4, 0x13, 0xdd, 0xf2, 0xb1, 0x87, 0xb5, 0x9a, 0x08, 0x08, 0xf1, 0xb1, 0x83, 0xc2, 0xc4, 0xc6, 
    0xdc, 0xf3, 0xb9, 0xac, 0xf8, 0xd8, 0xf3, 0xb2, 0xb6, 0xba, 0xad, 0xf8, 0x8d, 0x9d, 0xab, 0x11, 
    0xd9, 0xad, 0xde, 0xf4, 0x13, 0xdd, 0xf2, 0xb1, 0x87, 0xb5, 0x9a, 0x28, 0x28, 0xf1, 0xb0, 0x88, 
    0xc2, 0xc4, 0xc6, 0xdc, 0xf3, 0xb9, 0xac, 0xf8, 0xd8, 0xf2, 0xb2, 0xb6, 0xba, 0xaa, 0xf8, 0x8a, 
    0x9a, 0xab, 0x11, 0xd9, 0xaa, 0xde, 0xf4, 0x13, 0xdd, 0xf2, 0xb1, 0x86, 0xb5, 0x9a, 0x48, 0x48, 
    0xf1, 0xb2, 0x8b, 0xc2, 0xc4, 0xc6, 0xdc, 0xf3, 0xb9, 0xac, 0xf8, 0xd8, 0xf3, 0xb2, 0xb6, 0xba, 
    0xad, 0xfa, 0x8d, 0x9d, 0xab, 0x39, 0xd9, 0xad, 0xdf, 0xf4, 0x12, 0xdd, 0xf3, 0xb1, 0x87, 0xb5, 
    0x9a, 0x08, 0xf2, 0xf2, 0x88, 0xc2, 0xc4, 0xc6, 0xdc, 0xf3, 0xb9, 0xac, 0xf8, 0xd8, 0xf2, 0xb2, 
    /* bank # 8 */
    0xb6, 0xba, 0xaa, 0xfa, 0x8a, 0x9a, 0xab, 0x39, 0xd9, 0xaa, 0xdf, 0xf4, 0x12, 0xdd, 0xf2, 0xb1, 
    0x86, 0xb5, 0x9a, 0x68, 0xf3, 0xf3, 0x88, 0xc2, 0xc4, 0xc6, 0xdc, 0xf3, 0xb9, 0xac, 0xf8, 0xd8, 
    0xf2, 0xb2, 0xb6, 0xba, 0xae, 0xf8, 0x8e, 0x9e, 0xab, 0x11, 0xd9, 0xae, 0xde, 0xf4, 0x12, 0xdd, 
    0xf3, 0xb1, 0x87, 0xb5, 0x9a, 0x68, 0xf2, 0xb0, 0x80, 0xc0, 0xc8, 0xc2, 0xdc, 0xf3, 0xb9, 0xac, 
    0xf8, 0xd8, 0xf2, 0xb2, 0xb6, 0xba, 0xae, 0xfa, 0x8e, 0x9e, 0xab, 0x39, 0xd9, 0xae, 0xdf, 0xf4, 
    0x12, 0xdd, 0xf2, 0xb1, 0x87, 0xb5, 0x9a, 0x68, 0xf2, 0xb0, 0x80, 0xc4, 0xcc, 0xc6, 0xdc, 0xf3, 
    0xb9, 0xac, 0xf8, 0xd8, 0xf3, 0xb2, 0xb6, 0xba, 0xae, 0xf8, 0x8e, 0x9e, 0xab, 0x11, 0xd9, 0xae, 
    0xde, 0xf4, 0x12, 0xdd, 0xf3, 0xb1, 0x87, 0xb5, 0x9a, 0x48, 0xf2, 0xb0, 0x81, 0xc0, 0xc8, 0xc2, 
    0xdc, 0xf3, 0xb9, 0xac, 0xf8, 0xd8, 0xf3, 0xb2, 0xb6, 0xba, 0xae, 0xfa, 0x8e, 0x9e, 0xab, 0x39, 
    0xd9, 0xae, 0xdf, 0xf4, 0x12, 0xdd, 0xf2, 0xb1, 0x87, 0xb5, 0x9a, 0x48, 0xf2, 0xb0, 0x81, 0xc4, 
    0xcc, 0xc6, 0xdc, 0xf3, 0xb9, 0xac, 0xf8, 0xd8, 0xf2, 0xb0, 0x88, 0xb9, 0xa8, 0xc3, 0xc5, 0xc7, 
    0xf3, 0xb2, 0x8b, 0xa8, 0xcb, 0xcd, 0xcf, 0xb1, 0xb5, 0xb9, 0xf3, 0xac, 0xfa, 0xf9, 0xd1, 0xda, 
    0xf4, 0x10, 0xd8, 0xf3, 0xac, 0xf8, 0xf9, 0xd1, 0xda, 0xf1, 0xb9, 0xaa, 0xdf, 0xf4, 0x1c, 0xd9, 
    0xf4, 0x1a, 0xd8, 0xf3, 0xac, 0xf8, 0xf9, 0xd1, 0xd9, 0xf4, 0x1d, 0xd8, 0xf2, 0x8c, 0xac, 0xf8, 
    0xf9, 0xd1, 0xd9, 0xc2, 0xd8, 0xf2, 0xf9, 0xd9, 0xde, 0xf4, 0x09, 0x1d, 0x16, 0xda, 0xf2, 0xdd, 
    0xc6, 0xdc, 0xf4, 0x30, 0xf1, 0xb9, 0xaa, 0xdf, 0xd8, 0xf1, 0xab, 0xfa, 0x8b, 0x9b, 0xa3, 0x69, 
    /* bank # 9 */
    0xd9, 0xf4, 0x07, 0x06, 0xda, 0xf1, 0xb9, 0xab, 0xdf, 0xfe, 0xd8, 0xf1, 0xbb, 0xb3, 0xb7, 0xaa, 
    0xf9, 0xda, 0xff, 0xd9, 0x80, 0x9a, 0xaa, 0x28, 0xb4, 0x80, 0x98, 0xa7, 0x20, 0xd8, 0xf1, 0xb3, 
    0xb7, 0xbb, 0x87, 0xaa, 0xd0, 0xc1, 0xa7, 0x81, 0x97, 0x62, 0x93, 0xf0, 0x71, 0x71, 0x60, 0x85, 
    0x94, 0x01, 0x29, 0x51, 0x79, 0x90, 0xa5, 0xf1, 0x28, 0x4c, 0x6c, 0x87, 0x0c, 0x95, 0x18, 0x85, 
    0x78, 0xa3, 0x83, 0x90, 0x28, 0x4c, 0x6c, 0x8a, 0x6c, 0xf1, 0xb0, 0xb4, 0xb8, 0x87, 0x98, 0xa5, 
    0x20, 0x95, 0xa3, 0x20, 0xa5, 0x83, 0x95, 0x22, 0x9d, 0xf0, 0x31, 0x31, 0x20, 0x8e, 0x94, 0x01, 
    0x29, 0x51, 0x79, 0x97, 0xae, 0xf1, 0x28, 0x4c, 0x6c, 0x85, 0x0c, 0x9e, 0x18, 0x8e, 0x78, 0xad, 
    0x8d, 0x97, 0x28, 0x4c, 0x6c, 0x83, 0x0c, 0x87, 0x98, 0xa5, 0x20, 0x95, 0xa3, 0x20, 0xa5, 0x83, 
    0x95, 0x22, 0x9f, 0xf0, 0x31, 0x31, 0x20, 0xb1, 0x89, 0x94, 0x01, 0x95, 0x29, 0x51, 0x79, 0x97, 
    0xb9, 0xa9, 0xf1, 0x28, 0x4c, 0x6c, 0xb0, 0x85, 0x0c, 0xb5, 0x99, 0x18, 0xb1, 0x89, 0x78, 0xb8, 
    0xaf, 0xb0, 0x8f, 0xb4, 0x97, 0x28, 0x4c, 0x6c, 0x83, 0x0c, 0xf2, 0x83, 0x93, 0xa3, 0x71, 0xd9, 
    0xa3, 0xd0, 0xf8, 0xf5, 0x8e, 0x9e, 0xb9, 0xa3, 0x78, 0xf1, 0xb1, 0x8a, 0xb5, 0x93, 0x45, 0xb2, 
    0x8c, 0x2e, 0xb1, 0x8a, 0x54, 0x83, 0xaa, 0xd0, 0xc6, 0xf5, 0x89, 0x99, 0xa3, 0x78, 0xf1, 0x8a, 
    0x93, 0x65, 0xb2, 0x8c, 0x2e, 0xb1, 0x8a, 0x74, 0x83, 0xaa, 0xd0, 0xc7, 0xd8, 0xf2, 0xb0, 0x83, 
    0xb8, 0xa5, 0xc6, 0xf9, 0xa3, 0xb4, 0x95, 0x41, 0xd9, 0xf1, 0xb1, 0x8a, 0xba, 0xa6, 0xc5, 0xc7, 
    0xf2, 0xb8, 0xa3, 0xd0, 0xde, 0xf1, 0xb2, 0xb6, 0xb9, 0x86, 0x96, 0xa3, 0x50, 0xb1, 0x83, 0xb5, 
    /* bank # 10 */
    0x93, 0x04, 0x2c, 0xb6, 0x9c, 0x56, 0xf3, 0xb8, 0xa3, 0xd0, 0xde, 0xf8, 0xf1, 0xb1, 0x83, 0xb6, 
    0x96, 0xa3, 0xdb, 0x69, 0xf3, 0xa3, 0xd0, 0xde, 0xd8, 0xf3, 0xbb, 0xb3, 0xb7, 0x90, 0xa2, 0x82, 
    0x00, 0xf2, 0x10, 0xa8, 0x92, 0x19, 0x80, 0xa2, 0xf2, 0xd9, 0x26, 0xf3, 0xa7, 0xd0, 0xdf, 0xd8, 
    0xf1, 0xa8, 0x89, 0x95, 0x19, 0xa9, 0x80, 0xd9, 0x38, 0xd8, 0xa8, 0x89, 0x39, 0xa9, 0x80, 0xda, 
    0x3c, 0xd8, 0xa8, 0x2e, 0x85, 0xf5, 0x75, 0xda, 0xff, 0xd8, 0x71, 0x80, 0xa9, 0xda, 0xf1, 0xff, 
    0xd8, 0xf1, 0xb2, 0xb7, 0xba, 0x87, 0x99, 0xa4, 0x09, 0x84, 0x96, 0x0a, 0xb6, 0x97, 0xa7, 0x00, 
    0x87, 0xa4, 0x09, 0xa7, 0xd0, 0xd9, 0xb0, 0x87, 0xc4, 0xda, 0x86, 0xc0, 0xd8, 0xf1, 0xb2, 0xb7, 
    0xba, 0x83, 0x99, 0xa4, 0x09, 0xda, 0xa3, 0xdf, 0xf9, 0xc7, 0xf4, 0x0b, 0xd8, 0xf1, 0xa3, 0xfb, 
    0xf8, 0xdb, 0xfb, 0xd9, 0xde, 0xa3, 0xdf, 0xd8, 0xf1, 0xa3, 0xfa, 0xf9, 0xbb, 0xa9, 0xd0, 0x84, 
    0xb6, 0x97, 0xda, 0x32, 0xd9, 0x52, 0xd8, 0xb3, 0xb7, 0x82, 0xa7, 0xf3, 0xc1, 0xf2, 0x80, 0xc2, 
    0xf1, 0x97, 0x86, 0x49, 0x2e, 0xa6, 0xd0, 0x50, 0x96, 0x86, 0xa8, 0x75, 0xd9, 0x88, 0xa2, 0xd0, 
    0xf3, 0xc4, 0xc7, 0xf1, 0xda, 0x88, 0x96, 0xa2, 0xd0, 0xf3, 0xc2, 0xc3, 0x82, 0xb4, 0x93, 0x78, 
    0x78, 0xf1, 0xd8, 0x80, 0xb7, 0x90, 0xa8, 0x0d, 0x89, 0x99, 0xa8, 0x10, 0x80, 0x98, 0x21, 0xda, 
    0x2e, 0xd8, 0x89, 0x99, 0xa8, 0x31, 0x80, 0xda, 0x2e, 0xd8, 0xa8, 0x82, 0x92, 0xf3, 0x41, 0x80, 
    0xf1, 0xd9, 0x2e, 0xd8, 0xa8, 0x82, 0xf3, 0x19, 0x80, 0xf1, 0xd9, 0x2e, 0xd8, 0xf1, 0x80, 0xb6, 
    0x97, 0xa8, 0x19, 0x89, 0xb7, 0x98, 0xba, 0xa4, 0x21, 0xd9, 0x82, 0xbb, 0xac, 0xf3, 0xc0, 0xa2, 
    /* bank # 11 */
    0x80, 0x92, 0x22, 0xf1, 0xa9, 0x22, 0x26, 0xd8, 0xb7, 0x98, 0xa8, 0x29, 0xda, 0xac, 0xde, 0xff, 
    0xd8, 0xa2, 0xf2, 0xde, 0xf1, 0xa9, 0xdf, 0xf3, 0xb8, 0xa3, 0xd0, 0xf8, 0xf9, 0xd1, 0xd9, 0xff, 
    0xd8, 0xbb, 0x97, 0x8c, 0xa8, 0xf3, 0x79, 0xda, 0xf1, 0xf1, 0xf1, 0xf1, 0xb1, 0x88, 0xb9, 0xac, 
    0xd0, 0xc0, 0xb3, 0xf3, 0xf3, 0xb9, 0xaa, 0xfa, 0xf1, 0xbb, 0xaa, 0xd0, 0xf8, 0xf4, 0x10, 0xd8, 
    0xf3, 0xa7, 0xd0, 0xfa, 0x97, 0x8c, 0xa8, 0x79, 0xda, 0xf1, 0x87, 0x9a, 0xaa, 0xd0, 0x70, 0xd8, 
    0xf2, 0x82, 0x92, 0xa8, 0x31, 0x80, 0xa6, 0x96, 0xf1, 0xd9, 0x00, 0xac, 0x8c, 0x9c, 0x0c, 0x30, 
    0xac, 0xde, 0xd0, 0xde, 0xf1, 0xff, 0xd8, 0x8c, 0x9c, 0xac, 0xd0, 0x10, 0xac, 0xde, 0x80, 0x92, 
    0xa2, 0xf2, 0x4c, 0x82, 0xa8, 0xf1, 0xca, 0xf2, 0x35, 0xf1, 0x96, 0x88, 0xa6, 0xd9, 0x00, 0xd8, 
    0xff
};

#define DMP_VERSION (dmpMemory)

inv_error_t inv_write_dmp_data(FILE *fd, const unsigned char *dmp, size_t len)
{
    inv_error_t result = INV_SUCCESS;
    int bytesWritten = 0;
   
    if (len <= 0) {
        MPL_LOGE("Nothing to write");
        return INV_ERROR_FILE_WRITE;
    }
    else {
        MPL_LOGI("dmp firmware size to write = %d", len);
    }
    if ( fd == NULL ) {
        return INV_ERROR_FILE_OPEN;
    }
    bytesWritten = fwrite(dmp, 1, len, fd);
    if (bytesWritten != len) {
        MPL_LOGE("bytes written (%d) don't match requested length (%d)\n",
                 bytesWritten, len);
        result = INV_ERROR_FILE_WRITE;
    }
    else {
        MPL_LOGI("Bytes written = %d", bytesWritten);
    }
    return result;
}

inv_error_t inv_load_dmp(FILE *fd)
{
    inv_error_t result = INV_SUCCESS;
    result = inv_write_dmp_data(fd, DMP_VERSION, DMP_CODE_SIZE);
    return result;
}

void read_dmp_img(char *dmp_path, char* out_file)
{
    MPL_LOGI("read_dmp_img"); 
    FILE *fp;
    int i;
    int dmpSize = DMP_CODE_SIZE;
    char dmp_img[dmpSize];

    if ((fp = fopen(dmp_path, "rb")) < 0) {
        perror("dmp fail");
    }
    i = fread(dmp_img, 1, dmpSize, fp);
    MPL_LOGI("Result=%d", i);
    fclose(fp);
    fp = fopen(out_file, "wt");
    if(fp == NULL) {
    MPL_LOGE("error open out file:%s", out_file);
        return;
    }
    fprintf(fp, "char rec[]={\n");
    for(i = 0; i < dmpSize; i++) {
        fprintf(fp, "0x%02x, ", dmp_img[i]);
        if(((i + 1) % 16) == 0) {
            fprintf(fp, "\n");
        }
    }
    fprintf(fp, "};\n ");
    fclose(fp);
}

/**
 *  @}
 */
