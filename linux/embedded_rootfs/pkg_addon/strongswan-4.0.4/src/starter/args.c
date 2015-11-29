/* automatic handling of confread struct arguments
 * Copyright (C) 2006 Andreas Steffen
 * Hochschule fuer Technik Rapperswil, Switzerland
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
 * RCSID $Id: args.c 619 2007-06-25 11:18:20Z encounter $
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <freeswan.h>

#include "../pluto/constants.h"
#include "../pluto/defs.h"
#include "../pluto/log.h"

#include "keywords.h"
#include "parser.h"
#include "confread.h"
#include "args.h"

/* argument types */

typedef enum {
    ARG_NONE,
    ARG_ENUM,
    ARG_UINT,
    ARG_TIME,
    ARG_ULNG,
    ARG_PCNT,
    ARG_STR,
    ARG_LST,
    ARG_MISC
} arg_t;

/* various keyword lists */

static const char *LST_bool[] = {
    "no",
    "yes",
     NULL
};

static const char *LST_sendcert[] = {
    "always",
    "ifasked",
    "never",
    "yes",
    "no",
     NULL
};

static const char *LST_dpd_action[] = {
    "none",
    "clear",
    "hold",
    "restart",
     NULL
};

static const char *LST_startup[] = {
    "ignore",
    "add",
    "route",
    "start",
     NULL
};

static const char *LST_packetdefault[] = {
    "drop",
    "reject",
    "pass",
     NULL
};

static const char *LST_keyexchange[] = {
    "ike",
    "ikev1",
    "ikev2",
     NULL
};

static const char *LST_pfsgroup[] = {
    "modp1024",
    "modp1536",
    "modp2048",
    "modp3072",
    "modp4096",
    "modp6144",
    "modp8192",
     NULL
};

static const char *LST_plutodebug[] = {
    "none",
    "all",
    "raw",
    "crypt",
    "parsing",
    "emitting",
    "control",
    "lifecycle",
    "klips",
    "dns",
    "natt",
    "oppo",
    "controlmore",
    "private",
     NULL
};

static const char *LST_klipsdebug[] = {
    "tunnel",
    "tunnel-xmit",
    "pfkey",
    "xform",
    "eroute",
    "spi",
    "radij",
    "esp",
    "ah",
    "ipcomp",
    "verbose",
    "all",
    "none",
     NULL
};

typedef struct {
    arg_t       type;
    size_t      offset;
    const char  **list;
} token_info_t;

