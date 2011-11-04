/**
 * @brief Platform defines
 *
 * This is where platforms are defind and which.
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

#ifdef SVOX_PLATFORM_CLEANUP_DEFINES

/*
 * This is used in SVOX_Types.h to remove the platform defines.
 * SVOX_Types.h is the only file allowed to include SVOX_Platform.h
 */
#undef SVOX_PLATFORM
#undef SVOX_PLATFORM_STRING
#undef SVOX_PLATFORM_THREADING
#undef SVOX_PLATFORM_THREADING_WIN32

#undef SVOX_PLATFORM_Windows
#undef SVOX_PLATFORM_WinCE
#undef SVOX_PLATFORM_Darwin
#undef SVOX_PLATFORM_Solaris
#undef SVOX_PLATFORM_Linux
#undef SVOX_PLATFORM_Symbian
#undef SVOX_PLATFORM_QNX
#undef SVOX_PLATFORM_VxWorks
#undef SVOX_PLATFORM_iPhone
#undef SVOX_PLATFORM_ADS
#undef SVOX_PLATFORM_XANAVI
#undef SVOX_PLATFORM_Android
#undef SVOX_PLATFORM_CYGWIN
#undef SVOX_PLATFORM_GENERIC

#undef __SVOX_PLATFORM_H__

#else

#ifndef __SVOX_PLATFORM_H__
#define __SVOX_PLATFORM_H__

#ifndef SVOX_PAL_IMPLEMENTATION_FILE
#error "SVOX_Platform.h must not be used outside PAL source files!"
#endif

#define SVOX_PLATFORM_Windows 1 /*!< Windows: 2000, XP, Vista, Windows 7 */
#define SVOX_PLATFORM_WinCE   2 /*!< Windows CE */
#define SVOX_PLATFORM_QNX     3 /*!< QNX */
#define SVOX_PLATFORM_Linux   4 /*!< Linux */
#define SVOX_PLATFORM_Darwin  5 /*!< Darwin / MacOSX */
#define SVOX_PLATFORM_iPhone  6 /*!< MacOSX auf iPhone / iPod */
#define SVOX_PLATFORM_ADS     7 /*!< ARM Developer Suite */
#define SVOX_PLATFORM_Android 8 /*!< ANDROID */
#define SVOX_PLATFORM_CYGWIN  9 /*!< Cygwin */
#define SVOX_PLATFORM_Symbian 10 /*!< Symbian */

/*
 * -------------------------
 * Definitions for threading
 * -------------------------
 */
#define SVOX_PLATFORM_THREADING_NONE 0    /*!< Platform does not use threads */
#define SVOX_PLATFORM_THREADING_PTHREAD 1 /*!< Platform uses PThreads */
#define SVOX_PLATFORM_THREADING_WINDOWS 2 /*!< Platform uses Windows native threads */

/*
 * ------------------------
 * Definitions for Atomics
 * ------------------------
 */
#define SVOX_PLATFORM_ATOMIC_NONE 0
#define SVOX_PLATFORM_ATOMIC_WINDOWS 1
#define SVOX_PLATFORM_ATOMIC_GCC 2
#define SVOX_PLATFORM_ATOMIC_MACOS 3
#define SVOX_PLATFORM_ATOMIC_QNX 4


/*
 * Set the external settings to the internal platform defines
 */
#if defined(SVOX_BUILD_FOR_Windows)

#define SVOX_PLATFORM SVOX_PLATFORM_Windows
#define SVOX_PLATFORM_STRING "Windows"
#define SVOX_PLATFORM_THREADING SVOX_PLATFORM_THREADING_WINDOWS
#define SVOX_PLATFORM_ATOMIC SVOX_PLATFORM_ATOMIC_WINDOWS

#elif defined(SVOX_BUILD_FOR_WinCE)

#define SVOX_PLATFORM_STRING "WinCE"
#define SVOX_PLATFORM SVOX_PLATFORM_WinCE
#define SVOX_PLATFORM_THREADING SVOX_PLATFORM_THREADING_WINDOWS
#define SVOX_PLATFORM_ATOMIC SVOX_PLATFORM_ATOMIC_WINDOWS

#elif defined(SVOX_BUILD_FOR_QNX)

