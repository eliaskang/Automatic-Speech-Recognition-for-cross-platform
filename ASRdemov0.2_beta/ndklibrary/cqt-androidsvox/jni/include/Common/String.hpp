#ifndef SVOX_COMMON_STRING_HPP
#define SVOX_COMMON_STRING_HPP
/*
 * @file   String.hpp
 * @date   2010/04/07 
 * @author stefan.sablatnoeg
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
/*! \brief An Implicit Shared String class.
 * 
 * @file   String.hpp
 *
 * This file contains the API of the svox::common::String container class. 
 */

#include "SVOX_Types.h"

#include "RCPtr.hpp"
#include "RCObject.hpp"
#include "BaseObject.hpp"

namespace svox
{
namespace common
{
    class StringList;
    
 /**
  * @brief An Implicit Shared String Class.
  * 
  * This class implements an implicit shared string class. The sharing
  * and reference counting is abstracted out into the \a RCObject, \a
  * RCObjectFactory, and \a RCPtr classes. This class only contains
  * the user frontend, which points to the actual instance, which is a
  * \a RCObject instance which is under the control of the reference
  * counting pointer \a RCPtr. This class will not give away any
  * writeable pointers or refererences to its content. @see Scott
  * Meyer, More Effective C++, pp. 189-213.
  */
class String : public BaseObject
    {
    public:
        
        // keep these around until clear we don't need them
        // const SVOX_Char &operator[](SVOX_size_t index) const;

#if SVOX_STRING_ADD_NON_CONST_ACCESSORS
        char &operator[](size_t index);
#endif /* SVOX_STRING_ADD_NON_CONST_ACCESSORS */
    

/** @brief Returns a pointer to the character data stored in
         *  this string
         *
         *  The returned string is 0-terminated. If String::isNull()
         *  is true, a 0-pointer is returned.\n Note: this pointer
         *  remains only valid as long as no other methods are called
         *  on the String object
         *
         * @return const pointer to the string
         */
        operator const SVOX_Char*() const;

        /** @brief Returns a pointer to the character data stored in
         *  this string
         *
         *  The returned string is 0-terminated. If String::isNull()
         *  is true, a 0-pointer is returned.\n Note: this pointer
         *  remains only valid as long as no other methods are called
         *  on the String object
         *
         * @return const pointer to the string
         */
        const SVOX_Char *c_str() const;

        /** @brief Returns true if this string is equal to \a other 
         *
         * @param other [in] The string to compare with as
         * @return true if the strings are equal, false otherwise 
         */
        bool operator==(const SVOX_Char *other) const;

        /** @brief Returns true if this string is not equal to \a other 
         *
         * @param other [in] The string to compare with as
         * @return true if the strings are not equal, false otherwise 
         */
        bool operator!=(const SVOX_Char *other) const;

        /** @brief Returns true if this string is equal to \a other 
         *
         * @param other [in] The string to compare with
         * @return true if the strings are equal, false otherwise 
         */
        bool operator==(const String &other) const;

        /** @brief Returns true if this string is not equal to \a other 
         *
         * @param other [in] The string to compare with
         * @return true if the strings are not equal, false otherwise 
         */
        bool operator!=(const String &other) const;

        /** @brief Returns true, if the String contains a null
         *  pointer 
         *
         * @return true if the string is null, false otherwise
         **/
        bool isNull() const;

        /** @brief Returns true, if the string is empty; a null string
         *  is also empty 
         *
         * @return true if the string is null or empty, false
         * otherwise
         */
        bool isEmpty() const;

        /** @brief Deletes the content of the string; the string is
         *  afterwards null 
         */
        void clear();
        /** @brief Returns the size of the contained string in bytes
         * (without terminating 0) 
         *
         * @return number of bytes (without the terminating 0)
         */
        SVOX_32S byteCount() const;

        /** @brief Returns the size of the contained string in
         *  characters (UTF-8, without terminating 0) 
         *
         * @return number of characters
         */
        SVOX_32S charCount() const;

