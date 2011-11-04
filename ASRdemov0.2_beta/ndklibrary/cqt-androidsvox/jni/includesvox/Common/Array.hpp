#ifndef SVOX_COMMON_ARRAY_HPP
#define SVOX_COMMON_ARRAY_HPP
/* @file   Array.hpp
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
/** \brief An Implicit Shared Array Class (template).
 * @file   Array.hpp
 *
 * This file contains the API of the svox::common::Array container class. 
 */

#include "SVOX_Types.h"

#include "RCPtr.hpp"
#include "RCObject.hpp"
#include "RCObjectFactory.hpp"
#include "BaseObject.hpp"


/** \brief A namespace for all SVOX modules
 * 
 * This namespace collects every namespace, class, function or
 * defintion of SVOX. All classes or namespaces use this as a prefix,
 * to avoid name clashes when integrating our software into a larger
 * project.
 */
namespace svox {
/** \brief A namespace modules that are of %common interest
 * 
 * This namespace collects modules of different domains. Mainly it
 * provides our container classes for Arrays and Strings.
 */
namespace common {

   
/**
 *  @brief This class implements an array container.
 *
 *  The internal array representation is "implicit shared", i.e. the copy
 *  constructor and the the assignment operation are very fast, and an
 *  additional copy of an array consumes only a few bytes of additional memory.
 *  If a shared Array object is modified, the array data is copied ("copy on write").
 *  This requires a heap memory allocation.
 *
 *  Each method which (possibly) tries to allocate dynamic memory can
 *  fail. If a memory allocation fails, the (optional) \a valid
 *  parameter is set to false, and the Array object is internally
 *  marked as "invalid".  If a memory allocation fails, or a method is
 *  called on an already "invalid" Array, the contents of the array
 *  remain unchanged and \a valid is set to false. If a constructor
 *  fails, an empty array is created and marked as "invalid". If a
 *  method is successful, the \a valid parameter is not modified.
 *
 *  The error codes need not be checked after each operation, they can
 *  be collected as shown in the following example:
 *
 *  \code
 *      Array<int> squares(len);
 *      for (int i = 0; i < len; i++)
 *      {
 *          squares.set(i, i*i);
 *      }
 *      if (!squares.isValid())
 *      {
 *          return ERROR_OUT_OF_MEMORY;
 *      }
 *  \endcode
 *
 *  If objects derived from \a BaseObject are added to the array, the
 *  array checks if they are valid. If not, the array itself is also
 *  marked as invalid. This simplifies error handling even more:
 *
 *  \code
 *      Array<String> strList;
 *      strList.append(String("Hello"));
 *      strList.append(String("World"));
 *      strList.append(String("!"));
 *
 *      if (!strList.isValid())
 *          // ...
 *  \endcode
 *
 *  The minimum requirements for classes T in Array<T> are
 *   - Copy construcable
 *   - Default constuctable
 *   - comparism operator
 *
 *
 */
template <typename T>
class Array : public BaseObject
{
public:
    /**
     *  @name methods which do not allocate dynamic memory
     *  @{
     */

    /** 
     * @brief Creates an empty array 
     *
     * does not allocate memory, only refers to a shared internal
     * "empty" array.
     *
     * @code 
     *  Array::Array(const Array &other) 
     * @endcode
     *
     * @brief Creates a copy of \a other; this is very fast, as Array
     * is an implicit shared class
     *
     * This
     * constructor is not implemented explicitly, the compiler
     * provided default does magically the right thing.
     *
     * @li other [in] string to take a copy from 
     *
     * @see svox::common::RCPtr, svox::common::RCObject
     *
     * @code 
     * Array::~Array() 
     * @endcode
     *
     * @brief Destroys the array object, correctly handles sharing
     * 
     * Destroy our reference to the (possibly) shared array that
     * was represented by this class instance. The actual array
     * is only discarded iff the reference count of it goes to
     * zero. This constructor is not implemented explicitly, the
     * compiler provided default does magically the right thing.
     *
     * @see svox::common::RCPtr, svox::common::RCObject
     *
     * @code 
     *  void Array::operator=(const Array &other) 
     * @endcode
     * 
     * @brief Assigns \a other to this array; this is very fast,
     * as Array is an implicit shared class. 
     *
     * Assigns \a other to this array; this is very fast, as
     * Array is an implicit shared class. A copy is only taken if
     * the source array \a other is marked non shareable. This
     * constructor is not implemented explicitly, the compiler
     * provided default does magically the right thing.
     *
     * @li other [in]  array to take a copy from 
     *
     * @see svox::common::RCPtr, svox::common::RCObject
     */
    Array();


