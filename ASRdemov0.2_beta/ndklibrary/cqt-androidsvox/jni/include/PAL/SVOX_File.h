/**
 * @brief Functions for file management
 *
 * This file contains the common functions for
 * opening, reading and writing files,
 * renaming and deleting files.
 *
 * All functions work in an operating system
 * independent way so it does not matter if you
 * use "/" vs "\" as path separator.
 *
 * Future versions of the implementation will
 * contain functions for resource tracking to check
 * if handles are not closed etc.
 *
 * Another set of functions delivers paths on the
 * operating system in which temp files could be
 * saved, persistend files could be written
 * and read only resources could be found.
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


#ifndef __SVOX_File_H__
#define __SVOX_File_H__

#include "SVOX_BasicString.h"
#include "SVOX_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------*/

typedef struct SVOX_File_ SVOX_File;

typedef enum { 
    SVOX_FOPEN_INVALID             = 0, 
    /**
     * Open text file for reading. The stream is positioned at the
     * beginning of the file.
     */
    SVOX_FOPEN_READ                = 1, 
    /**
     * Open for reading and writing. The stream is positioned at the
     * beginning of the file.
     */
    SVOX_FOPEN_READ_WRITE          = 2, 
    /**
     * Truncate file to zero length or create text file for
     * writing. The stream is positioned at the beginning of the file.
     */
    SVOX_FOPEN_WRITE_TRUNCATE      = 3, 
    /**
     * Open for reading and writing. The file is created if it does
     * not exist, otherwise it is truncated. The stream is positioned
     * at the beginning of the file.
     */
    SVOX_FOPEN_READ_WRITE_TRUNCATE = 4, 
    /**
     * Open for appending (writing at end of file). The file is
     * created if it does not exist. The stream is positioned at the
     * end of the file.
     */
    SVOX_FOPEN_WRITE_APPEND        = 5, 
    /**
     * Open for reading and appending (writing at end of file). The
     * file is created if it does not exist. The initial file position
     * for reading is at the beginning of the file, but output is
     * always appended to the end of the file.
     */
    SVOX_FOPEN_READ_WRITE_APPEND   = 6, 
    SVOX_FOPEN_LAST_MODE           = SVOX_FOPEN_READ_WRITE_APPEND 
} SVOX_FOpenMode;

/**
 * Modes for SVOX_setvbuf(). these constants specify different
 * output buffering modes that can be selected using
 * SVOX_setvbuf().
 */
typedef enum 
{ 
    /**
     * unbuffered output, i.e. every output is passed to the
     * system immediately
     */
    SVOX__IONBF, 
    /**
     * full output buffering, i.e. output is passed to the system
     * only if the buffer is filled or an intervening input
     * occurs.
     */
    SVOX__IOFBF, 
    /**
     * line buffering, i.e. output is passed to the system only
     * when a line ending is encountered or the buffer is full, or
     * an intervening input occurs.
     */
    SVOX__IOLBF    
} SVOX_VBUF_MODE;
    
#define SVOX_EOF (-1)
#define SVOX_BUFSIZ 512

/**
 * @brief Mode for seeking
 */
typedef enum
{
    SVOX_SEEK_SET, /**< Seek from start of file */
    SVOX_SEEK_CUR, /**< Seek from current position */
    SVOX_SEEK_END  /**< Seek from end of file */
} SVOX_File_SEEKORIGIN;


/**
 * @brief Init file handling, monitoring and base paths.
 *
 * @retval SVOX_RESULT_OK if ol
 * @retval SVOX_RESULT_NO_MEMORY in case of no memory
 */
SVOX_Result SVOX_File_Init
    ();

/**
 * @brief Deinit file handling
 *
 * (currently just a dummy)
 */
SVOX_Result SVOX_File_Deinit
    ();

/**
 * @brief Returns the "base path" of the executable.
 *
 * On iPhone this is the application's home directory.
 * On Windows & Linux the path of the executable.
 */
const SVOX_Char *SVOX_File_GetBasePath
    ();

/**
 * @brief Returns a path to a temporary directory
 */
const SVOX_Char *SVOX_File_GetTempPath
    ();

