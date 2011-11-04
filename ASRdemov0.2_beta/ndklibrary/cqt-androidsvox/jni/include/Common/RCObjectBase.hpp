#ifndef __RCBase__
#define __RCBase__

/**
 * @brief Reference Counted Object Base
 * 
 * @date $Date:$
 * @author Dr. Stefan Sablatnoeg
 * 
 * @file RCObjectBase.hpp
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
class RCObjectBase 
{
private:
    RCObjectBase(const RCObjectBase &);
    RCObjectBase &operator=(const RCObjectBase &);
    
public:
    
    /**
     * @brief Constructor
     */
    RCObjectBase();

    /**
     * @brief Increase Reference Count
     * 
     * Increase Reference Count. This is called when appropriate by
     * the RCPtr<T> class. Do no use directly. Reference counting is
     * done atomically.
     */
    void addReference();

    /**
     * @brief Decrease Reference Count and maybe self destruct
     * 
     * Decrease Reference Count. This is called when appropriate by
     * the RCPtr<T> class. Do no use directly. Reference counting and
     * zero testing is done atomically. If zero is reached, the object
     * commits suicide (including the payload of course).
     */
    void removeReference();
    
#if SVOX_STRING_ADD_NON_CONST_ACCESSORS
    /**
     * @brief Mark the object as unshareable
     * 
     * Mark the object as unshareable. This is needed once the
     * object API gives away non const references or pointers. 
     * 
     */
    void markUnshareable();

    /**
     * @brief Check if the object is shareable
     * 
     * Check if the object is shareable. It would be not if someone
     * has received a non const pointer or reference to it before.
     * 
     * @return true if it is shareable, false if not.
     */
    bool isShareable() const;
#endif /* SVOX_STRING_ADD_NON_CONST_ACCESSORS*/
    
    /**
     * @brief Retrieve the size of the payload
     * 
     * Retrieve the size of the payload. The actual payload size may
     * vary and therefore we need to store it as a member. This method
     * returns it.
     * 
     * @return size of the payload
     */
    SVOX_size_t getSize() const { return mSize; };

    /**
     * @brief Retrieve the number of elements in the payload
     * 
     * Retrieve the number of elements in the payload. The actual
     * number of elements may be less or equal to msize/sizeof(T) and
     * therefore we need to store it as a member. This method returns
     * it.
     * 
     * @return number of elements in the payload
     */
    SVOX_size_t getCount() const { return mCount; };
 
  /**
     * @brief Set the number of elements in the payload
     * 
     * Set the number of elements in the payload. The actual number of
     * elements may be less or equal to msize/sizeof(T) and therefore
     * we need to store it as a member. This method modifies
     * this. This does not trigger allocation or deallocations, thsi
     * must be done via RCObjectFactory::construct/destruct
     * 
     * @param count [in] new number of elements in the payload
     */
    void setCount(SVOX_size_t count) { mCount = count; };
    
    /**
     * @brief Check if the object is already shared.
     * 
     * Check if the object is already shared. This is a simple check
     * of the reference counter beein larger than one.
     * 
     * @return true if there is more than one object referencing this shared object, false if not.
     */
    bool isShared() const;
    

protected:

    /**
     * Reference count
     */
    volatile SVOX_32S mRefCount;

    /**
     *  Size of the payload 
     */
    SVOX_size_t       mSize;

    /**
     *  Number of elements in the payload 
     */
    SVOX_size_t       mCount;


#if SVOX_STRING_ADD_NON_CONST_ACCESSORS
    /** Flag to know if we ever gave away a non const pointer or
     * reference. If we did we can no longer be used shared.  
     *
     */
    bool              mShareable;

#endif /* SVOX_STRING_ADD_NON_CONST_ACCESSORS */

    
};


}
}

#endif