        /** @brief Returns the (byte) position of the first occurrence
         *  of \a substring within this String; returns -1 if this
         *  String does not contain substring.
         *
         * Search is started at index \a from, counted in byte positions. 
         *
         * @param substring [in] string pattern to search
         * @param from [in] first index to include in the search, that
         * ends at strlen(). \a from counts byte positions.
         * @return index of frist occurence
         */
        SVOX_32S indexOf(const SVOX_Char *substring, SVOX_32S from = 0) const;

        /** @brief Returns the (byte) position of the first occurrence
         *  of \a substring within this String 
         *
         * If the string does not conatain \a substring it returns -1.
         *
         * Search is started at index \a from, counted in byte positions.   
         *
         * @param substring [in] string pattern to search
         * @param from [in] first index to include in the search, that
         * ends at strlen(). \a from counts byte positions.
         * @return index of frist occurence
         */
        SVOX_32S indexOf(const String &substring, SVOX_32S from = 0) const;

        /** @brief Returns the (byte) position of the last occurrence
         *  of \a substring within this String.
         *
         * If this string does not contain \a substring -1 is
         * returned.  The range [from...byteCount()] is seached
         * backward.
         *
         * @param substring [in] string pattern to search
         * @param from [in] last index to include in the backward
         * search, that starts at strlen(). \a from counts byte
         * positions.
         * @return index of last occurence
         */
        SVOX_32S lastIndexOf(const SVOX_Char *substring, SVOX_32S from = 0) const;

        /** @brief Returns the (byte) position of the last occurrence
         *  of \a substring within this String.
         *
         * If this string does not contain \a substring -1 is
         * returned. The range [from...byteCount()] is seached
         * backward. (i.e. from counts byte positions)
         *
         * @param substring [in] string pattern to search
         * @param from [in] last index to include in the backward
         * search, that starts at strlen(). \a from counts byte
         * positions.
         * @return index of last occurence
         */
        SVOX_32S lastIndexOf(const String &substring, SVOX_32S from = 0) const;

        /** @}
         *  @name memory allocating methods
         *   see "Detailed Description" for information about error handling.
         *  @{
         */

        /** 
         * @brief Creates a String from the passed UTF-8 encoded SVOX_Char[].
         *
         * If \a utf8String is 0, an instance of the shared NULL
         * string is used. All characters in utf8string are copied. A
         * terminating zero is always added to the resulting string.
         *
         * Note: The character sequence is not checked for valid
         * UTF-8.  The parameter valid only refers to memory
         * allocation problems.
         *
         * Note: You should always consider using the three parameter
         * version, that provides an error checking mechanism
         *
         * @param utf8String [in]
         *
         * @code
         *  svox::common::String::String(const String &other) 
         * @endcode
         *
         * @brief Creates a copy of \a other; this is very fast, as
         * String is an implicit shared class.
         *
         * Creates a copy of \a other; this is very fast, as String is
         * an implicit shared class. A copy is only taken if the
         * source string \a other is marked non shareable. This
         * constructor is not implemented explicitly, the compiler
         * provided default does magically the right thing.
         *
         * @li other string to take a copy from 
         *
         * @see \a svox::common::RCPtr, \a svox::common::RCObject
         *
         * @code
         *  svox::common::String::~String()
         * @endcode
         *
         * @brief Destroys the string object, correctly handles sharing
         * 
         * Destroy our reference to the (possibly) shared string that
         * was represented by this class instance. The actual string
         * is only discarded iff the reference count of it goes to
         * zero. This constructor is not implemented explicitly, the
         * compiler provided default does magically the right thing.
         *
         * @see \a svox::common::RCPtr, \a svox::common::RCObject
         *
         * @code
         *  void svox::common::String::operator=(const String &other)
         * @endcode
         * 
         * @brief Assigns \a other to this string; this is very fast,
         * as String is an implicit shared class. 
         *
         * Assigns \a other to this string; this is very fast, as
         * String is an implicit shared class. A copy is only taken if
         * the source string \a other is marked non shareable. This
         * constructor is not implemented explicitly, the compiler
         * provided default does magically the right thing.
         *
         * @li other string to take a copy from 
         *
         * @see \a svox::common::RCPtr, \a svox::common::RCObject
         */
        explicit String(const SVOX_Char *utf8String = 0);

