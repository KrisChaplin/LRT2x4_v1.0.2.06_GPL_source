/* Test gmp_urandomb_ui.

Copyright 2003, 2005 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */

#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"
#include "gmp-impl.h"
#include "tests.h"

/* Expect numbers generated by rstate to obey the number of bits requested.
   No point testing bits==BITS_PER_ULONG, since any return is acceptable in
   that case.  */
void
check_one (const char *name, gmp_randstate_ptr rstate)
{
  unsigned long  bits, limit, got;
  int    i;

  for (bits = 0; bits < BITS_PER_ULONG; bits++)
    {
      /* will demand got < limit */
      limit = (1L << bits);

      for (i = 0; i < 5; i++)
        {
          got = gmp_urandomb_ui (rstate, bits);
          if (got >= limit)
            {
              printf ("Return value out of range:\n");
              printf ("  algorithm: %s\n", name);
              printf ("  bits:  %lu\n", bits);
              printf ("  limit: %#lx\n", limit);
              printf ("  got:   %#lx\n", got);
              abort ();
            }
        }
    }
}

int
main (int argc, char *argv[])
{
  tests_start ();

  call_rand_algs (check_one);

  tests_end ();
  exit (0);
}
