package com.klvideoplayer;

import android.content.res.AssetManager;

public class KLJniInterface {
    public static int PLAY_STATUS_PLAY = 0;
    public static int PLAY_STATUS_PAUSE = 1;

    static {
        System.loadLibrary("klmirror");
    }
    public static native void       graphicInit();
    public static native void       onResize(int width, int height);
    public static native void       renderFrame();
    public static native void       setPlayStatus(int status);
    public static native void       initAssetMGR(AssetManager assetManager);
    public static native void       configMediaInfo(int mode, String media);
    public static native void       controlSwith(boolean isOpen);
    public static native long       getDurationTimeMs();
    public static native long       getPresentTimeMs();
}
