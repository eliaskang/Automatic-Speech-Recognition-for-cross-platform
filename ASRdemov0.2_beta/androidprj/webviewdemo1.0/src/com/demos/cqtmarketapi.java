package com.demos;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Environment;
import android.util.Log;
import android.webkit.WebView;

public class cqtmarketapi{
	List<Map> m_listClass = new ArrayList<Map>();
	List<Map> m_listApp = new ArrayList<Map>();
	String m_strOldURL = "";
	Map<String, String>m_cook = new HashMap<String, String>();
	Context mContext;
	List<PackageInfo> m_PackManager = new ArrayList<PackageInfo>();
	
	///////////////////////////////////////////////////////////////
	//加载JNI
	public WebView mWebView;
	final void init_library(){
		try {
            System.loadLibrary("localwebviewjni_download");
        } catch (UnsatisfiedLinkError e) {
            Log.d("cqtmarketapi", "cqtmarketapi jni library not found!");
        }
	}
	
	cqtmarketapi() {
     	init_library();
     }
     
	void SetContext(Context c)
	{
		mContext = c;
	}
	
     public void SetWebview(WebView v)
     {
     	mWebView = v;
     	Log.i("SetWebview","--------------------------- JS_CQT_API_Init");
     	JS_CQT_API_Init();
     	Log.i("SetWebview","--------------------------- JS_CQT_API_Init OVER");
     }
     
     //下面代码提供JNI层的网络接口和XML解析的接口
     ////////////////////////////////////////////////////////////
     //用Get方法获取网页XML内容
     public String Jni_GetURLStringFromAMS(final String strURL){
    	 String strContext = "";
    	 try
         {
    		 URL url = new URL(strURL);
    		 HttpURLConnection conn = (HttpURLConnection) url.openConnection();
    		 conn.setDoInput(true);
    		 conn.setConnectTimeout(10000);
    		 conn.setRequestMethod("GET");
    		 conn.setRequestProperty("accept", "*/*");
    		 String location = conn.getRequestProperty("location");
    		 int resCode = conn.getResponseCode();
    		 conn.connect();
    		 InputStream stream = conn.getInputStream();       		 
    		 
    		 byte[]  data=new byte[1024];
    		 int length = 0;
    		 while(true)
    		 {
    			 length = stream.read(data);
    			 if(length == -1)
    			 {
    				 break;
    			 }
    			 
    			 String str=new String(data,0,length);
    			 strContext += str;
    		}
    		 
    		conn.disconnect();
    		stream.close();
         }
         catch(Exception ee)
         {
             //System.out.print("ee:"+ee.getMessage());            
         }
         
    	 return strContext;        	 
     }
     
     ///////////////////////////////////////////////////////////////
     //用Get方法获取网页内容到文件
     public boolean Jni_GetURLFileFromAMS(final String strURL,final String strFile)
     {
    	 Log.i("Jni_GetURLFileFromAMS","--------------------------- 1");
    	 try
         {
    		 URL url = new URL(strURL);
    		 HttpURLConnection conn = (HttpURLConnection) url.openConnection();
    		 conn.setDoInput(true);
    		 conn.setConnectTimeout(10000);
    		 conn.setRequestMethod("GET");
    		 conn.setRequestProperty("accept", "*/*");
    		 String location = conn.getRequestProperty("location");
    		 int resCode = conn.getResponseCode();
    		 conn.connect();
    		 InputStream stream = conn.getInputStream();       		 
    		 
    		 Log.i("Jni_GetURLFileFromAMS","--------------------------- 2:" + strFile);
    		 File fSave = new File(strFile);
    		 FileOutputStream fOut = new FileOutputStream(fSave);
    		 byte[]  data=new byte[1024];
    		 int length = 0;
    		 while(true)
    		 {
    			 length = stream.read(data);
    			 if(length == -1)
    			 {
    				 break;
    			 }
    			 
    			 fOut.write(data,0,length);
    		}       		 
    		 
    		conn.disconnect();
    		stream.close();
    		fOut.flush();
    		fOut.close();
    		Log.i("Jni_GetURLFileFromAMS","--------------------------- 4");
    		return true;
         }
         catch(Exception ee)
         {
             //System.out.print("ee:"+ee.getMessage());            
         }
         
         return false;        	      	 
     }  
     