/**
 * @brief Returns a pointer to a stdout handle.
 *
 * The handle is managed internally and must
 * not be closed.
 *
 * @retval 0 in case of an error or if SVOX_File_Init()
 * was not called
 * @retval handle if ok
 */
SVOX_File *SVOX_File_GetStdout
    ();

/**
 * @brief Returns a pointer to a stdin handle.
 *
 * The handle is managed internally and must
 * not be closed.
 *
 * @retval 0 in case of an error or if SVOX_File_Init()
 * was not called
 * @retval handle if ok
 */
SVOX_File *SVOX_File_GetStdin
    ();

/**
 * @brief Returns a pointer to a stderr handle.
 *
 * The handle is managed internally and must
 * not be closed.
 *
 * @retval 0 in case of an error or if SVOX_File_Init()
 * was not called
 * @retval handle if ok
 */
SVOX_File *SVOX_File_GetStderr
    ();

/**
 * Define for easier access to stdout
 */
#define SVOX_stdout SVOX_File_GetStdout()

/**
 * Define for easier access to stdin
 */
#define SVOX_stdin SVOX_File_GetStdin()

/**
 * Define for easier access to stderr
 */
#define SVOX_stderr SVOX_File_GetStderr()
    
/**
 * @brief Open a file
 *
 * Use / as path separator on all platforms.
 * SVOX_fopen automatically converts the separators
 * to the target format if needed.
 *
 * The function also checks if the path name to
 * be opened is a directory and avoids this.
 *
 * The argument mode points to a string which must be @a exactly one
 * of the following sequences:
 *
 * @li r Open text file for reading. The stream is positioned at the
 *   beginning of the file.
 *
 * @li r+ Open for reading and writing. The stream is positioned at
 *    the beginning of the file.
 *
 * @li w Truncate file to zero length or create text file for
 *   writing. The stream is positioned at the beginning of the file.
 *
 * @li w+ Open for reading and writing. The file is created if it does
 *   not exist, otherwise it is truncated. The stream is positioned at
 *   the beginning of the file.
 *
 * @li a Open for appending (writing at end of file). The file is
 *   created if it does not exist. The stream is positioned at the end
 *   of the file.
 *
 * @li a+ Open for reading and appending (writing at end of file). The
 *    file is created if it does not exist. The initial file position
 *    for reading is at the beginning of the file, but output is
 *    always appended to the end of the file.
 *
 * The file is always opened "binary" if the platform requires this to
 * be specified. I.e. it is not allowed to specify @a b or @a t or other
 * modifiers.
 *
 * @param[in] filename name of the file to open
 * @param[in] mode mode to open
 *
 * @retval NULL in case of error, if the file does not exist
 *         or the path is a directory
 * @retval filehande otherwise
 *
 * @deprecated use SVOX_fopen_ex instead.
 */
/* SVOX_DEPRECATED_FUNCTION(SVOX_fopen_ex) */ SVOX_File *SVOX_fopen
(
    const SVOX_Char *filename,
    const SVOX_Char *mode
);
    

    
/**
 * @brief Open a file
 *
 * Use / as path separator on all platforms.
 * SVOX_fopen automatically converts the separators
 * to the target format if needed.
 *
 * The function also checks if the path name to
 * be opened is a directory and avoids this.
 *
 * The argument mode points to a string which must be @a exactly one
 * of the following sequences:
 *
 * @li SVOX_FOPEN_READ Open text file for reading. The stream is
 *   positioned at the beginning of the file.
 *
 * @li SVOX_FOPEN_READ_WRITE Open for reading and writing. The stream is
 *    positioned at the beginning of the file.
 *
 * @li SVOX_FOPEN_WRITE_TRUNCATE Truncate file to zero length or create text file
 *   for writing. The stream is positioned at the beginning of the
 *   file.
 *
 * @li SVOX_FOPEN_READ_WRITE_TRUNCATE Open for reading and writing. The file is
 *   created if it does not exist, otherwise it is truncated. The
 *   stream is positioned at the beginning of the file.
 *
 * @li SVOX_FOPEN_WRITE_APPEND Open for appending (writing at end of file). The
 *   file is created if it does not exist. The stream is positioned at
 *   the end of the file.
 *
 * @li SVOX_FOPEN_READ_WRITE_APPEND Open for reading and appending (writing at
 *    end of file). The file is created if it does not exist. The
 *    initial file position for reading is at the beginning of the
 *    file, but output is always appended to the end of the file.
 *
 * The file is always opened "binary" if the platform requires this to
 * be specified. I.e. it is not allowed to specify @a b or @a t or other
 * modifiers.
 *
 * @param[in] filename name of the file to open
 * @param[in] mode mode to open
 *
 * @retval NULL in case of error, if the file does not exist
 *         or the path is a directory
 * @retval filehande otherwise
 */
