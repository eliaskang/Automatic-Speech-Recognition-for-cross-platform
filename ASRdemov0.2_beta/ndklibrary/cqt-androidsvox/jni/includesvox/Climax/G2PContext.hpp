/*! @brief climax G2Pcontext type definition.
 * 
 * This file contains the definition of the climax G2Pcontext type.
 *
 * @file   G2PContext.hpp
 * @date   2009/11/27 8:35
 * @author dirk.buehler
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
#ifndef CLIMAX_G2PCONTEXT_HPP
#define CLIMAX_G2PCONTEXT_HPP


////////////////////////////////////////////////////////////////////////////////
////
//// INCLUDES
////

//// svox::common
#include "String.hpp"

//// svox::climax
#include "ClimaxBasic.hpp"

//// Classes used from svox::asr.
namespace svox { namespace common { class Configuration; } }


////////////////////////////////////////////////////////////////////////////////
////
//// NAMESPACES
////
namespace svox { namespace climax {


////////////////////////////////////////////////////////////////////////////////
////
//// DECLARATIONS
////


    /** @brief This class controls a G2P process and maintains its
     ** internal data (resource handles).
     ** 
     ** A G2PContext instance represents the information needed for a
     ** G2P process taking place within a ComplexListManager instance.
     ** 
     ** Usually it is not necessary to create instances of this type
     ** explicitly.  However, a G2PContext instance may be re-used
     ** among different invocations of
     ** ComplexListManager::performG2P() in order to retain internal
     ** data, e.g. for caching.
     ** 
     **/
    class G2PContext 
    {
        //// Disallowed assignment operator and copy constructor. 
        void operator=(const G2PContext&);
        G2PContext(const G2PContext&);

    protected:
        /** @brief  Creates a G2P context. 
         **/
        G2PContext();

    public:

        /** @brief  Creates a new G2PContext instance. **/
        static G2PContext *New(const svox::common::String& locale, const svox::common::Configuration *cfg);

        /** @brief  Disposes a G2PContext. **/
        virtual ~G2PContext();

        /** @brief  Get the locale used for this G2PContext instance. **/
        virtual svox::common::String getLocale() const = 0;
    };






} }
////
//// NAMESPACES
////
////////////////////////////////////////////////////////////////////////////////


#endif
