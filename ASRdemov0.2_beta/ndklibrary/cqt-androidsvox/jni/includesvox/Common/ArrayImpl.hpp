#ifndef SVOX_COMMON_ARRAYIMPL_HPP
#define SVOX_COMMON_ARRAYIMPL_HPP

/*! \brief An Implicit Shared Array Class (template).
 * 
 *
 * @file   ArrayImpl.hpp
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

#include "SVOX_Types.h"

#include "RCPtr.hpp"
#include "RCObject.hpp"
#include "RCObjectFactory.hpp"
#include "BaseObject.hpp"


namespace svox {
namespace common {

   
/* Creates an empty array */
template <typename T>
Array<T>::Array()
    : value( sharedEmpty() )
{
}

template <typename T>
void Array<T>::setValid()
{
    BaseObject::setValid();
}



/* Returns the number of elements stored in the array */
template <typename T>
SVOX_32S Array<T>::count() const
{
    if (isValid())
    {
        return static_cast<SVOX_32S>(value->getCount());
    }
    else
    {
        return 0;
    }
}




/* Returns a pointer to the data stored in this Array */
template <typename T>
const T * Array<T>::data() const
{
    if (isValid())
    {
        return value->data();
    }
    else
    {
        return NULL;
    }
}


/* Returns true, if the array is empty */
template <typename T>
bool Array<T>::isEmpty() const
{
    if (isValid())
    {
        return (0 == value->getCount());
    }
    else
    {
        return false;
    }
}


/* Deletes the content of the array */
template <typename T>
void Array<T>::clear()
{
    value = sharedEmpty();
}


/* Returns the element at position index */
template <typename T>
const T &Array<T>::get(SVOX_32S index) const
{
    if ((index >= 0) && (index < count()) && isValid())
    {
        T *ptr = value->data();
        return ptr[index];
    }
    else
    {
        // return dummy element
        return dummy();
    }
}

template <typename T>
T &Array<T>::getWritable(SVOX_32S index, bool *valid)
{
    if ((index >= 0) && (index < count()) && isValid())
    {
        if (value->isShared())
        {
            // keep us alive ...
            Array<T> help(*this);
            SVOX_32S capacity = help.count();
            value = RCObjectFactory<T>::create(static_cast<const T *>(0),
                                            capacity * sizeof(T), 
                                            0, 
                                            // member count must be reduced, if we shrink the array 
                                            capacity);
            
            // we could fail to allocate
            if (check(valid,help.value))
            {
                T *dest = value->data();

                RCObjectFactory<T>::construct(&dest[0],
                                           help.data(), 
                                           capacity);
                // no need to call destructors because we did copy the
                // assignment of value to the new instance did call
                // all the destructors
            }
            else
            {
                // this is bad, but we save an extra check below
                // which safes runtime for the usual good case
                return dummy();
            }
        }
        // if allocation failed, we have bailed out via a return
        // above, so no need to check again
        T *ptr = value->data();
        return ptr[index];
    }
    else
    {
        // return dummy element
        return dummy();
    }
}


/* returns the index of rhs in this array. Search is performed within index range [from,to]. */
template <typename T>
SVOX_32S Array<T>::indexOf(const T &rhs, SVOX_32S from, SVOX_32S to) const
{
    if ((from >= 0) && (from < count()) && isValid())
    {
        T *ptr = value->data();
            
        if ((-1 == to) || (to > count()))
        {
            to = count();
        }
    
        while ((from < to) && (!(ptr[from] == rhs)))
        {
            ++from;
        }
        // not found
        if (!(from<to))
        {
            from = -1;
        }
        return from;
    }
    else
    {
        return -1;
    }
        
}


template <typename T>
bool Array<T>::check(bool *valid, RCPtr<RCObject<T> > &restoreValue)
{
    // check if any allocation failed
    // this cannot be written the otherway around, because
    // otherwise the compiler cannot find the right operator
    // without an evil cast
    if (value == 0)
    {
        // in case of an allocation failure, we restore the old array
        value = restoreValue;

        setInvalid();

        if (valid) // this refers to the pointer !!!
        {
            *valid = false;
        }        
        return false;
    }
    else
    {
        return true;
    }
}