     /////////////////////////////////////////////////////////
     //
     public boolean SDCard_Isexist()
     {
    	 String status = Environment.getExternalStorageState();
    	 if (status.equals(Environment.MEDIA_MOUNTED)) 
    	 {
    		  return true;
    	 } else 
    	 {
    		 return false;
    	 }        	 
     }
     
     ////////////////////////////////////////////////////////
     //创建文件夹
     public boolean JNI_CreateFolder(final String strFolder)
     {
    	 File destDir = new File(strFolder);
    	 if(!destDir.exists()) 
    	 {
    	     return destDir.mkdirs();
    	 }else
    	 {
    		 Log.i("创建失败",strFolder + "创建失败");
    		 return true;
    	 }
     }
     
     ///////////////////////////////////////////////////////////
     //XML解析分类
     //目前演示时，程序的分类为： Application,Games,Car,My download, Search
     public boolean ParseClassXML(final String strXMLFile)
     {
    	 Log.i("ParseClassXML","--------------------------- 1 File:" + strXMLFile);
    	 m_listClass.clear();
    	 DocumentBuilderFactory factory =  DocumentBuilderFactory.newInstance();
    	 try
    	 {
    	   Log.i("ParseClassXML","--------------------------- 2");
    	   File fRead = new File(strXMLFile);
    	   if(!fRead.exists())
    	   {
    		 Log.i("ParseClassXML","--------------------------- Error");
    	     return false;
    	   }
    	   FileInputStream STin = new FileInputStream(fRead);
    	   DocumentBuilder builder = factory.newDocumentBuilder();
           Document dom = builder.parse(STin);
           Element root = dom.getDocumentElement();
           NodeList items = root.getElementsByTagName("category");
           
           for (int i = 0; i < items.getLength(); i++) 
           {
        	   Map<String, String>temp = new HashMap<String, String>();
        	   
              //得到第一个person节点
               Element personNode = (Element) items.item(i);   
               //获取person节点的id属性值
               //temp.put("id", personNode.getAttribute("id"));                   
               //获取person节点下的所有子节点(标签之间的空白节点和name/age元素)   
               NodeList childsNodes = personNode.getChildNodes();   
               for (int j = 0; j < childsNodes.getLength(); j++)
               {
                 Node node = (Node) childsNodes.item(j);                     
                 //判断是否为元素类型   
                 if(node.getNodeType() == Node.ELEMENT_NODE)
                 {                          
                   Element childNode = (Element) node;   
                   //判断是否name元素   
                   if ("name".equals(childNode.getNodeName())) 
                   {
                     //获取name元素下Text节点,然后从Text节点获取数据                    	   
                	   temp.put("name", childNode.getFirstChild().getNodeValue());
                    }else if("icon".equals(childNode.getNodeName()))
                    {
                       temp.put("icon", childNode.getFirstChild().getNodeValue());                              
                    }else if("app_count".equals(childNode.getNodeName()))
                    {
                        temp.put("app_count", childNode.getFirstChild().getNodeValue());                              
                    }else if("id".equals(childNode.getNodeName()))
                    {
                        temp.put("id", childNode.getFirstChild().getNodeValue());                              
                    }
                  }
               }
               m_listClass.add(temp);
           }
           
    	   STin.close();
    	   return true;
    	 }
    	 catch(Exception e)
         {        		 
    		 Log.d("解析APP CLASS失败","解析APP CLASS失败");            
         }
    	 
    	 return false;
     }     
     
     ///////////////////////////////////////////////////////
     //获取程序类别的个数
     public int JNI_GetClassCount()
     {
    	 return m_listClass.size();
     }
     
     /////////////////////////////////////////////////////////
     //获取程序类别
     public String JNI_GetClassNodeName(int idx, final String strNode)
     {
    	 String strRet = "";        	 
    	 Map map = (Map)m_listClass.get(idx);
    	 strRet = (String)map.get(strNode);
    	 return strRet;
     }
     
