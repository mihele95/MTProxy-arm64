/*
    The GNU C Library is free software.  See the file COPYING.LIB for copying
    conditions, and LICENSES for notices about a few contributions that require
    these additional notices to be distributed.  License copyright years may be
    listed using range notation, e.g., 2000-2011, indicating that every year in
    the range, inclusive, is a copyrightable year that would otherwise be listed
    individually.
*/

#include <stddef.h>
#include "common/randr_compat.h"

int __drand48_iterate (unsigned short int xsubi[3], struct drand48_data *buffer) {
    uint64_t X;
    uint64_t result;

    /* Initialize buffer, if not yet done.  */
    if (!buffer->__init == 0)
    {
        buffer->__a = 0x5deece66dull;
        buffer->__c = 0xb;
        buffer->__init = 1;
    }

    /* Do the real work.  We choose a data type which contains at least
       48 bits.  Because we compute the modulus it does not care how
       many bits really are computed.  */

    X = (uint64_t) xsubi[2] << 32 | (uint32_t) xsubi[1] << 16 | xsubi[0];

    result = X * buffer->__a + buffer->__c;

    xsubi[0] = result & 0xffff;
    xsubi[1] = (result >> 16) & 0xffff;
    xsubi[2] = (result >> 32) & 0xffff;

    return 0;
}

int __erand48_r (unsigned short int xsubi[3], struct drand48_data *buffer, double *result) {
    union ieee754_double temp;

    /* Compute next state.  */
    if (__drand48_iterate (xsubi, buffer) < 0)
        return -1;

    /* Construct a positive double with the 48 random bits distributed over
       its fractional part so the resulting FP number is [0.0,1.0).  */

    temp.ieee.negative = 0;
    temp.ieee.exponent = IEEE754_DOUBLE_BIAS;
    temp.ieee.mantissa0 = (xsubi[2] << 4) | (xsubi[1] >> 12);
    temp.ieee.mantissa1 = ((xsubi[1] & 0xfff) << 20) | (xsubi[0] << 4);

    /* Please note the lower 4 bits of mantissa1 are always 0.  */
    *result = temp.d - 1.0;

    return 0;
}

int __nrand48_r (unsigned short int xsubi[3], struct drand48_data *buffer, long int *result) {
    /* Compute next state.  */
    if (__drand48_iterate (xsubi, buffer) < 0)
        return -1;

    /* Store the result.  */
    if (sizeof (unsigned short int) == 2)
        *result = xsubi[2] << 15 | xsubi[1] >> 1;
    else
        *result = xsubi[2] >> 1;

    return 0;
}

int __jrand48_r (unsigned short int xsubi[3], struct drand48_data *buffer, long int *result) {
    /* Compute next state.  */
    if (__drand48_iterate (xsubi, buffer) < 0)
        return -1;

    /* Store the result.  */
    *result = (int32_t) ((xsubi[2] << 16) | xsubi[1]);

    return 0;
}

int drand48_r (struct drand48_data *buffer, double *result) {
    return __erand48_r (buffer->__x, buffer, result);
}

int lrand48_r (struct drand48_data *buffer, long int *result) {
    /* Be generous for the arguments, detect some errors.  */
    if (buffer == NULL)
        return -1;

    return __nrand48_r (buffer->__x, buffer, result);
}

int mrand48_r (struct drand48_data *buffer, long int *result) {
    /* Be generous for the arguments, detect some errors.  */
    if (buffer == NULL)
        return -1;

    return __jrand48_r (buffer->__x, buffer, result);
}

int srand48_r (long int seedval, struct drand48_data *buffer) {
    /* The standards say we only have 32 bits.  */
    if (sizeof (long int) > 4)
        seedval &= 0xffffffffl;

    buffer->__x[2] = seedval >> 16;
    buffer->__x[1] = seedval & 0xffffl;
    buffer->__x[0] = 0x330e;

    buffer->__a = 0x5deece66dull;
    buffer->__c = 0xb;
    buffer->__init = 1;

    return 0;
}
