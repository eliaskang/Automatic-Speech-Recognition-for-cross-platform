/**
 * @brief Timing and timer functions
 *
 * This file contains functions for time measurement,
 * sleeping, getting date and time.
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

#ifndef __SVOX_TIME_H__
#define __SVOX_TIME_H__

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------*/

#include "SVOX_Types.h"

/**
 * Waits at least for the given number of microseconds.
 * Depeding on the system the period may be longer.
 *
 * Maximum value for microseconds is 1.000.000
 *
 * @warning Do not use SVOX_usleep(0) to force a thread change.
 * If you want to suspend the current time slice of a thread
 * use SVOX_Thread_yield() instead. On some systems a call to
 * SVOX_usleep(0) is simply ignored!
 *
 * @warning Some systems only support millisecond granularity.
 *          Values are rounded up to the next millisecond value.
 *
 * @see SVOX_Thread_yield
 *
 * @param[in] microseconds Number of microseconds to wait
 */
void SVOX_usleep
(
    SVOX_32S microseconds
);

/**
 * Waits at lease for the given number of seconds.
 *
 * For finer granularity use SVOX_usleep.
 *
 * @param[in] seconds number of seconds to sleep
 *
 * @see SVOX_usleep
 */
void SVOX_sleep
(
    SVOX_32S seconds
);

#ifdef NOT_YET_IMPLEMENTED
#define SVOX_CLOCKS_PER_SEC 1000000

/**
 * Returns the number of cpu clocks used since the program was
 * started.
 *
 * To get the number of seconds divide the value by
 * SVOX_CLOCKS_PER_SEC
 *
 * Granulatiry of the value may change from system to system.
 * On unix it us usually microsconds, windows only milliseconds
 *
 * @return number of milliseconds since the system was started
 */
SVOX_32U SVOX_clock
(
    void
);

#endif


/**
 * Returns a time in ms.
 *
 * The value must not be used as absolute time but
 * for time measurement using the difference of two
 * values.
 *
 * Overflow of the value occurs about every 50 days.
 * But since the starting value of the timer
 * is not known the overflow may occur every time.
 *
 * Use SVOX_Time_elapsed to calculate the difference
 * between 2 time stamps with automatic overflow
 * correction
 *
 * @see SVOX_Time_elapsed
 * @retval ms
 */
SVOX_32U SVOX_Time_GetMilliseconds
    ();


/**
 * Returns the difference between two points in time,
 * correcting overflows
 *
 * @param[in] before the first time value
 * @param[in] after the second time value
 *
 * @retval the difference between the two time points
 */
SVOX_32U SVOX_Time_Difference
(
    SVOX_32U before,
    SVOX_32U after
);

/**
 * Returns the current date
 *
 * You may pass in NULL pointers for fields that you are not interested in.
 *
 * @param[out] day pointer to a SVOX_32S in which the day is written [1..31]
 * @param[out] month pointer to a SVOX_32S in which the month is written [1..12]
 * @param[out] year pointer to a SVOX_32S in which the year is written [1601...32827]
 *
 * @retval SVOX_RESULT_OK     ok
 * @retval SVOX_RESULT_ERROR  not ok, parameters filled with 0 are returned
 */
SVOX_Result SVOX_Time_GetDate
(
    SVOX_32S *day,
    SVOX_32S *month,
    SVOX_32S *year
);

/**
 * Returns the current time
 *
 * You may pass in NULL pointers for fields that you are not interested in.
 *
 * @param[out] hour pointer to a SVOX_32S in which the hours are written [0..23]
 * @param[out] minute pointer to a SVOX_32S in which the minutes  are written [0..59]
 * @param[out] second pointer to a SVOX_32S in which the seconds are written [0...59(60)] 60 only for leap seconds
 * @param[out] dst pointer to a SVOX_32S which is 0 if no dst is active, positive if active, -1 if information is not available
 *
 * @retval SVOX_RESULT_OK ok
 * @retval SVOX_RESULT_ERROR  not ok, parameters filled with 0 are returned, except dst which is -1 in this case
 */
SVOX_Result SVOX_Time_GetTime
(
    SVOX_32S  *hour,
    SVOX_32S  *minute,
    SVOX_32S  *second,
    SVOX_32S  *dst
);
    
/**
 * Returns the current date and time
 *
 * If any of the result pointers is null, it will be ignored. 
 *
 * @param[out] day pointer to a SVOX_32S in which the day is written [1..31]
 * @param[out] month pointer to a SVOX_32S in which the month is written [1..12]
 * @param[out] year pointer to a SVOX_32S in which the year is written [1601...32827]
 * @param[out] hour pointer to a SVOX_32S in which the hours are written [0..23]
 * @param[out] minute pointer to a SVOX_32S in which the minutes  are written [0..59]
 * @param[out] second pointer to a SVOX_32S in which the seconds are written [0...59(60)] 60 only for leap seconds
 * @param[out] dst pointer to a SVOX_32S which is 0 if no dst is active, positive if active, negative if information is not available
 *
 * @retval SVOX_RESULT_OK ok
 * @retval SVOX_RESULT_ERROR  not ok, parameters filled with 0 are returned, except dst which is -1 in this case
 */
SVOX_Result SVOX_Time_GetDateTime
(
    SVOX_32S  *day,
    SVOX_32S  *month,
    SVOX_32S  *year,
    SVOX_32S  *hour,
    SVOX_32S  *minute,
    SVOX_32S  *second,
    SVOX_32S  *dst
);

/* -----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