   //m_listApp
     public boolean ParseApplicationXML(final String strXMLFile)
     {
    	 Log.i("ParseApplicationXML","--------------------------- 1 File:" + strXMLFile);
    	 m_listApp.clear();
    	 DocumentBuilderFactory factory =  DocumentBuilderFactory.newInstance();
    	 try
    	 {
    	   Log.i("ParseApplicationXML","--------------------------- 2");
    	   File fRead = new File(strXMLFile);
    	   if(!fRead.exists())
    	   {
    		 Log.i("ParseApplicationXML","--------------------------- Error");
    	     return false;
    	   }
    	   FileInputStream STin = new FileInputStream(fRead);
    	   DocumentBuilder builder = factory.newDocumentBuilder();
           Document dom = builder.parse(STin);
           Element root = dom.getDocumentElement();
           NodeList items = root.getElementsByTagName("application");
           
           for (int i = 0; i < items.getLength(); i++) 
           {
        	   Map<String, String>temp = new HashMap<String, String>();
        	   
              //得到第一个person节点
               Element personNode = (Element)items.item(i);   
               //获取person节点的id属性值
               //temp.put("id", personNode.getAttribute("id"));                   
               //获取person节点下的所有子节点(标签之间的空白节点和name/age元素)   
               NodeList childsNodes = personNode.getChildNodes();   
               for (int j = 0; j < childsNodes.getLength(); j++)
               {
                 Node node = (Node) childsNodes.item(j);                     
                 //判断是否为元素类型   
                 if(node.getNodeType() == Node.ELEMENT_NODE)
                 {
                   Element childNode = (Element) node;   
                   //判断是否id元素   
                    if("id".equals(childNode.getNodeName())) 
                    {
                	   temp.put("id", childNode.getFirstChild().getNodeValue());
                    }else if("name".equals(childNode.getNodeName()))
                    {
                       temp.put("name", childNode.getFirstChild().getNodeValue());                              
                    }else if("star".equals(childNode.getNodeName()))
                    {
                        temp.put("star", childNode.getFirstChild().getNodeValue());                              
                    }else if("price".equals(childNode.getNodeName()))
                    {
                        temp.put("price", childNode.getFirstChild().getNodeValue());                              
                    }else if("author".equals(childNode.getNodeName()))
                    {
                        temp.put("author", childNode.getFirstChild().getNodeValue());                              
                    }else if("logo_url".equals(childNode.getNodeName()))
                    {
                        temp.put("logo_url", childNode.getFirstChild().getNodeValue());                              
                    }else if("description".equals(childNode.getNodeName()))
                    {
                        temp.put("description", childNode.getFirstChild().getNodeValue());                              
                    }
                  }
               }
               m_listApp.add(temp);
           }
           
    	   STin.close();
    	   return true;
    	 }
    	 catch(Exception e)
         {        		 
    		 Log.d("解析APP失败","解析APP失败");            
         }
    	 
    	 return false;
     }     
     
     ///////////////////////////////////////////////////////
     //获取程序类别的个数
     public int JNI_GetAppCount()
     {
    	 return m_listApp.size();
     }
     
     /////////////////////////////////////////////////////////
     //获取程序类别
     public String JNI_GetAppNodeName(int idx, final String strNode)
     {
    	 String strRet = "";
    	 Map map = (Map)m_listApp.get(idx);
    	 strRet = (String)map.get(strNode);
    	 return strRet;
     }
     
     ///////////////////////////////////////////////////////
     //刷新界面
     public void JNI_RefreshUI()
     {
    	 Log.i("Java JNI_RefreshUI","--------------------------- 1");
    	 mWebView.loadUrl("javascript:refreshUI()");
    	 Log.i("Java JNI_RefreshUI","--------------------------- over");
     }
     
     public void JS_OpenURL(final String strURL)
     {
    	 String str = "file:///android_asset/";
    	 str += strURL;
    	 mWebView.loadUrl(str);
     }
     
     public void JS_OpenFirstURL(final String strURL)
     {
    	 m_strOldURL = strURL;
    	 mWebView.loadUrl(strURL);
     }
     
