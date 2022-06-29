package com.knightboost.sliver;

import android.util.Log;

import java.lang.reflect.Field;

public class NativeLib {

    // Used to load the 'sliver' library on application startup.
    static {
        System.loadLibrary("sliver");
    }
    //触发so架子啊
    public static void doNothing(){

    }

    public static void trace(Thread t,long nativePeer){
        traceStack(t,nativePeer);
    }

    public static long getThreadNativePeer(Thread t){
        try {
            Field nativePeerField = Thread.class.getDeclaredField("nativePeer");
            nativePeerField.setAccessible(true);
            long nativePeer = (long) nativePeerField.get(t);
            return nativePeer;
        } catch (NoSuchFieldException | IllegalAccessException e) {
            //should never happend
        }
        throw new IllegalStateException("should never happen");
    }

    public static native void traceStack(Thread thread,long nativePeer);
}