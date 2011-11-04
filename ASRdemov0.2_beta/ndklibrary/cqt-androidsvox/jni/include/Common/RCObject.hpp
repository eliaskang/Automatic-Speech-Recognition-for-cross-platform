#ifndef __RCObject__
#define __RCObject__

/**
 * @brief Reference Counted Object
 * 
 * @date $Date:$
 * @author Dr. Stefan Sablatnoeg
 * 
 * @file RCObject.hpp
 * Reference Counted Object. This is a derivative from Scott
 * Meyers design for a reference counted string class. It it superior
 * in terms of memory usage and number of necessary
 * allocations. Unfortunately the design is no longer as clear and
 * elegenat as it was. The basic idea is that each reference counted
 * object is allocated in one chunk of memory together with this
 * class, which keeps the necessary maintenance information. The
 * actual data starts immediately after the class. So we find the
 * actual address of the embedded object by adding this and
 * sizeof(RCObject). Client classes use this class through
 * RCPtr<RCObject> which handles calls to the reference counting
 * mechanism at the appropriate places.
 * 
 * @ingroup Common
 * 
 * Copyright by SVOX Deutschland GmbH 
 * 
 * Transmittal, reproduction, dissemination and/or editing of this 
 * source code as well as utilization of its contents and communication 
 * thereof to others without express authorization are prohibited. 
 * Offenders will be held liable for payment of damages. All rights 
 * created by patent grant or registration of a utility model or design 
 * patent are reserved. 
 * 
 */

#include "SVOX_Types.h"
#include "SVOX_Memory.h"
#include "SVOX_Assert.h"
#include "SVOX_Atomic.h"

#include "RCObjectBase.hpp"
#include "RCObjectDestruction.hpp"

namespace svox
{
namespace common
{


/**
 * @brief Reference Counted Object
 * 
 * Reference Counted Object, keeps the maintenance information
 * of a reference counted object: refcount, size of payload,
 * shareablity. Use this object through RCPtr<RCObject>. Payload
 * is assumed to start immediately after this object (this +
 * sizeof(RCObject)). This design decision is based on the fact
 * that an additional heap allocation is costly.
 * 
 */
template <class T>
class RCObject : public RCObjectBase
{
public:

    /**
     * @brief Commonly used constructor
     * 
     * This constructor takes a pointer to preallocated anonymous
     * memory and a size parameter. It is usually called through a
     * placement new, on a larger memory chunk. The size parameter
     * tells the constructor how much space the preallocated memory
     * can hold in addition to sizeof(RCObject). This additionsl
     * space is called the payload. The payload is initialized from
     * initValue. 
     * 
     * @param initValue [in] points to data that should be copied to payload.
     * @param size [in] size of the area reserved for payload in bytes.
     * @param copysize [in] number fo bytes to copy to payload if less than size.
     * @param count [in] Object count initialization value.
     */
    RCObject(const void *initValue, SVOX_size_t size, SVOX_32S copysize = -1, SVOX_size_t count = 0);

    /**
     * This should be never called, implemented as assert(0) to make sure
     * this is the case, otherwise our reference counting mechanism might
     * be broken. */
    RCObject();
    /**
     * This should be never called, implemented as assert(0) to make sure
     * this is the case, otherwise our referenc coutning mechanism might
     * be broken. 
     *
     * @param rhs [in] other RCObject to assign from
     */
    RCObject(const RCObject &rhs);

    /**
     * @brief Assignment Operator
     * 
     * This should be never called, implemented as assert(0) to make sure
     * this is the case, otherwise our referenc coutning mechanism might
     * be broken. 
     *
     * @param rhs [in] other RCObject to assign from
     * @return reference to (*this)
     */
    RCObject &operator=(const RCObject &rhs);

    /**
     * @brief Destructor
     * 
     * Destructor, explicitly calls destructors for all objects in the payload.
     */
    ~RCObject();


