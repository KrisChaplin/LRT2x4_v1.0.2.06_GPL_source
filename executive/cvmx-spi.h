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
 * This file contains defines for the SPI interface

 * $Id: cvmx-spi.h 12995 2013-12-05 08:15:17Z incifer $ $Name$
 *
 *
 */
#ifndef __CVMX_SPI_H__
#define __CVMX_SPI_H__

#ifdef	__cplusplus
extern "C" {
#endif

/* CSR typedefs have been moved to cvmx-csr-*.h */

typedef enum
{
    CVMX_SPI_MODE_UNKNOWN = 0,
    CVMX_SPI_MODE_TX_HALFPLEX = 1,
    CVMX_SPI_MODE_RX_HALFPLEX = 2,
    CVMX_SPI_MODE_DUPLEX = 3
} cvmx_spi_mode_t;

/**
 * Return true if the supplied interface is configured for SPI
 *
 * @param interface Interface to check
 * @return True if interface is SPI
 */
static inline int cvmx_spi_is_spi_interface(int interface)
{
    uint64_t gmxState = cvmx_read_csr(CVMX_GMXX_INF_MODE(interface));
    return ((gmxState & 0x2) && (gmxState & 0x1));
}

/**
 * Initialize and start the SPI interface.
 *
 * @param interface The identifier of the packet interface to configure and
 *                  use as a SPI interface.
 * @param mode      The operating mode for the SPI interface. The interface
 *                  can operate as a full duplex (both Tx and Rx data paths
 *                  active) or as a halfplex (either the Tx data path is
 *                  active or the Rx data path is active, but not both).
 * @param timeout   Timeout to wait for clock synchronization in seconds
 * @return Zero on success, negative of failure.
 */
extern int cvmx_spi_start_interface(int interface, cvmx_spi_mode_t mode, int timeout);

/**
 * This routine restarts the SPI interface after it has lost synchronization
 * with its corespondant system.
 *
 * @param interface The identifier of the packet interface to configure and
 *                  use as a SPI interface.
 * @param mode      The operating mode for the SPI interface. The interface
 *                  can operate as a full duplex (both Tx and Rx data paths
 *                  active) or as a halfplex (either the Tx data path is
 *                  active or the Rx data path is active, but not both).
 * @param timeout   Timeout to wait for clock synchronization in seconds
 * @return Zero on success, negative of failure.
 */
extern int cvmx_spi_restart_interface(int interface, cvmx_spi_mode_t mode, int timeout);

/**
 * Initialize the SPI4000 for use
 *
 * @param interface SPI interface the SPI4000 is connected to
 */
extern int cvmx_spi4000_initialize(int interface);

/**
 * Poll all the SPI4000 port and check its speed
 *
 * @param interface Interface the SPI4000 is on
 * @param port      Port to poll (0-9)
 * @return Status of the port. 0=down. All other values the port is up.
 */
extern cvmx_gmxx_rxx_rx_inbnd_t cvmx_spi4000_check_speed(int interface, int port);

#ifdef	__cplusplus
}
#endif

#endif  /* __CVMX_SPI_H__ */
