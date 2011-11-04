/** 
 * @brief BasicString
 *
 * Basic strings are a simple and secure way to handle 
 * stirngs in pure ANSI-C.
 * The strings are allocated on heap.
 *
 * All functions are desgined to be robust agains NULL
 * pointers.
 * 
 * It is ok if the user calls several functions on a string
 * and checks for NULL at the end of all operations.
 * This keeps error handling simple.
 *
 * @code
 * SVOX_BasicString *string = SVOX_BasicString_InitWithFormat(100, "hxllo %s", "welt");
 * string = SVOX_BasicString_Append(string, "!", -1);
 * string = SVOX_BasicString_ReplaceChar(string, 'x', 'a');
 * const SVOX_Char *stringBuffer = SVOX_BasicString_GetBuffer(string);
 * 
 * if(HNULL != stringBuffer)
 * {
 * SVOX_printf("The string is: %s\n", string);
 * }
 * 
 * SVOX_BasicString_Release(string);
 * @endcode
 *
 * The strings are modified on the fly returning a
 * new pointer to a new basic string if resizing is 
 * required.
 * 
 * @warning It is important not to keep an old string
 * pointer if calling one of the functions.
 * If the string is modified a new pointer is returned!
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

#ifndef __SVOX_BASICSTRING_H__
#define __SVOX_BASICSTRING_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "SVOX_Types.h"

typedef struct SVOX_BasicString_ SVOX_BasicString;

/**
 * Creates an empty basic string.
 * The string is initialised with the initSize number of bytes.
 *
 * @param[in] initSize size of characters to reserve for the string
 *        (not including the trailing 0)
 *
 * @result new pointer to the string, NULL if out of memory
 *
 */
SVOX_BasicString *SVOX_BasicString_Init
(
    SVOX_32S initSize
);

/**
 * Creates a basic string by copying from a source string.
 * Copying can be truncated by using numberToCopy.
 * Initial size can be set if the string will be extended.
 * Copying automatically ends with the first 0-byte found.
 * If source string is NULL the resulting SVOX_BasicString is also NULL.
 *
 * @param[in] otherString source string to init from
 * @param[in] numberToCopy number of bytes to copy, -1 to copy the complete string
 * @param[in] initSize initial size of the buffer (will be expanded to at least
 *                 the size of the string), -1 for autodetect
 *
 * @result new pointer to the string, NULL if out of memory
 *
 */
SVOX_BasicString *SVOX_BasicString_InitWithString
(
    const SVOX_Char *otherString,
    SVOX_32S         numberToCopy,
    SVOX_32S         initSize
);

/**
 * Creates a basic string by copying from a source string with format.
 * Copying can be truncated by using numberToCopy.
 * Initial size can be set if the string will be extended.
 * Copying automatically ends with the first 0-byte found.
 *
 * @param[in] initSize size of characters to reserve for the string
 *        (not including the trailing 0)
 * @param[in] format format of the string to copy
 *
 * @result new pointer to the string, NULL if out of memory
 *
 */
SVOX_BasicString *SVOX_BasicString_InitWithFormat
(
    SVOX_32S         initSize,
    const SVOX_Char *format,
    ...
);


/**
 * Creates a basic string by copying from a source string with format
 * using var args for the parameters
 * Copying can be truncated by using numberToCopy.
 * Initial size can be set if the string will be extended.
 * Copying automatically ends with the first 0-byte found.
 *
 * @param[in] initSize size of characters to reserve for the string
 *        (not including the trailing 0)
 * @param[in] format format of the string to copy
 * @param[in] list var arg list of parameters
 *
 * @result new pointer to the string, NULL if out of memory
 *
 * @warning This function may change list so that it points
 *          to the end of the var args. If you need list
 *          after calling this function you have to copy
 *          it using SVOX_va_copy before calling the function.
 */
SVOX_BasicString *SVOX_BasicString_InitWithVarArgs
(
    SVOX_32S         initSize,
    const SVOX_Char *format,
    SVOX_va_list     list
);

/**
 * @brief Creates a new string using a substring
 * from the given string
 *
 * @param[in] basicString the string to cut
 * @param[in] start start of the string, negative numbers start from the end of the string
 * @param[in] len length of the string to cut, <0 for the rest of the string
 *
 * @retval the string handle
 */
SVOX_BasicString *SVOX_BasicString_InitWithSubString
(
    SVOX_BasicString *basicString,
    SVOX_32S          start,
    SVOX_32S          len
);

/**
 * @brief Clone a basic string
 *
 * Creates a copy from a basic string
 *
 * @param[in] basicString string to clone
 * @result clone of the string, NULL if string was NULL of out of memory
 */
SVOX_BasicString *SVOX_BasicString_InitWithBasicString
(
    SVOX_BasicString *basicString
);


/**
 * Releases the memory for the basic string.
 * This function is robust to 0-Pointers.
 * @warning: Do not use any string handles taken from
 * the string after it is destroyed!
 *
 * @param[in] basicString String to delete
 */
