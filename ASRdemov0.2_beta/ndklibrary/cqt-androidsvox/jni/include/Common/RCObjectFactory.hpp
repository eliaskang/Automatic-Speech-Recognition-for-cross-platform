#ifndef __RCObjectFactory__
#define __RCObjectFactory__

#include "SVOX_Types.h"
#include "RCObject.hpp"

/* we need placement new */
#include <new>

namespace svox
{
namespace common
{

/**
 * @brief Factory for RCObjects.
 *
 * This class collects the free template functions that we use to
 * create RCObject's with. By using template factory functions, we can avoid RCObject to
 * be a template, which could result in code bloat and would render it
 * more difficult to hide away most of its implementation.
 */
template <class T>
class RCObjectFactory
{
public:

    /**
     * @brief helper that does placement new.
     * 
     * This helper does placement new for as much objects of type T that
     * would fit in the space defined by size (size is in bytes). 
     * 
     * @param start [in] start of the area where the construction of T objects should take place
     * @param size [in] size of the area.
     */
    static void construct(T *start, SVOX_size_t size);
    
    /**
     * @brief helper that does placement new with the copy constructor (used by array).
     * 
     * This helper does a copy via placement news. It takes the relative
     * position into account, so overlapping copies are possible. 
     *
     * Note: This does not call destructors, so if you really use
     * overlapping areas you will not see what you wanted.
     * 
     * @param at [in] start of the area where the construction of T objects should take place
     * @param from [in] start of the area where the source T objects are located
     * @param count [in] number of T elements copy
     */
    static void construct(T * at, const T* from, SVOX_size_t count);

    /**
     * @brief helper that does copy construct and destruct to \a at and
     * from \a from count instances. The destuctors are called after every
     * individual copy beeing copied at the position \a at. This is done
     * element by element, so overlapping is handled right. (The relative
     * position is taken into account to use the right sequence in cae of
     * overlaps.)
     *
     * @param at [in] start of the area where the construction of T objects should take place
     * @param from [in] start of the area where the source T objects are located
     * @param count [in] number of T elements copy
     */
    static void constructMove(T * at, const T* from, SVOX_size_t count)
    {
        // to handle overlapping copies correct we need to check the
        // direction here
        if (at < from)
        {
        
            for (SVOX_size_t i = 0; i < count; i++)
            {
                new (&at[i]) T(from[i]);
                (&from[i])->~T();
            }
        }
        else
        {
            for (SVOX_32S i = count-1; i >= 0; i--)
            {
                new (&at[i]) T(from[i]);
                (&from[i])->~T();
            }
        }
    
    }



    /**
     * @brief Factory method to create an RCObject that holds T typed objects.
     * 
     * Factory method to create an RCObject that holds T typed
     * objects. This method allocates as much space as is necessary to
     * hold an RCObject and the requested payload in bytes. It
     * instantiates Objects of type T in the payload area.
     * 
     * @param initValue [in] start of the area which should be copied in the payload
     * @param size [in] size of the area.
     * @param copysize [in] number of elements to coyp fom \a initValue,
     * if less than \a size
     * @param count [in] number constructed elements in payload, this
     * initializes the count, but does not trigger construction
     * @return A new instance of a reference counted object with a trailing payload of size
     */
    static RCObject<T>* create(const T *initValue, SVOX_size_t size, SVOX_32S copysize = -1, SVOX_size_t count = 0)
    {
        RCObject<T> *rc = 0;
        rc = static_cast<RCObject<T>* >(SVOX_malloc( size + sizeof(RCObject<T>)));
        // we might got no memory
        if (0 != rc)
        {
            new (rc) RCObject<T>(initValue, size, copysize, count);
            construct(rc->data(), ((copysize == -1) ? size : copysize));
        }
    

        return rc;
    }

};

// These name the explicit instantiations we will make, this avoid implicit instantiation
template <>
void RCObjectFactory<SVOX_Char>::construct(SVOX_Char *start, SVOX_size_t size);

template <class T>
void RCObjectFactory<T>::construct(T *start, SVOX_size_t size)
{

    T *ptr = start;
    SVOX_size_t cnt = 0;
    
    for (; 
         cnt < size; 
         cnt++ )
    {
        new (&ptr[cnt]) T();
    }
}

template <class T>
void RCObjectFactory<T>::construct(T * at, const T* from, SVOX_size_t count)
{
    // to handle overlapping copies correct we need to check the
    // direction here
    if (at < from)
    {
        
        for (SVOX_size_t i = 0; i < count; i++)
        {
            new (&at[i]) T(from[i]);
        }
    }
    else
    {
        for (SVOX_32S i = count-1; i >= 0; i--)
        {
            new (&at[i]) T(from[i]);
        }
    }
    
}

}
}


#endif