    /** @brief Returns the number of elements stored in the array 
     *
     * This refers to the number of constructed elements. Will return
     * 0 if the internal state is invalid.
     */
    SVOX_32S count() const;

    /** @brief Returns a pointer to the data stored in this Array
     *
     *  Will return NULL if the internal state is invalid.
     *
     *  Note: this pointer remains only valid as long as no other
     *  methods are called on the Array object
     */
    const T *data() const;

    /** @brief Returns true, if the array is empty 
     *
     * Empty refers to zero number of constructed elements.  Will
     * return false if the internal state is invalid.
     */
    bool isEmpty() const;

    /** @brief Deletes the content of the array 
     *
     * Removes all elements in the array, calls destructors of
     * elements.  Won't fail, because the result refers to the shared
     * instance of the empty Array, therefore no valid parameter.
     * Will work even if the state is invalid. In this case you will
     * definitely loose the data that might have been recoverable
     * through setValid().
     */
    void clear();

    /** @brief Returns the element at position \a index
     *
     *  If no element at that position does exist, it returns the
     *  reference to an internal dummy object, that is default created.
     *  Will also return the dummy object if the internal state is invalid.
     *
     *  Note: the returned reference remains only valid as long as the
     *  array is not modified.
     *
     * @param index [in] to retrieve
     */
    const T &get(SVOX_32S index) const;

    /** @brief returns the index of the first element that is equal to
     * \a rhs in this array. Search is performed within index range
     * [from,to].
     *
     * If the element is found the index is returned, if not, -1 is
     * returned. The same happens, if the search area is out of bounds
     * Will return -1 if the internal state is invalid.
     *
     * @param rhs [in] reference object that is compared against the elements
     * @param from [in] start index
     * @param to [in] last index to check
     */
    SVOX_32S indexOf(const T &rhs, SVOX_32S from = 0, SVOX_32S to = -1) const;

    /** @brief resets the state of the Array to valid.
     *
     * To be able to acces the still existing information in the Array
     * in case of allocation failure, the state of the Array can be
     * reset to valid.  All routines that set the state to invalid
     * will keep the state before the failed call. The contents will
     * be inaccessible while the state is invalid.
     *
     */
    void setValid();
    
    /**
     *  @}
     *  @name memory allocating methods
     *   see "Detailed Description" for information about error handling.
     *  @{
     */

 
    /** @brief Creates an array of preallocated size \a count *
     * sizeof(count) count elements constructed via the default
     * constructor 
     *
     * @param size [in] number of element to construct in the new array
     * @param valid [out] set to false if allocation goes wrong
     */
    explicit Array(SVOX_32S size, bool *valid = 0);

    /** @brief Resizes the Array. 
     * 
     * This method will allocate in construct new elements resulting
     * in an array of the given size with \a size elements. If \a size
     * is less than \a count(), count()-size elements are destructed
     * and removed from the end of the array. The current
     * implementation does alway reallocate if the existing capacity
     * is not equal to the requested size (even if it shrinks, this
     * might change in future.
     *
     * @note Will do nothing if the internal state is invalid.
     *
     * @param size [in] number of element for the array to have after this call
     * @param valid [out] set to false if allocation goes wrong
     */
    void resize(SVOX_32S size, bool *valid = 0);

    /** @brief Resizes the internal buffer. 
     * 
     * This method does not increase or decrease count().
     * This method can be used to improve the performance if append()
     * is called multiple times. It can also be used to shrink the
     * internal buffer after deleting array elements.  This method
     * will not destroy elements. If \a capacity is less than \a
     * count() the actual capacity is set to count().
     *
     * @see resize() for actually creating destroying elements
     *
     * @note Will do nothing if the internal state is invalid.
     *
     * @param capacity [in] number of elements to have room for in the
     * array after this call
     * @param valid [out] set to false if allocation goes wrong
     */
    void reserve(SVOX_32S capacity, bool *valid = 0);

    /** @brief Appends the passed element \a rhs to the end of the array 
     *
     * @param rhs [in] element to add at the end of the array
     * @param valid [out] set to false if allocation goes wrong
     */
    void append(const T &rhs, bool *valid = 0);

