/*
    The GNU C Library is free software.  See the file COPYING.LIB for copying
    conditions, and LICENSES for notices about a few contributions that require
    these additional notices to be distributed.  License copyright years may be
    listed using range notation, e.g., 2000-2011, indicating that every year in
    the range, inclusive, is a copyrightable year that would otherwise be listed
    individually.
*/

#pragma once

#include <endian.h>
#include <pthread.h>

struct drand48_data {
    unsigned short int __x[3];	/* Current state.  */
    unsigned short int __old_x[3]; /* Old state.  */
    unsigned short int __c;	/* Additive const. in congruential formula.  */
    unsigned short int __init;	/* Flag for initializing.  */
    unsigned long long int __a;	/* Factor in congruential formula.  */
};

union ieee754_double
{
    double d;

    /* This is the IEEE 754 double-precision format.  */
    struct
    {
#if	__BYTE_ORDER == __BIG_ENDIAN
        unsigned int negative:1;
        unsigned int exponent:11;
        /* Together these comprise the mantissa.  */
        unsigned int mantissa0:20;
        unsigned int mantissa1:32;
#endif				/* Big endian.  */
#if	__BYTE_ORDER == __LITTLE_ENDIAN
        /* Together these comprise the mantissa.  */
        unsigned int mantissa1:32;
        unsigned int mantissa0:20;
        unsigned int exponent:11;
        unsigned int negative:1;
#endif				/* Little endian.  */
    } ieee;

    /* This format makes it easier to see if a NaN is a signalling NaN.  */
    struct
    {
#if	__BYTE_ORDER == __BIG_ENDIAN
        unsigned int negative:1;
        unsigned int exponent:11;
        unsigned int quiet_nan:1;
        /* Together these comprise the mantissa.  */
        unsigned int mantissa0:19;
        unsigned int mantissa1:32;
#else
        /* Together these comprise the mantissa.  */
        unsigned int mantissa1:32;
        unsigned int mantissa0:19;
        unsigned int quiet_nan:1;
        unsigned int exponent:11;
        unsigned int negative:1;
#endif
    } ieee_nan;
};

#define IEEE754_DOUBLE_BIAS	0x3ff /* Added to exponent.  */

int drand48_r (struct drand48_data *buffer, double *result);
int lrand48_r (struct drand48_data *buffer, long int *result);
int mrand48_r (struct drand48_data *buffer, long int *result);
int srand48_r (long int seedval, struct drand48_data *buffer);
