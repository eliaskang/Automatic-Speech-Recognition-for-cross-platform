/*
 * @file   RecognitionResult.hpp
 * @date   2010/03/10 19:39
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
/*! \brief Recognizer result declarations.
 * 
 * @file   RecognitionResult.hpp
 *
 * This file contains the declaration of the class RecogResult, which
 * is used to store and transport recognition results.
 */
#ifndef SVOX_ASR_RECOGRESULT_HPP
#define SVOX_ASR_RECOGRESULT_HPP


/*** Includes + Macros ************************************************************************************************/

#include "SVOX_Types.h"
#include "String.hpp"
#include "Array.hpp"


/*** Class declaration ************************************************************************************************/

namespace svox
{

namespace climax { class ComplexListManager; }

namespace asr
{

class Grammar;
class RecognizerImpl;


/** \brief Class to hold a single word within a recognition result
 * utterance.
 *
 * This class holds a single word within a recognition utterance in
 * svox::asr::RecognizedUtterance class. An instance of this type can be
 * retrieved by svox::asr::RecognizedUtterance::getWord(...).
 *
 * A recognized word must not necessarily be a single word in the
 * linguistic sense.
 */
class RecognizedWord
{
public:
    const SVOX_Char *getSpelling() const; 
    const SVOX_Char *getTag() const; 

    const SVOX_Char *getClassname() const; 
    const SVOX_Char *getPhonetics() const;
    SVOX_32S getLogicalId() const; 

    SVOX_32S getConfidence() const; 
    SVOX_32S getAcousticalScore() const; 
    SVOX_32S getStartTime() const; 
    SVOX_32S getDuration() const; 

    /** @brief This function returns the list manager-related information of this word, if any.
     *
     * The list manager handle returned (if non-NULL) is the list
     * manager that was registered with the text enrollment rule name
     * that this word result belongs to.
     *
     * The column and tokenID provide the necessary information to
     * filter the list manager's entries, e.g. if spelling "Michael
     * Jackson" was recognized (belonging to the "artist" column in a
     * media entries list manager), the tokenID can be used to
     * restrict the list manager's filter to all entries by Michael
     * Jackson.
     * 
     * @param clm     [out]  The list manager handle, or NULL if none registered.
     * @param column  [out]  The column within the list manager this word belongs to.
     * @param tokenID [out]  The token ID this word has in the respective list manager column.
     *
     * @return  true  if and only if a list manager instance != NULL has been found.
     */
    bool getListManagerInfo( svox::climax::ComplexListManager *& clm, 
                             SVOX_32S& column,
                             SVOX_32S& tokenID ) const;

private:
    svox::common::String mSpelling;        // contains the spelling of the word (if available)
    svox::common::String mTag;             // normal grammar: contains <alias>;<classname>
                                           // OSD grammar: contains hex encoded database id
    svox::common::String mClassname;
    svox::common::String mPhonetics;

    SVOX_32S   mPhraseId;        // logical ID (24 bit)
    SVOX_32S   mConfidence;      // confidence measure for word
    SVOX_32S   mAcousticalScore;
    SVOX_32S   mStartTime;       // in ms
    SVOX_32S   mDuration;        // in ms

    //// Listmanager-related members
    svox::climax::ComplexListManager *mClm;
    SVOX_32S   mColumn;
    SVOX_32S   mTokenID;

    friend class svox::asr::RecognizerImpl;
};

/** \brief Class to hold a single utterance (hypothesis) of a
 * recognition result
 *
 * This class holds a single utterance of a recognition result in
 * svox::asr::RecognitionResult class. An instance of this type can be
 * retrieved by svox::asr::RecognitionResult::getUtterance(...).
 *
 * A recognized utterance consists of zero or more words stored in
 * instances of svox::asr::RecognizedWord.
 */
class RecognizedUtterance
{
public:
    SVOX_32S getNumWords() const;
    const RecognizedWord *getWord(SVOX_32S i) const;
    SVOX_32S getConfidence() const;
    Grammar *getGrammar() const; 

    RecognizedUtterance(SVOX_32S capacity = -1);

private:
    svox::common::Array<RecognizedWord> mWords;      // words of the utterance
    SVOX_32S mConfidence;                            // confidence for utterance
    Grammar *mGrammar;

    friend class svox::asr::RecognizerImpl;
};


/** \brief Class to hold a recognition result
 *
 * This class holds a set of recognition results for the
 * svox::asr::Recognizer class. An instance of this type can be
 * retrieved by svox::asr::Recognizer::getRecognitionResults().
 *
 * A recognition result consists of zero or more utterances stored in
 * instances of svox::asr::RecognizedUtterance.
 */
class RecognitionResult
{
public:
    SVOX_32S getNumUtterances() const;
    const RecognizedUtterance *getUtterance(SVOX_32S i) const;

    RecognitionResult(SVOX_32S capacity = -1);

private:
    svox::common::Array<RecognizedUtterance> mUtterances;

    friend class svox::asr::RecognizerImpl;
};


}} // namespaces

#endif // SVOX_ASR_RECOGRESULT_HPP

/**********************************************************************************************************************/