SVOX_File *SVOX_fopen_ex
(
    const SVOX_Char *filename,
    SVOX_FOpenMode mode
);
/**
 * @brief Closes an opened file.
 *
 * @param[in] stream file to close
 *
 * @retval 0 if ok
 * @retval SVOX_EOF in case of an error
 */
SVOX_32S SVOX_fclose
(
    SVOX_File *stream
);

/**
 * @brief Reposition stream position indicator
 *
 * @param[in] stream stream handle
 * @param[in] offset offset, may not be negative if (and only if) SVOX_SET_SEEK is specified
 * @param[in] origin Position from where offset is added. It is specified by one of the following constants
 *                SVOX_SEEK_SET, SVOX_SEEK_CUR, SVOX_SEEK_END
 *
 * @retval 0 if ok
 * @retval nonzero on error
 */
SVOX_32S SVOX_fseek
(
    SVOX_File           *stream,
    SVOX_32S             offset,
    SVOX_File_SEEKORIGIN origin
);

/**
 * @brief Read data from a stream
 *
 * Reads an array of nobj elements, each one with a size of size bytes, from the stream and stores them in the block of memory specified by ptr.
 * The position indicator of the stream is advanced by the total amount of bytes read.
 * The total amount of bytes read if successful is (size * nobj)
 *
 * @param[in] ptr ptr to store the data to
 * @param[in] size size of an element to read
 * @param[in] nobj number of elements to read
 * @param[in] stream pointer to the stream to read from
 *
 * @result The total number of elements successfully read is returned.
 * If this number differs from the nobj parameter, either an error occurred or the End Of File was reached.
 *
 */
SVOX_32S SVOX_fread
(
    void       *ptr,
    SVOX_size_t size,
    SVOX_size_t nobj,
    SVOX_File  *stream
);

/*
 * @brief Check End-of-File indicator
 *
 * Checks whether the End-of-File indicator associated with stream is set,
 * returning a value different from zero if it is.
 * This indicator is generally set by a previous operation on the stream
 * that reached the End-of-File.
 * Further operations on the stream once the End-of-File has been reached
 * will fail until either  SVOX_fseek is successfully called to set the
 * position indicator to a new value.
 *
 * @param[in] stream the stream handle
 *
 * @result  A non-zero value is returned in the case that the End-of-File indicator
 *  associated with the stream is set.
 *  Otherwise, a zero value is returned.
 *
 */
SVOX_32S SVOX_feof
(
    SVOX_File *stream
);

/*
 * @brief Get character from stream
 *
 * Returns the character currently pointed by the internal file position
 * indicator of the specified stream. The internal file position indicator
 * is then advanced by one character to point to the next character.
 *
 * @param[in] stream the stream handle
 *
 * @result The character read is returned as an int value, SVOX_EOF
 *         if the end of file is reached.
 */
SVOX_32S SVOX_fgetc
(
    SVOX_File *stream
);

/**
 * @brief Get string from stream
 *
 * Reads characters from stream and stores them as a C string into str
 * until (num-1) characters have been read or either a newline or a the
 * End-of-File is reached, whichever comes first.
 *
 * A newline character makes fgets stop reading, but it is considered a valid
 * character and therefore it is included in the string copied to str.
 *
 * A null character is automatically appended in str after the characters
 * read to signal the end of the C string.
 *
 * @param[in] str Pointer to an array of chars where the string read is stored.
 * @param[in] num Maximum number of characters to be read
 *            (including the final null-character). Usually, the length of
 *            the array passed as str is used.
 * @param[in] stream  Pointer to a FILE object that identifies the stream where
 *                characters are read from
 *
 * @result On success, the function returns the same str parameter.
 *         If the End-of-File is encountered and no characters have been read,
 *         the contents of str remain unchanged and a null pointer is returned.
 *         If an error occurs, a null pointer is returned.
 */