/* The usual constructor */
template <typename T>
Array<T>::Array(SVOX_32S size, bool *valid)
       : value(sharedEmpty())
{
    if (0 != size)
    {
        // Just allocates the memory, element count is size
        value = RCObjectFactory<T>::create(static_cast<const T *>(0),
                                        size * sizeof(T), 
                                        0, 
                                        size);
    
        // we could fail to allocate
        if (check(valid,sharedEmpty()))
        {
            T *dest = value->data();
            RCObjectFactory<T>::construct(&dest[0],
                                       size);
        }
    }
}



/* Resizes the internal buffer */
template <typename T>
void Array<T>::resize(SVOX_32S capacity, bool *valid)
{
    if (isValid())
    {
        if (0 >= capacity)
        {
            value = sharedEmpty();
        }
        else
        {
            // no matter if shared or not, we need to reallocate iff the
            // size changes content remains unchanged iff reserved size
            // increases, but is copied.  content shrinks iff size
            // decreases, but as we always do a copy in this case, this is
            // not problem for the shared instances
            if (value->getSize() != static_cast<SVOX_size_t>(capacity * sizeof(T)))
            {
                // keep us alive ...
                Array<T> help(*this);
                SVOX_32S newCnt = capacity;
                if (capacity >= count())
                {
                    newCnt = count();
                }
                
                value = RCObjectFactory<T>::create(static_cast<const T *>(0),
                                                capacity * sizeof(T), 
                                                0, 
                                                // member count must be reduced, if we shrink the array 
                                                capacity);
                // we could fail to allocate
                if (check(valid, help.value))
                {
                    T *dest = value->data();
                    
                    if (newCnt > 0)
                    {
                        RCObjectFactory<T>::construct(&dest[0],
                                                   help.data(), 
                                                   newCnt);
                    }
                    
                    // no need to call destructors because we did copy the
                    // assignment of value to the new instance did call
                    // all the destructors

                    if (capacity > newCnt)
                    {
                        RCObjectFactory<T>::construct(&dest[newCnt],
                                                   capacity - newCnt);
                        
                    }
                }
            }
        }
    }
    else
    {
        if (valid)
        {
            *valid = false;
            
        }        
    }
}


/* Resizes the internal buffer */
template <typename T>
void Array<T>::reserve(SVOX_32S capacity, bool *valid)
{
    if (isValid())
    {
        // we will not shrink below count()
        if (capacity < count())
        {
            capacity = count();
        }
        
        // no matter if shared or not, we need to reallocate iff the
        // size changes content remains unchanged iff reserved size
        // increases, but is copied.  content shrinks iff size
        // decreases, but as we always do a copy in this case, this is
        // not problem for the shared instances
        if (value->getSize() != static_cast<SVOX_size_t>(capacity * sizeof(T)))
        {
            // keep us alive ...
            Array<T> help(*this);
            
            value = RCObjectFactory<T>::create(static_cast<const T *>(0),
                                            capacity * sizeof(T), 
                                            0, 
                                            // member is unchanged, because we have set capacity to count() if below
                                            help.count());
            
            // we could fail to allocate
            if (check(valid, help.value))
            {
                T *dest = value->data();

                RCObjectFactory<T>::construct(&dest[0],
                                           help.data(), 
                                           help.count());
                // no need to call destructors because we did copy the
                // assignment of value to the new instance did call
                // all the destructors
            }
        }
    }
    else
    {
        if (valid)
        {
            *valid = false;
            
        }        
    }
}


/* Appends the passed rhs to the end of the array */
template <typename T>
void Array<T>::append(const T &rhs, bool *valid)
{
    // this is just a special case of insertion
    insert(count(),rhs,valid);
}


/* Appends numElems passed elements to the end of the array */
template <typename T>
void Array<T>::appendAll(const SVOX_32S numElems, const T* elems, bool *valid)
{
    reserve(count() + numElems, valid);
    for (SVOX_32S i = 0; isValid() && i < numElems; ++i)
    {
        append(elems[i], valid);
    }
}


/* Removes the last element from the array and returns it */
template <typename T>
T Array<T>::popLast(bool *valid)
{
    // this is solved by retrieval and erasure
    if ((count()>0) && isValid())
    {
        T result = get(count()-1);
        erase(count()-1,1,valid);
        return result;
    }
    else
    {
        // return a dummy if we popped from an empty array
        return dummy();
    }
}