    /**
     * @brief Decrease Reference Count and maybe self destruct
     * 
     * Decrease Reference Count. This is called when appropriate by
     * the RCPtr<T> class. Do no use directly. Reference counting and
     * zero testing is done atomically. If zero is reached, the object
     * commits suicide (including the payload of course).
     */
    void removeReference();
    
    
    /**
     * @brief Shortcut for pointer calculations (const)
     * 
     * Shortcut for pointer calculations, to get the address of the
     * payload. This is the const version. Implemented as a friend, to
     * avoid RCObject to be a template, for the obvious
     * reasons. Inline to avoid the need for explicit instantiation
     * and for performance reasons.
     * 
     * @return Const pointer to first payload element of type T
     */
    const T *data() const;
    
    /**
     * @brief Shortcut for pointer calculations (non const)
     * 
     * Shortcut for pointer calculations, to get the address of the
     * payload. This is the non const version. Implemented as a
     * friend, to avoid RCObject to be a template, for the obvious
     * reasons. Inline to avoid the need for explicit instantiation
     * and for performance reasons.
     * 
     * @return Const pointer to first payload element of type T
     */
    T *data();

   
};

/* default constructor should not be called, to make shure we assert(0) here */
template <class T>
RCObject<T>::RCObject() 
    : RCObjectBase()
{
    /* This should be never called, to make sure we add an assert, so
     * we avoid a wrong default implementation */
    SVOX_Assert(0);
        
}

/* default constructor should not be called, to make shure we assert(0) here */
template <class T>
RCObject<T>::RCObject(const RCObject<T> &) 
    : RCObjectBase()
{
    /* This should be never called, to make sure we add an assert, so
     * we avoid a wrong default implementation */
    SVOX_Assert(0);
}

/* default constructor should not be called, to make shure we assert(0) here */
template <class T>
RCObject<T> &RCObject<T>::operator=(const RCObject<T> &)
{
    /* This should be never called, to make sure we add an assert, so
     * we avoid a wrong default implementation */
     SVOX_Assert(0);
     
     return *this;
}


/* helper to get the payload pointer as const */
template <class T>
const T *RCObject<T>::data() const
{ 
    return reinterpret_cast<const T *>( reinterpret_cast<const RCObject *>(this) + 1 ); 
}

/* helper to get the payload pointer as non const */
template <class T>
T *RCObject<T>::data()
{ 
    return reinterpret_cast<T *>( reinterpret_cast<RCObject *>(this) + 1 ); 
}


/* This is the only valid constructor, has defaults on all args */
template <class T>
RCObject<T>::RCObject(const void *initValue, SVOX_size_t size, SVOX_32S copysize, SVOX_size_t count) 
    : RCObjectBase() /* default does initialize, refcount and sharable*/ 
{
    mSize = size;
    mCount = count;

    
    /* this is the default, i.e. we fill the whole area */
    if (-1 == copysize)
    {
        copysize = size;
    }
    // copy into payload, don't care for its type, we can't use it
    SVOX_memcpy(data(), initValue, copysize);

}

/* tell compiler that we will do a partial specialication and we will
 * provide the code later, this are the basic types, no destructor
 * calls in payload for these */
template <>
RCObject<SVOX_Char>::~RCObject();

template <>
RCObject<SVOX_8U>::~RCObject();

template <>
RCObject<SVOX_8S>::~RCObject();

template <>
RCObject<SVOX_32S>::~RCObject();

template <>
RCObject<SVOX_32U>::~RCObject();

template <>
RCObject<SVOX_16S>::~RCObject();

template <>
RCObject<SVOX_16U>::~RCObject();


/* destructor for all other types of payload, these must have
 * destructors */    
template <class T>
RCObject<T>::~RCObject()
{
    // call destructors on data, no memory to be deleted.
    svox::common::RCObjectDestruction::destruct<T>(data(), mCount);        
    mCount = 0;
    
}


    
template <class T>
void RCObject<T>::removeReference()
{
    if (SVOX_FALSE == SVOX_Atomic_Decrement(&mRefCount)) 
    {
        /* start self destruct sequence ... */
        this->~RCObject();
        SVOX_free(this);
    }
}


}
}

#endif