SVOX_Char *SVOX_fgets
(
    SVOX_Char *str,
    SVOX_32S   num,
    SVOX_File *stream
);

/**
 * @brief Get current position in stream
 *
 * Returns the current value of the position indicator of the stream.
 * For binary streams, the value returned corresponds to the number of
 * bytes from the beginning of the file.
 * For text streams, the value is not guaranteed to be the exact number
 * of bytes from the beginning of the file, but the value returned
 * can still be used to restore the position indicator
 * to this position using fseek.
 *
 * @param[in] file Pointer to a FILE object that identifies the stream.
 *
 * @result On success, the current value of the position indicator is returned.
 *         If an error occurs, -1L is returned
 */
SVOX_32S SVOX_ftell
(
    SVOX_File *file
);

/**
 * @brief Write block of data to stream
 * Writes an array of nobj elements, each one with a size of size bytes,
 * from the block of memory pointed by ptr to the current position in the stream.
 * The position indicator of the stream is advanced by the total number
 * of bytes written. The total amount of bytes written is (size * nobj).
 *
 * @param[in] ptr  Pointer to the array of elements to be written.
 * @param[in] size Size in bytes of each element to be written.
 * @param[in] nobj Number of elements, each one with a size of size bytes.
 * @param[in] stream Pointer to a FILE object that specifies an output stream.
 *
 * @result The total number of elements successfully written is returned
 *        If this number differs from the nobj parameter, it indicates an error.
 */

SVOX_32S SVOX_fwrite
(
    const void *ptr,
    SVOX_size_t size,
    SVOX_size_t nobj,
    SVOX_File  *stream
);

/**
 * @brief Flush stream
 *
 * If the given stream was open for writing and the last i/o operation was
 * an output operation, any unwritten data in the output buffer is written
 * to the file.
 *
 * If it was open for reading and the last operation was an input operation,
 * the behaviour of the function is not defined. It may on some systems
 * clear the input buffer.
 *
 * The stream remains open after this call.
 *
 * When a file is closed, either because of a call to fclose or
 * because the program terminates, all the buffers associated
 * with it are automatically flushed.
 *
 * @param[in] stream Pointer to a SVOX_File  that specifies a buffered stream
 *
 * @return A zero value indicates success.
 * If an error occurs, SVOX_EOF is returned
 */
SVOX_32S SVOX_fflush
(
    SVOX_File *stream
);

/**
 * @brief Write character to stream
 *
 * Writes a character to the stream and advances the position indicator.
 *
 * The character is written at the current position of the stream as indicated
 * by the internal position indicator, which is then advanced one character.
 *
 * @param[in] c Character to be written
 * @param[in] stream Pointer to a FILE object that identifies the stream where the character is to be written.
 *
 * @result If there are no errors, the same character that has been written is returned.
 * If an error occurs, SVOX_EOF is returned
 */
SVOX_32S SVOX_fputc
(
    SVOX_Char  c,
    SVOX_File *stream
);

/**
 * @brief  Write formatted output to stream
 * Writes to the specified stream a sequence of data formatted as the
 * format argument specifies. After the format parameter, the function expects
 * at least as many additional arguments as specified in format.
 *
 * @param[in] stream Pointer to a FILE object that identifies the stream.
 * @param[in] format formatstring to write
 *
 * @result On success, the total number of characters written is returned.
 *          On failure, a negative number is returned.
 *
 * @remark The current implementation uses the operating system's
 *         function to write the string.
 *         Future implementations will use own formatting routine
 *         to be consistent over all platforms.
 */
SVOX_32S SVOX_fprintf
(
    SVOX_File       *stream,
    const SVOX_Char *format,
    ...
);

