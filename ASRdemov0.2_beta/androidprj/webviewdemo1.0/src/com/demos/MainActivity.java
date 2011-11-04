package com.demos;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.KeyEvent;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;


public class MainActivity extends Activity {
    private WebView mWebView;
    final Handler cwjHandler = new Handler();
 
    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        setContentView(R.layout.main);
        mWebView = (WebView) findViewById(R.id.webview);

        WebSettings webSettings = mWebView.getSettings();
        webSettings.setSavePassword(false);
        webSettings.setSaveFormData(false);
        webSettings.setJavaScriptEnabled(true);
        
        Log.i("MainActivity","^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^CqtMarketApiInterface");        
        cqtmarketapi javainterface = new cqtmarketapi();  
        javainterface.SetContext(this);
        javainterface.SetWebview(mWebView);
        
        CQTSapi javaCQTSapi = new CQTSapi();
        javaCQTSapi.SetContext(this);
        javaCQTSapi.SetWebview(mWebView);
        
        mWebView.addJavascriptInterface(javainterface, "appdownload");
        mWebView.addJavascriptInterface(javaCQTSapi, "CQTSapi");

        mWebView.setWebViewClient(new HelloWebViewClient());
        javainterface.JS_OpenFirstURL("file:///android_asset/market_main.htm");
        //javaCQTSapi.JS_OpenFirstURL("file:///android_asset/sapi_sample.htm");
}

    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if ((keyCode == KeyEvent.KEYCODE_BACK) && mWebView.canGoBack()) {
            mWebView.goBack();
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    private class HelloWebViewClient extends WebViewClient {
        @Override
        public boolean shouldOverrideUrlLoading(WebView view, String url) {
            view.loadUrl(url);
            return true;
        }
    }    
}