/* Inserts rhs at position index into the array. Index must be in the
 * range of [0,count()]. */
template <typename T>
void Array<T>::insert(SVOX_32S index, const T &rhs, bool *valid)
{
    if (isValid())
    {
        if ((0 <= index) && (index <= count()))
        {
            // keep us alive ...
            Array<T> help;
            SVOX_size_t capacity = value->getCount() + 1;
            T *dest = value->data();

            // reallocation happens if we are shared or if we do not have
            // enough space prereserved
            if ((value->isShared()) || (value->getSize() < capacity * sizeof(T)))
            {
                help = (*this);
                value = RCObjectFactory<T>::create(static_cast<const T *>(0),
                                                capacity * sizeof(T), 
                                                0, 
                                                // member count is also increased by 1
                                                capacity);
            
                // we could fail to allocate
                if (check(valid, help.value))
                {
                    // dest has changed
                    dest = value->data();

                    // we need to instantiate these via inplace copy constructor
                    if (index > 0)
                    {
                        RCObjectFactory<T>::construct(&dest[0], 
                                                   help.data(), 
                                                   index);
                    }

                    if ((index+1) < static_cast<SVOX_32S>(capacity))
                    {
                        RCObjectFactory<T>::construct(&dest[index + 1], 
                                                   &help.get(index), 
                                                   (capacity - index - 1));
                    }
                }
                else
                {
                    // this is bad, but we save an extra check below
                    // which safes runtime for the usual good case
                    return;
                }
            }
            else
            {
                // we change the existing array, so the first part may stay in place
                help = (*this);
                // this is the only thing we need to do to the allocation
                value->setCount(capacity);

                if ((index+1) < static_cast<SVOX_32S>(capacity))
                {
                    RCObjectFactory<T>::constructMove(&dest[index + 1], 
                                                   &help.get(index), 
                                                   (capacity - index - 1));
                }
            }
            // now add the new one if allocation failed, we have
            // bailed out via a return above, so no need to check
            // again
            RCObjectFactory<T>::construct(&dest[index], 
                                       &rhs, 
                                       1);
        }
    }
    else
    {
        if (valid)
        {
            *valid = false;
            
        }        
    }
}


/* Copies rhs to position index of the array */
template <typename T>
bool Array<T>::set(SVOX_32S index, const T &rhs, bool *valid)
{
    if (isValid())
    {
        if ((0 <= index) && (index < count()))
        {
            SVOX_size_t capacity = value->getCount();
            T *dest = value->data();

            if (value->isShared())
            {
                // keep us alive ...
                Array<T> help;

                help = (*this);
                value = RCObjectFactory<T>::create(static_cast<const T *>(0),
                                                capacity * sizeof(T), 
                                                0, 
                                                // member count is unchanged
                                                capacity);

                // we could fail to allocate
                if (check(valid, help.value))
                {
                    dest = value->data();

                    // we need to instantiate these via inplace copy constructor
                    if (index > 0)
                    {
                        RCObjectFactory<T>::construct(&dest[0], 
                                                   help.data(), 
                                                   index);
                    }
                    if ((index+1) < static_cast<SVOX_32S>(capacity))
                    {
                        RCObjectFactory<T>::construct(&dest[index + 1], 
                                                   &help.get(index + 1), 
                                                   (capacity - index - 1));
                    }
                }
                else
                {
                    // this is bad, but we save an extra check below
                    // which safes runtime for the usual good case
                    return false;
                }
            }
            else
            {
                // call destructor for element that is replaced, but only
                // if we did not copy
                (&dest[index])->~T();

            }
            // now we set the new value one if allocation failed,
            // we have bailed out via a return above, so no need
            // to check again
            RCObjectFactory<T>::construct(&dest[index],
                                       &rhs, 
                                       1);
        }
        else
        {
            return false;
        }
        
    }
    else
    {
        if (valid)
        {
            *valid = false;
            
        }        
    }
    
    return isValid();
    
 }