/**
 * @brief  Write formatted output to stream
 * Writes to the specified stream a sequence of data formatted as the
 * format argument specifies. After the format parameter, the function expects
 * at least as many additional arguments as specified in format.
 *
 * @param[in] stream Pointer to a FILE object that identifies the stream.
 * @param[in] format format string to write
 * @param[in,out] list argument list
 *
 * @result On success, the total number of characters written is returned.
 *          On failure, a negative number is returned.
 *
 * @remark The current implementation uses the operating system's
 *         function to write the string.
 *         Future implementations will use own formatting routine
 *         to be consistent over all platforms.
 *
 * @warning This function may change list so that it points
 *          to the end of the var args. If you need list
 *          after calling this function you have to copy
 *          it using SVOX_va_copy before calling the function.
 */
SVOX_32S SVOX_vfprintf
(
    SVOX_File       *stream,
    const SVOX_Char *format,
    SVOX_va_list     list
);


/**
 * @brief Scan formatted string from file.
 *
 * Reads a formatted string from file and assigns
 * values to the variable pointers given
 * as parameters
 *
 * @see SVOX_vfscanf for datailed description of the
 *      format values accepted
 *
 * @param[in] stream the file to read from
 * @param[in] format format of the data to read
 * @param[in] ... parameter list to fit he format
 *
 * @result number of filled fields, not including %n values and %* values.
 *        SVOX_EOF in case of an error
 *
 */
SVOX_32S SVOX_fscanf
(
    SVOX_File       *stream,
    const SVOX_Char *format,
    ...
);

/**
 * @brief Scan formatted string from file.
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
 * @param[in] stream file handle to read from
 * @param[in] format format string, see above for details
 * @param[in] list SVOX_va_list containing the variable arguments matching the format
 *
 * @retval SVOX_EOF in case of error
 * @result Number of values assigned, even if the file ended
 *
 * @remark If you need additionals chars at the end of the
 * string to be parsed, e.G. (%d) use "(%d)%n" and
 * check for 2 assigned values.
 *
 * @warning This function may change list so that it points
 *          to the end of the var args. If you need list
 *          after calling this function you have to copy
 *          it using SVOX_va_copy before calling the function.
 */
SVOX_32S SVOX_vfscanf
(
    SVOX_File       *stream,
    const SVOX_Char *format,
    SVOX_va_list     list
);


/**
 * @brief Print string to console
 *
 * @remark should be reimplemented to use own string
 * formatting routines
 *
 * @param[in] format format to print
 */
SVOX_32S SVOX_printf
(
    const SVOX_Char *format,
    ...
);

/**
 * @brief Print string to console, using varargs
 *
 * @remark should be reimplemented to use own string
 * formatting routines
 *
 * @param[in] format format to print
 * @param[in] list parameter list
 *
 * @warning This function may change list so that it points
 *          to the end of the var args. If you need list
 *          after calling this function you have to copy
 *          it using SVOX_va_copy before calling the function.
 */
SVOX_32S SVOX_vprintf
(
    const SVOX_Char *format,
    SVOX_va_list     list
);

/**
 * Get the length of the file
 *
 * @param[in] stream handle of the file
 *
 * @retval number of bytes in the file
 * @retval -1 on error
 */
SVOX_32S SVOX_File_GetSize
(
    SVOX_File *stream
);
    
/**
 * @brief Remove a file from the file system.
 *
 * @param[in] filename name of the file to delete
 *
 * @remark please check for SVOX_RESULT_OK,
 *        may be extended to more detailed error
 *        code in the future
 *
 * @retval SVOX_RESULT_OK if ok
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 filename is NULL
 */
SVOX_Result SVOX_File_Delete
(
    const SVOX_Char *filename
);

/**
 * @brief Copy a file.
 *
 * @param[in] srcname  Name of the file to copy
 * @param[in] destname New name of the file
 *
 * @remark Copying is only suppoorted for plain files, especially
 *         directories are not supported.
 *
 * @remark please check for SVOX_RESULT_OK,
 *        may be extended to more detailed error
 *        code in the future
 *
 * @remark if destname does already exist, it is replaced without further notice.
 *
 * @retval SVOX_RESULT_OK if ok
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 srcname is NULL
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 destname is NULL
 * @retval SVOX_RESULT_ERROR_NOT_FOUND if a problem occured during opening the source file
 * @retval SVOX_RESULT_ERROR_NO_MEMORY if no not enough memory is available (may only happen for win32)
 * @retval SVOX_RESULT_ERROR for anything else that can go wrong
 */
