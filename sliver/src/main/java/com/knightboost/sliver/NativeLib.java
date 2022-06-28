package com.knightboost.sliver;

public class NativeLib {

    // Used to load the 'sliver' library on application startup.
    static {
        System.loadLibrary("sliver");
    }

    /**
     * A native method that is implemented by the 'sliver' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}