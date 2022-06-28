package com.knightboost.sliver.demo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.knightboost.sliver.NativeLib

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }

    override fun onResume() {
        super.onResume()
        NativeLib().stringFromJNI()
    }
}