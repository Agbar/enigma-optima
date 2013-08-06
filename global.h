#ifndef GLOBAL_H
#define GLOBAL_H

#define WINDOWS

#define H 0
#define M3 1
#define M4 2

#define NLINE1 76
#define NLINE2 58
#define CT 250
#define NCT CT+3
#define TICKET 21
#define NTICKET 34
#define NDIALOG 18

#ifndef DBL_EPSILON
#define DBL_EPSILON 0.000000001
#endif

#include <limits.h>
#if INT_MAX < 0x7FFFFFFFL
#error "Only platforms with 32 bits or more are supported."
#endif


enum { SW_ONSTART, SW_OTHER, SW_ALL, SW_NONE, SINGLE_KEY };


// copy from x264
#ifdef __ICL
#define DECLARE_ALIGNED( var, n ) __declspec(align(n)) var
#else
#define DECLARE_ALIGNED( var, n ) var __attribute__((aligned(n)))
#endif
#define ALIGNED_32( var ) DECLARE_ALIGNED( var, 32 )
#define ALIGNED_16( var ) DECLARE_ALIGNED( var, 16 )
#define ALIGNED_8( var )  DECLARE_ALIGNED( var, 8 )
#define ALIGNED_4( var )  DECLARE_ALIGNED( var, 4 )


#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
