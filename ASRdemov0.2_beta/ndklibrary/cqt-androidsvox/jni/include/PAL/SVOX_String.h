/**
 * @brief String functions
 *
 * All kinds of string functions found in the ansi-c
 * libs.
 *
 * The functions are modified to be robust agains NULL-ptrs.
 *
 * It also contains an scanf implementation which
 * delivers the SVOX specific datatypes.
 *
 * @date $Date: 2010-10-06 14:52:19 +0200 (Wed, 06 Oct 2010) $
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

#ifndef __SVOX_STRING_H__
#define __SVOX_STRING_H__

#ifdef __cplusplus
extern "C" {
#endif


/* -----------------------------------------------------------------------*/

#include "SVOX_Types.h"

/**
 * Returns the length of the string cs by searching for the
 * first 0-byte.
 *
 * @remark This implementation is robust to 0-pointers.
 * It returns 0 if the pointer is NULL
 *
 * @param[in] cs Pointer to the string
 *
 * @return Length of the string, 0 if cs was NULL
 */
SVOX_size_t SVOX_strlen
(
    const SVOX_Char *cs
);

/**
 * This function starts comparing the first character of each string.
 * If they are equal to each other, it continues with the following pairs
 * until the characters differ or until a terminating null-character is
 * reached.
 *
 * @remark This implementation is robust to 0-pointers.
 * Two 0-pointers are considered equal, if only one string is
 * NULL this string is considered smaller.
 *
 * @param[in] cs The first string
 * @param[in] ct The second string
 *
 * @retval 0 Both strings are equal or NULL
 * @retval 1 The first character that does not match in cs > ct
 * @retval 1 ct is NULL
 * @retval -1 The first character that does not match in cs < ct
 * @retval -1 cs is NULL
 */
SVOX_32S SVOX_strcmp
(
    const SVOX_Char *cs,
    const SVOX_Char *ct
);

/**
 * This function starts comparing the first character of each string.
 * If they are equal to each other, it continues with the following pairs
 * until the characters differ or until a terminating null-character is
 * reached.
 *
 * @remark This implementation is robust to 0-pointers.
 * Two 0-pointers are considered equal, if only one string is
 * NULL this string is considered smaller.
 *
 * @param[in] cs The first string
 * @param[in] ct The second string
 * @param[in] n Maximum number of characters to compare
 *
 * @retval 0 Both strings are equal or NULL or n<0
 * @retval 1 The first character that does not match in cs > ct
 * @retval 1 ct is NULL
 * @retval -1 The first character that does not match in cs < ct
 * @retval -1 cs is NULL
 */
SVOX_32S SVOX_strncmp
(
    const SVOX_Char *cs,
    const SVOX_Char *ct,
    SVOX_size_t      n
);


/**
 * Finds the first occurrence of character in the C-String str
 *
 * @param[in] str pointer to the string
 * @param[in] character character to find
 *
 * @retval 0 if the character was not found or str is NULL
 * @retval !=NULL pointer to the first occurrence of character
 *
 * @remark Use SVOX_strchr_const with const pointers
 */
SVOX_Char *SVOX_strchr
(
    SVOX_Char *str,
    SVOX_Char  character
);

/**
 * Const version of SVOX_strchr
 */
const SVOX_Char *SVOX_strchr_const
(
    const SVOX_Char *str,
    SVOX_Char        character
);


/**
 * Finds the last occurrence of character in the C-String str
 *
 * @param[in] str pointer to the string
 * @param[in] SVOX_Character character to find
 *
 * @retval 0 if the character was not found or str is NULL
 * @retval !=NULL pointer to the first occurrence of character
 *
 * @remark Use SVOX_strrchr_const with const pointers
 */
SVOX_Char *SVOX_strrchr
(
    SVOX_Char *str,
    SVOX_Char  SVOX_Character
);

/**
 * Const version of SVOX_strrchr
 */
const SVOX_Char *SVOX_strrchr_const
(
    const SVOX_Char *str,
    SVOX_Char        SVOX_Character
);


/**
 * Returns a pointer to the first occurrence of str2 in str1,
 * or a null pointer if str2 is not part of str1.
 *
 * @param[in] str1 pointer to the string
 * @param[in] str2 pointer to the substring
 *
 * @retval 0 if str2 was not found or str1 or str2 is NULL
 * @retval !=NULL pointer to the first occurrence of character
 *
 * @remark Use SVOX_strstr_const with const pointers
 */