static const token_info_t token_info[] =
{
    /* config setup keywords */
    { ARG_LST,  offsetof(starter_config_t, setup.interfaces), NULL                 },
    { ARG_STR,  offsetof(starter_config_t, setup.dumpdir), NULL                    },
    { ARG_ENUM, offsetof(starter_config_t, setup.charonstart), LST_bool            },
    { ARG_ENUM, offsetof(starter_config_t, setup.plutostart), LST_bool             },

    /* pluto keywords */
    { ARG_LST,  offsetof(starter_config_t, setup.plutodebug), LST_plutodebug       },
    { ARG_STR,  offsetof(starter_config_t, setup.prepluto), NULL                   },
    { ARG_STR,  offsetof(starter_config_t, setup.postpluto), NULL                  },
    { ARG_ENUM, offsetof(starter_config_t, setup.uniqueids), LST_bool              },
    { ARG_UINT, offsetof(starter_config_t, setup.overridemtu), NULL                },
    { ARG_TIME, offsetof(starter_config_t, setup.crlcheckinterval), NULL           },
    { ARG_ENUM, offsetof(starter_config_t, setup.cachecrls), LST_bool              },
    { ARG_ENUM, offsetof(starter_config_t, setup.strictcrlpolicy), LST_bool        },
    { ARG_ENUM, offsetof(starter_config_t, setup.nocrsend), LST_bool               },
    { ARG_ENUM, offsetof(starter_config_t, setup.nat_traversal), LST_bool          },
    { ARG_TIME, offsetof(starter_config_t, setup.keep_alive), NULL                 },
    { ARG_STR,  offsetof(starter_config_t, setup.virtual_private), NULL            },
    { ARG_STR,  offsetof(starter_config_t, setup.pkcs11module), NULL               },
    { ARG_ENUM, offsetof(starter_config_t, setup.pkcs11keepstate), LST_bool        },
    { ARG_ENUM, offsetof(starter_config_t, setup.pkcs11proxy), LST_bool            },

    /* KLIPS keywords */
    { ARG_LST,  offsetof(starter_config_t, setup.klipsdebug), LST_klipsdebug       },
    { ARG_ENUM, offsetof(starter_config_t, setup.fragicmp), LST_bool               },
    { ARG_STR,  offsetof(starter_config_t, setup.packetdefault), LST_packetdefault },
    { ARG_ENUM, offsetof(starter_config_t, setup.hidetos), LST_bool                },

    /* conn section keywords */
    { ARG_STR,  offsetof(starter_conn_t, name), NULL                               },
    { ARG_ENUM, offsetof(starter_conn_t, startup), LST_startup                     },
    { ARG_ENUM, offsetof(starter_conn_t, keyexchange), LST_keyexchange             },
    { ARG_MISC, 0, NULL  /* KW_TYPE */                                             },
    { ARG_MISC, 0, NULL  /* KW_PFS */                                              },
    { ARG_MISC, 0, NULL  /* KW_COMPRESS */                                         },
    { ARG_MISC, 0, NULL  /* KW_AUTH */                                             },
    { ARG_MISC, 0, NULL  /* KW_AUTHBY */                                           },
    { ARG_TIME, offsetof(starter_conn_t, sa_ike_life_seconds), NULL                },
    { ARG_TIME, offsetof(starter_conn_t, sa_ipsec_life_seconds), NULL              },
    { ARG_TIME, offsetof(starter_conn_t, sa_rekey_margin), NULL                    },
    { ARG_ULNG, offsetof(starter_conn_t, sa_keying_tries), NULL                    },
    { ARG_PCNT, offsetof(starter_conn_t, sa_rekey_fuzz), NULL                      },
    { ARG_MISC, 0, NULL  /* KW_REKEY */                                            },
    { ARG_STR,  offsetof(starter_conn_t, ike), NULL                                },
    { ARG_STR,  offsetof(starter_conn_t, esp), NULL                                },
    { ARG_STR,  offsetof(starter_conn_t, pfsgroup), LST_pfsgroup                   },
    { ARG_TIME, offsetof(starter_conn_t, dpd_delay), NULL                          },
    { ARG_TIME, offsetof(starter_conn_t, dpd_timeout), NULL                        },
    { ARG_ENUM, offsetof(starter_conn_t, dpd_action), LST_dpd_action               },

    /* ca section keywords */
    { ARG_STR,  offsetof(starter_ca_t, name), NULL                                 },
    { ARG_ENUM, offsetof(starter_ca_t, startup), LST_startup                       },
    { ARG_STR,  offsetof(starter_ca_t, cacert), NULL                               },
    { ARG_STR,  offsetof(starter_ca_t, ldaphost), NULL                             },
    { ARG_STR,  offsetof(starter_ca_t, ldapbase), NULL                             },
    { ARG_STR,  offsetof(starter_ca_t, crluri), NULL                               },
    { ARG_STR,  offsetof(starter_ca_t, crluri2), NULL                              },
    { ARG_STR,  offsetof(starter_ca_t, ocspuri), NULL                              },

    /* end keywords */
    { ARG_MISC, 0, NULL  /* KW_HOST */                                             },
    { ARG_MISC, 0, NULL  /* KW_NEXTHOP */                                          },
    { ARG_MISC, 0, NULL  /* KW_SUBNET */                                           },
    { ARG_MISC, 0, NULL  /* KW_SUBNETWITHIN */                                     },
    { ARG_MISC, 0, NULL  /* KW_PROTOPORT */                                        },
    { ARG_MISC, 0, NULL  /* KW_SOURCEIP */                                         },
    { ARG_MISC, 0, NULL  /* KW_NATIP */                                            },
    { ARG_ENUM, offsetof(starter_end_t, firewall), LST_bool                        },
    { ARG_ENUM, offsetof(starter_end_t, hostaccess), LST_bool                      },
    { ARG_STR,  offsetof(starter_end_t, updown), NULL                              },
    { ARG_STR,  offsetof(starter_end_t, id), NULL                                  },
    { ARG_STR,  offsetof(starter_end_t, rsakey), NULL                              },
    { ARG_STR,  offsetof(starter_end_t, cert), NULL                                },
    { ARG_ENUM, offsetof(starter_end_t, sendcert), LST_sendcert                    },
    { ARG_STR,  offsetof(starter_end_t, ca), NULL                                  },
    { ARG_STR,  offsetof(starter_end_t, groups), NULL                              },
    { ARG_STR,  offsetof(starter_end_t, iface), NULL                               }
};

