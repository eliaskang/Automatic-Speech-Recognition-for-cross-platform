/*! \brief LogServer declaration.
 * 
 * This file contains the declaration of the LogServer.
 *
 * @file   LogServer.hpp
 * @date   2010/05/11 13:03
 * @author joachim.grill
 *
 * Copyright by SVOX Deutschland GmbH
 *
 * Transmittal, reproduction, dissemination and/or editing of this
 * source code as well as utilization of its contents and communication
 * thereof to others without express authorization are prohibited.
 * Offenders will be held liable for payment of damages.  All rights
 * created by patent grant or registration of a utility model or design
 * patent are reserved.
 */

#ifndef SVOX_COMMON_LOGSERVER_HPP
#define SVOX_COMMON_LOGSERVER_HPP

#include "SVOX_Types.h"
#include "SVOX_Mutex.h"
#include "Array.hpp"


namespace svox {
namespace common {

class LogListener;


/*** LogServer class declaration **************************************************************************************/

/**
 *  This class receives the messages from all LogClient objects. It passes the log messages to all registered listeners.
 *  If no client is registered, WARNING and ERROR messages are printed to stderr.
 */
class LogServer
{
public:
    enum Level
    {
        LOG_DEBUG,      //!< debug output, only interesting for SVOX ISS software developers
        LOG_INFO,       //!< information messages, e.g. status infos for the customer using the ISS SDK
        LOG_WARNING,    //!< warning: a problem occurred, but the functionality is still available
        LOG_ERROR       //!< an error occurred, a function or use-case fails
    };

    /**
     *  Registers an object which should receive all log messages. As long as not listener object is registered with
     *  this method, the log messages are displayed using printf on stderr.
     *
     *  @param listener a pointer to a class derived from LogListener
     *
     */
    static void registerListener(LogListener *listener);

    /**
     *  De-registers a previously registered LogListener instance.
     *
     *  @param listener a pointer to a class derived from LogListener
     *
     */
    static void deregisterListener(LogListener *listener);

private:
    LogServer();
    ~LogServer();

    static LogServer *getInstance();
    static void destroyInstance();

    static void logMessage(Level level, const SVOX_Char *module, const SVOX_Char *file, SVOX_32S line,
                           const SVOX_Char *message, SVOX_va_list argList);

    static LogServer *volatile mInstance;
    Array<LogListener *> mListeners;
    SVOX_Mutex *mMutex;

    friend class LogClient;
};


/*** Abstract listener interface **************************************************************************************/
    
/**
 *  The customer can derive from this class and implement the pure virtual method. If the implementation is registered
 *  at the LogServer class, the method is called for each log message.
 */
class LogListener
{
public:
    virtual ~LogListener();

    /**
     *  This method is called by the the LogServer class for each log message. Its called directly by the thread which
     *  generated the message without any queuing, i.e. the implementation of this method should run very fast.
     *
     *  @param level   the severity of the log message
     *  @param module  the name of the module which generated the message
     *  @param file    the source code file name where the message is generated
     *  @param line    the line number within the source code file
     *  @param message the log message itself; it may contain argument placeholders in printf syntax
     *  @param argList the arguments which should be inserted in the message
     *
     */
    virtual void logMessage(LogServer::Level level, const SVOX_Char *module, const SVOX_Char *file, SVOX_32S line,
                            const SVOX_Char *message, SVOX_va_list argList) = 0;
};


} } // namespaces

#endif // SVOX_COMMON_LOGSERVER_HPP

/**********************************************************************************************************************/
