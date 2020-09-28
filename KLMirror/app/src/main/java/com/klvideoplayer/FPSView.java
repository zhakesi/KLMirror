package com.klvideoplayer;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.os.Handler;
import android.os.Message;
import android.view.Gravity;
import android.widget.FrameLayout;
import android.widget.TextView;

import java.util.Timer;
import java.util.TimerTask;

public class FPSView {
    private TextView mFPSLabel;
    private Timer timer;
    private TimerTask task;
    private static int count = 0;
    public void InitCompents(Context ctx) {
        mFPSLabel = new TextView(ctx);
        mFPSLabel.setText("FPS:0");
        mFPSLabel.setTextColor(Color.GREEN);

        timer = new Timer();

        @SuppressLint("HandlerLeak") final Handler handler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                // TODO Auto-generated method stub
                mFPSLabel.setText("FPS:" + (count+1));
                count = 0;
                super.handleMessage(msg);
            }
        };

        task = new TimerTask() {
            @Override
            public void run() {
                // TODO Auto-generated method stub
                Message message = new Message();
                message.what = 1;
                handler.sendMessage(message);
            }
        };
        timer.schedule(task, 1000, 1000);
    }

    public void AddToActivity(Activity activity) {
        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams
                (FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
        params.gravity= Gravity.TOP|Gravity.LEFT;
        params.leftMargin = 20;
        params.topMargin = 0;
        activity.addContentView(mFPSLabel, params);
    }

    public static void CountOnce(){
        count++;
    }
}