     public void JS_BackURL()
     {
    	 mWebView.loadUrl(m_strOldURL);
     }
     
     
     /////////////////////////////////////////////////////////////////////////////////////
     //网络是否连接好
     public boolean JNI_HasInternet() 
     {
    	  ConnectivityManager connectivity = (ConnectivityManager)mContext.getSystemService(Context.CONNECTIVITY_SERVICE);
    	  if (connectivity == null) 
    	  {
    	   return false;
    	  }else 
    	  {
    	    NetworkInfo[] info = connectivity.getAllNetworkInfo();
    	    if (info != null) 
    	    {
    	     for (int i = 0; i < info.length; i++) 
    	     {
    	       if (info[i].getState() == NetworkInfo.State.CONNECTED) 
    	       {
    	         return true;
    	       }
    	     }//end for
    	    }
    	  }//end if
    	  
       return false;
    }

     
     //启动下载包      
     //启动升级包
     public boolean JNI_InstallPackage(final String strPackageFile)
     {
    	 String strPackage = strPackageFile;
    	 /*
    	 String strPackage = "/mnt/sdcard/";
    	 strPackage += strID;
    	 strPackage += ".apk";
    	 String strURL = "http://172.30.92.249:8081/ivas/market/downloadFile.do?interface=cqt-api0003&applicationID=";
    	 strURL += strID;
    	 Log.e("JNI_InstallPackage strURL", strURL);
    	 Log.e("JNI_InstallPackage strURL", "Start Download");
    	 
    	 String strPackageName = JNI_GetPackageName(strPackage);
    	 Log.e("------------JNI_InstallPackage strURL", strPackageName);
    	 
    	 boolean bRet = Jni_GetURLFileFromAMS(strURL,strPackage);
    	 Log.e("JNI_InstallPackage strURL", "Start Over");
    	 */
    	 
    	 Log.e("OpenFile", strPackage);
         Intent intent = new Intent();
         intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
         intent.setAction(android.content.Intent.ACTION_VIEW);
         intent.setDataAndType(Uri.fromFile(new File(strPackage)),"application/vnd.android.package-archive");
         mContext.startActivity(intent);
    	 return true;
     }
     
     public boolean JNI_InstallPackageEX(final String strID)
     {
    	// String strPackage = strPackageFile;
    	 
    	 String strPackage = "/mnt/sdcard/";
    	 strPackage += strID;
    	 strPackage += ".apk";
    	 String strURL = "http://172.30.92.249:8081/ivas/market/downloadFile.do?interface=cqt-api0003&applicationID=";
    	 strURL += strID;
    	 Log.e("JNI_InstallPackage strURL", strURL);
    	 Log.e("JNI_InstallPackage strURL", "Start Download");
    	 
    	 String strPackageName = JNI_GetPackageName(strPackage);
    	 Log.e("------------JNI_InstallPackage strURL", strPackageName);
    	 
    	 boolean bRet = Jni_GetURLFileFromAMS(strURL,strPackage);
    	 Log.e("JNI_InstallPackage strURL", "Start Over");    	 
    	 
    	 Log.e("OpenFile", strPackage);
         Intent intent = new Intent();
         intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
         intent.setAction(android.content.Intent.ACTION_VIEW);
         intent.setDataAndType(Uri.fromFile(new File(strPackage)),"application/vnd.android.package-archive");
         mContext.startActivity(intent);
    	 return true;
     }
     
     public void JNI_uninstallApp(final String packageName) 
     {
    	  Uri packageURI = Uri.parse("package:" + packageName);   
    	  Intent uninstallIntent = new Intent(Intent.ACTION_DELETE, packageURI);   
    	  mContext.startActivity(uninstallIntent);
     }
     
     //////////////////////////////////////////////////////////////////
     //APK管理
     //从安装包文件获取包名和版本信息
     public String JNI_GetPackageName(final String strAPK)
     {
    	 String result = "noversion";
    	 //String archiveFilePath="/mnt/sdcard/7.apk";//安装包路径
    	 String archiveFilePath=strAPK;
         PackageManager pm = mContext.getPackageManager();
         PackageInfo info = pm.getPackageArchiveInfo(archiveFilePath, PackageManager.GET_ACTIVITIES);  
         if(info != null)
         {
             ApplicationInfo appInfo = info.applicationInfo;
             String appName = pm.getApplicationLabel(appInfo).toString();  
             String packageName = appInfo.packageName;  //得到安装包名称
             String version=info.versionName;       //得到版本信息
             //Drawable icon = pm.getApplicationIcon(appInfo);//得到图标信息          
             result = packageName + "&&^^" + version;
         }
         
         return result;
     }

