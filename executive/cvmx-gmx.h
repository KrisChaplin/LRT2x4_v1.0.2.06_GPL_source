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
 * Interface to the GMX hardware.
 *
 * $Id: cvmx-gmx.h 12995 2013-12-05 08:15:17Z incifer $ $Name$
 */
#include "cvmx-csr.h"

#ifndef __CVMX_GMX_H__
#define __CVMX_GMX_H__

#ifdef	__cplusplus
extern "C" {
#endif

/* CSR typedefs have been moved to cvmx-csr-*.h */

/**
 * Disables the sending of flow control (pause) frames on the specified
 * RGMII port(s).
 *
 * @param interface Which interface (0 or 1)
 * @param port_mask Mask (4bits) of which ports on the interface to disable
 *                  backpressure on.
 *                  1 => disable backpressure
 *                  0 => enable backpressure
 *
 * @return 0 on success
 *         -1 on error
 */

static inline int cvmx_gmx_set_backpressure_override(uint32_t interface, uint32_t port_mask)
{
    /* Check for valid arguments */
    if (port_mask & ~0xf || interface & ~0x1)
        return(-1);
    cvmx_write_csr(CVMX_GMXX_TX_OVR_BP(interface), port_mask << 8 | port_mask);
    return(0);

}

#ifdef	__cplusplus
}
#endif

#endif

