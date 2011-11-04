/**
 * @brief Math functions
 *
 * This file implements math functions.
 * Using these functions may have animpact
 * on performance on some processors which use
 * intrinsic functions for math operations.
 *
 * Current implementation is for double because
 * the functions in the SVOXPALANSI were all double.
 *
 * Future versions will deliver float too.
 *
 * It is asumed that the linker does not link math libs
 * if none of these functions is used in the product.
 * There was a decision not to use a define to prevent
 * these functions from beeing in the pal because
 * printf and scanf also use double and float...
 *
 * @date $Date: 2010-10-06 10:41:15 +0200 (Wed, 06 Oct 2010) $
 * @author Andreas Loew
 * @file
 *
 * Copyright by SVOX Deutschland GmbH
 *
 * Transmittal, reproduction, dissemination and/or editing of this
 * source code as well as utilization of its contents and communication
 * thereof to others without express authorization are prohibited.
 * Offenders will be held liable for payment of damages. All rights
 * created by patent grant or registration of a utility model or design
 * patent are reserved.
 */

#ifndef __SVOX_MATH_H__
#define __SVOX_MATH_H__

#include "SVOX_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SVOX_HUGE_VAL SVOX_Math_GetHugeVal()
/* -----------------------------------------------------------------------*/

SVOX_Double SVOX_Math_GetHugeVal();
    
/* -----------------------------------------------------------------------*/

SVOX_Double SVOX_sqrt
(
    SVOX_Double x
);

SVOX_Double SVOX_sin
(
    SVOX_Double x
);

SVOX_Double SVOX_cos
(
    SVOX_Double x
);

SVOX_Double SVOX_exp
(
    SVOX_Double x
);

SVOX_Double SVOX_log
(
    SVOX_Double x
);

SVOX_Double SVOX_atan
(
    SVOX_Double x
);

SVOX_Double SVOX_pow
(
    SVOX_Double x,
    SVOX_Double y
);

SVOX_Double SVOX_fabs
(
    SVOX_Double x
);
    

SVOX_Double SVOX_sinh
(
    SVOX_Double x
);
    

SVOX_Double SVOX_cosh
(
    SVOX_Double x
);
    

SVOX_Double SVOX_tan
(
    SVOX_Double x
);
    

SVOX_Double SVOX_tanh
(
    SVOX_Double x
);
    

SVOX_Double SVOX_asin
(
    SVOX_Double x
);
    

SVOX_Double SVOX_acos
(
    SVOX_Double x
);
    

SVOX_Double SVOX_atan2
(
    SVOX_Double x,
    SVOX_Double y
);
    

SVOX_Double SVOX_ceil
(
    SVOX_Double x
);
    

SVOX_Double SVOX_floor
(
    SVOX_Double x
);
    

SVOX_Double SVOX_fmod
(
    SVOX_Double x,
    SVOX_Double y
);
    

SVOX_Double SVOX_modf
(
    SVOX_Double x,
    SVOX_Double *i
);
    

SVOX_Double SVOX_log10
(
    SVOX_Double x
);
    

SVOX_Double SVOX_frexp
(
    SVOX_Double x,
    SVOX_32S *e
);
    

SVOX_Double SVOX_ldexp
(
    SVOX_Double x,
    SVOX_32S e
);
    

SVOX_32S SVOX_abs
(
    SVOX_32S x
);

/**
 * Checks if the value is the biggest possible SVOX_double value.
 * Use this function to check for invalid value results
 * e.g. SVOX_strtod
 *
 * @param[in] x value to check
 *
 * @retval -1 value is -HUGE_VAL
 * @retval 1 value is HUGE_VAL
 * @retval 0 value is not huge
 */
SVOX_32S SVOX_isHugeVal
(
    SVOX_Double x
);

/* -----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __SVOX_MATH_H__ */

