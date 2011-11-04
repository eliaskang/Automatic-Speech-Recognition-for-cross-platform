
function CQT_AppDownload()
{
 this.Class_GetCount = function()
 {
   return appdownload.JS_Class_GetCount();
 }
 
 this.Class_GetName = function(idx)
 {
  return appdownload.JS_Class_GetName(idx);
 }
 
 this.Class_GetID = function(idx)
 {
  return appdownload.JS_Class_GetID(idx);
 }
 
 this.Class_GetImg = function(idx)
 {
  return appdownload.JS_Class_GetImg(idx);
 }
 
 this.AP_GetName = function(strID)
 {
  return appdownload.JS_AP_GetName(strID);
 }
 
 this.AP_GetCompany = function(strID)
 {
 return appdownload.JS_AP_GetCompany(strID);
 }
 
 this.AP_GetImg = function(strID)
 {
 return appdownload.JS_AP_GetImg(strID);
 }
 
 this.AP_GetStar = function(strID)
 {
 return appdownload.JS_AP_GetStar(strID);
 }
  
 this.AP_GetPrice = function(strID)
 {
 return appdownload.JS_AP_GetPrice(strID);
 }
 
 this.AP_GetDescribe = function(strID)
 {
 return appdownload.JS_AP_GetDescribe(strID);
 }
 
 this.AP_GetVersion = function(strID)
 {
 return appdownload.JS_AP_GetVersion(strID);
 }
 
 this.AP_GetDownloadCount = function(strID)
 {
 return appdownload.JS_AP_GetDownloadCount(strID);
 }
 
 this.GetRecommendApCount = function()
 {
 return appdownload.JS_GetRecommendApCount();
 }
 
 this.GetRecommendAppID = function(nIdx)
 {
 return appdownload.JS_GetRecommendAppID(nIdx);
 }
 
 this.GetRecommendAppID = function()
 {
 return appdownload.JS_GetRecommendAppID();
 }
 
 this.CLASS_AP_GetCount = function(strID)
 {
 return appdownload.JS_CLASS_AP_GetCount(strID);
 }
 
 this.CLASS_AP_GetAppID = function(strID,idx)
 {
 return appdownload.JS_CLASS_AP_GetAppID(strID,idx);
 }
 
 this.OpenURL = function(str)
 {
	return appdownload.JS_OpenURL(str);
 }
 
 this.BackURL = function()
 {
	return appdownload.JS_BackURL();
 }
 
 this.SetCook = function(strKey,strValue)
 {
	 return appdownload.JS_COOK_Add(strKey, strValue);
 }
 
 this.GetCook = function(strKey)
 {
	 return appdownload.JS_COOK_Get(strKey);
 }
 
 this.InstallPackageEX = function(strID)
 {
 	return appdownload.JNI_InstallPackageEX(strID);
 } 
 
 this.InstallPackage = function(strID)
 {
 	return appdownload.JS_InstallPackage(strID);
 } 
 
 this.IsInstallPackage = function(strID)
 {
 	return appdownload.JS_IsInstallPackage(strID);
 } 
 
 this.UnintallPackage = function(strID)
 {
 	return appdownload.JS_UnintallPackage(strID);
 } 
}