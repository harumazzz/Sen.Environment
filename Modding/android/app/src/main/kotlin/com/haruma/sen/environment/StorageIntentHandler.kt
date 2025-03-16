package com.haruma.sen.environment

import android.Manifest
import android.app.Activity
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.os.Build
import android.os.Environment
import android.provider.DocumentsContract
import android.provider.Settings
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.runBlocking
import java.io.File

class StorageIntentHandler(private val activity: Activity) {

    companion object {

        private const val REQUEST_PICK_STORAGE_ITEM = 1001

        private const val REQUEST_REQUEST_EXTERNAL_STORAGE_PERMISSION = 1002

    }

    private val continuation = Channel<Any?>()

    suspend fun handleMethodCall(
        call: MethodCall, 
        result: MethodChannel.Result
    ): Unit {
        try {
            when (call.method) {
                "pick_file" -> result.success(pickStorageFileFromDocument(call.argument("initialDirectory")))
                "pick_directory" -> result.success(pickDirectoryFromDocument(call.argument("initialDirectory")))
                "save_file" -> result.success(this.pickSaveFileFromDocument())
                "request_storage_permission" -> result.success(this.requestStoragePermission())
                "check_storage_permission" -> result.success(this.checkStoragePermission())
                else -> result.notImplemented()
            }
        } catch (e: Exception) {
            result.error("ERROR", e.stackTraceToString(), null)
        }
    }

    fun handleActivityResult(
        requestCode: Int,
        data: Intent?
    ): Unit {
        when (requestCode) {
            REQUEST_PICK_STORAGE_ITEM, REQUEST_REQUEST_EXTERNAL_STORAGE_PERMISSION -> {
                runBlocking { continuation.send(data?.data) }
            }
        }
    }

    fun handleRequestPermissionsResult(
        requestCode: Int
    ): Unit {
        if (requestCode == REQUEST_REQUEST_EXTERNAL_STORAGE_PERMISSION) {
            runBlocking { continuation.send(null) }
        }
    }

    private suspend fun pickStorageFileFromDocument(
        initialDirectory: String?
    ): String? {
        val intent = Intent(Intent.ACTION_OPEN_DOCUMENT).apply {
            addCategory(Intent.CATEGORY_OPENABLE)
            type = "*/*"
            putExtra(Intent.EXTRA_LOCAL_ONLY, true)
            putExtra(DocumentsContract.EXTRA_INITIAL_URI, getInitialUri(initialDirectory))
        }
        activity.startActivityForResult(intent, REQUEST_PICK_STORAGE_ITEM)
        return (continuation.receive() as? Uri)?.let { this.resolveUri(it) }
    }

    private suspend fun pickSaveFileFromDocument(

    ): String? {
        val intent = Intent(Intent.ACTION_CREATE_DOCUMENT).apply {
            addCategory(Intent.CATEGORY_OPENABLE)
            type = "*/*"
            putExtra(Intent.EXTRA_LOCAL_ONLY, true)
        }
        activity.startActivityForResult(intent, REQUEST_PICK_STORAGE_ITEM)
        return (continuation.receive() as? Uri)?.let { this.resolveUri(it) }
    }

    private suspend fun pickDirectoryFromDocument(
        initialDirectory: String?
    ): String? {
        val intent = Intent(Intent.ACTION_OPEN_DOCUMENT_TREE).apply {
            putExtra(Intent.EXTRA_LOCAL_ONLY, true)
            putExtra(DocumentsContract.EXTRA_INITIAL_URI, getInitialUri(initialDirectory))
        }
        activity.startActivityForResult(intent, REQUEST_PICK_STORAGE_ITEM)
        return (continuation.receive() as? Uri)?.let { this.resolveUri(it) }
    }

    private suspend fun requestStoragePermission(

    ): Boolean {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.R) {
            activity.requestPermissions(
                arrayOf(Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE),
                REQUEST_REQUEST_EXTERNAL_STORAGE_PERMISSION
            )
        } else {
            val intent = Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION, Uri.parse("package:${activity.packageName}"))
            activity.startActivityForResult(intent, REQUEST_REQUEST_EXTERNAL_STORAGE_PERMISSION)
        }
        continuation.receive()
        return checkStoragePermission()
    }

    private fun checkStoragePermission(

    ): Boolean {
        return if (Build.VERSION.SDK_INT < Build.VERSION_CODES.R) {
            activity.checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED &&
                    activity.checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED
        } else {
            Environment.isExternalStorageManager()
        }
    }

    private fun getInitialUri(
        initialDirectory: String?
    ): Uri {
        val primaryDirectory = Environment.getExternalStorageDirectory().absolutePath + "/"
        val safeDirectory = if (initialDirectory?.startsWith(primaryDirectory) == true) {
            initialDirectory.substring(primaryDirectory.length)
        } else {
            ""
        }
        return Uri.parse("content://com.android.externalstorage.documents/document/primary%3A${Uri.encode(safeDirectory)}")
    }

    public suspend fun resolveUri(uri: Uri): String? {
        var result: String? = null
        val provider = uri.authority
        var path = uri.path?.let { Uri.decode(it) }
        when (provider) {
            // AOSP DocumentsUI
            "com.android.externalstorage.documents" -> {
                // /document/primary:<path-relative-external-storage>
                if (path?.startsWith("/document/primary:") == true) {
                    result = path.substring("/document/primary:".length)
                    result = "${queryExternalStoragePath()}${if (result.isNullOrEmpty()) "" else "/$result"}"
                }
                // /tree/primary:<path-relative-external-storage>
                if (path?.startsWith("/tree/primary:") == true) {
                    result = path.substring("/tree/primary:".length)
                    result = "${queryExternalStoragePath()}${if (result.isNullOrEmpty()) "" else "/$result"}"
                }
            }
            // Material Files
            "me.zhanghai.android.files.file_provider" -> {
                path = Uri.decode(path ?: "")
                // /file://<path-absolute>
                if (path?.startsWith("/file://") == true) {
                    result = Uri.parse(path.substring(1)).path
                }
            }
            // Root Explorer
            "com.speedsoftware.rootexplorer.fileprovider" -> {
                // /root/<path-relative-root>
                if (path?.startsWith("/root/") == true) {
                    result = path.substring("/root".length)
                }
            }
            // Solid Explorer
            "pl.solidexplorer2.files" -> {
                result = path
            }
            // MT Manager
            "bin.mt.plus.fp" -> {
                result = path
            }
            // NMM
            "in.mfile.files" -> {
                result = path
            }
            else -> {
                if (path != null && path.startsWith("/") && exist(path)) {
                    result = path
                }
            }
        }

        return result
    }

    private fun queryExternalStoragePath(): String {
        return Environment.getExternalStorageDirectory().absolutePath
    }

    private fun exist(path: String): Boolean {
        val file = File(path)
        return file.exists()
    }
}
