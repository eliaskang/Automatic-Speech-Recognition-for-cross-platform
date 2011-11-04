#ifndef __RCObjectDestruction__
#define __RCObjectDestruction__

#include "SVOX_Types.h"

/* we need placement new */
#include <new>

namespace svox
{
namespace common
{

/**
 * @brief Factory for RCObjects.
 *
 * This class collects template functions that we use to
 * destruct RCObject's with. By using template factory/destruction 
 * functions, we can avoid RCObject to
 * be a template, which could result in code bloat and would render it
 * more difficult to hide away most of its implementation.
 */
class RCObjectDestruction
{

public:



/**
 * This is placed here, to avoid circular header dependency, for docs
 * see also RCObjectFactory
 */
/**
 * @brief helper that does call destructors for the payload of an RCObject.
 * 
 * Used by RCObject to call the destructors for as much objects of
 * type T that would fit in payload of size (size is in bytes). By
 * making this outside of RCObject, we can avoid RCObject to be a
 * template, which could result in code bloat and would render it more
 * difficult to hide away most of its implementation.  The actual
 * template code is in the file RCObject.hpp, to avoid circular
 * dependencies.
 * 
 * @param start [in] start of the area where the destruction of T objects should take place
 * @param number [in] size of the area.
 */
 
template <class T>
static void destruct(T *start, SVOX_size_t number)
{
    SVOX_size_t cnt=0; 
    
    for(;
        cnt<number; 
        cnt++, start++)
    {
        reinterpret_cast<T*>(start)->~T();
    }
        
}




};


}

}

#endif