SVOX_Char *SVOX_strstr
(
    SVOX_Char       *str1,
    const SVOX_Char *str2
);

/**
 * Const version of SVOX_strstr
 */
const SVOX_Char *SVOX_strstr_const
(
    const SVOX_Char *str1,
    const SVOX_Char *str2
);


/**
 * Copies a string into a new memory block. The block is
 * big enough to contain the string and a terminating 0-byte.
 *
 * @warning: The user is responsible to free the memory
 * by calling SVOX_free()
 *
 * @param[in] str pointer to the string
 *
 * @retval 0 If there is not enough memory or str is NULL.
 * @return Pointer to the copy of the string
 */
SVOX_Char *SVOX_strdup
(
    const SVOX_Char *str
);

/**
 * Print formatted data to a string.
 * This function prints formated data into a character buffer.
 * the function does not write more
 * than size bytes (including the trailing '\\0'). If the output was
 * truncated due to this limit then the return value is the number of
 * characters (not including the trailing '\\0') which would have been written to
 * the final string if enough space had been  available. Thus, a return
 * value of size or more means that the output was truncated.
 *
 * @param[in,out] string the string to write the data to
 * @param[in] size the number of bytes to write including the trailing 0
 * @param[in] format the format string
 * @param[in] ... the parameters for the string
 *
 * @result number of bytes the formatted string will take (not including
 *         the trailing 0).
 * @result -1 if one of pointer args is invalid (i.e. NULL)
 */
SVOX_32S SVOX_snprintf
(
    SVOX_Char       *string,
    SVOX_size_t      size,
    const SVOX_Char *format,
    ...
);


/**
 * Parses the C string str interpreting its content as a floating point
 * number and returns its value as a double. If endptr is not a null
 * pointer, the function also sets the value pointed by endptr to point to
 * the first character after the number.
 *
 * The function first discards as many whitespace characters as necessary
 * until the first non-whitespace character is found. Then, starting from
 * this character, takes as many characters as possible that are valid
 * following a syntax resembling that of floating point literals, and
 * interprets them as a numerical value. A pointer to the rest of the
 * string after the last valid character is stored in the object
 * pointed by endptr.
 *
 * A valid floating point number for strtod is formed by a succession of:
 * @li An optional plus or minus sign
 * @li A sequence of digits, optionally containing a decimal-point character
 * @li An optional exponent part, which itself consists on an 'e' or 'E'
 *       character followed by an optional sign and a sequence of digits.
 *
 * If the first sequence of non-whitespace characters in str does not form
 * a valid floating-point number as just defined, or if no such sequence
 * exists because either str is empty or contains only whitespace
 * characters, no conversion is performed.
 *
 * @param[in] str C string beginning with the representation of a
 *                floating-point number.
 *
 * @param[out] endp Reference to an already allocated object of type char*,
 *                  whose value is set by the function to the next character
 *                  in str after the numerical value.
 *                  This parameter can also be a null pointer, in which
 *                  case it is not used.
 *
 * @retval double On success, the function returns the converted floating
 *                point number as a double value.
 * @retval 0.0    If no valid conversion could be performed, a zero value
 *                (0.0) is returned.
 *                If the value is too small it is also set to 0.0
 * @retval HUGE_VAL If the correct value is out of the range of representable
 *                values, a positive or negative HUGE_VAL is returned.
 *                This value can't be directly accessed but you can check
 *                for the value by using SVOX_isHugeVal(x)
 *
 */
SVOX_Double SVOX_strtod
(
    const SVOX_Char *str,
    SVOX_Char      **endp
);

/**
 * The SVOX_strtol() function converts the initial portion of of str into an SVOX_32S integer.
 * The integer is interpreted with base, which is 0, or ranges from 2 to 36 inclusive.
 * If base is 0, the base is 10, unless the number starts with 0x, then the base is 16.
 * If endp is not NULL, SVOX_strtol() stores the address of the first invalid
 * character in *endp.  If there were no digits at all, SVOX_strtol()  stores
 * the  original value of str in *endp (and returns 0).  In particular,
 * if *str is not ’\\0’ but **endp is ’\\0’ on return, the entire  string
 * is valid.
 *
 * @param[in] str string that should be converted to an integer
 * @param[out] endp pointer to SVOX_Char*, see above
 * @param[in] base base for the conversion
 *
 * @result SVOX_32S integer representation of the number in string, 0 if there is no
 *         number or the parameter str is NULL
 */