/* Deletes count element starting at position index */
template <typename T>
void Array<T>::erase(SVOX_32S index, SVOX_32S cnt, bool *valid)
{
    if (isValid())
    {
        if ((0 <= index) && (cnt > 0) && (index < count()))
        {
            // keep us alive ...
            Array<T> help;
            T *dest = value->data();

            // assure we do not run over the end
            if ((index + cnt) > count())
            {
                cnt = count() - index;
            }
        
            SVOX_size_t capacity = value->getCount() - cnt;
            if (value->isShared())
            {
                help = (*this);
                value = RCObjectFactory<T>::create(static_cast<const T *>(0),
                                                capacity * sizeof(T), 
                                                0, 
                                                // member count is reduced by 1
                                                capacity);

                // we could fail to allocate
                if (check(valid, help.value))
                {
                    dest = value->data();

                    // we need to instantiate these via inplace copy constructor
                    if (index > 0)
                    {
                        RCObjectFactory<T>::construct(&dest[0], 
                                                   help.data(), 
                                                   index);
                    }

                    if ((index + cnt) < static_cast<SVOX_32S>(help.count()))
                    {
                        RCObjectFactory<T>::construct(&dest[index], 
                                                   &help.get(index + cnt), 
                                                   (help.count() - index - cnt));
                    }
                }
            }
            else
            {
                help = (*this);

                // we did not reallocate, so the items to erase must die
            
                // call destructor on elements erased, but only if we did reallocate
                for (SVOX_32S pos = index; pos < index + cnt; pos++)
                {
                    (&dest[pos])->~T();
                }

                if ((index + cnt) < static_cast<SVOX_32S>(help.count()))
                {
                    RCObjectFactory<T>::constructMove(&dest[index], 
                                                   &help.get(index + cnt), 
                                                   (help.count() - index - cnt));
                }

                value->setCount(capacity);
            }
        }
    }
    else
    {
        if (valid)
        {
            *valid = false;
            
        }        
    }
}


/* Deletes all elements from the array which equal rhs */
template <typename T>
void Array<T>::eraseAll(const T& rhs, bool *valid)
{
    if (isValid())
    {
        SVOX_size_t capacity = value->getCount();

        for (SVOX_32S i = 0; i < count(); i++)
        {
            if (rhs == get(i))
            {
                capacity--;
            }
        }

        // take a shortcut if there is nothing to erase
        if (capacity != value->getCount())
        {
            // keep us alive ...
            Array<T> help;
            bool allocated = false;
        
            if (value->isShared())
            {
                help = (*this);
                value = RCObjectFactory<T>::create(static_cast<const T *>(0),
                                                capacity * sizeof(T), 
                                                0, 
                                                // member count as calculated
                                                capacity);
                allocated = true;
            }
            else
            {
                help = (*this);

                // call destructors for all matching elements, 
                // because we did not reallocate
                T *hdata = value->data();
                for (SVOX_32S i = 0; i < count(); i++)
                {
                    if (rhs == get(i))
                    {
                        (&hdata[i])->~T();
                    }
                }
            }
    

            // we could fail to allocate
            if (check(valid, help.value))        
            {
                SVOX_size_t index = 0;

                T *dest = value->data();

                // TODO improve by range copies
                for (SVOX_32S i = 0; i < help.count(); i++)
                {
                    if (!(rhs == help.get(i)))
                    {
                        if (allocated)
                        {
                            RCObjectFactory<T>::construct(&dest[index], 
                                                       &help.get(i),
                                                       1);
                        }
                        else
                        {
                            if ((&dest[index]) != (&help.get(i)))
                            {
                                RCObjectFactory<T>::constructMove(&dest[index], 
                                                               &help.get(i),
                                                               1);
                            }
                        }
                        index ++;
                    }
                }
                value->setCount(capacity);
            }
        }
    }
    else
    {
        if (valid)
        {
            *valid = false;
            
        }        
    }
}


// this must be present for every type so we must have it in the header
template <class T>
RCPtr<RCObject<T> > &Array<T>::sharedEmpty()
{
    /* avoid allocation on the heap, because we cannot handle
     * allocation failures */
    
    static RCObject<T> Empty(static_cast<const T *>(0),
                             0,
                             0,
                             0);

    /* avoid this to be ever deleted */
    Empty.addReference();
    
    static RCPtr<RCObject<T> > sharedEmptyPtr(&Empty);

    return sharedEmptyPtr;

}


// this must be present for every type so we must have it in the header
template <class T>
T &Array<T>::dummy()
{
    static T dummyT;
    return dummyT;
}



} } // namespaces


#endif // SVOX_COMMON_ARRAYIMPL_HPP
