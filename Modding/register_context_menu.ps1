$InstallPath = Get-AppxPackage -Name "com.haruma.sen.environment" | Select-Object -ExpandProperty InstallLocation

$DllPath = "$InstallPath\explorer.dll"

Start-Process -FilePath "regsvr32.exe" -ArgumentList "/s `"$DllPath`"" -NoNewWindow -Wait

Start-Process -FilePath "$InstallPath\virtual.exe" -NoNewWindow