        /** @brief Creates a String 
         *
         * Creates a String from the passed UTF-8 encoded SVOX_Char[],
         * if \a utf8String is 0, an instance of the shared NULL
         * string is used. Only \a len characters are copied, if \a
         * len is -1 all characters up to the terminating 0 in \a
         * utf8String are copied. \a len refers to real characters and
         * does not include the terminating zero, a terminating zero
         * is always added to the resulting string.
         *
         * Note: The character sequence is not checked for valid
         * UTF-8.  The parameter valid only refers to memory
         * allocation problems.
         *
         * @param utf8String [in]
         * @param len [in]
         * @param valid [out] set to false if allocation goes wrong
          */
        String(const SVOX_Char *utf8String, SVOX_32S len, bool *valid);


        /** @brief Creates a String from the passed UCS-2 encoded SVOX_WChar[] 
         *
         * Creates a String from the passed UCS-2 encoded
         * SVOX_WChar[]. The UCS-2 characters are recoded into UTF-8.
         * if \a ucs2String is 0, an instance of the shared NULL
         * string is used. Only \a len (UCS-2) characters are copied, if \a
         * len is -1 all characters up to the terminating 0 in \a
         * ucs2String are copied. \a len refers to real characters and
         * does not include the terminating zero, a terminating zero
         * is always added to the resulting string.
         *
         * Note: The character sequence is not checked for valid
         * UTF-8.  The parameter valid only refers to memory
         * allocation problems.
         *
         * @param ucs2String [in]
         * @param len [in]
         * @param valid [out] set to false if allocation goes wrong
         */
        explicit String(const SVOX_WChar *ucs2String, SVOX_32S len = -1, bool *valid = 0);

        /** @brief Appends the passed string \a str to this string;
         *  returns this String 
         *
         * The string to append must be valid. 
         *
         * @param str [in] Source string to use in UTF-8
         * @param valid [out] set to false if allocation goes wrong
         * @return Reference to reference to this
         */
        String &append(const String &str, bool *valid = 0);

        /** @brief Appends the passed character sequence \a str to this string;
         * 
         * if \a len is -1 all characters up to a terminating 0 are
         * used otherwise only \a len characters are added.
         * 
         * @param str [in] Source string to use in UTF-8
         * @param len [in] number of characters to use from \a str must be larger than -1
         * @param valid [out] set to false if allocation goes wrong
         * @returns Reference to this String
         *
         */
        String &append(const SVOX_Char *str, SVOX_32S len = -1, bool *valid = 0);

        /** @brief Prepends the passed string \a str to the beginning
         *  of this string; returns this String 
         *
         * @param str [in] Source string to use in UTF-8
         * @param valid [out] set to false if allocation goes wrong
         * @returns Reference to this String
         */
        String &prepend(const String &str, bool *valid = 0);

        /** @brief Prepends the passed string \a str to the beginning
         *  of this string; returns this String
         *
         * @param str [in] Source string to use in UTF-8
         * @param len [in] number of characters to use from \a str must be larger than -1
         * @param valid [out] set to false if allocation goes wrong
         * @returns Reference to this String
         */
        String &prepend(const SVOX_Char *str, SVOX_32S len = -1, bool *valid = 0);

        /** @brief Replaces all occurrences of \a oldStr in this
         *  string with \a newStr; returns this String
         *
         * If necessary the string length is extended. If oldStr is
         * empty nothing is changed. \a newStr maybe empty, then
         * oldStr gets removed.
         *
         * @param oldStr [in] string to be replaced in UTF-8
         * @param newStr [in] string to be inserted instead of \a oldStr in UTF-8
         * @param valid [out] set to false if allocation goes wrong
         * @returns Reference to this String
         */
        String &replace(const SVOX_Char *oldStr, const SVOX_Char *newStr, bool *valid = 0);

        /** @brief Replaces all occurrences of \a oldStr in this
         *  string with \a newStr; returns this String
         *
         * If necessary the string length is extended. If oldStr is
         * empty nothing is changed.
         *         
         * @param oldStr [in] string to be replaced in UTF-8
         * @param newStr [in] string to be inserted instead of \a oldStr in UTF-8
         * @param valid [out] set to false if allocation goes wrong
         * @returns Reference to this String
         */
        String &replace(const String &oldStr, const String &newStr, bool *valid = 0);

