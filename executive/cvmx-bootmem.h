/***********************license start***************
 * Copyright (c) 2003-2010  Cavium Networks (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.

 *   * Neither the name of Cavium Networks nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.

 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.

 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM  NETWORKS MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

/**
 * @file
 * Simple allocate only memory allocator.  Used to allocate memory at application
 * start time.
 *
 * $Id: cvmx-bootmem.h 12995 2013-12-05 08:15:17Z incifer $
 *
 */

#ifndef __CVMX_BOOTMEM_H__
#define __CVMX_BOOTMEM_H__


#include <octeon-app-init.h>
#include "open-license/cvmx-bootmem-shared.h"

#ifdef	__cplusplus
extern "C" {
#endif




/**
 * Initialize the boot alloc memory structures. This is
 * normally called inside of cvmx_user_app_init()
 *
 * @param mem_desc_ptr	Address of the free memory list
 * @return
 */
extern int cvmx_bootmem_init(void *mem_desc_ptr);


/**
 * Allocate a block of memory from the free list that was passed
 * to the application by the bootloader.
 * This is an allocate-only algorithm, so freeing memory is not possible.
 *
 * @param size      Size in bytes of block to allocate
 * @param alignment Alignment required - must be power of 2
 *
 * @return pointer to block of memory, NULL on error
 */
extern void *cvmx_bootmem_alloc(uint64_t size, uint64_t alignment);

/**
 * Allocate a block of memory from the free list that was
 * passed to the application by the bootloader at a specific
 * address. This is an allocate-only algorithm, so
 * freeing memory is not possible. Allocation will fail if
 * memory cannot be allocated at the specified address.
 *
 * @param size      Size in bytes of block to allocate
 * @param address   Physical address to allocate memory at.  If this memory is not
 *                  available, the allocation fails.
 * @param alignment Alignment required - must be power of 2
 * @return pointer to block of memory, NULL on error
 */
extern void *cvmx_bootmem_alloc_address(uint64_t size, uint64_t address, uint64_t alignment);



/**
 * Allocate a block of memory from the free list that was
 * passed to the application by the bootloader within a specified
 * address range. This is an allocate-only algorithm, so
 * freeing memory is not possible. Allocation will fail if
 * memory cannot be allocated in the requested range.
 *
 * @param size      Size in bytes of block to allocate
 * @param min_addr  defines the minimum address of the range
 * @param max_addr  defines the maximum address of the range
 * @param alignment Alignment required - must be power of 2
 * @return pointer to block of memory, NULL on error
 */
extern void *cvmx_bootmem_alloc_range(uint64_t size, uint64_t alignment, uint64_t min_addr, uint64_t max_addr);


/**
 * Allocate a block of memory from the free list that was passed
 * to the application by the bootloader, and assign it a name in the
 * global named block table.  (part of the cvmx_bootmem_descriptor_t structure)
 * Named blocks can later be freed.
 *
 * @param size      Size in bytes of block to allocate
 * @param alignment Alignment required - must be power of 2
 * @param name      name of block - must be less than CVMX_BOOTMEM_NAME_LEN bytes
 *
 * @return pointer to block of memory, NULL on error
 */
extern void *cvmx_bootmem_alloc_named(uint64_t size, uint64_t alignment, char *name);



/**
 * Allocate a block of memory from the free list that was passed
 * to the application by the bootloader, and assign it a name in the
 * global named block table.  (part of the cvmx_bootmem_descriptor_t structure)
 * Named blocks can later be freed.
 *
 * @param size      Size in bytes of block to allocate
 * @param address   Physical address to allocate memory at.  If this memory is not
 *                  available, the allocation fails.
 * @param name      name of block - must be less than CVMX_BOOTMEM_NAME_LEN bytes
 *
 * @return pointer to block of memory, NULL on error
 */
extern void *cvmx_bootmem_alloc_named_address(uint64_t size, uint64_t address, char *name);



/**
 * Allocate a block of memory from a specific range of the free list that was passed
 * to the application by the bootloader, and assign it a name in the
 * global named block table.  (part of the cvmx_bootmem_descriptor_t structure)
 * Named blocks can later be freed.
 * If request cannot be satisfied within the address range specified, NULL is returned
 *
 * @param size      Size in bytes of block to allocate
 * @param min_addr  minimum address of range
 * @param max_addr  maximum address of range
 * @param align  Alignment of memory to be allocated. (must be a power of 2)
 * @param name      name of block - must be less than CVMX_BOOTMEM_NAME_LEN bytes
 *
 * @return pointer to block of memory, NULL on error
 */
extern void *cvmx_bootmem_alloc_named_range(uint64_t size, uint64_t min_addr, uint64_t max_addr, uint64_t align, char *name);

/**
 * Frees a previously allocated named bootmem block.
 * 
 * @param name   name of block to free
 * 
 * @return 0 on failure,
 *         !0 on success
 */
extern int cvmx_bootmem_free_named(char *name);


/**
 * Finds a named bootmem block by name.
 * 
 * @param name   name of block to free
 * 
 * @return pointer to named block descriptor on success
 *         0 on failure
 */
cvmx_bootmem_named_block_desc_t * cvmx_bootmem_find_named_block(char *name);



/**
 * Returns the size of available memory in bytes, only
 * counting blocks that are at least as big as the minimum block
 * size.
 * 
 * @param min_block_size
 *               Minimum block size to count in total.
 * 
 * @return Number of bytes available for allocation that meet the block size requirement
 */
uint64_t cvmx_bootmem_available_mem(uint64_t min_block_size);

#ifdef	__cplusplus
}
#endif

#endif /*   __CVMX_BOOTMEM_H__ */