SVOX_32S SVOX_strtol
(
    const SVOX_Char *str,
    SVOX_Char      **endp,
    SVOX_32S         base
);

/**
 * The SVOX_strtoul() function converts the initial portion of of str into an SVOX_32U integer.
 * The integer is interpreted with base, which is 0, or ranges from 2 to 36 inclusive.
 * If base is 0, the base is 10, unless the number starts with 0x, then the base is 16.
 * If endp is not NULL, SVOX_strtoul() stores the address of the first invalid
 * character in *endp.  If there were no digits at all, SVOX_strtol()  stores
 * the  original value of str in *endp (and returns 0).  In particular,
 * if *str is not ’\\0’ but **endp is ’\\0’ on return, the entire  string
 * is valid.
 *
 * @param[in] str string that should be converted to an integer
 * @param[out] endp pointer to SVOX_Char*, see above
 * @param[in] base base for the conversion
 *
 * @result SVOX_32U integer representation of the number in string, 0 if there is no
 *         number or the parameter str is NULL
 */
SVOX_32U SVOX_strtoul
(
    const SVOX_Char *str,
    SVOX_Char      **endp,
    SVOX_32S         base
);

/**
 * The SVOX_atoi function converts the initial portion of of str into an SVOX_32S integer.
 * No errors are detected. The integer is always interpreted to the base 10.
 *
 * @param[in] str string that should be converted to an integer
 *
 * @result SVOX_32S integer representation of the number in string, 0 if there is no
 *         number or the parameter str is NULL
 */
SVOX_32S SVOX_atoi
(
    const SVOX_Char *str
);

/**
 * Copies the first num characters of source to destination. If the end of
 * the source C string (which is signaled by a null-character) is found
 * before num characters have been copied, destination is padded with
 * at least one 0-byte.
 *
 * No null-character is implicitly appended to the end of destination,
 * so destination will only be null-terminated if the length of the C string
 * in source is less than num.
 *
 * @remark This function is robust to null-pointers. If destination is NULL
 * nothing happens, if source is NULL destination[0]=0
 * If n<0 is null nothing happens.
 *
 * @param[in] destination Pointer to the destination array.
 * @param[in] source String to copy.
 * @param[in] n Number of chars to copy.
 *
 * @return destination
 */
/*SVOX_DEPRECATED_FUNCTION(SVOX_strncpy_s)*/ SVOX_Char *SVOX_strncpy
(
    SVOX_Char       *destination,
    const SVOX_Char *source,
    SVOX_size_t      n
);


/**
 * Varargs version of SVOX_snprintf.
 *
 * Writes at most size-1 bytes and adds a trailing 0.
 *
 * @warning This behaviour differs from the native windows
 *          implementation!
 *
 * Formats a string into a buffer.
 *
 * @remark This function should be reimplemented in future versions
 *         of the PAL to create a consistent behavior over all
 *         systems.
 *
 * @see SVOX_snprintf
 *
 * @param[in,out] string the string to write the data to
 * @param[in] size the number of bytes to write including the trailing 0
 * @param[in] format the format string
 * @param[in] va_list the variable arguments list
 *
 * @result number of bytes the formatted string will take (not including
 *         the trailing 0).
 *         on some cases the result may be -1 which indicates
 *         that the buffer is too small but the amount of chars
 *         needed could not be determined.
 * @result -1 if one of pointer args is invalid (i.e. NULL)
 *
 * @warning This function may change list so that it points
 *          to the end of the var args. If you need list
 *          after calling this function you have to copy
 *          it using SVOX_va_copy before calling the function.
 */
/*SVOX_DEPRECATED_FUNCTION(SVOX_vsnprintf_s)*/
SVOX_32S SVOX_vsnprintf
(
    SVOX_Char       *string,
    SVOX_size_t      size,
    const SVOX_Char *format,
    SVOX_va_list     va_list
);