static void
free_list(char **list)
{
    char **s;

    for (s = list; *s; s++)
	pfree(*s);
    pfree(list);
}

char **
new_list(char *value)
{
    char *val, *b, *e, *end, **ret;
    int count;

    val = value ? clone_str(value, "list value") : NULL;
    if (!val)
	return NULL;
    end = val + strlen(val);
    for (b = val, count = 0; b < end;)
    {
	for (e = b; ((*e != ' ') && (*e != '\0')); e++);
	*e = '\0';
	if (e != b)
	    count++;
	b = e + 1;
    }
    if (count == 0)
    {
	pfree(val);
	return NULL;
    }
    ret = (char **)alloc_bytes((count+1) * sizeof(char *), "list");

    for (b = val, count = 0; b < end; )
    {
	for (e = b; (*e != '\0'); e++);
	if (e != b)
	    ret[count++] = clone_str(b, "list value");
	b = e + 1;
    }
    ret[count] = NULL;
    pfree(val);
    return ret;
}


/*
 * assigns an argument value to a struct field
 */
bool
assign_arg(kw_token_t token, kw_token_t first, kw_list_t *kw, char *base
    , bool *assigned)
{
    char *p = base + token_info[token].offset;
    const char **list = token_info[token].list;

    int index = -1;  /* used for enumeration arguments */

    lset_t *seen = (lset_t *)base;    /* seen flags are at the top of the struct */
    lset_t f = LELEM(token - first);  /* compute flag position of argument */

    *assigned = FALSE;

    DBG(DBG_CONTROLMORE,
	DBG_log("  %s=%s", kw->entry->name, kw->value)
    )

    if (*seen & f)
    {
	plog("# duplicate '%s' option", kw->entry->name);
	return FALSE;
    }

    /* set flag that this argument has been seen */
    *seen |= f;

    /* is there a keyword list? */
    if (list != NULL && token_info[token].type != ARG_LST)
    {
	bool match = FALSE;

	while (*list != NULL && !match)
	{
	    index++;
	    match = streq(kw->value, *list++);
	}
	if (!match)
	{
	    plog("# bad value: %s=%s", kw->entry->name, kw->value);
	    return FALSE;
	}
    }

    switch (token_info[token].type)
    {
    case ARG_NONE:
	plog("# option '%s' not supported yet", kw->entry->name);
	return FALSE;
    case ARG_ENUM:
	{
	    int *i = (int *)p;

	    if (index < 0)
	    {
		plog("# bad enumeration value: %s=%s (%d)"
		    , kw->entry->name, kw->value, index);
		return FALSE;
	    }
	    *i = index;
	}
	break;

    case ARG_UINT:
	{
	    char *endptr;
	    u_int *u = (u_int *)p; 

	    *u = strtoul(kw->value, &endptr, 10);

	    if (*endptr != '\0')
	    {
		plog("# bad integer value: %s=%s", kw->entry->name, kw->value);
		return FALSE;
	    }
	}
	break;
    case ARG_ULNG:
    case ARG_PCNT:
	{
	    char *endptr;
	    unsigned long *l = (unsigned long *)p;

	    *l = strtoul(kw->value, &endptr, 10);

	    if (token_info[token].type == ARG_ULNG)
	    {
		if (*endptr != '\0')
		{
		    plog("# bad integer value: %s=%s", kw->entry->name, kw->value);
		    return FALSE;
		}
	    }
            else
	    {
		if ((*endptr != '%') || (endptr[1] != '\0') || endptr == kw->value)
		{
		    plog("# bad percent value: %s=%s", kw->entry->name, kw->value);
		    return FALSE;
		}
	    }
 
	}
	break;
    case ARG_TIME:
	{
	    char *endptr;
	    time_t *t = (time_t *)p;

	    *t = strtoul(kw->value, &endptr, 10);

	    /* time in seconds? */
	    if (*endptr == '\0' || (*endptr == 's' && endptr[1] == '\0'))
		break;

	    if (endptr[1] == '\0')
	    {
		if (*endptr == 'm')  /* time in minutes? */
		{
		    *t *= 60;
		    break;
		}
		if (*endptr == 'h')  /* time in hours? */
		{
		    *t *= 3600;
		    break;
		}
		if (*endptr == 'd')  /* time in days? */
		{
		    *t *= 3600*24;
		    break;
		}
	    }
	    plog("# bad duration value: %s=%s", kw->entry->name, kw->value);
	    return FALSE;
	}
    case ARG_STR:
	{
	    char **cp = (char **)p;

	    /* free any existing string */
	    pfreeany(*cp);

	    /* assign the new string */
	    *cp = clone_str(kw->value, "str_value");
	}
	break;
    case ARG_LST:
	{
	    char ***listp = (char ***)p;

	    /* free any existing list */
	    if (*listp != NULL)
		free_list(*listp);

	    /* create a new list and assign values */
	    *listp = new_list(kw->value);

	    /* is there a keyword list? */
	    if (list != NULL)
	    {
		char ** lst;

		for (lst = *listp; lst && *lst; lst++) 
		{
		    bool match = FALSE;

		    list = token_info[token].list;
		
		    while (*list != NULL && !match)
		    {
			match = streq(*lst, *list++);
		    }
		    if (!match)
		    {
			plog("# bad value: %s=%s", kw->entry->name, *lst);
			return FALSE;
		    }
		}
	    }
	}
    default:
	return TRUE;
    }

    *assigned = TRUE;
    return TRUE;
}

