package com.knightboost.sliver.demo;

import android.util.Log;

public class LockThreadCase {
    private static final Object lock = new Object();

    public static Thread targetThread(){
            new Thread(new Runnable() {
                @Override
                public void run() {
                    synchronized (lock){
                        try {
                            Thread.sleep(10_000);
                        } catch (InterruptedException e) {
                        }
                    }
                }
            }).start();
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                }

                synchronized (lock) {
                    try {
                        Thread.sleep(8000);
                    } catch (InterruptedException e) {
                    }
                }
            }
        });
        thread.start();
        return  thread;
    }
}
