/*
 * Copyright (c) 2009 by
 * Ralf Corsépius, Ulm, Germany. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * is freely granted, provided that this notice is preserved.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int
main (void)
{
  struct stat buf;
  int status;

  status = lstat ("/tmp/foo", &buf);

  return 0;
}
