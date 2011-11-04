/*
 *  SVOX_ErrorCodes.h
 *  aurioTouch
 *
 *  Created by Andreas Löw on 09.11.09.
 *  Copyright 2009 SVOX. All rights reserved.
 *
 */

#ifndef __SVOX_CONFIGFILE_H__
#define __SVOX_CONFIGFILE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------*/

#include "SVOX_Types.h"

typedef enum
    {
        SVOX_CONFIG_VALUE_INT,
        SVOX_CONFIG_VALUE_FLOAT,
        SVOX_CONFIG_VALUE_BOOL,
        SVOX_CONFIG_VALUE_TABLE,
        SVOX_CONFIG_VALUE_STRING
    } SVOX_ConfigFile_ValueType;
    
/**
 */
struct SVOX_ConfigFile_;

/**
 * Handle which keeps the thread data.
 */
typedef struct SVOX_ConfigFile_ SVOX_ConfigFile;

/**
 * Iterator for browsing through lists and structs
 */
typedef struct SVOX_ConfigFile_Iterator_ SVOX_ConfigFile_Iterator;
    
#define SVOX_CONFIGFILE_ROOTITERATOR 0
    
    /**
     * Opens a config file from a string
     *
     * @param[in] data string containing the config file
     * @param[out] configPtr Pointer to a variable 
     *
     * @retval SVOX_RESULT_OK if ok
     *
     * @remark Use SVOX_ConfigFile_GetLastErrorString to get an error message
     */
    SVOX_Result SVOX_ConfigFile_OpenFromString
(
    const SVOX_Char *data,
    SVOX_ConfigFile **configPtr
);

    /**
     * Opens a config file from the file system
     *
     * @param[in] fileName name of the file
     * @param[out] configPtr Pointer to a variable 
     *
     * @retval SVOX_RESULT_OK if ok
     *
     * @remark Use SVOX_ConfigFile_GetLastErrorString to get an error message
     */
SVOX_Result SVOX_ConfigFile_Open
(
    const SVOX_Char *fileName,
    SVOX_ConfigFile **configPtr
);

    /**
     * Closes the config file handle and releases all memory
     * @param[in] configFile Config File
     */    
void SVOX_ConfigFile_Close
(
    SVOX_ConfigFile *configFile
);

    /**
     * Returns a value as interger
     * 
     * This function does not convert the value. If the type of the value
     * is not integer SVOX_RESULT_ERROR_INVALID is returned.
     *
     * @param[in] configFile configFile
     * @param[in] expression expression to evaluate
     * @param[out] valuePtr pointer to a SVOX_32S variable in which the result is stored
     *
     * @retval SVOX_RESULT_OK ok
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 configFile is NULL
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 expression is NULL
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_3 intPtr is NULL
     * @retval SVOX_RESULT_ERROR_INVALID type conflict (in expression or result)
     * @retval SVOX_RESULT_ERROR_NOT_FOUND value was not found
     * @retval SVOX_RESULT_ERROR_SYNTAX expression is not valid
     *
     * @remark Use SVOX_ConfigFile_GetLastErrorString to get an error message
     *
     */