/**
 * the strcpy function copies the ct string into the memorz starting at s.
 * It must be ensured that s has enough space reserved to hold the string ct
 *
 * @param[in,out] dest destination pointer
 * @param[in] src source pointer
 *
 * @result is equal to s, unless any of the arguments is NULL, then NULL is returned
 */
SVOX_Char *SVOX_strcpy
(
    SVOX_Char       *dest,
    const SVOX_Char *src
);

/**
 * The  strcat() function appends the src string to the dest string, over‐
 * writing the null byte (’\\0’) at the end of dest, and then adds a
 * terminating  null  byte.   The  strings may not overlap, and the dest string
 * must have enough space for the result.
 *
 * @param[in,out] dest the destination string
 * @param[in] src the source sting
 *
 * @result dest is returned unless any of the arguments is NULL, then NULL is returned
 */
SVOX_Char *SVOX_strcat
(
    SVOX_Char       *dest,
    const SVOX_Char *src
);

/**
 * @brief Append characters from string
 *
 * Appends the first num characters of source to destination, plus a terminating
 * null-character. If the length of the C string in source is less than num,
 * only the content up to the terminating null-character is copied.
 *
 * @param[in] destination Pointer to the destination array, which should contain
 *        a C string, and be large enough to contain the concatenated resulting
 *        string, including the additional null-character.
 * @param[in] source C string to be appended.
 * @param[in] num Maximum number of characters to be appended.
 *
 * @result destination is returned
 */
SVOX_Char *SVOX_strncat
(
    SVOX_Char       *destination,
    const SVOX_Char *source,
    SVOX_size_t      num
);


/**
 *
 * Format a string into a buffer.
 *
 * @remark This function should be reimplemented in future versions
 *         of the PAL to create a consistent behavior over all
 *         systems.
 *
 * @see SVOX_vsnprintf
 *
 * @param[in,out] str the string to write the data to
 * @param[in] format the formst string
 * @param[in] ... the variable arguments list
 *
 * @result number of bytes the formatted string will take (not including
 *         the trailing 0).
 *         on some cases the result may be -1 which indicates
 *         that the buffer is too small but the amount of chars
 *         needed could not be determined.
 * @result -1 if one of pointer args is invalid (i.e. NULL)
 *
 * @warning This function may change list so that it points
 *          to the end of the var args. If you need list
 *          after calling this function you have to copy
 *          it using SVOX_va_copy before calling the function.
 */
SVOX_32S SVOX_sprintf
(
    SVOX_Char       *str,
    const SVOX_Char *format,
    ...
);

