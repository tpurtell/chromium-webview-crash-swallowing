package com.example.hellojni;

import android.app.Activity;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.os.Bundle;
import android.webkit.WebView;


public class HelloJni extends Activity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
		
		LinearLayout l = new LinearLayout(this);

		//install a logger for the signal
		Button hook_button = new Button(this);
		hook_button.setText("Hook");
		hook_button.setOnClickListener(new View.OnClickListener() {
             public void onClick(View v) {
                 hook(11);
             }
        });
		l.addView(hook_button);

		//make a webview to initialize crash handler etc
		Button webview_button = new Button(this);
		webview_button.setText("WebView");
		webview_button.setOnClickListener(new View.OnClickListener() {
             public void onClick(View v) {
                 WebView wv = new WebView(HelloJni.this);
             }
        });
		l.addView(webview_button);

		//button to cause a sigsegv
		Button sigsegv_button = new Button(this);
		sigsegv_button.setText("SIGSEGV");
		sigsegv_button.setOnClickListener(new View.OnClickListener() {
             public void onClick(View v) {
                 sigsegv();
             }
        });
		l.addView(sigsegv_button);		
		
        setContentView(l);
    }

	public native void hook(int signal);
	public native void sigsegv();
    static {
        System.loadLibrary("hello-jni");
    }
}
