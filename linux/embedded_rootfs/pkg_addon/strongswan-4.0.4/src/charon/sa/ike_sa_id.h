/**
 * @file ike_sa_id.h
 *
 * @brief Interface of ike_sa_id_t.
 *
 */

/*
 * Copyright (C) 2005-2006 Martin Willi
 * Copyright (C) 2005 Jan Hutter
 * Hochschule fuer Technik Rapperswil
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */


#ifndef IKE_SA_ID_H_
#define IKE_SA_ID_H_

#include <types.h>


typedef struct ike_sa_id_t ike_sa_id_t;

/**
 * @brief An object of type ike_sa_id_t is used to identify an IKE_SA.
 *
 * An IKE_SA is identified by its initiator and responder spi's.
 * Additionaly it contains the role of the actual running IKEv2-Daemon
 * for the specific IKE_SA (original initiator or responder).
 * 
 * @b Constructors:
 *  - ike_sa_id_create()
 * 
 * @ingroup sa
 */
struct ike_sa_id_t {

	/**
	 * @brief Set the SPI of the responder.
	 *
	 * This function is called when a request or reply of a IKE_SA_INIT is received.
	 *
	 * @param this 				calling object
	 * @param responder_spi 	SPI of responder to set
	 */
	void (*set_responder_spi) (ike_sa_id_t *this, u_int64_t responder_spi);

	/**
	 * @brief Set the SPI of the initiator.
	 *
	 * @param this 				calling object
	 * @param initiator_spi 	SPI to set
	 */
	void (*set_initiator_spi) (ike_sa_id_t *this, u_int64_t initiator_spi);

	/**
	 * @brief Get the initiator SPI.
	 *
	 * @param this 				calling object
	 * @return 					SPI of the initiator
	 */
	u_int64_t (*get_initiator_spi) (ike_sa_id_t *this);

	/**
	 * @brief Get the responder SPI.
	 *
	 * @param this 				calling object
	 * @return 					SPI of the responder
	 */
	u_int64_t (*get_responder_spi) (ike_sa_id_t *this);

	/**
	 * @brief Check if two ike_sa_id_t objects are equal.
	 * 
	 * Two ike_sa_id_t objects are equal if both SPI values and the role matches.
	 *
	 * @param this 				calling object
 	 * @param other 			ike_sa_id_t object to check if equal
 	 * @return 					TRUE if given ike_sa_id_t are equal, FALSE otherwise
	 */
	bool (*equals) (ike_sa_id_t *this, ike_sa_id_t *other);

	/**
	 * @brief Replace all values of a given ike_sa_id_t object with values.
	 * from another ike_sa_id_t object.
	 * 
	 * After calling this function, both objects are equal.
	 *
	 * @param this 				calling object
 	 * @param other 			ike_sa_id_t object from which values will be taken
	 */
	void (*replace_values) (ike_sa_id_t *this, ike_sa_id_t *other);

	/**
	 * @brief Get the initiator flag.
	 *
	 * @param this 				calling object
	 * @return 					TRUE if we are the original initator
	 */
	bool (*is_initiator) (ike_sa_id_t *this);

	/**
	 * @brief Switche the original initiator flag.
	 * 
	 * @param this 				calling object
	 * @return 					TRUE if we are the original initator after switch, FALSE otherwise
	 */
	bool (*switch_initiator) (ike_sa_id_t *this);

	/**
	 * @brief Clones a given ike_sa_id_t object.
	 *
	 * @param this				calling object
	 * @return 					cloned ike_sa_id_t object
	 */
	ike_sa_id_t *(*clone) (ike_sa_id_t *this);

	/**
	 * @brief Destroys an ike_sa_id_t object.
	 *
	 * @param this 				calling object
	 */
	void (*destroy) (ike_sa_id_t *this);
};

/**
 * @brief Creates an ike_sa_id_t object with specific SPI's and defined role.
 *
 * @param initiator_spi			initiators SPI
 * @param responder_spi			responders SPI
 * @param is_initiaor			TRUE if we are the original initiator
 * @return						ike_sa_id_t object
 * 
 * @ingroup sa
 */
ike_sa_id_t * ike_sa_id_create(u_int64_t initiator_spi, u_int64_t responder_spi, bool is_initiaor);

#endif /*IKE_SA_ID_H_*/
