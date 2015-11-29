/* Stroke for charon is the counterpart to whack from pluto
 * Copyright (C) 2006 Martin Willi - Hochschule fuer Technik Rapperswil
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
 *
 * RCSID $Id: starterstroke.h 619 2007-06-25 11:18:20Z encounter $
 */

#ifndef _STARTER_STROKE_H_
#define _STARTER_STROKE_H_

#include "confread.h"

extern int starter_stroke_add_conn(starter_conn_t *conn);
extern int starter_stroke_del_conn(starter_conn_t *conn);
extern int starter_stroke_route_conn(starter_conn_t *conn);
extern int starter_stroke_initiate_conn(starter_conn_t *conn);

#endif /* _STARTER_STROKE_H_ */
