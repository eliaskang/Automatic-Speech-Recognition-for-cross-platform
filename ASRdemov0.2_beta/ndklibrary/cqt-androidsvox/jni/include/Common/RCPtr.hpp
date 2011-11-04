#ifndef __RCPTR__
#define __RCPTR__

/**
 * @brief Abstract Pointer to Reference Counted Class
 * 
 * @date $Date:$
 * @author Dr. Stefan Sablatnoeg
 * 
 * @file RCPtr.hpp
 * The class behaves as a pointer, but its (copy) constructors
 * and assignment operators are such that they correctly trigger
 * reference count methods of a reference counted object. T must
 * provide these.
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




namespace svox
{
namespace common
{
    
/**
 * @brief Abstract Pointer to Refernce Counted Object
 * 
 * Abstract pointer, i.e. a class that behaves like a pointer. It has
 * an operator* and operator->.  Its main functionality it to call
 * addReference() and removeReference() on the client object of type
 * T. Can be constructed from pointer to an instance of T of from
 * another of these ptrs. For details see Scott Meyers: More Effective
 * C++, pp.183-213.
 * 
 */
template <class T>
class RCPtr 
{
public:
    /**
     * @brief Construct from pointer to client object, corresponds to the copy constructor of the pointer to T.
     * 
     * Construct from pointer to client object, corresponds to the
     * copy constructor of the pointer to T. In this case a refernce
     * counted copy is allocated and its reference count is increase
     * (i.e. set to 1).
     * 
     * @param realPtr pointer to client object, defaults to 0
     */
    RCPtr(T* realPtr = 0);

    /**
     * @brief Copy constructor
     * 
     * Copy constructor, override is necessary to handle reference
     * counting correctly, in this case increase refernce count if the
     * object is shareable, else initiate a real copy.
     * 
     * @param rhs [in]source for copy
     */
    RCPtr(const RCPtr &rhs);

    /**
     * @brief Destructor
     * 
     * The desctructor of a pointer to a reference counted object just
     * needs to decrease the reference counter. The reference counted
     * object is expected to commit suicide once its reference count
     * reaches zero.
     * 
     */
    ~RCPtr();

    /**
     * @brief Assignment operator
     * 
     * The assignment operator is overwritten because it need to
     * handle the reference coutnign of the client objects. The old
     * object we pointed to reduces its reference coutn, the new
     * object's count is increased. Self assignment is handled.
     * 
     * @param rhs [in] object to assign us from
     * @return reference to (*this)
     */
    RCPtr &operator=(const RCPtr &rhs);

    /**
     * @brief comparism operator
     * 
     * The comparism operator is overwritten to allow checks agains 0
     * without referencing to operatior->()
     */
    bool operator==(const RCPtr &rhs) const;
    

    /**
     * @brief Pointer opererator
     * 
     * Pointer opererator, this is just a forward to the real
     * pointer. Does not affect reference counting. Inlined to allow
     * optimization to eliminate function call.
     * 
     * @return pointer to reference counted object
     */
    T *operator->() const
    {
        return pointee;
    }


    /**
     * @brief Pointer Dereferencing Operator
     * 
     * Pointer dereferencing opererator, this is just a forward to the
     * dereferencing of the real pointer. Does not affect reference
     * counting.  Inline to allow optimization to eliminate function
     * call.
     * 
     * @return object referenced by pointer.
     */
    T &operator*() const
    {
        return *pointee;
    }
        
private:
    /**
     * @brief pointer that holds the address of the reference counted object
     * 
     * pointer that holds the address of the reference counted object
     * 
     */
    T *pointee;

    /**
     * @brief Initialization
     * 
     * Initialization method, collection most work needed by nearly all constructors and assignment operators
     * 
     */
    void init();
};

template <class T>
RCPtr<T>::RCPtr(T* realPtr)    
    : pointee(realPtr)
{
    init();
}

template <class T>
RCPtr<T>::RCPtr(const RCPtr<T> &rhs)
    : pointee(rhs.pointee)
{
    init();
}
    
template <class T>
RCPtr<T>::~RCPtr()
{
    if (pointee) 
    {
        /* removeReference will trigger self destruction of (*pointee)
         * iff appropriate */
        pointee->removeReference();
    }
}

template <class T>
bool RCPtr<T>::operator==(const RCPtr &rhs) const
{
    return (pointee == rhs.pointee);
}


template <class T>
RCPtr<T> &RCPtr<T>::operator=(const RCPtr<T> &rhs)
{
    if (pointee != rhs.pointee)
    {
        T *oldPointee = pointee;
        pointee = rhs.pointee;
        init();
        if (oldPointee)
        {
            oldPointee->removeReference();
        }
    }
    
    return *this;
}
 
   
template <class T>
void RCPtr<T>::init()
{
    /* this only happens if made by the default constructor */
    if (0 == pointee) return;

#if SVOX_STRING_ADD_NON_CONST_ACCESSORS
    /* Fallback to a new instance through copy construction */
    if (false == pointee->isShareable())
    {
        pointee = new (SVOX_malloc( pointee->getSize() + sizeof(T) )) T(*pointee);
    }
#endif /* SVOX_STRING_ADD_NON_CONST_ACCESSORS */

    /* increase reference count */
    pointee->addReference();
}

}
}

#endif