SVOX_Result SVOX_ConfigFile_GetInteger
(
    SVOX_ConfigFile *configFile, const SVOX_Char *expression, SVOX_32S *valuePtr
);

    /**
     * Returns a value as float
     * 
     * This function converts int values to float.
     *
     * @param[in] configFile configFile
     * @param[in] expression expression to evaluate
     * @param[out] valuePtr pointer to a SVOX_Float variable in which the result is stored
     *
     * @retval SVOX_RESULT_OK ok
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 configFile is NULL
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 expression is NULL
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_3 intPtr is NULL
     * @retval SVOX_RESULT_ERROR_INVALID type conflict (in expression or result)
     * @retval SVOX_RESULT_ERROR_NOT_FOUND value was not found
     * @retval SVOX_RESULT_ERROR_SYNTAX expression is not valid
     *
     * @remark Use SVOX_ConfigFile_GetLastErrorString to get an error message
     *
     */
    SVOX_Result SVOX_ConfigFile_GetFloat
    (
     SVOX_ConfigFile *configFile, const SVOX_Char *expression, SVOX_Float *valuePtr
     );
    
    /**
     * Returns a value as string
     * 
     * This function does not convert the value. If the type of the value
     * is not string SVOX_RESULT_ERROR_INVALID is returned.
     * 
     * Memory management is inside ConfigFile, do not release the 
     * memory.
     *
     * The pointer is valid until the SVOX_ConfigFile_Close.
     *
     * @param[in] configFile configFile
     * @param[in] expression expression to evaluate
     * @param[out] valuePtr pointer to a SVOX_Char * variable in which the result is stored
     *
     * @retval SVOX_RESULT_OK ok
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 configFile is NULL
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 expression is NULL
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_3 intPtr is NULL
     * @retval SVOX_RESULT_ERROR_INVALID type conflict (in expression or result)
     * @retval SVOX_RESULT_ERROR_NOT_FOUND value was not found
     * @retval SVOX_RESULT_ERROR_SYNTAX expression is not valid
     *
     * @remark Use SVOX_ConfigFile_GetLastErrorString to get an error message
     *
     */
    SVOX_Result SVOX_ConfigFile_GetString
    (
     SVOX_ConfigFile *configFile, const SVOX_Char *expression, const SVOX_Char **valuePtr
     );
    
    
    /**
     * Returns a value as bool
     *
     * This function does not convert the value. If the type of the value
     * is not bool SVOX_RESULT_ERROR_INVALID is returned.
     * 
     * @param[in] configFile configFile
     * @param[in] expression expression to evaluate
     * @param[out] valuePtr pointer to a SVOX_Bool variable in which the result is stored
     *
     * @retval SVOX_RESULT_OK ok
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 configFile is NULL
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 expression is NULL
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_3 intPtr is NULL
     * @retval SVOX_RESULT_ERROR_INVALID type conflict (in expression or result)
     * @retval SVOX_RESULT_ERROR_NOT_FOUND value was not found
     * @retval SVOX_RESULT_ERROR_SYNTAX expression is not valid
     *
     * @remark Use SVOX_ConfigFile_GetLastErrorString to get an error message
     *
     */
    SVOX_Result SVOX_ConfigFile_GetBool
    (
     SVOX_ConfigFile *configFile, const SVOX_Char *expression, SVOX_Bool *valuePtr
     );
    
    
    /**
     * Get an iterator to the config file.
     *
     * The iterator is handled by the config file module. There is no
     * need in releasing it.
     *
     * The iterator is NULL in case of an error e.g. if the 
     * varPath does not point to a table element or if
     * the list element is empty.
     *
     * Using SVOX_CONFIGFILE_ROOTITERATOR as variablePath returns the
     * root iterator.
     *
     * The iterator is valid until SVOX_ConfigFile_Close.
     *
     * @param[in] conf configFile to get the iterator for
     * @param[in] variablePath to get the iterator for, SVOX_CONFIGFILE_ROOTITERATOR for the root element
     * @param[out] iteratorPtr pointer in which the iterator is stored,
     *             the value is NULL if the varPath does not point
     *             to a table or if the table is empty.
     *
     * @retval SVOX_RESULT_OK ok
     * @retval SVOX_RESULT_AT_END the table element is empty
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 configFile is NULL
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_3 interatorPtr is NULL
     * @retval SVOX_RESULT_ERROR_SYNTAX syntax error in varPath
     * @retval SVOX_RESULT_ERROR_NOT_FOUND the table element does not exist
     * @retval SVOX_RESULT_ERROR_INVALID the element is found but the type is not table
     *
     */
    SVOX_Result SVOX_ConfigFile_GetIterator
    (
     SVOX_ConfigFile *conf, const SVOX_Char *variablePath, const SVOX_ConfigFile_Iterator **iteratorPtr
     );
    
    /**
     * Sets the iterator to the next element
     * 
     * Sets the value pointed to to NULL if the iterator is at the end of the list.
     *
     * @param[in,out] iteratorPtr pointer to an iterator 
     *
     * @retval SVOX_RESULT_OK ok
     * @retval SVOX_RESULT_AT_END if the list is at the end or the value pointed to is NULL
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 if iteratorPtr is NULL
     */
    SVOX_Result SVOX_ConfigFile_Iterator_Next
    (
        const SVOX_ConfigFile_Iterator **iteratorPtr
    );

    /**
     * Returns the name of the element to which the iterator points.
     *
     * ConfigFile handles the key, no need to release the key's memory.
     *
     * @param[in] iteratorPtr Iterator
     * @param[out] key pointer in which a pointer to the key string is stored
     *
     * @retval SVOX_RESULT_OK ok
     * @retval SVOX_RESULT_AT_END iterator is NULL or at the end of the list
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 typePtr is NULL
     */  
   SVOX_Result SVOX_ConfigFile_Iterator_GetKey
    (
        const SVOX_ConfigFile_Iterator *iteratorPtr,
        const SVOX_Char **key
    );

    /**
     * Returns the type of the value to which the iterator points.
     *
     * @param[in] iterator Iterator
     * @param[out] typePtr pointer in which the type is stored
     *
     * @retval SVOX_RESULT_OK ok
     * @retval SVOX_RESULT_AT_END iterator is NULL or at the end of the list
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 typePtr is NULL
     * @retval SVOX_RESULT_ERROR_FATAL internal error
     */    
    SVOX_Result SVOX_ConfigFile_Iterator_GetType
    (
     const SVOX_ConfigFile_Iterator *iterator,
     SVOX_ConfigFile_ValueType *typePtr
     );
    
    /**
     * Returns the value as integer to which the iterator points.
     *
     * This function does not convert the value e.g. the string "123"
     * or float value 1.12 returns SVOX_RESULT_ERROR_INVALID
     *
     * The result value stays untouched in case of an error which allows
     * the user to fill it with a default value.
     *
     * @param[in] iteratorPtr Iterator
     * @param[out] intPtr pointer to a SVOX_32S in which the value is stored
     *
     * @retval SVOX_RESULT_OK ok
     * @retval SVOX_RESULT_AT_END iterator is NULL or at the end of the list
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 intPtr is NULL
     * @retval SVOX_RESULT_ERROR_INVALID type of the element is not integer
     */  
    SVOX_Result SVOX_ConfigFile_Iterator_GetInteger
    (
        const SVOX_ConfigFile_Iterator *iteratorPtr,
        SVOX_32S *intPtr
    );
    
    /**
     * Returns the value as string to which the iterator points.
     *
     * This function does not convert the value e.g. the int value 123
     * or float value 1.12 returns SVOX_RESULT_ERROR_INVALID
     *
     * The string is managed by ConfigFile, do not release it! 
     * The value is valid until SVOX_ConfigFile_Close.
     *
     * The result value stays untouched in case of an error which allows
     * the user to fill it with a default value.
     *
     * @param[in] iterator Iterator
     * @param[out] stringPtr pointer to a SVOX_Char* in which the value is stored
     *
     * @retval SVOX_RESULT_OK ok
     * @retval SVOX_RESULT_AT_END iterator is NULL or at the end of the list
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 stringPtr is NULL
     * @retval SVOX_RESULT_ERROR_INVALID type of the element is not string
     */  
    SVOX_Result SVOX_ConfigFile_Iterator_GetString
    (
     const SVOX_ConfigFile_Iterator *iterator,
     const SVOX_Char **stringPtr
     );

    /**
     * Returns the value as float to which the iterator points.
     *
     * This function automatically converts an integer value to float.
     * All other values return SVOX_RESULT_ERROR_INVALID.
     *
     * The result value stays untouched in case of an error which allows
     * the user to fill it with a default value.
     *
     * @param[in] iterator Iterator
     * @param[out] floatPtr pointer to a SVOX_Float in which the value is stored
     *
     * @retval SVOX_RESULT_OK ok
     * @retval SVOX_RESULT_AT_END iterator is NULL or at the end of the list
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 floatPtr is NULL
     * @retval SVOX_RESULT_ERROR_INVALID type of the element is not float or integer
     */  
    SVOX_Result SVOX_ConfigFile_Iterator_GetFloat
    (
     const SVOX_ConfigFile_Iterator *iterator,
     SVOX_Float *floatPtr
     );
    
    /**
     * Returns the value as bool to which the iterator points.
     *
     * This function does not convert any values.
     *
     * The result value stays untouched in case of an error which allows
     * the user to fill it with a default value.
     *
     * @param[in] iterator Iterator
     * @param[out] boolPtr pointer to a SVOX_Bool in which the value is stored
     *
     * @retval SVOX_RESULT_OK ok
     * @retval SVOX_RESULT_AT_END iterator is NULL or at the end of the list
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 boolPtr is NULL
     * @retval SVOX_RESULT_ERROR_INVALID type of the element is not bool
     */  
    SVOX_Result SVOX_ConfigFile_Iterator_GetBool
    (
     const SVOX_ConfigFile_Iterator *iterator,
     SVOX_Bool *boolPtr
     );

    /**
     * Returns a iterator to a subtree element.
     *
     * This function does not convert any values.
     *
     * The subIteratorPtr is set oo NULL in case of an error.
     *
     * @param[in] iterator Iterator
     * @param[out] subIteratorPtr pointer to a SVOX_ConfigFile_Iterator * in which the value is stored
     *
     * @retval SVOX_RESULT_OK ok
     * @retval SVOX_RESULT_AT_END iterator is NULL or at the end of the list
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 subIteratorPtr is NULL
     * @retval SVOX_RESULT_ERROR_INVALID type of the element is not a table
     */  
    SVOX_Result SVOX_ConfigFile_Iterator_GetIterator
    (
     const SVOX_ConfigFile_Iterator *iterator,
     const SVOX_ConfigFile_Iterator **subIteratorPtr
     );
        
    
    /**
     * Returns the last error string, NULL-Pointer in case of no error.
     *
     * You can use the pointer until the next call to SVOX_ConfigFile_*.
     * You MUST NOT free or modify the pointer, SVOX_ConfigFile is responsible
     * for the memory management of it.
     * 
     * @param[in] configFile Config file handle
     * @param[out] errorString Pointer to a variable in which the string is stored.
     * 
     * @retval SVOX_RESULT_OK errorString was set
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 configFile is NULL
     * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 errorStringPtr is NULL
     *
     */    
SVOX_Result SVOX_ConfigFile_GetLastErrorString
(
    SVOX_ConfigFile *configFile,
    const SVOX_Char **errorString
);
    
    
    /**
     * Prints a config file to console for debugging purposes
     * 
     * @param configFile configfile to print
     */
    void SVOX_ConfigFile_Print(SVOX_ConfigFile *configFile);

    
/* -----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