SVOX_Result SVOX_File_Copy
(
    const SVOX_Char *srcname,
    const SVOX_Char *destname
);

/**
 * @brief Rename a file or directory.
 *
 * @param[in] oldname Name of the file to rename
 * @param[in] newname New name of the file
 *
 * @remark @li  Please check for SVOX_RESULT_OK,
 *        may be extended to more detailed error
 *        code in the future.
 *
 * @remark @li If newname refers to an existing file,
 *  the new file will replace the old file.
 *  on some platforms the new file is deleted
 *  and then renamed
 *
 * @remark @li On posix platforms the following is true:
 *    The `rename' function renames the file OLDNAME to NEWNAME.  The
 *    file formerly accessible under the name OLDNAME is afterwards
 *    accessible as NEWNAME instead.  (If the file had any other names
 *    aside from OLDNAME, it continues to have those names.)
 *
 *    @remark @li The directory containing the name NEWNAME must be on the same file
 *    system as the directory containing the name OLDNAME.
 *
 *    @remark @li One special case for `rename' is when OLDNAME and NEWNAME are two
 *    names for the same file.  The consistent way to handle this case
 *    is to delete OLDNAME.  However, in this case POSIX requires that
 *    `rename' do nothing and report success--which is inconsistent.  We
 *    don't know what your operating system will do.
 *
 *    @remark @li If OLDNAME is not a directory, then any existing file named
 *    NEWNAME is removed during the renaming operation.  However, if
 *    NEWNAME is the name of a directory, `rename' fails in this case.
 *
 *    @remark @li If OLDNAME is a directory, then either NEWNAME must not exist or
 *    it must name a directory that is empty.  In the latter case, the
 *    existing directory named NEWNAME is deleted first.  The name
 *    NEWNAME must not specify a subdirectory of the directory `oldname'
 *    which is being renamed.
 *
 *    @remark @li One useful feature of `rename' is that the meaning of NEWNAME
 *    changes "atomically" from any previously existing file by that
 *    name to its new meaning (i.e., the file that was called OLDNAME).
 *    There is no instant at which NEWNAME is non-existent "in between"
 *    the old meaning and the new meaning.  If there is a system crash
 *    during the operation, it is possible for both names to still
 *    exist; but NEWNAME will always be intact if it exists at all.
 *
 *
 *
 * @remark @li If the rename crosses filesystem boundaries on UNIX
 * platforms or fails on other platforms, the rename will be done by a
 * copy/delete sequence. So it may happen, that you end up with both
 * (the old and new) files in place. In this case the behaviour for an
 * existing file may differ from the normal case.  The copy is
 * implemented using SVOX_File_Copy, so it will only work for files
 * not for directories as rename might in the normal case.
 *
 * @retval SVOX_RESULT_OK if ok
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 oldname is NULL
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 newname is NULL
 */
SVOX_Result SVOX_File_Rename
(
    const SVOX_Char *oldname,
    const SVOX_Char *newname
);

/**
 * Checks if a file exists
 *
 * Is only true for \a files. Is false for directories.
 *
 * @warning When testing for the existency of a file
 * it could be created directly after the test by somebody...
 *
 * @param[in] filename name of the file to check
 *
 * @retval SVOX_TRUE if it exists
 * @retval SVOX_FALSE if it does not exist or an error occured
 */
SVOX_Bool SVOX_File_Exists
(
    const SVOX_Char *filename
);

/**
 * @brief Get character from stdin
 *
 * Returns the character read from the console
 *
 * @result The character read is returned as an int value, SVOX_EOF
 *         if the end of file is reached.
 */
SVOX_32S SVOX_getchar
    ();


