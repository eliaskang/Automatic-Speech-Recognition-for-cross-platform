/*! @brief climax basic definitions.
 * 
 * This file contains the definition of basic types used in the climax
 * module and interface.
 *
 * @file   ClimaxBasic.hpp
 * @date   2009/11/27 8:35
 * @author dirk.buehler
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
#ifndef CLIMAXBASIC_HPP
#define CLIMAXBASIC_HPP


////////////////////////////////////////////////////////////////////////////////
////
//// DEFINES
////


//
#ifndef CLIMAX_DEBUG
#  define CLIMAX_DEBUG 1
#endif


// for the following symbols macros are defined in windows.h; if the customer includes windows.h before including
// the ISS headers we get conflicts, as the preprocessor does not respect namespaces
#undef ERROR_INVALID_STATE
#undef ERROR_INVALID_OPERATION


////////////////////////////////////////////////////////////////////////////////
////
//// INCLUDES
////


//// PAL
#include "SVOX_Types.h"

//// svox::common
#include "Array.hpp"


////////////////////////////////////////////////////////////////////////////////
////
//// DECLARATIONS
////

namespace svox { 

/** 
 ** @brief  A namespace for complex list vocabulary preprocessing. 
 ** 
 **/
namespace climax {





    /** @brief Result code enumeration. 
     **/
    typedef enum 
    { OK = 0,                           /**< Everything worked fine. **/
      ERROR_INVALID_ARGUMENT,           /**< An invalid parameter value was passed to the call. **/
      ERROR_INVALID_STATE,              /**< The respective operation was not allowed in this state (e.g. configuration was already finished). **/
      ERROR_OUT_OF_MEMORY,              /**< Heap space was exhausted during this operation. **/
      ERROR_INCONSISTENT_DATA,          /**< Data supplied as entries is inconsistent (E.g. non-unique ID values. Note: that there is not a guarantee that this will be detected). **/
      ERROR_INVALID_OPERATION,          /**< E.g. no modification allowed, no save allowed **/
      ERROR_VSR,                        /**< An error occurred in the VSR subsystem. **/
      ERROR_ASSERTION_FAILED,           /**< Assertion failed. **/
      ERROR_IO,                         /**< Input/output operation failed (e.g. file access). **/
      ERROR_NOT_FOUND,                  /**< An ID value (e.g. column or token ID) was not found. **/
      ERROR_INTERNAL                    /**< Unexpected error. **/
    } ResultCode;



    /** @brief Column type enumeration. 
     **
     ** These values are used for determining which columns to use when creating a specific knowledge base.
     **/
    typedef enum 
    { COL_GENERIC                       /**< Not associated with a particular use case. **/
    } ColumnType;



    /** @brief  Generic ID type. **/
    typedef SVOX_32S UserID;
    // typedef UINT64 UserID;


    /** @brief  ID type for identifying a Token within a Column. **/
    typedef UserID TokenID;


    /** @brief  ID type for identifying an Entry within a ComplexListManager instance. 
     **/
    // ** [Actually, EntryID could be seen as managed internally by CLM. (Thus, limitation to 24bit seems possible.)]
    typedef UserID EntryID;


    /** @brief  ID type for identifying a Column in a ComplexListManager instance. 
     **/
    typedef SVOX_32S ColumnID;


    /** @brief  ID type for identifying a word (phonetic variant) in a vocabulary. **/
    typedef SVOX_32S WordID;


    /** @brief  ID type for identifying a class within a grammar. **/
    typedef SVOX_32S ClassID;



    /** @brief  List of token ID values. **/
    typedef svox::common::Array<TokenID> TokenIDList;

    /** @brief  List of column ID values. **/
    typedef svox::common::Array<TokenID> ColumnIDList;

    /** @brief  List of word ID values. **/
    typedef svox::common::Array<WordID> WordIDList;



    /** @brief Progress info. 
     **/
    typedef SVOX_32S ProgressInfo;



    /** @brief  Hash value type for spellings. **/
    typedef SVOX_32S HashValue;


    /** @brief  File path type used for loading and storing resources. **/
    typedef const SVOX_Char *FilePath;


////////////////////////////////////////////////////////////////////////////////
////
//// CONSTANTS
////

    /** @brief  The value to be used for indicating that no ::UserID is available. 
     **/
    const UserID CLIMAX_NO_ID = (-1);

    /** @brief  The value to be used for indicating that no word ID is available. 
     **/
    const WordID CLIMAX_NO_WORD_ID = (-1);



////////////////////////////////////////////////////////////////////////////////
////
//// FORWARD DECLARATIONS
////

    class ComplexListManager;

    class G2PConfigurationImpl;


} } // namespaces

#endif

