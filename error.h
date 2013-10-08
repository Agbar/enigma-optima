#ifndef ERROR_H
#define ERROR_H

#include "global.h"

enum { ERR_A_ONLY, ERR_EXCL_A, ERR_RING_SHORTCUT };

NO_RETURN
void err_alloc_fatal(const char *s);
NO_RETURN
void err_open_fatal(const char *s);
NO_RETURN
void err_open_fatal_resume(const char *s);
NO_RETURN
void err_stream_fatal(const char *s);
NO_RETURN
void err_illegal_char_fatal(const char *s);
NO_RETURN
void err_sigaction_fatal(int signum);
NO_RETURN
void err_input_fatal(int type);

/* log message */
void hillclimb_log(const char *s);

#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