void SVOX_BasicString_Release
(
    SVOX_BasicString *basicString
);

/**
 * Clear
 *
 * Clears the internal buffer, not resizing the memory used
 * for the string.
 *
 * @param[in] basicString basic string handle
 *
 * @result returns the original handle
 *
 */
SVOX_BasicString *SVOX_BasicString_Clear
(
    SVOX_BasicString *basicString
);

/**
 * Get the internal buffer of the string.
 * If basicString is NULL a pointer a NULL-pointer is returned
 * @warning: Any call to SVOX_BasicString invalidates the buffer!
 *
 * @param[in] basicString basic string handle
 *
 * @result pointer to the internal buffer, NULL if basicString is NULL
 */
const SVOX_Char *SVOX_BasicString_GetBuffer
(
    SVOX_BasicString *basicString
);

/**
 * Get the internal buffer of the string.
 * If basicString is NULL a pointer a NULL-pointer is returned
 * @warning: Any call to SVOX_BasicString invalidates the buffer!
 *
 * @param[in] basicString basic string handle
 *
 * @result pointer to the internal buffer, NULL if basicString is NULL
 */
SVOX_Char *SVOX_BasicString_GetMutableBuffer
(
    SVOX_BasicString *basicString
);

/**
 * Append other string.
 * @warning: This may cause a reallocation of the string, invalidatin all handles.
 *
 * @param[in] basicString String handle
 * @param[in] otherString String to append
 * @param[in] numberToCopy number of bytes to copy (not including trailing 0),
 *                       -1 for autodetect
 * @result new string handle, NULL if out of memory
 */
SVOX_BasicString *SVOX_BasicString_Append
(
    SVOX_BasicString *basicString,
    const SVOX_Char  *otherString,
    SVOX_32S          numberToCopy
);

/**
 * Set the string to a new value.
 * @warning: This may cause a reallocation of the string, invalidatin all handles.
 * This is a convenience function (calling SVOX_BasicString_Clear & SVOX_BasicString_Append)
 *
 * @param[in] basicString String handle
 * @param[in] otherString String to append
 * @param[in] numberToCopy number of bytes to copy (not including trailing 0),
 *                       -1 for autodetect
 * @result new string handle, NULL if out of memory
 */
SVOX_BasicString *SVOX_BasicString_Set
(
    SVOX_BasicString *basicString,
    const SVOX_Char  *otherString,
    SVOX_32S          numberToCopy
);

/**
 * Append formated string
 * @warning: This may cause a reallocation of the basic string, invalidating all handles.
 *
 * @param[in] basicString String handle
 * @param[in] format format for the string to append (using SVOX_sprintf)
 * @param[in] ... parameters to the format
 *
 * @result new string handle, NULL if out of memory or if basicString is NULL
 */
SVOX_BasicString *SVOX_BasicString_AppendFormat
(
    SVOX_BasicString *basicString,
    const SVOX_Char  *format,
    ...
);

/**
 * Appends format with var arg list.
 * @warning: This may cause a reallocation of the basic string, invalidating all handles.
 *
 * @param[in] basicString String handle
 * @param[in] format format for the string to append (using SVOX_sprintf)
 * @param[in] list list of  parameters to the format
 *
 * @result new string handle, NULL if out of memory or if basicString is NULL
 *
 * @warning This function may change list so that it points
 *          to the end of the var args. If you need list
 *          after calling this function you have to copy
 *          it using SVOX_va_copy before calling the function.
 */
SVOX_BasicString *SVOX_BasicString_AppendVarArgs
(
    SVOX_BasicString *basicString,
    const SVOX_Char  *format,
    SVOX_va_list      list
);

/**
 * Returns the number of characters in the basic string not including the
 * trailing 0.
 * @remark This is not size of the buffer (!)
 *
 * @param[in] basicString String handle
 *
 * @result number of bytes in the string, -1 if NULL-Pointer
 */
SVOX_32S SVOX_BasicString_Length
(
    SVOX_BasicString *basicString
);

/**
 * Replace all occurences of a char with another
 *
 * @remark replacing 0 oder replacing with 0 does not change the string
 *
 * @param[in] basicString String handle
 * @param[in] replace char to replace
 * @param[in] with char to write
 *
 * @result string handle
 */
SVOX_BasicString *SVOX_BasicString_ReplaceChar
(
    SVOX_BasicString *basicString,
    SVOX_Char         replace,
    SVOX_Char         with
);

/**
 * @brief Search char within a string
 *
 * Searches a string for the first occurance of a character.
 *
 * @param[in] basicString the String pointer
 * @param[in] c character to find
 * @param[in] startIndex start index within the string, negative numbers start from the end of the string
 *
 * @retval index from the start of the string if found
 * @retval -1 if not found
 *
 */
SVOX_32S SVOX_BasicString_Find
(
    SVOX_BasicString *basicString,
    SVOX_Char         c,
    SVOX_32S          startIndex
);

