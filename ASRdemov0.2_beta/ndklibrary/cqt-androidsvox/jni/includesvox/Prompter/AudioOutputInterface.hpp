/*
 * @file   AudioOutputInterface.hpp
 * @date   2010/05/03 10:49
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
/*! \brief AudioOutputInterface definition.
 * 
 * @file   AudioOutputInterface.hpp
 *
 * This file contains the definition of the prompter audio interface.
 */
#ifndef SVOX_PROMPTER_AUDIO_OUTPUT_INTERFACE_HPP
#define SVOX_PROMPTER_AUDIO_OUTPUT_INTERFACE_HPP

/* pal */
#include "SVOX_Types.h"

namespace svox 
{
namespace prompter 
{

/*! \brief Audio Interface of the SVOX prompting module.
*
* This class defines the audio interface of the SVOX prompting module.
* The user has to implement the methods to get and handle the generated audio data and 
* hand an instance of this self-implemented object to the currently used 
* prompter-instance.
*/
class AudioOutputInterface
{
public:
    enum ResultCode
    {
        AUDIO_OK,
        AUDIO_ERROR
    };

    virtual ~AudioOutputInterface();

    /*! \brief overwrite to get the marker when set.
   *
   * This method is empty and should be overwritten by the user of the interface
   * if there is any interest in knowing when a position marker was found.
   *
   * It will only be activated if the audio-data is processed externally.
   *
   * @param marker [in] the id of the marker
   * @see SetEventAdapter
   * @return nothing
   */
   virtual void ttsPosMarker(const SVOX_Char *marker);

   /*! \brief overwrite to get the viseme id's for the playing tts stream.
    *
    * currently this method is not connected internally and therefore not called
    *
    * This method is empty and should be overwritten by the user of the interface
    * if there is any interest in knowing which viseme id's belong to the 
    * current tts stream.
    *
    * It will only be activated if the audio-data is processed externally.
    *
    * The list of viseme id's used here is based on MS SAPI5 and the
    * original Disney visemes. The following table lists the possible viseme
    * id values together with phonetic examples for UK English (SAMPA notation,
    * cf. SVOX Developer Manual).
    *
    *  <table border=0>
    *  <tr>
    *     <td><b> VisemeId </b></td>
    *     <td><b> example </b></td>
    *  </tr>
    *  <tr><td>0</td>  <td>silence     </td></tr>
    *  <tr><td>1</td>  <td>{ @ V       </td></tr>
    *  <tr><td>2</td>  <td>A           </td></tr>
    *  <tr><td>3</td>  <td>O:          </td></tr>
    *  <tr><td>4</td>  <td>e eI e@ U U@@ </td></tr>
    *  <tr><td>5</td>  <td>3:          </td></tr>
    *  <tr><td>6</td>  <td>j i: I I@   </td></tr>
    *  <tr><td>7</td>  <td>w u:        </td></tr>
    *  <tr><td>8</td>  <td>@@U         </td></tr>
    *  <tr><td>9</td>  <td>aU          </td></tr>
    *  <tr><td>10</td> <td>OI          </td></tr>
    *  <tr><td>11</td> <td>aI          </td></tr>
    *  <tr><td>12</td> <td>h           </td></tr>
    *  <tr><td>13</td> <td>r           </td></tr>
    *  <tr><td>14</td> <td>l           </td></tr>
    *  <tr><td>15</td> <td>s z         </td></tr>
    *  <tr><td>16</td> <td>S Z tS dZ   </td></tr>
    *  <tr><td>17</td> <td>T D         </td></tr>
    *  <tr><td>18</td> <td>f v         </td></tr>
    *  <tr><td>19</td> <td>d t n       </td></tr>
    *  <tr><td>20</td> <td>k g N       </td></tr>
    *  <tr><td>21</td> <td>p b m       </td></tr>
    *  </table> 
    *
    * @param visemeId [in]: this value contains the current viseme id
    * @param duration [in]: the duration of the viseme in microseconds (10^-6)
    */
   virtual void ttsViseme(SVOX_32S visemeId, SVOX_32S duration);

    /** \brief hand the generated audio data to the user.
    *
    * This method will hand the generated audio data to the user of the module.
    * The user has to decide what to do with the data.
    * The memory is only valid until this method returns. 
    * Storing just the pointer and reading the data later will lead to a unpredictable contend 
    * of the memory block, since it will be overwritten right after returning of this method.
    *
    * @param audioData [in]: memory block containing the generated pcm data
    * @param sampleCount [in]: number of samples (signed 16Bit pcm data) stored in the audioData block
    * @return ResultCode
    */
    virtual ResultCode writeAudioData(const SVOX_16S *audioData, SVOX_32S sampleCount) = 0;

    /** \brief wait until all audio data is processed.
    *
    * This method will block until all handed audio data is processed.
    * It will return immediately if no audio data is currently handled.
    *
    * @see writeAudioData
    * @return ResultCode
    */
    virtual ResultCode waitUntilProcessed() = 0;
};

}} /* namespaces */


#endif /* header guard */
