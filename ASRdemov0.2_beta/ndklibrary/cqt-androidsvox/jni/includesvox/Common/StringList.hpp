#ifndef SVOX_COMMON_STRING_LIST_HPP
#define SVOX_COMMON_STRING_LIST_HPP
/*
* @file   StringList.hpp
* @date   2010/09/01 
* @author andreas.kirbach
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
/*! \brief A String List based on an Implicit Shared String class.
* 
* @file   StringList.hpp
*
* This file contains the definition of the string list based on the svox::common::String container class. 
*/

/* pal */
#include "SVOX_Types.h"

/* common */
#include "String.hpp"
#include "Array.hpp"


namespace svox
{
    namespace common
    {
        class String;
        class StringList: public Array<String>
        {
            
        };
    } /* namespace common */
} /* namespace svox */

#endif // SVOX_COMMON_STRING_LIST_HPP