/**
 * @brief Search char within a string
 *
 * Searches a stirng from the end for a character.
 *
 * @param[in] basicString the String pointer
 * @param[in] c character to find
 * @param[in] startIndex start index within the string, negative numbers start from the end of the string
 *
 * @retval index from the start of the string if found
 * @retval -1 if not found
 *
 */
SVOX_32S SVOX_BasicString_ReverseFind
(
    SVOX_BasicString *basicString,
    SVOX_Char         c,
    SVOX_32S          startIndex
);

/**
 * @brief Comare a BasicString with a char
 *
 *
 * @param[in] basicString the string
 * @param[in] string the char string to compare to
 *
 * @retval 0 Both strings are equal or NULL
 * @retval >0 The first character that does not match in basicString > string
 * @retval 1 string is NULL
 * @retval <0 The first character that does not match in basicString < string
 * @retval -1 basicString is NULL
 */
SVOX_32S SVOX_BasicString_Strcmp
(
    const SVOX_BasicString *basicString,
    const SVOX_Char        *string
);

/**
 * @brief Comare a BasicString with a char
 *
 *
 * @param[in] basicString1 the first string
 * @param[in] basicString2 the second string
 *
 * @retval 0 Both strings are equal or NULL
 * @retval >0 The first character that does not match in basicString1 > basicString2
 * @retval 1 basicString2 is NULL
 * @retval <0 The first character that does not match in basicString1 < basicString2
 * @retval -1 basicString1 is NULL
 */
SVOX_32S SVOX_BasicString_Compare
(
 const SVOX_BasicString *basicString1,
 const SVOX_BasicString *basicString2
);

/**
 * @brief Cut substring from the current string
 *
 * Cuts parts of the string to create a substring.
 * This modifies the original string.
 *
 * @param[in] basicString the string to cut
 * @param[in] start start of the string, negative numbers start from the end of the string
 * @param[in] len length of the string to cut, < 0 for the rest of the string from start
 *
 * @retval new the string handle
 */
SVOX_BasicString *SVOX_BasicString_SubString
(
    SVOX_BasicString *basicString,
    SVOX_32S          start,
    SVOX_32S          len
);

/** 
 * @brief Checks if a string starts with the given sequence
 *
 * @param[in] basicString the string
 * @param[in] startsWith the string to compare with
 * 
 * @retval SVOX_TRUE if the string starts with the other string
 * @retval SVOX_FALSE if the string does not starth with the other string
 *                    of one of both is a NULL-string
 *
 */
SVOX_Bool SVOX_BasicString_StartsWith
(
    SVOX_BasicString *basicString,
    const SVOX_Char  *startsWith
);

/**
 * @brief Cut the string and return right most chars
 *
 * Cuts the string leaving only the right most parts
 * of the string.
 *
 * @remark This is just a macro calling SVOX_BasicString_SubString
 *
 * @param[in] basicString SVOX_BasicString to take the right side of
 * @param[in] count number of chars to keep
 *
 * @retval new string
 */
#define SVOX_BasicString_Right(basicString, count) SVOX_BasicString_SubString(basicString, -count, -1)

/**
 * @brief Cut the string and return left most chars
 *
 * Cuts the string leaving only the left most parts
 * of the string.
 *
 * If count is greater than the length of the string
 * the complete string is returned.
 *
 * If count is < 0 the complete string is returned
 *
 * @remark This is just a macro calling SVOX_BasicString_SubString
 *
 * @param[in] basicString SVOX_BasicString to take the right side of
 * @param[in] count number of chars to keep
 *
 * @retval new string
 */
#define SVOX_BasicString_Left(basicString, count) SVOX_BasicString_SubString(basicString, 0, count)

/**
 * Creates a basic string by copying from a wide source string.
 * Copying automatically ends with the first 0-byte found.
 * If source string is NULL the resulting SVOX_BasicString is also NULL.
 *
 * @param[in] wideString source string to init from
 *
 * @result new pointer to SVOX_BasicString, NULL if out of memory
 *
 */
SVOX_BasicString *SVOX_BasicString_InitWithWideChar(SVOX_WChar *wideString);

/**
 * @brief create a wide char version on the heap
 *
 * this creates a coyp of the string on the heap, 
 * the new string is a SVOX_WChar *
 *
 * @remark Call SVOX_BasicString_DestroyWideChar() to release the copy
 *
 * @param[in] basicString: utf8 codes data to be used to create UCS-2 variant from
 *
 * @retval LPWSTR
 */
SVOX_WChar *SVOX_BasicString_MakeWideChar(SVOX_BasicString *basicString);

/**
 * @brief destroys a wide char string on the heap
 *
 * removes a heap object of type SVOX_WChar * returned by SVOX_BasicString_MakeWideChar
 *
 *
 * @param[in] wideString: pointer to data to be freed
 *
 * @retval none
 */
void SVOX_BasicString_DestroyWideChar(SVOX_WChar *wideString);



#ifdef __cplusplus
}
#endif

#endif

