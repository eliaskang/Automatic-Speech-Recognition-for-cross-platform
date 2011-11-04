/*! @brief climax listener type definition.
 * 
 * This file contains the definition of the climax listener type.
 *
 * @file   Listener.hpp
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
#ifndef CLIMAX_LISTENER_HPP
#define CLIMAX_LISTENER_HPP


////////////////////////////////////////////////////////////////////////////////
////
//// INCLUDES
////

#include "ClimaxBasic.hpp"

// svox::common
#include "String.hpp"

////////////////////////////////////////////////////////////////////////////////
////
//// NAMESPACES
////
namespace svox { namespace climax {


////////////////////////////////////////////////////////////////////////////////
////
//// DECLARATIONS
////


    /**
     ** @brief Listener interface for ComplexListManager.
     **
     ** Modification of list is not allowed within Listener call-backs.
     **
     **/
    class Listener
    {
        void operator=(const Listener&);
        Listener(const Listener&);

    public:
        Listener() {}

        virtual ~Listener();

        /** @name Configuration methods
         ** @{ **/

        /** @brief Indicates the number of entries that should be supplied to
         ** the onEntriesFound() call.  Default value is 1. 
         ** @param newEntryCount [in] The preferred number of entries.
         ** @return ::OK in case of success.
         **/
        ResultCode setEntryBlockSize(SVOX_32S newEntryCount)
        {
            mEntryCount = newEntryCount;
            return OK;
        }

        /** @brief Returns the number of entries that should be supplied to
         ** the onEntriesFound() call. 
         ** @param entryCount [out] The preferred number of entries.
         ** @return ::OK in case of success.
         **/
        ResultCode getEntryBlockSize(SVOX_32S& entryCount)
        {
            entryCount = mEntryCount;
            return OK;
        }
        /** @} **/


        /** @name "Call-back" methods
         ** @{ **/

        /** @brief Called when an entry is retrieved in a query.
         **
         ** The entry supplied as argument to this call by
         ** the ComplexListManager satisfy the its current filter
         ** state.
         **
         ** @param tokenIDs   [in]  The array containing the token ID values for each column of this entry.
         ** @param stop       [out] Signals the enclosing query to stop gracefully.
         ** 
         ** @return Returning a value other than ::OK signals the enclosing query to stop.
         **/
        virtual ResultCode onEntryFound(const svox::common::Array<TokenID>& tokenIDs, bool& stop);

        /** INTERNAL. **/
        virtual ResultCode onEntriesFoundInternal(SVOX_32S entryCount, void* es[]);

        /** @brief Called when a selectDistinctColumns() query returns individual values. 
         ** @param c [in] The column ID.
         ** @param tokens [in] The array of token IDs.
         ** @return Returning a value other than ::OK will abort the enclosing operation.
         **/
        virtual ResultCode onTokensFound(ColumnID c, const TokenIDList& tokens);

        /** @brief Called when a getDistinctWords() query returns individual values. 
         ** @param  c           [in] The column ID.
         ** @param  tid         [in] The token ID.
         ** @param  wordIDs     [in] The array of wordID values.
         ** @return Returning a value other than ::OK will abort the enclosing operation.
         **/
        virtual ResultCode onWordsFound( ColumnID c, 
                                         TokenID tid, 
                                         const WordIDList& wordIDs );

        /** @brief Called on progress, may indicate to abort operation.
         ** @param info [in] A progress indication.
         ** @return Returning TRUE signals the enclosing operation to abort (gracefully).
         **/
        virtual bool onProgress(ProgressInfo info);

        /** @brief Called when an entry could not be phonetized. 
         ** @param rcvsr [in]  The result obtained from the internal G2P component.
         ** @param c [in]      The column the failure occurred in.
         ** @param t [in]      The token that was to be phonetized.
         ** @param s [in]      The spelling used for the token.
         ** @return Returning a value other than ::OK signals the enclosing performG2P() operation to stop.
         **/
        virtual ResultCode onG2PFailed(SVOX_32S rcvsr, ColumnID c, TokenID t, const svox::common::String& s);

        /** @brief Called when a query or operation has finished. 
         ** @return ::OK in case of success.
         **/
        virtual ResultCode onDone();

        /** @} **/

    protected:
        SVOX_32S mEntryCount;
    };







} }
////
//// NAMESPACES
////
////////////////////////////////////////////////////////////////////////////////


#endif
