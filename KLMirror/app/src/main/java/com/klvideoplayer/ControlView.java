package com.klvideoplayer;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.Gravity;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.FrameLayout;

public class ControlView {
    private CheckBox _pauseBox;
    private CheckBox _filterBox;
    @SuppressLint("ResourceType")
    public void InitCompents(Context ctx) {
        _pauseBox = new CheckBox(ctx);
        _pauseBox.setChecked(false);
        _pauseBox.setButtonDrawable(R.drawable.play_status);
        //_pauseBox.setButtonDrawable(Color.TRANSPARENT);
        /// _pauseBox.setBackground
        _pauseBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener()
        {
            @Override
            public void onCheckedChanged(CompoundButton buttonView,
                                         boolean isChecked) {
                // TODO Auto-generated method stub
                int status = KLJniInterface.PLAY_STATUS_PLAY;
                if (isChecked)
                    status = KLJniInterface.PLAY_STATUS_PAUSE;
                KLJniInterface.setPlayStatus(status);
            }
        });

        _filterBox = new CheckBox(ctx);
        _filterBox.setText("Anime4K");
        _filterBox.setChecked(false);
        _filterBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener()
        {
            @Override
            public void onCheckedChanged(CompoundButton buttonView,
                                         boolean isChecked) {
                // TODO Auto-generated method stub
                KLJniInterface.controlSwith(isChecked);
            }
        });
    }

    public void AddToActivity(Activity activity) {
        Display display = activity.getWindowManager().getDefaultDisplay();
        DisplayMetrics metrics = new DisplayMetrics();
        display.getMetrics(metrics);

        FrameLayout.LayoutParams pauseParam = new FrameLayout.LayoutParams
                (FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
        pauseParam.gravity= Gravity.TOP|Gravity.LEFT;
        pauseParam.leftMargin = 80;
        pauseParam.topMargin = metrics.heightPixels - 140;;
        activity.addContentView(_pauseBox, pauseParam);

        FrameLayout.LayoutParams filterParam = new FrameLayout.LayoutParams
                (FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
        filterParam.gravity= Gravity.TOP|Gravity.RIGHT;
        filterParam.rightMargin = 40;
        filterParam.topMargin = 0;;
        activity.addContentView(_filterBox, filterParam);
    }
}
