package com.klvideoplayer;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.os.Handler;
import android.os.Message;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.Gravity;
import android.widget.FrameLayout;
import android.widget.ProgressBar;
import android.widget.TextView;

import java.util.Timer;
import java.util.TimerTask;

public class ProgressView {
    private ProgressBar _progressbar;
    private TextView _timeText;

    private Timer timer;
    private TimerTask task;
    private boolean _init = false;
    private int durationTimeS;
    private int presentTimeS;

    private String TranTimeFormat(int timeS) {
        int seconds = timeS % 60;
        int minutes = timeS / 60;
        String text = String.format("%02d",minutes) + ":"+ String.format("%02d",seconds);
        return text;
    }

    public void InitCompents(Context ctx) {
        _timeText = new TextView(ctx);
        _timeText.setTextSize(10);
        _timeText.setText("00:00 / 00:00");
        _timeText.setTextColor(Color.GRAY);

        _progressbar=new ProgressBar(ctx,null,android.R.attr.progressBarStyleHorizontal);//水平条形
        _progressbar.setMax(100);
        _progressbar.setProgress(0);
        //_progressbar.setSecondaryProgress(75);

        timer = new Timer();

        @SuppressLint("HandlerLeak") final Handler handler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                // TODO Auto-generated method stub
                if (_init == false) {
                    _init = true;
                    durationTimeS = (int)(KLJniInterface.getDurationTimeMs() / 1000);
                    _progressbar.setMax(durationTimeS);
                }
                presentTimeS = (int)( KLJniInterface.getPresentTimeMs() / 1000);
                _progressbar.setProgress(presentTimeS);
                String timePrensent = TranTimeFormat(presentTimeS);
                String timeDuration = TranTimeFormat(durationTimeS);
                _timeText.setText(timePrensent + " / " + timeDuration);

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

        Display display = activity.getWindowManager().getDefaultDisplay();
        DisplayMetrics metrics = new DisplayMetrics();
        display.getMetrics(metrics);

        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams
                (metrics.widthPixels - 80, 20);
        params.gravity= Gravity.TOP|Gravity.LEFT;
        params.leftMargin = 40;
        params.topMargin = metrics.heightPixels - 200;
        activity.addContentView(_progressbar, params);

        FrameLayout.LayoutParams timeParam = new FrameLayout.LayoutParams
                (FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
        timeParam.gravity= Gravity.TOP|Gravity.LEFT;
        timeParam.leftMargin = 240;
        timeParam.topMargin = metrics.heightPixels - 130;;
        activity.addContentView(_timeText, timeParam);
    }

    public void UpdateProgress(int pros)
    {
        _progressbar.setProgress(pros);
    }
}