/**
 * @brief Scan formatted string from string.
 *
 * The function scans input according to a format, as
 * described below.  This format may contain conversion specifiers;
 * the results from such conversions, if any, are stored through the pointer
 * arguments.
 *
 * Each successive pointer argument must correspond properly with each
 * successive conversion specifier (but see the * conversion below).
 * All conversions are introduced by the % (percent sign) character.
 * The format string may also contain other characters.
 * White space (such as blanks, tabs, or newlines) in the format string
 * match any amount of white space, including none, in the input.
 * Everything else matches only itself.  Scanning stops when an input
 * character does not match such a format character.
 * Scanning also stops when an input conversion cannot be made (see below).
 *
 * Following the % character introducing a conversion, there may be a
 * number of flag characters, as follows:
 *
 * @li * Suppresses assignment.  The conversion that follows occurs as usual,
 *      but no pointer is used; the result of the conversion is simply
 *      discarded.
 * @li h Indicates that the conversion will be one of dioux or n and the
 *       next pointer is a pointer to a SVOX_16S or SVOX_16U (rather than int).
 * @li l Indicates that the conversion will be one of dioux or n and the
 *       next pointer is a pointer to a SVOX_32S or SVOX_32U (which means that
 *       the flag is ignored in the PAL),
 *       that the conversion will be one
 *       of e, f, or g and the next pointer is a pointer to SVOX_Double
 *       (rather than SVOX_Float)
 * @li % Matches the literal '%'
 * @li d Matches a signed decimal integer SVOX_32S
 * @li i Matches an optionally signed integer; the next pointer must be a
 *       pointer to SVOX_32U.  The integer is read in base 16 if it begins
 *       with `0x' or `0X', in base 8 if it begins with `0', and in base
 *       10 otherwise.  Only characters that correspond to the base are used.
 * @li o Matches an octal integer, pointer must be SVOX_32U
 * @li u Matches an unsigned integer, pointer must be SVOX_32U
 * @li x, X Matches hexadezimal integer, pointer must be SVOX_32U
 * @li e,E,f,F,g,G Matches a floating-point number, pointer must be SVOX_Float
 * @li s Matches a sequence of non-white-space characters; the next pointer
 *       must be a pointer to char, and the array must be large enough to
 *       accept all the sequence and the terminating NUL character.  The
 *       input string stops at white space or at the maximum field width,
 *       whichever occurs first.
 *       l-modifer is not supported!
 * @li c Matches a sequence of width count characters (default 1); the next
 *       pointer must be a pointer to char, and there must be enough room
 *       for all the characters (no terminating NUL is added).
 *       The usual skip of leading white space is suppressed.
 *       To skip white space first, use an explicit space in the format.
 * @li [ Matches a nonempty sequence of characters from the specified set
 *       of accepted characters; the next pointer must be a pointer to char,
 *       and there must be enough room for all the characters in the string,
 *       plus a terminating NUL character.  The usual skip of leading white
 *       space is suppressed.  The string is to be made up of characters
 *       in (or not in) a particular set; the set is defined by the
 *       characters between the open bracket [ character and a close bracket
 *       ] character.  The set excludes those characters if the first
 *       character after the open bracket is a circumflex ^.
 *       To include a close bracket in the set, make it the first character
 *       after the open bracket or the circumflex; any other position will
 *       end the set.  The hyphen character - is also special;
 *       when placed between two other characters, it adds all intervening
 *       characters to the set.  To include a hyphen, make it the
 *       last character before the final close bracket.
 *       For instance, '[^]0-9-]' means the set "everything except close
 *       bracket, zero through nine, and hyphen".  The string ends with
 *       the appearance of a character not in the
 *       (or, with a circumflex, in) set or when the field width runs out.
 * @n    Nothing is expected; instead, the number of characters consumed
 *       thus far from the input is stored through the next pointer, which
 *       must be a pointer to int. This is not a conversion, although
 *       it can be suppressed with the * flag.
 *
 * Summary :
 * @li %% escaped %
 * @li * do not assign to a variable
 * @li l[eEfFgG] SVOX_Double
 * @li l[di] SVOX_32S
 * @li l[uoxX] SVOX_32U
 * @li h[di] SVOX_16S
 * @li h[uoxX] SVOX_16U
 * @li d,i SVOX_32S
 * @li o,u,x,X SVOX_32U
 * @li e,E,f,F,g,G SVOX_Float
 * @li c,s,[...] SVOX_Char*
 * @li n SVOX_32S Returns the number of chars read from the beginning
 *     of the input
 *
 * @param[in] string string to read the data from
 * @param[in] format format string, see above for details
 *
 * @retval -1 in case of error
 * @result Number of values assigned, even if the file ended
 *
 * @remark If you need additionals chars at the end of the
 * string to be parsed, e.G. (%d) use "(%d)%n" and
 * check for 2 assigned values.
 *
 */
SVOX_32S SVOX_sscanf
(
    const SVOX_Char *string,
    const SVOX_Char *format,
    ...
);

/**
 * @brief Scans values from a string using varargs
 *
 * SVOX_sscanf for format documentation
 *
 * @param[in] string the string to scan
 * @param[in] format format string
 * @param[in] arg_list list of pointers in which the results are stored
 *
 * @retval -1 in case of error
 * @result Number of values assigned, even if the file ended
 *
 * @see SVOX_sscanf
 */
SVOX_32S SVOX_vscanf
(
    const SVOX_Char *string,
    const SVOX_Char *format,
    SVOX_va_list     arg_list
);

/* -----------------------------------------------------------------------*/
/**
 * @brief Check for alphanumerical character
 *
 * returns SVOX_TRUE for a-zA-Z
 *
 * @note This function is not utf8 or unicode aware
 *
 * @param[in] c character to check
 */
SVOX_Bool SVOX_isalpha(SVOX_32S c);

/**
 * @brief Check for control character
 *
 * returns SVOX_TRUE for 0x0..0x1f
 *
 * @note This function is not utf8 or unicode aware
 *
 * @param[in] c character to check
 */
