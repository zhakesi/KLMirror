package com.klvideoplayer;

import android.app.Activity;
import android.os.Bundle;
import android.view.WindowManager;

public class EnterActivity extends Activity {
    private PlayerView mView;
    private FPSView mFPSView;
    private ProgressView mProgressView;
    private ControlView mControlView;
    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);

        intConfigure(PICTURE_MODE, "dd.png");
        mView = new PlayerView(getApplication());
        setContentView(mView);

        getWindow().setFlags(
                WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
                WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        mFPSView = new FPSView();
        mFPSView.InitCompents(getApplicationContext());
        mFPSView.AddToActivity(this);

        mProgressView = new ProgressView();
        mProgressView.InitCompents(getApplicationContext());
        mProgressView.AddToActivity(this);

        mControlView = new ControlView();
        mControlView.InitCompents(getApplicationContext());
        mControlView.AddToActivity(this);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
    private static final int VIDEO_MODE = 0;
    private static final int PICTURE_MODE = 1;
    private void intConfigure(int mode, String media)
    {
        KLJniInterface.initAssetMGR(getAssets());
        KLJniInterface.configMediaInfo(mode, media);
    }

    public ProgressView GetProgressView()
    {
        return mProgressView;
    }
}