/*
 *  frees all dynamically allocated arguments in a struct
 */
void
free_args(kw_token_t first, kw_token_t last, char *base)
{
    kw_token_t token;

    for (token = first; token <= last; token++)
    {
	char *p = base + token_info[token].offset;

	switch (token_info[token].type)
	{
	case ARG_STR:
	    {
		char **cp = (char **)p;

		pfreeany(*cp);
		*cp = NULL;
	    }
	    break;
	case ARG_LST:
	    {
		char ***listp = (char ***)p;

		if (*listp != NULL)
		{
		    free_list(*listp);
		    *listp = NULL;
		 }
	    }
	    break;
	default:
	    break;
	}
    }
}

/*
 *  clone all dynamically allocated arguments in a struct
 */
void
clone_args(kw_token_t first, kw_token_t last, char *base1, char *base2)
{
    kw_token_t token;

    for (token = first; token <= last; token++)
    {
	if (token_info[token].type == ARG_STR)
	{
	    char **cp1 = (char **)(base1 + token_info[token].offset);
	    char **cp2 = (char **)(base2 + token_info[token].offset);

	    *cp1 = clone_str(*cp2, "cloned str");
	}
    }
}

static bool
cmp_list(char **list1, char **list2)
{
    if ((list1 == NULL) && (list2 == NULL))
	return TRUE;
    if ((list1 == NULL) || (list2 == NULL))
	return FALSE;

    for ( ; *list1 && *list2; list1++, list2++)
    {
	if (strcmp(*list1,*list2) != 0)
	    return FALSE;
    }

    if ((*list1 != NULL) || (*list2 != NULL))
	return FALSE;

    return TRUE;
}

/*
 *  compare all arguments in a struct
 */
bool
cmp_args(kw_token_t first, kw_token_t last, char *base1, char *base2)
{
    kw_token_t token;

    for (token = first; token <= last; token++)
    {
	char *p1 = base1 + token_info[token].offset;
	char *p2 = base2 + token_info[token].offset;

	switch (token_info[token].type)
	{
	case ARG_ENUM:
	    {
		int *i1 = (int *)p1;
		int *i2 = (int *)p2;

		if (*i1 != *i2)
		    return FALSE;
	    }
	    break;
	case ARG_UINT:
	    {
		u_int *u1 = (u_int *)p1;
		u_int *u2 = (u_int *)p2;

		if (*u1 != *u2)
		    return FALSE;
	    }
	    break;
	case ARG_ULNG:
	case ARG_PCNT:
	    {
		unsigned long *l1 = (unsigned long *)p1;
		unsigned long *l2 = (unsigned long *)p2;

		if (*l1 != *l2)
		    return FALSE;
	    }
	    break;
	case ARG_TIME:
	    {
		time_t *t1 = (time_t *)p1;
		time_t *t2 = (time_t *)p2;

		if (*t1 != *t2)
		    return FALSE;
	    }
	    break;
	case ARG_STR:
	    {
		char **cp1 = (char **)p1;
		char **cp2 = (char **)p2;

		if (*cp1 == NULL && *cp2 == NULL)
		    break;
		if (*cp1 == NULL || *cp2 == NULL || strcmp(*cp1, *cp2) != 0)
		    return FALSE;
	    }
	    break;
	case ARG_LST:
	    {
		char ***listp1 = (char ***)p1;
		char ***listp2 = (char ***)p2;

		if (!cmp_list(*listp1, *listp2))
		    return FALSE;
	    }
	    break;
	default:
	    break;
	}
    }
    return TRUE;
}
