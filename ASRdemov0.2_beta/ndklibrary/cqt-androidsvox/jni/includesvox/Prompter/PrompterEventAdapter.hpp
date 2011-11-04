/*
 * @file   PrompterEventAdapter.hpp
 * @date   2009/10/14 10:49
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
/*! \brief PrompterEventAdapter definition.
 * 
 * @file   PrompterEventAdapter.hpp
 *
 * This file contains the definition of the prompter event adapter.
 */
#ifndef SVOX_PROMPTER_EVENT_ADAPTER_HPP
#define SVOX_PROMPTER_EVENT_ADAPTER_HPP

#include "PrompterTypes.hpp"

namespace svox 
{
namespace prompter 
{

/*! \brief Event Interface of the SVOX prompting module.
*
* This class defines the event interface of the SVOX prompting module.
* The user has to implement the event-methods it is interested in and 
* hand an instance of this self-implemented object to the currently used 
* prompter-instance.
*/
class PrompterEventAdapter
{
public:
   PrompterEventAdapter();
   virtual ~PrompterEventAdapter();

   /*! \brief overwrite to get the state when set.
   *
   * This method is empty and should be overwritten by the user of the 
   * interface if there is any interest in knowing when the internal state 
   * of the module is set.
   *
   * @param state [in] state currently set
   * @see SetEventAdapter
   * @return nothing
   */
   virtual void eventStateChanged(prompter::PState state);

};

}} /* namespaces */

#endif /* SVOX_PROMPTER_EVENT_ADAPTER_HPP */
