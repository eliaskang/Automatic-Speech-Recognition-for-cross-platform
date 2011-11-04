/**
 * @brief setjmp/longjmp
 *
 * SetJump & LongJump
 *
 * This is currently the only file which realy uses external headers
 * because on some platforms the implementation is too big to
 * be copied here.
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


#ifndef __SVOX_SETJMP_H__
#define __SVOX_SETJMP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <setjmp.h>

typedef jmp_buf SVOX_jmp_buf;

#define SVOX_setjmp(env) setjmp(env)
#define SVOX_longjmp(env, val) longjmp((env), (val))

#ifdef __cplusplus
}
#endif

#endif

