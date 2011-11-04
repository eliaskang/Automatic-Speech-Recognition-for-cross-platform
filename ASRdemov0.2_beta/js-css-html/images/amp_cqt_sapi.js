
function CQT_SAPI()
{
 this.JS_SAPI_Init = function(strResoucePath,strConfigName)
 {
   return CQTSapi.JS_SAPI_Init(strResoucePath,strConfigName);
 }
 
 this.JS_SAPI_ASRClear = function(strASRVar)
 {
   return CQTSapi.JS_SAPI_ASRClear(strASRVar);
 }
 
 this.JS_SAPI_SetASRRuleValue = function(strASRVar,strValue)
 {
   return CQTSapi.JS_SAPI_SetASRRuleValue(strASRVar,strValue);
 }
 
 this.JS_SAPI_TTSClear = function(strTTSVar)
 {
   return CQTSapi.JS_SAPI_TTSClear(strTTSVar);
 }
 
 this.JS_SAPI_SetTTSRuleValue = function(strTTSVar,strValue)
 {
   return CQTSapi.JS_SAPI_SetTTSRuleValue(strTTSVar,strValue);
 }
 
 this.JS_SAPI_Stop = function()
 {
   return CQTSapi.JS_SAPI_Stop();
 }
 
 this.JS_SAPI_Hi_Start = function()
 {
   return CQTSapi.JS_SAPI_Hi_Start();
 }
 
 this.JS_SAPI_Lo_ASRRule = function(strRule)
 {
   return CQTSapi.JS_SAPI_Lo_ASRRule(strRule);
 }
 
 this.JS_SAPI_Lo_TTSRule = function(strRule)
 {
   return CQTSapi.JS_SAPI_Lo_TTSRule(strRule);
 }
 
 this.JS_SAPI_Lo_VoiceRule = function(strRule)
 {
   return CQTSapi.JS_SAPI_Lo_VoiceRule(strRule);
 }
 
 this.JS_SAPI_Lo_TTSText = function(strText)
 {
   return CQTSapi.JS_SAPI_Lo_TTSText(strText);
 }
 
 this.JS_SAPI_Lo_VoiceFile = function(strFile)
 {
   return CQTSapi.JS_SAPI_Lo_VoiceFile(strFile);
 }
}