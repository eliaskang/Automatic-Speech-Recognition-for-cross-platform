/*! @brief climax module definition.
 * 
 * This file contains the definition of the climax module.
 * It implements the fundamental interface to the svox::climax namespace, 
 * including the ComplexListManager.
 *
 * @file   ComplexListManager.hpp
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
#ifndef CLIMAX_COMPLEXLISTMANAGER_HPP
#define CLIMAX_COMPLEXLISTMANAGER_HPP


////////////////////////////////////////////////////////////////////////////////
////
//// INCLUDES
////

//// PAL types
#include "SVOX_Types.h"

//// svox::common
#include "Array.hpp"

//// svox::climax
#include "ClimaxBasic.hpp"
#include "Listener.hpp"
#include "G2PContext.hpp"

//// Classes used from svox::asr.
namespace svox { namespace common { class Configuration; } }


////////////////////////////////////////////////////////////////////////////////
////
//// DECLARATIONS
////

namespace svox { namespace climax {


    /** 
     ** @brief This class provides a manager for complex, connected, and dynamic lists used for ASR.
     **
     ** The ComplexListManager is a module for managing "complex lists". 
     ** 
     ** The general task of a ComplexListManager instance is to enable speech recognition on this "complex list".
     ** 
     ** A complex list is a table consisting of rows and columns. 
     ** The rows are the elements of the complex lists (referred to as "entries"). 
     ** The columns are used for specifying different aspects of an entry; 
     ** for instance, the genre of a media entry is represented in a user-defined "genre" column.     
     ** The "connection info" (which media item refers to which genre etc.) is contained in the entry.
     ** The possible values within one column are referred to as "tokens".
     **
     ** It is also possible to store information related to entries which is not intended for speech recognition; 
     ** for instance, an externally provided entry ID value can be stored in a separate, non-speakable, column.
     ** The advantage of doing this is that such information can be retrieved upon a recognition, or it can be used 
     ** to restrict the recognition to a certain subset of the whole list.
     **
     ** One ComplexListManager manages one list by providing functions to add and remove entries, 
     ** as well as to perform G2P (through VSVM), and functions for "browsing" the data.
     ** 
     ** Each ComplexListManager stores the "phonetics" (generated by VSVM) in one vocabulary instance.
     ** Under certain conditions, a ComplexListManager instance may use an "external" vocabulary instance, 
     ** i.e. a vocabulary that contains words it does not manage (it has not created and will not delete etc.)
     **
     **
     **/

    /*
     ** For each (conceivable) "use case" (e.g. OSD, ADB, MP3) one ComplexListManager instance is required.
     ** 
     ** @section tasks Tasks:
     ** - Managing "table" entries.
     ** - Generate and store phonetics.
     ** - Generate recognition knowledge bases.
     ** - Translate between graphemic values and ID values.
     **
     ** @section concepts Terminology:     
     ** - Complex/Connected list
     ** - Entry
     ** - Column
     ** - Filter
     ** - Filter state
     ** - Use case: GENERIC | MP3 | ADB | OSD
     **     
     **/

    /*
     ** @section issues Issues:
     ** - ? G2P configuration: to be read from a (static) configuration file, when the instance is created (opened).
     ** - The template grammar used for creating a KnowledgeBase is known from the configuration file.
     ** - ? The configuration file is language-dependent ?
     ** - ? New instance after language change ?
     ** - ? Treat MSID as connection info ? (yes, "non-speakable column", usable as a filter)
     ** - ? How to ensure efficient processing of multiple instances of ComplexListManager, each of which using the same G2P config (BMW simple lists case) ?
     ** - ? Is it necessary to specify a ::ColumnType value when configuring the columns ?
     ** - ? Is it necessary to specify a ::KnowledgeBaseType value when creating the knowledge base ?  
     **   - Alternative?: recognize from applied filters
     **   - Alternative?: recognize from knowledge base name
     ** - ? ::ColumnID == ::ColumnType ?
     ** - ? Saving only to be done when phonetics have been created ? (Reason: saving to .voc)
     ** - ? Is it OK to save into multiple files ? (Or, do we need some sort of "repository"?)
     ** - ? What happens with words deleted from the vocabulary ? Need to maintain a list of "open word IDs" ?
     ** - ? Support fuzzy matching / word spotting ?
     **
     ** @section use_cases Use cases:
     ** - OSD (city - street): 
     **   - No dynamic entries, no save(), no load().
     **   - Only column usable for filtering is &lt;city&gt;, only filtered grammar is &lt;street&gt;.
     ** - Simplistic MP3: one ComplexListManager for each list of genre, artist, album, title. Connection info: MSID(!).  External filtering. How to re-use G2P? 
     ** - MP3 with connection info (any 1 or 2 of {artist, album, title})
     ** - Simple ADB: Columns: "firstname", "lastname"; Note: need to generate "f-l l-f l"
     **
     ** @section configuration Configuration requirements:
     ** - An external configuration (file) needs to be loaded. 
     **   - Or, is it better to just provide configuration methods?
     ** - The configuration file needs to define the following aspects:
     **   - Which columns are used? 
     **   - Which columns are speakable (or, otherwise, just used for filtering)?
     **   - Which columns provide ID values?
     **
     ** @section invariants Invariants:
     ** - ...
     **
     **/

    class ComplexListManager
    {
        //// Hiding the assignment operator and copy constructor.
        void operator=(const ComplexListManager&);
        ComplexListManager(const ComplexListManager&);

    protected: 
        /** @name Constructors, Destructors
         ** @{
         **/
        /** Initialize the ComplexListManager. 
         **/

        ComplexListManager();

    public:
        /** De-Initialize the ComplexListManager. **/
        virtual ~ComplexListManager();

        /**
         **  Creates a new ComplexListManager instance.
         **
         **  @return ComplexListManager instance.
         **/
        static ComplexListManager *New();


        /** @} **/


        /** @name Configuration methods.
         ** These methods have to be called after initialization.
         **
         ** The end of the configuration phase is signaled using setConfigurationDone(). 
         ** Afterwards, an attempt to call one of these methods results in an ERROR_INVALID_STATE result.
         ** @{
         **/

        /** @brief  Add a connected vocabulary ("column") specification to this instance. 
         **
         ** The columns added using this function can be referred to
         ** using ColumnID values 0, 1, ... and so on.
         **
         ** @param  columnType [in]   Specifies the type of column to be added.
         **
         ** @return ::OK in case of success.
         **
         **/
        virtual ResultCode addColumn( ColumnType columnType = COL_GENERIC ) = 0;

        /* @param  hasIDs [in]       Specifies whether the column has IDs or not. 
         **                          (or alternatively, just contain application-specific data.) */
        virtual ResultCode setColumnHasIDs( ColumnID c, bool hasIDs ) = 0;

        /* @param  isSpeakable [in]  Specifies whether the contents are to be phonetized  */
        virtual ResultCode setColumnSpeakable( ColumnID c, bool isSpeakable ) = 0;

        /* @param  dataCategory [in] Specifies the G2P category to be used for phonetization. */
        virtual ResultCode setColumnDataCategory( ColumnID c, SVOX_32S dataCategory ) = 0;


        /** @brief  Set option to store spellings in vocabulary.
         ** @param  doStore [in] Indicates whether or not spellings are to be stored.  Default is false.
         ** @return ::OK if successful.
         **/
        virtual ResultCode setStoreSpellings(bool doStore) = 0;


        /** @brief  Set option to store spellings in the word's alias entry in the vocabulary.
         ** @param  doStore [in] Indicates whether or not spellings are to be stored.  Default is false.
         ** @return ::OK if successful.
         **/
        virtual ResultCode setStoreSpellingInAlias(bool doStore) = 0;


        /** @brief  Finishes the configuration of this instance. 
         ** @return ::OK in case of success.
         **/
        virtual ResultCode setConfigurationDone() = 0;

        /** @} **/


        /** @name Getting information.
         ** These methods can be used for obtaining information about a ComplexListManager instance.
         ** @{
         **/

        /** @brief  Retrieve the count of columns in this instance. 
         ** @param  columnCount [out] Reference to the output column count.
         ** @return ::OK in case of success.
         **/
        virtual ResultCode getColumnCount(SVOX_32S& columnCount) const = 0;

        /** @brief  Retrieve the count of columns in this instance. 
         ** @return The number of columns or a negative value in case of an error.
         **/
        virtual SVOX_32S getColumnCount() const = 0;

        /** @brief  Retrieve the count of tokens in a specific columns. 
         ** @param  c [in] Identifies the column for which the count is to be obtained.
         ** @param  countOfTokens [out] The number of tokens in this column.
         ** @return ::OK in case of success.
         **/
        virtual ResultCode getColumnDataCount( ColumnID c, 
                                               SVOX_32S& countOfTokens ) const = 0;

        /** @brief  Retrieve the token ID for a specific spelling in a given column.
         **
         ** @param  c         [in]  Identifies the column for which the count is to be obtained.
         ** @param  spelling  [in]  The spelling to be looked up.
         ** @param  found     [out] Indicates whether the spelling was found.
         ** @param  tokenID   [out] The returned token ID (only guaranteed to be valid if found == true).
         **
         ** @return ::OK in case of success.
         **/
        virtual ResultCode getTokenIDForSpelling( 
            const ColumnID c, 
            const svox::common::String& spelling,
            bool& found,
            TokenID& tokenID ) = 0;


        /** @} **/


        /** @name Incremental updates.
         ** These methods can be used to add or delete data from the ComplexListManager instance.
         ** @{
         **/

        typedef enum { UPDATE_INCREMENTAL, UPDATE_REPLACING } UpdateType;

        /** @brief  Beginning an update. 
         **/
        virtual ResultCode beginUpdate(UpdateType ut = UPDATE_INCREMENTAL) = 0;

        /** @brief  Finishing an update. 
         **/
        virtual ResultCode endUpdate() = 0;

        /** @brief  Adding a spelling with ID to a column. 
         **
         ** @param  c        [in] Identifies the column.
         ** @param  tokenID  [in] The TokenID for this spelling.
         ** @param  spelling [in] The spelling.
         ** @param  li       [in] Specifies a listener for this operation.
         ** @return ::OK in case of success.
         **/
        virtual ResultCode addColumnSpelling( ColumnID c, 
                                              TokenID tokenID, 
                                              const svox::common::String& spelling, 
                                              Listener *li = NULL ) = 0;

        /** @brief  Retrieve the spelling of a given token in a column.
         **
         ** @param  c        [in]  Identifies the column.
         ** @param  tokenID  [in]  The TokenID for which the spelling is to be retrieved.
         ** @param  spelling [out] The retrieved spelling.
         **
         ** @return ::OK in case of success.
         **/
        virtual ResultCode getTokenSpelling( ColumnID c, 
                                             TokenID tokenID,
                                             svox::common::String& spelling ) = 0;

        /** @brief  Obtain a new entry for this list manager instance. 
         **
         ** Post condition: The list manager is in "current entry mode".
         **
         ** @return The entry instance.
         **/
        virtual ResultCode beginEntry() = 0; 

        /** 
         ** @brief Set the column value of this entry using a spelling.
         **
         ** @param c        [in]  The column whose value is to be set.
         ** @param spelling [in]  The spelling to use.
         ** @return               Error code indicating ::OK or the reason for failure.
         **/
        virtual ResultCode setColumnValueBySpelling( ColumnID c,
                                                     const svox::common::String& spelling ) = 0;

        /** 
         ** @brief Set the column value of this entry using an ID value.
         **
         ** Pre condition: The list manager is in "current entry mode".
         **
         ** @param c   [in]  The column whose value is to be set.
         ** @param tid [in]  The ID value of the token to use.
         **
         ** @return          Error code indicating ::OK or the reason for failure.
         **/
        virtual ResultCode setColumnValueByID(ColumnID c, TokenID tid) = 0;

        /** @brief  Dispose an entry obtained from this list manager instance (via createEntry()). 
         **
         ** Pre condition: The list manager is in "current entry mode".
         **
         ** @return ::OK in case of success.
         **/
        virtual ResultCode cancelEntry() = 0;

        /** @brief  Adding the current entry. 
         **
         ** If this function succeeds, the current entry will be added
         ** and a new entry has to be obtained for adding.  If the
         ** function fails, the state regarding the current entry is
         ** not changed.
         **
         ** Pre condition:  The list manager is in "current entry mode".
         ** Post condition: The list manager is not in "current entry mode".
         **
         ** @return ::OK in case of success.
         ** 
         **/
        virtual ResultCode addEntry() = 0;
        virtual ResultCode addEntry(const svox::common::String& spelling, Listener *li = NULL) = 0;

        /** @brief  Deleting entries matching the current filter state. 
         ** @return ::OK in case of success.
         **/
        virtual ResultCode deleteEntries() = 0;

        /** @brief  Deleting unreferenced tokens. 
         ** @return ::OK in case of success.
         **/
        virtual ResultCode deleteUnreferencedColumnData() = 0;
        /** @} **/


        /** @name Filtering entries.
         **
         ** The idea of filtering is to restrict the list of entries to a sub-list. 
         ** Filters are managed in a filter stack, each level further restricting the list.
         **
         ** Filters are specified by a constraint of the following form: column value  element-of  {tokenID1, tokenID2, ...}. 
         ** @{
         **/

        virtual ResultCode pushFilter(ColumnID c, const TokenIDList& tokenIDs) = 0;

        /** @brief  Removing the last filter. 
         ** @return ::OK if successful.
         **/
        virtual ResultCode popFilter() = 0;

        /** @brief  Removing all filters. 
         ** @return ::OK if successful.
         **/
        virtual ResultCode popAllFilters() = 0;
        /** @} **/


        /** @name Querying.
         **
         ** These methods can be used to execute queries on the data that is managed by the ComplexListManager.
         **
         ** For instance, related entries for a given recognition result can be obtained.
         **
         ** @{
         **/

        /** Retrieve filtered entries, e.g. for pick-list presentation. **/
        virtual ResultCode query(Listener*) = 0;

        /** @brief  Retrieve distinct column values for filtered entries. 
         **
         ** In contrast to an SQL query, "distinct" refers to each
         ** column, not to the column tuples, i.e. for each column no
         ** token is reported twice.
         **
         ** @param cs [in] The list of column IDs to be used.  If this
         **                list is empty, all columns are used.
         ** @param li [in] Specifies a listener for this operation.
         ** @return ::OK if successful.
         **/
        virtual ResultCode selectDistinctColumns(const ColumnIDList& cs, Listener *li) = 0;

        /** @brief  Retrieve all distinct column values.
         ** @param  li [in] Specifies a listener for this operation.
         ** @return ::OK if successful.
         **/
        virtual ResultCode selectDistinctColumns(Listener *li) = 0;

        /** @brief  Retrieve the number of entries in the current filter state. 
         ** @param  count [out] The output number of entries.
         ** @return ::OK if successful.
         **/
        virtual ResultCode getCount(SVOX_32S& count) const = 0;
        /** @} **/



        /** @name Performing G2P.
         **
         ** These methods are used for instructing the ComplexListManger to generate phonetics
         ** or to discard the phonetics it has already generated.
         **
         ** The generation of phonetics is incremental, 
         ** i.e. phonetization is only performed on entries added 
         ** since the last phonetization.
         **
         ** Either a G2PConfiguration or a G2PContext has to be provided.
         ** @{
         **/

        /** @brief  Performs pending G2P tasks (new entries). 
         **
         ** Obtains G2P configuration from the configuration file.
         **
         ** @param ctx [in] The G2P context to be used for this phonetization process.
         ** @param li [in] The listener to be used for this operation.
         ** @return ::OK if successful.
         **/
        virtual ResultCode performG2P(G2PContext *ctx, Listener *li = NULL) = 0;

        /** @brief  This function will create a temporary G2PContext from the G2PConfiguration 
         ** and use it for this G2P process only. 
         ** @param  locale [in] Language for G2P (e.g. de-DE).
         ** @param  cfg [in] The G2P configuration to be used for this phonetization process.
         ** @param  li [in] The listener to be used for this operation.
         ** @return ::OK if successful.
         **/
        virtual ResultCode performG2P( const svox::common::String& locale, 
                                       const svox::common::Configuration *cfg, 
                                       Listener* li = NULL) = 0;

        /** @brief  Disposes all phonetics generated by performG2P(). 
         ** @param  li [in] The listener to be used for this operation.
         ** @return ::OK in case of success.
         **/
        virtual ResultCode unperformG2P(Listener *li = NULL) = 0;
        /** @} **/


        /** @brief  Get word IDs referring to distinct column tokens.
         ** @param  c  [in] Identifies the column from which the data is to be taken from.
         ** @param  li [in] The listener that will be called back to receive the wordIDs via Listener::onWordsFound().
         ** @return ::OK upon success.
         **/
        virtual ResultCode getDistinctWords( ColumnID c, Listener* li ) = 0;


        /** @name WordID-related functions.
         **
         ** These functions provide low-level data access operations, 
         ** for instance, for interpreting recognition results.
         **
         ** @{
         **/

        /** @brief  Retrieves the column ID that refers to the WordID as a phonetic variant. 
         ** @param  wid [in] The word ID to get the column ID for.
         ** @param  c [out] The output column ID.
         ** @return ::OK if successful.
         **/
        virtual ResultCode getColumnIDForWord(WordID wid, ColumnID& c) const = 0;

        /** @brief  Retrieves the column ID that refers to the WordID as a phonetic variant. 
         ** @param  wid [in] The word ID to get the column ID for.
         ** @param  c [out] The output column ID.
         ** @param  t [out] The output token ID.
         ** @return ::OK               if successful.
         **         ::ERROR_NOT_FOUND  if the word ID was not found.
         **/
        virtual ResultCode getWordInfo(WordID wid, ColumnID& c, TokenID& t) const = 0;

        /** @brief  Get the first word ID belonging to the specified token (ID).
         ** 
         ** @param  c   [in]  Identifies the column to which the token belongs.
         ** @param  tid [in]  Identifies the token within the column c.
         ** @param wid [out]  Reference to the output word ID, or
         **                   ::CLIMAX_NO_WORD_ID if no word ID matches.
         ** @return ::OK in case of success.
         **/        
        virtual ResultCode getFirstWordIDForToken(ColumnID c, TokenID tid, WordID& wid) = 0;

        /** @brief Get the next word ID belonging to the specified token.
         ** 
         ** If WordID==::CLIMAX_NO_WORD_ID, then "representative" is returned.
         **
         ** If no more WordIDs are available, then ::CLIMAX_NO_WORD_ID is returned in wid.
         **
         ** @param c [in] Identifies the column to which the token belongs.
         ** @param tid [in] Identifies the token within the column c.
         ** @param wid [inout] Reference to the output word ID.
         ** @return ::OK in case of success.
         **/
        virtual ResultCode getNextWordIDForToken(ColumnID c, TokenID tid, WordID& wid) = 0;
        /** @} **/



        /** @name Managing persistence.
         ** These methods are used for sync'ing with external storage.
         ** 
         **/
         /* [Note: Currently only the storage of the vocabulary is implemented, i.e. no connection info, no loading.]
         ** [Note: May need to write multiple files]
         ** @{
         **/

        /** @brief  Sync'ing entries from persistent storage. 
         ** 
         ** The content of this instance is replaced with the one loaded from the persistent storage.
         ** The persistent storage is organized as described in save().
         **
         ** If this method fails, it is recommended to discard this list manager instance and allocate a new one.
         ** 
         ** No filters may be set.
         **
         ** @param  filepath [in] The base path for saving.
         ** @return ::OK       in case of success.
         ** @return ::ERROR_IO in case of an I/O operation failure.
         ** @see save()
         **/
        virtual ResultCode load(FilePath filepath = "climax-saved") = 0;

        /** @brief  Sync'ing entries to persistent storage. 
         **
         ** Two files will be written with extensions '.clx' and
         ** '.voc' appended to the base filepath, respectively.  The
         ** first one will contain the connection information.  The
         ** second one is a VSR vocabulary that contains the
         ** associated phonetics.
         **
         ** The content of this list manager instance is unchanged,
         ** even in case of failure.
         **
         ** No filters may be set.
         **
         ** @param  filepath [in] The base path for saving.
         ** @return ::OK       in case of success.
         ** @return ::ERROR_IO in case of an I/O operation failure.
         ** @see load()
         **/
        virtual ResultCode save(FilePath filepath = "climax-saved") const = 0;

        /** @} **/


        /*  @name Miscellaneous functions.
         ** These functions provide data access that may be useful
         ** for optimizing the behavior of the speech application.
         ** @{
         **/

        /*  @} **/

    };




} } // namespaces

#endif // CLIMAX_COMPLEXLISTMANAGER_HPP