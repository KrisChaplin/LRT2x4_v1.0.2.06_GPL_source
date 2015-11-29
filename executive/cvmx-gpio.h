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
 *
 * General Purpose IO interface.
 *
 * $Id: cvmx-gpio.h 12995 2013-12-05 08:15:17Z incifer $ $Name$
 */
#include "cvmx-csr.h"

#ifndef __CVMX_GPIO_H__
#define __CVMX_GPIO_H__

#ifdef	__cplusplus
extern "C" {
#endif

/* CSR typedefs have been moved to cvmx-csr-*.h */

/**
 * Clear the interrupt rising edge detector for the supplied pins in the mask
 *
 * @param clear_mask Mask of pins to clear
 */
static inline void cvmx_gpio_interrupt_clear(uint16_t clear_mask)
{
    cvmx_write_csr(CVMX_GPIO_INT_CLR, clear_mask);
}


/**
 * GPIO Read Data
 *
 * @return Status of the GPIO pins
 */
static inline uint16_t cvmx_gpio_read(void)
{
    return cvmx_read_csr(CVMX_GPIO_RX_DAT);
}


/**
 * GPIO Clear pin
 *
 * @param clear_mask Bit mask to indicate which bits to drive to '0'.
 */
static inline void cvmx_gpio_clear(uint16_t clear_mask)
{
    cvmx_write_csr(CVMX_GPIO_TX_CLR, clear_mask);
}


/**
 * GPIO Set pin
 *
 * @param set_mask Bit mask to indicate which bits to drive to '1'.
 */
static inline void cvmx_gpio_set(uint16_t set_mask)
{
    cvmx_write_csr(CVMX_GPIO_TX_SET, set_mask);
}

#ifdef	__cplusplus
}
#endif

#endif

