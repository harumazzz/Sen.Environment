package com.haruma.sen.environment.utility

import java.io.File
import java.io.FileInputStream
import java.io.FileOutputStream
import java.io.IOException
import java.io.InputStream
import java.util.zip.ZipEntry
import java.util.zip.ZipInputStream
import android.os.Environment

class ZipHelper {

    public fun unzip (
        source: String,
        destination: String,
    ): Boolean
    {
        try {
            val zipFile = File(source)
            val destDir = File(destination)
            if (!destDir.exists()) {
                destDir.mkdirs()
            }
            val zipInputStream = ZipInputStream(FileInputStream(zipFile))
            var zipEntry: ZipEntry? = null
            while (zipInputStream.nextEntry.also { zipEntry = it } != null) {
                val fileName = zipEntry!!.name
                val newFile = File(destDir, fileName)
                if (zipEntry!!.isDirectory) {
                    newFile.mkdirs()
                } else {
                    val parentFile = newFile.parentFile
                    if (!parentFile.exists()) {
                        parentFile.mkdirs()
                    }
                    val fileOutputStream = FileOutputStream(newFile)
                    val buffer = ByteArray(1024)
                    var length: Int
                    while (zipInputStream.read(buffer).also { length = it } != -1) {
                        fileOutputStream.write(buffer, 0, length)
                    }
                    fileOutputStream.close()
                }
                zipInputStream.closeEntry()
            }
            zipInputStream.close()
            return true
        } catch (e: IOException) {
            return false
        }
    }

}