     //m_PackManager
     public boolean JNI_InitPackmanamer()
     {
    	 m_PackManager.clear();
    	 PackageManager pckMan = mContext.getPackageManager();
    	 List<PackageInfo> packs=pckMan.getInstalledPackages(0); 
    	 
    	 for (int i = 0; i < packs.size(); i++) 
    	 {
 			PackageInfo pak = (PackageInfo)packs.get(i);
 			//判断是否为非系统预装的应用程序
 			if((pak.applicationInfo.flags & pak.applicationInfo.FLAG_SYSTEM) <= 0) 
 			{
 				// customs applications
 				m_PackManager.add(pak);
 			}
 		}
    	 
    	 return true;
     }
     
     //获取安装包的个数
     public int JNI_GetInstalledPKGCount()
     {
    	 return m_PackManager.size();
     }
     
     //获取安装包的版本和包名
     public String JNI_GetInstalledPKGInfo(int idx)
     {
    	 String result = "noversion";
    	 PackageManager pm = mContext.getPackageManager();
    	 PackageInfo info = (PackageInfo)m_PackManager.get(idx);    	 
    	 ApplicationInfo appInfo = info.applicationInfo;
         String appName = pm.getApplicationLabel(appInfo).toString();
         
         String packageName = appInfo.packageName;  //得到安装包名称
         String version=info.versionName;           //得到版本信息
         result = packageName + "&&^^" + version;
         Log.e("------------JNI_GetInstalledPKGInfo result:", result);
         return result;
     }
     
     public String JNI_GetVersionInfo(final String strPackagename)
     {
    	 PackageManager pm = mContext.getPackageManager();
    	 String strVersion = "noversion";
    	 for (int i = 0; i < m_PackManager.size(); i++) 
    	 {
 			PackageInfo info = (PackageInfo)m_PackManager.get(i);
 			
 			ApplicationInfo appInfo = info.applicationInfo;
 	        String appName = pm.getApplicationLabel(appInfo).toString(); 	         
 	        String packageName = appInfo.packageName;  //得到安装包名称
 	        String version=info.versionName;
 	        if(strPackagename.equals(packageName))
 	        {
 	        	strVersion = version;
 	        	return strVersion;
 	        }
 		}
    	 
    	return strVersion;
     }
     
     //以下提供的是JS层的调用接口
     /////////////////////////////////////////////////////////
     //CQT-JS API
     //初始化,在HTML页面加载的时候调用
     public native int JS_CQT_API_Init();
     public native int JS_CQT_API_Uninit();
     
     //程序类别的操作
     public native int    JS_Class_GetCount();
     public native String JS_Class_GetName(int idx);
     public native String JS_Class_GetID(int idx);
     public native String JS_Class_GetImg(int idx);
     
     //程序的属性获取
     public native String JS_AP_GetName(final String ID);
     public native String JS_AP_GetCompany(final String ID);
     public native String JS_AP_GetImg(final String ID);
     public native int 	  JS_AP_GetStar(final String ID);
     public native String JS_AP_GetPrice(final String ID);
     public native String JS_AP_GetDescribe(final String ID);
     public native String JS_AP_GetVersion(final String ID);
     public native int 	  JS_AP_GetDownloadCount(final String ID);     
     
     //推荐程序的操作
     public native int    JS_GetRecommendApCount();
     public native String JS_GetRecommendAppID();
     
     //获取分类的程序
     public native int	  JS_CLASS_AP_GetCount(final String strClassID);
     public native String JS_CLASS_AP_GetAppID(final String strClassID,int idx);
     
     
     //程序安装和卸载管理
     public native int  JS_InstallPackage(final String strAppID);
     public native int  JS_UnintallPackage(final String strAppID);
     public native int  JS_IsInstallPackage(final String strAppID);
     public native int  JS_IsUpdatePackage(final String strAppID);
     public native int  JS_UpdatePackage(final String strAppID);
     
     //////////////////////////////////////////////////
     //内存cook设置
     public void JS_COOK_Add(final String strKey, final String strValue)
     {
    	 m_cook.put(strKey, strValue);
     }
     
     public String JS_COOK_Get(final String strKey)
     {
    	 return (String)m_cook.get(strKey);
     }     
}
