/*! \brief Definition of BaseObject class and new/delete macros
 * 
 * This file contains the definition of the BaseObject class which should be used as base class
 * for all memory-allocating classes. It also provides a set of new/delete macros which should
 * be used instead of the C++ new/delete statements.
 *
 * @file   BaseObject.hpp
 * @date   2010/02/25
 * @author joachim.grill
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

#ifndef BASEOBJECT_HPP
#define BASEOBJECT_HPP

#include "SVOX_Types.h"
#include "SVOX_Memory.h"


namespace svox {
namespace common {


/*** BaseObject definition ********************************************************************************************/

/**
 *  This class can be used as base class for classes, which allocate dynamic memory. It manages a bool flag which
 *  indicates if a former memory allocation failed.
 */
class BaseObject
{
public:
    /** @brief Returns false, if the class is no longer valid. This happens, if a fatal error (e.g. out-of-memory)
        has occured and the class should no longer be used. All methods of a class shall be implemented in way that
        they do not modifiy an object if it is marked as invalid */
    bool isValid() const { return mValid; }

protected:
    /** @brief Initializes the class as "valid" */
    BaseObject() : mValid(true) {}

    /** @brief Marks the class as "invalid". This should be done, if a fatal error (e.g. out-of-memory) has occured and
        the class can no longer be used. */
    void setInvalid() { mValid = false; }

    /** @brief Resets the internal "valid" flag. Use this method to recover from a fatal error. */
    void setValid() { mValid = true; }

private:
    bool mValid;
};


} }

#endif // BASEOBJECT_HPP
