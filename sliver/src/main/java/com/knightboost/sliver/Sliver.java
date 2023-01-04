package com.knightboost.sliver;

import android.annotation.SuppressLint;
import android.util.Log;

import androidx.annotation.Keep;

import java.lang.reflect.Field;
import java.util.Arrays;
import java.util.List;

public class Sliver {

    //Flag indicating if the sliver already has been initialized
    private static boolean initialized = false;

    private static boolean initializeSuccess = false;

    private static final String TAG = "sliver";

    /**
     * 初始化sliver
     */
    public static void init() {
        if (!initialized) {
            initialized = true;
            try {
                System.loadLibrary("sliver");
            } catch (Throwable e) {
                e.printStackTrace();
                initializeSuccess = false;
                //TODO add native init function
            }
        }
    }

    public static boolean isInit() {
        return initialized;
    }

    /**
     * 注：不能在当前线程获取同线程的调用栈
     *
     * @param thread
     * @return
     */
    public static String getSackTrace(Thread thread) {
        long nativePeer = getNativePeer(thread);
        long[] frames = nativeGetMethodStackTrace(thread, nativePeer);
        String[] methods = prettyMethods(frames);
        StringBuilder sb = new StringBuilder();
        for (String method : methods) {
            sb.append(method).append("\n");
        }
        if (sb.length() > 0) {
            sb.deleteCharAt(sb.length() - 1);
        }
        Log.e(TAG, "getStackTrace is" + sb.toString());
        return sb.toString();
    }

    public static long[] getMethodFrames(Thread thread, long nativePeer) {
        return nativeGetMethodStackTrace(thread, nativePeer);
    }

    /**
     * 获取制定线程当前函数栈信息，返回的数组为每个 函数(ArtMethod)的指针地址
     * @param thread 目标线程
     * @return 返回函数栈每个函数的指针地址的数组
     */
    public static long[] getMethodFrames(Thread thread) {
        return nativeGetMethodStackTrace(thread, getNativePeer(thread));
    }

    public static String prettyMethodsAsString(long[] methodIds){
        String[] methods = prettyMethods(methodIds);
        StringBuilder sb = new StringBuilder();
        for (String method : methods) {
            sb.append(method).append("\n");
        }
        if (sb.length()>0){
            sb.deleteCharAt(sb.length()-1);
        }
        return sb.toString();
    }

    public static native String[] prettyMethods(long[] methodIds);

    /**
     * 获取制定线程当前函数栈信息，返回的数组为每个 函数(ArtMethod)的指针地址
     * @param thread 目标线程
     * @param nativePeer 线程对于的 native thread指针地址
     * @return 返回函数栈每个函数的指针地址的数组
     */
    public static native long[] nativeGetMethodStackTrace(Thread thread, long nativePeer);

    //

    @Keep
    //this method called by native method
    public static void  onStackTrace(Thread thread,long methods,long timebase){

    };


    /**
     * @param thread thread
     * @return threadPeer value of the thread
     */
    @SuppressWarnings("ConstantConditions")
    @SuppressLint("DiscouragedPrivateApi")
    public static long getNativePeer(Thread thread) {
        try {
            Field nativePeerField = Thread.class.getDeclaredField("nativePeer");
            nativePeerField.setAccessible(true);
            return (long) nativePeerField.get(thread);
        } catch (NoSuchFieldException e) {
            //should never happen
            e.printStackTrace();
            return -1;
        } catch (IllegalAccessException e) {
            //should never happen
            e.printStackTrace();
            return -1;
        }

    }
}
