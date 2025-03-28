package com.haruma.sen.environment

import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import android.Manifest
import android.net.Uri
import android.os.Bundle
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugin.common.MethodChannel
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

class MainActivity : FlutterActivity() {

    private val channel = "com.haruma.sen.environment"

    private lateinit var storageIntentHandler: StorageIntentHandler

    companion object {
        private const val NOTIFICATION_PERMISSION_CODE: Int = 1
    }

    override fun configureFlutterEngine(flutterEngine: FlutterEngine) {
        super.configureFlutterEngine(flutterEngine)
        storageIntentHandler = StorageIntentHandler(this)
        MethodChannel(flutterEngine.dartExecutor.binaryMessenger, channel).setMethodCallHandler { call, result ->
            CoroutineScope(Dispatchers.Main).launch {
                storageIntentHandler.handleMethodCall(call, result)
            }
        }
    }

    override fun onCreate (
        savedInstanceState: Bundle?
    ): Unit {
        super.onCreate(savedInstanceState)
        this.requestNotificationPermission()
        CoroutineScope(Dispatchers.Main).launch {
            this@MainActivity.handleIncomingIntent(intent)
        }
    }

    override fun onActivityResult(
        requestCode: Int, 
        resultCode: Int, 
        data: Intent?
    ): Unit {
        super.onActivityResult(requestCode, resultCode, data)
        storageIntentHandler.handleActivityResult(requestCode, data)
    }

    private suspend fun handleIncomingIntent(intent: Intent?) {
        if (intent?.action?.contains("com.haruma.sen.environment") == true) {
            val resources = intent.getParcelableArrayListExtra<Uri>("resources")
            this.handleReceivedResources(resources)
        }
    }

    private fun handleReceivedResources(resources: List<Uri>?) {
        val result = resources?.map { uri ->
            this.storageIntentHandler.resolveUri(uri)
        } ?: emptyList()
        MethodChannel(flutterEngine!!.dartExecutor.binaryMessenger, this.channel)
            .invokeMethod("onResourcesReceived", result)
    }


    override fun onRequestPermissionsResult(
        requestCode: Int, 
        permissions: Array<out String>, 
        grantResults: IntArray
    ): Unit {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        storageIntentHandler.handleRequestPermissionsResult(requestCode)
    }

    private fun requestNotificationPermission(

    ): Unit {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            if (ContextCompat.checkSelfPermission(
                    this,
                    Manifest.permission.POST_NOTIFICATIONS
                ) != PackageManager.PERMISSION_GRANTED
            ) {
                ActivityCompat.requestPermissions(
                    this,
                    arrayOf(Manifest.permission.POST_NOTIFICATIONS),
                    NOTIFICATION_PERMISSION_CODE
                )
            }
        }
    }
}