        /** @brief Removes whitespace at the beginning and the end of
         *  the string; returns this String
         *
         * @param valid [out] set to false if allocation goes wrong
         * @returns Reference to this String
         */
        String &trim(bool *valid = 0);

        /** @brief Removes whitespace at the beginning and the end of
         *  the string and replace whitespace blocks within the string
         *  with one space; returns this String
         *
         * @param valid [out] set to false if allocation goes wrong
         * @returns Reference to this String
         */
        String &simplify(bool *valid = 0);

        /** @brief Returns a SVOX_WChar[] containing a UCS-2
         *  representation of the string. 
         *
         * The caller is responsible for the deletion of the returned
         * array through SVOX_free(). If isNull() is true for the
         * string, 0 is returned. This is no error condition valid is
         * unchanged. If the string is not a valid UTF-8 string, a
         * NULL pointer is returned as result. (*valid) is only set to
         * false incase of an out of memory condition. It does not
         * signal coding errors.
         * 
         * Allocation failures will not invalidate the String object,
         * this is not changed!
         *
         * @param valid [out] set to false if allocation goes wrong
         * @returns a pointer to a UCS-2 string.
         */
        SVOX_WChar *getUcs2Copy(bool *valid = 0) const;

        /** @brief Returns a new String which is a substring of this
         *  String, start is inclusive and end is exclusive!,
         *  i.e. (end - start - 1) is the number of real characters in
         *  the string.
         *
         * If start is outside of the string (i.e. beyond the
         * terminating zero character, an invalid string is
         * returned. If start is on the end character an empty string
         * is returned. If end is less than -1 an invalid string is
         * returned If start is ok and end is outside of the range,
         * the existing part of the range is returned.
         *
         * @param start [in] index to start the new string from
         * @param end [in] index beyond the last character to copy to new string
         * @param valid [out] set to false if allocation goes wrong
         * @returns The new String
         */
        String subString(SVOX_32S start = 0, SVOX_32S end = -1, bool *valid = 0) const;

        /** @brief simple check if utf8 is "correct".  
         *
         * At the moment We rely on SVOX_Utf8CharacterCount(), which does not
         * provide a full check, but a basic check.
         */
        bool isValidUtf8() const;

        /** @brief shared constants null string implemented as member function
         */
        static const String &Null();
        /** @brief shared constants empty string implemented as member function
         */
        static const String &Empty();
        /** @brief shared constant invalid string implemented as member function
         */
        static const String &Invalid();
        
        /** @brief splits string and creates string list
         *
         * Split a stirng into a StringList based on the set of delimiters passed
         * as parameters. 
         *
         * @param delimiterSet string containing the set of valid delimiters
         * @param skipDelimiterSequences if true Empty-Strings will be added into the list
         * @param valid true if valid
         * @return list containing the strings
         */
        StringList split(const String &delimiterSet, bool skipDelimiterSequences = true, bool *valid = 0) const;

        /** @brief converts string into number
         *
         * Converts string into number.
         * If conversion fails and not all characters are set ok is set to false
         *
         * @param[out] ok is set to true if ok
         *
         * @return string converted to number
         */
        SVOX_32S toInt(bool *ok=0) const;
        
    private:
        RCPtr<RCObject<SVOX_Char> > value;

        /**
         * returns true if the characer is a whitespace
         */
        bool isWhiteSpace(const SVOX_Char *character) const;

        /** @brief check value, used after creation
         *
         * private helper that checks if value is 0 and invalidates
         * the object in case. value is reset to shareNull and the
         * valid parameter is updated if an address is provided
         *
         * @param valid [out] pointer to boolean, is set to false if value is 0
         */
        void check(bool *valid);

        /** @brief private constructor that allows the creation of an
         * invalid String, used only to instantiate a shared invalid
         * string object. 
         */
        explicit String(bool valid, const SVOX_Char *val);


    };



}
}

#endif // SVOX_COMMON_STRING_HPP
