package com.knightboost.sliver.demo

import android.os.*
import android.util.Log
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.knightboost.sliver.Sliver
import java.lang.Thread.sleep

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Sliver.init();
        setContentView(R.layout.activity_main)

        //模拟真实的APP环境
        for (i in 1..200){
            Thread {
                while (true) {
                    sleep(300)
                    var f = 1000;
                    for (j in 1..1000) {
                        f = j / 3;
                    }
                }
            }.start()
        }

        findViewById<View>(R.id.btn_trace_on_same_thread)
            .setOnClickListener {
                val methodFrames = Sliver.getMethodFrames(Looper.getMainLooper().thread)
                val stackTraceBySliver = Sliver.prettyMethodsAsString(methodFrames);
                Log.e("sliver","主线程采集主线程调用栈 \n$stackTraceBySliver")
            }

        Build.VERSION_CODES.P
        findViewById<View>(R.id.btn_trace_mainThread)
            .setOnClickListener {
                val thread = Thread {
                    val thread = Looper.getMainLooper().thread
                    val nativePeer = Sliver.getNativePeer(thread);
                    var i =0
                    while (i<1000) {
                        sleep(10)
                        val t1 = SystemClock.elapsedRealtimeNanos()
                        var stackTrace = thread.stackTrace
                        val t2 = SystemClock.elapsedRealtimeNanos()
                        val methodFrames = Sliver.getMethodFrames(thread,nativePeer)
                        val t3 = SystemClock.elapsedRealtimeNanos()
                        val stackTraceBySliver = Sliver.prettyMethods(methodFrames);
                        val t4 = SystemClock.elapsedRealtimeNanos()
                        Log.e("zxw",
                            "thread.getStackTrace耗时 ${ (t2 - t1)/1000/1000f} ms, " +
                                    "通过 Sliver获取 1阶段耗时 ${ (t3-t2)/1000/1000f} ms"+
                                    "通过 Sliver获取 2阶段耗时 ${ (t4-t3)/1000/1000f} ms"
                        )
                        i++
                    }
                }
                thread.start()
            }

        findViewById<View>(R.id.btn_lock_monitor_test).setOnClickListener {
            val targetThread = LockThreadCase.targetThread();
            Thread.sleep(1000)
            Sliver.getSackTrace(targetThread);
            Thread.sleep(1000)
            Sliver.getSackTrace(targetThread);
            Thread.sleep(1000)
            Sliver.getSackTrace(targetThread);


        }
    }

    override fun onResume() {
        super.onResume()
    }
}