    /** @brief Appends \a numElems passed elements to the end of the array 
     *
     * @param numElems [in]  number of elements to add
     * @param elems    [in]  pointer to the elements to add
     * @param valid    [out] set to false if allocation goes wrong
     */
    void appendAll(const SVOX_32S numElems, const T* elems, bool *valid = 0);

    /** @brief Removes the last element from the array and returns it 
     * 
     * @note: The memory occupied is not freed.
     * @note: Will do nothing if the internal state is invalid.
     *
     * @param valid [out] set to false if allocation goes wrong
     * @return element that has been the last array entry before
     */
    T popLast(bool *valid = 0);

    /** @brief Inserts \a rhs at position \a index into the
     * array. Index must be in the range of [0,count()].
     *
     * The existing elements following index will be shifted (usually
     * through copy-construction and destruction) to their new
     * positions.
     *
     * Insertion at the end (i.e. at index count()) will append.
     *
     * @note: Will do nothing if the internal state is invalid.
     *
     * @param index [in] position for the new element to insert 
     * @param rhs [in] element to insert 
     * @param valid [out] set to false if allocation goes wrong
     */
    void insert(SVOX_32S index, const T &rhs, bool *valid = 0);

    /** @brief retrieve a temporary non const reference to change
     * elements in the array in place.
     *
     * If the array is already shared, it is copied first, to make
     * sure nobody else sees the changes applied later.  Therefore
     * this operation can destroy your Array (i.e. make it invalid) 
     * but only in the case of sharing.  Out of bounds access return
     * the dummy object.
     *
     *  Note: the returned reference remains only valid as long as the
     *  array is not modified.  Do not store the reference!  The
     *  object does not track whether it gave away writeable
     *  references, if you start sharing it, you will get problems,
     *  because you will change all objects that share the
     *  instance. Do not rely on this behaviour it might change to a
     *  more safe one!
     *
     * @note: Will return the dummy object if the internal state is invalid.
     *
     * for the validity check valid! 
     *
     * @param [in] index index of the element to return a reference to
     * @param [out] valid
     */
    T &getWritable(SVOX_32S index, bool *valid = 0);

    /** @brief Copies \a rhs to position \a index of the array
     *
     * The existing element at position \a index will be replaced.
     * If index is out of bounds the request is ignored.
     *
     * @note: Will do nothing if the internal state is invalid.
     *
     * @param index [in] position to replace with the new element 
     * @param rhs [in] new value for the element at position \a index 
     * @param valid [out] set to false if allocation goes wrong
     * @return true if the command succeeded
     * @return false if the index was out of range or the 
     *         allocation failed
     */
    bool set(SVOX_32S index, const T &rhs, bool *valid = 0);

    /** @brief Deletes \a count element starting at position \a
     * index 
     *
     * If the range to be deleted is full or partly out of bounds,
     * only the existing elements are deleted and the rest is ignored.
     *
     * @note: Will do nothing if the internal state is invalid.
     *
     * @param index [in] position of the first element to remove 
     * @param count [in] number of elements to remove 
     * @param valid [out] set to false if allocation goes wrong
     */
    void erase(SVOX_32S index, SVOX_32S count = 1, bool *valid = 0);

    /** @brief Deletes all elements from the array which equal \a rhs 
     *
     * All elements are checked against \a rhs, all the elements that
     * are equal are removed from the array.
     * 
     * @note: Will do nothing if the internal state is invalid.
     *
     * @param rhs [in] element * to compare against existing elements
     * @param valid [out] set to false if allocation goes wrong
     */
    void eraseAll(const T& rhs, bool *valid = 0);

private:
    /**
     * member function that returns a pointer to a shared instance of
     * an empty array. It is hidden through the function, to avoid
     * problems with the sequence of non-local static member
     * initialization. For details why this avoids the problem, see
     * Scott Meyers, Effective C++ 3rd Ed., Item 4 : "Make sure items
     * are initialized before they're used."
     */
    RCPtr<RCObject<T> > &sharedEmpty();
    
    /**
     * static member that hold a dummy object to be returned on outo
     * fo bounds accesses
     */
    static T& dummy();

    RCPtr<RCObject<T> > value;

    /**  @brief internal helper to check for allocation failure and
     *  handle the pointer to valid bool flag
     * 
     * @param [out] pointer to bool flag, is set to false, if value is
     * 0, i.e. iff allocation fails.
     */
    bool check(bool *valid, RCPtr<RCObject<T> > &restoreValue);

    
};

} } // namespaces

#include "ArrayImpl.hpp"



#endif // SVOX_COMMON_ARRAY_HPP
