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
            Thread({
                while (true){
                    Thread.sleep(300)
                    var f =1000;
                    for (i in 1..1000){
                         f = f/3;
                    }
                }
            }).start()
        }
        findViewById<View>(R.id.btn_trace_mainThread)
            .setOnClickListener {
                val thread = Thread {
                    val thread = Looper.getMainLooper().thread
                    val nativePeer = Sliver.getNativePeer(thread);

                    while (true) {
                        sleep(10)
                        val t1 = SystemClock.elapsedRealtimeNanos()
                        var stackTrace = thread.stackTrace
                        val t2 = SystemClock.elapsedRealtimeNanos()
                        val methodFrames = Sliver.getMethodFrames(thread,nativePeer)
                        val t3 = SystemClock.elapsedRealtimeNanos()
                        val stackTraceBySliver = Sliver.prettyMethods(methodFrames);
                        val t4 = SystemClock.elapsedRealtimeNanos()
                        Log.e("zxw",
                            "thread.getStackTrace耗时 ${t2 - t1}, " +
                                    "通过 Sliver获取 1阶段耗时 ${t3-t2}"+
                                    "通过 Sliver获取 2阶段耗时 ${t4-t3}"
                        )
                    }
                }
                thread.start()
            }
    }

    override fun onResume() {
        super.onResume()
    }
}