SVOX_Bool SVOX_iscntrl(SVOX_32S c);

/**
 * @brief Check for numerical digit
 *
 * returns SVOX_TRUE for 0-9
 *
 * @note This function is not utf8 or unicode aware
 *
 * @param[in] c character to check
 */
SVOX_Bool SVOX_isdigit(SVOX_32S c);

/**
 * @brief Check for lower case character
 *
 * returns SVOX_TRUE for a-z
 *
 * @note This function is not utf8 or unicode aware
 *
 * @param[in] c character to check
 */
SVOX_Bool SVOX_islower(SVOX_32S c);

/**
 * @brief Check for punctuation
 *
 * returns SVOX_TRUE for .,:;!?
 *
 * @note This function is not utf8 or unicode aware
 *
 * @param[in] c character to check
 */
SVOX_Bool SVOX_ispunct(SVOX_32S c);

/**
 * @brief Check for whitespace
 *
 * returns SVOX_TRUE for 0x20,\t\n
 *
 * @note This function is not utf8 or unicode aware
 *
 * @param[in] c character to check
 */
SVOX_Bool SVOX_isspace(SVOX_32S c);

/**
 * @brief Check for upper case character
 *
 * returns SVOX_TRUE for A-Z
 *
 * @note This function is not utf8 or unicode aware
 *
 * @param[in] c character to check
 */
SVOX_Bool SVOX_isupper(SVOX_32S c);

/**
 * @brief Check for alphanumerical
 *
 * returns SVOX_TRUE for a-zA-Z0-9
 *
 * @note This function is not utf8 or unicode aware
 *
 * @param[in] c character to check
 */
SVOX_Bool SVOX_isalnum(SVOX_32S c);

/**
 * @brief Check for hexadecimal digits
 *
 * returns SVOX_TRUE for a-fA-F0-9
 *
 * @note This function is not utf8 or unicode aware
 *
 * @param[in] c character to check
 */
SVOX_Bool SVOX_isxdigit(SVOX_32S c);

/**
 * @brief Check for printable character
 *
 * returns SVOX_TRUE for 32..126
 * @note This function is not utf8 or unicode aware
 *
 *
 * @param[in] c character to check
 */
SVOX_Bool SVOX_isprint(SVOX_32S c);

/**
 * @brief Check for graph character (i.e. any printable character
 * except space)
 *
 * returns SVOX_TRUE for 32..126
 * @note This function is not utf8 or unicode aware
 *
 *
 * @param[in] c character to check
 */
SVOX_Bool SVOX_isgraph(SVOX_32S c);

/** 
 * @brief convert character to lowercase if in 'A'..'Z'
 *
 * @note This function is not utf8 or unicode aware
 *
 * @param c character to convert
 * @return lowercase variant of character c
 */
SVOX_32S SVOX_tolower(SVOX_32S c);

/**
 * @brief convert character to uppercase if in 'a'..'z'
 * 
 * @note This function is not utf8 or unicode aware
 *
 * @param c character to convert
 * @return uppercase variant of character c
 */
SVOX_32S SVOX_toupper(SVOX_32S c);

/**
 * @brief count characters not present in string
 *
 * This function computes the length of the initial part of the string
 * pointed to by \a s1 which consists entirely of characters \a not
 * from the string pointed to by \a s2 (without the terminating null
 * character).
 *
 * @param[in] s1 string to search in
 * @param[in] s2 string containing the characters to search for
 * @return length of the prefix that does not contain any of the
 * characters in \a s2
 */
SVOX_size_t SVOX_strcspn
(
    const SVOX_Char *s1, 
    const SVOX_Char *s2
);

/**
 * @brief locate first character from set
 *
 * This function locates the first occurence in the string pointed to
 * by \a s1 of any character in string pointed to by \a s2 (excluding
 * the terminating null character).
 *
 * @param[in] s1 string to search in
 * @param[in] s2 string containing characters to seach for
 * @return SVOX_strpbrk() returns a pointer to the character found in
 *      \a s1, or a \a NULL pointer if no character from \a s2 occurs
 *      in \a s1.
 */
const SVOX_Char *SVOX_strpbrk
(
    const SVOX_Char *s1, 
    const SVOX_Char *s2
);

#ifdef __cplusplus
}
#endif

#endif