/**
 * @brief Get utf8 string from stdin
 *
 * Reads characters from stdin and stores them as a UTF8 C string into str
 * until (num-1) characters have been read or either a newline or a the
 * End-of-File is reached, whichever comes first.
 *
 * A newline character makes fgets stop reading, but it is considered a valid
 * character and therefore it is included in the string copied to str.
 *
 * A null character is automatically appended in str after the characters
 * read to signal the end of the C string.
 *
 * @param[in] str Pointer to an array of chars where the string read is stored.
 * @param[in] num Maximum number of characters to be read
 *            (including the final null-character). Usually, the length of
 *            the array passed as str is used.
 *
 * @result On success, the function returns the same str parameter.
 *         If the End-of-File is encountered and no characters have been read,
 *         the contents of str remain unchanged and a null pointer is returned.
 *         If an error occurs, a null pointer is returned.
 */
SVOX_Char *SVOX_getUtf8ConsoleString
(
    SVOX_Char *str,
    SVOX_32S   num
);

/**
 * @brief Converts a path to match OS sepecific needs
 *
 * Converts a path to an os specific version, e.g.
 * by replacing / etc.
 *
 * This function converts relative paths under Windows CE
 * to be relative to the executable's path, emulating
 * a working directory.
 *
 * @remark Use SVOX_BasicString_Release() to
 *         free the path.
 *
 * @param[in] path pointer to the path
 *
 * @return Pointer to a SVOX_BasicString containing the path
 * @retval NULL in case of error
 */
SVOX_BasicString *SVOX_File_ConvertPathToOS
(
    const SVOX_Char *path
);

/**
 * @brief Read a complete file into memory, return pointer
 * to the file.
 * 
 * @param[in] name name of the file to read
 * @param[in] appendZero set this flag to add a terminating 0 at the end of the file
 * @param[out] memPtr pointer in which the pointer to the memory is written
 * @param[out] sizePtr size of the file, may be 0
 * 
 * @retval SVOX_RESULT_OK if ok
 * @retval SVOX_RESULT_ERROR_NO_MEMORY
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_3 memPtr is NULL
 * @retval SVOX_RESULT_ERROR_NOT_FOUND file not found / open error
 * @retval SVOX_RESULT_ERROR_UNKNOWN unknown error during reading the file
 */
SVOX_Result  SVOX_File_ReadAll(const SVOX_Char *name, SVOX_Bool appendZero, SVOX_Char **memPtr, SVOX_size_t *sizePtr);

/**
 * @brief write a character string to a file
 *
 * Writes the zero terminated string s to the file given by fp. The
 * terminating zero is not written.
 *
 * @params s string to write to the file
 * @param fp file pointer the string should be written to
 * @retval 0 on success
 * @retval SVOX_EOF on failure
 */
SVOX_32S SVOX_fputs(const SVOX_Char *s, SVOX_File *fp);

/**
 * @brief create a unique temporary file
 *
 * creates a uniqueliy named temporary file, the creation is atomic.
 * If no template (i.e. NULL) is specified, a default is used which is placed in 
 * the directory returned by SVOX_File_GetTempPath(). 
 *
 * @param[in] template to use for the path
 * @return opened file pointer 
 */
SVOX_File *SVOX_File_OpenTmpFile(const SVOX_Char *nametemplate);

/**
 * @brief mark a file to be deleted when the file pointer is closed
 *
 * @param[in] fp file pointer to mark for deletion
 */
void SVOX_File_DeleteOnClose(SVOX_File *fp);
    
/**
 * @brief Set the kind of buffering for a file stream
 *
 * Set the kind of buffering for a file or stream. 
 *
 * @note May only be used immediately after opening the file or stream.
 *
 * @param fp file pointer of an open file
 * @param buf pointer to user memory that should be used for buffering
 * @param mode buffering mode see \a SVOX_VBUF_MODE
 * @param size of the buffer to use
 * @retval SVOX_EOF in case of error
 * @retval 0 on success
 */
SVOX_32S SVOX_setvbuf(SVOX_File *fp, SVOX_Char *buf, SVOX_VBUF_MODE mode, SVOX_size_t size);

/**
 * @brief retrieve the error state for an open file pointer
 *
 * @param fp file pointer to retrieve the error state for
 * @retval 0  if no errors have occurred; 
 * @retval a nonzero value otherwise.
 */
SVOX_32S SVOX_ferror(SVOX_File *fp);

/**
 * @brief clear the error state for an open file pointer
 *
 * @param fp file pointer to clear the error state in
 */
void SVOX_clearerr(SVOX_File *fp);

 /* -----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