#define SVOX_PLATFORM_STRING "QNX"
#define SVOX_PLATFORM_THREADING SVOX_PLATFORM_THREADING_PTHREAD
#define SVOX_PLATFORM SVOX_PLATFORM_QNX
#define SVOX_PLATFORM_ATOMIC SVOX_PLATFORM_ATOMIC_QNX

#elif defined(SVOX_BUILD_FOR_Linux)

#define SVOX_PLATFORM_STRING "Linux"
#define SVOX_PLATFORM_THREADING SVOX_PLATFORM_THREADING_PTHREAD
#define SVOX_PLATFORM SVOX_PLATFORM_Linux
#define SVOX_PLATFORM_ATOMIC SVOX_PLATFORM_ATOMIC_GCC

#elif defined(SVOX_BUILD_FOR_Android)

#define SVOX_PLATFORM_STRING "Android"
#define SVOX_PLATFORM_THREADING SVOX_PLATFORM_THREADING_PTHREAD
#define SVOX_PLATFORM SVOX_PLATFORM_Android
#define SVOX_PLATFORM_ATOMIC SVOX_PLATFORM_ATOMIC_GCC

#elif defined(SVOX_BUILD_FOR_Darwin)

#define SVOX_PLATFORM_STRING "Darwin"
#define SVOX_PLATFORM_THREADING SVOX_PLATFORM_THREADING_PTHREAD
#define SVOX_PLATFORM SVOX_PLATFORM_Darwin
#define SVOX_PLATFORM_ATOMIC SVOX_PLATFORM_ATOMIC_MACOS

#elif defined(SVOX_BUILD_FOR_iPhone)

#define SVOX_PLATFORM_STRING "iPhone"
#define SVOX_PLATFORM_THREADING SVOX_PLATFORM_THREADING_PTHREAD
#define SVOX_PLATFORM SVOX_PLATFORM_iPhone
#define SVOX_PLATFORM_ATOMIC SVOX_PLATFORM_ATOMIC_MACOS

#elif defined(SVOX_BUILD_FOR_ADS)

#define SVOX_PLATFORM_STRING "ADS"
#define SVOX_PLATFORM_THREADING SVOX_PLATFORM_THREADING_NONE
#define SVOX_PLATFORM SVOX_PLATFORM_ADS
#define SVOX_PLATFORM_ATOMIC SVOX_PLATFORM_ATOMIC_NONE

#elif defined(SVOX_BUILD_FOR_CYGWIN)

#define SVOX_PLATFORM_STRING "CYGWIN"
#define SVOX_PLATFORM_THREADING SVOX_PLATFORM_THREADING_PTHREAD
#define SVOX_PLATFORM SVOX_PLATFORM_CYGWIN
#define SVOX_PLATFORM_ATOMIC SVOX_PLATFORM_ATOMIC_GCC

#elif defined(SVOX_BUILD_FOR_Symbian)

#error "This is just a template, not tested or based on first hand experience edit SVOX_Platform.h"

#define SVOX_PLATFORM_STRING "Symbian"
#define SVOX_PLATFORM_THREADING SVOX_PLATFORM_THREADING_PTHREAD
#define SVOX_PLATFORM SVOX_PLATFORM_Symbian
#define SVOX_PLATFORM_ATOMIC SVOX_PLATFORM_ATOMIC_GCC

#else
#error "Please set SVOX_BUILD_FOR_* to the Platform you want to build for."
#endif

#define SVOX_PLATFORM_IS_WINDOWS ((SVOX_PLATFORM == SVOX_PLATFORM_Windows) || (SVOX_PLATFORM == SVOX_PLATFORM_WinCE))
#define SVOX_PLATFORM_IS_DARWIN ((SVOX_PLATFORM == SVOX_PLATFORM_Darwin) || (SVOX_PLATFORM == SVOX_PLATFORM_iPhone))
#define SVOX_PLATFORM_IS_UNIX                                                           \
    ((SVOX_PLATFORM == SVOX_PLATFORM_Linux) || (SVOX_PLATFORM == SVOX_PLATFORM_Darwin)  \
     || (SVOX_PLATFORM == SVOX_PLATFORM_QNX) || (SVOX_PLATFORM == SVOX_PLATFORM_iPhone) \
     || (SVOX_PLATFORM == SVOX_PLATFORM_CYGWIN) || (SVOX_PLATFORM == SVOX_PLATFORM_Android))


#endif

#endif

