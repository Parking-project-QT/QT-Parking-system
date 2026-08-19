$ErrorActionPreference = "Stop"

$installRoot = Join-Path $env:LOCALAPPDATA "QT-Parking-system"
$archive = Join-Path $PSScriptRoot "payload.zip"
$app = Join-Path $installRoot "qt-parking\build\release\qt_parking.exe"

New-Item -ItemType Directory -Force -Path $installRoot | Out-Null
Expand-Archive -LiteralPath $archive -DestinationPath $installRoot -Force

if (-not (Test-Path -LiteralPath $app)) {
    throw "Installed application was not found: $app"
}

$desktop = [Environment]::GetFolderPath("Desktop")
if ([string]::IsNullOrWhiteSpace($desktop) -or -not (Test-Path -LiteralPath $desktop)) {
    $oneDriveDesktop = if ($env:OneDrive) { Join-Path $env:OneDrive "Desktop" } else { "" }
    $desktop = if ($oneDriveDesktop -and (Test-Path -LiteralPath $oneDriveDesktop)) {
        $oneDriveDesktop
    } else {
        Join-Path $env:USERPROFILE "Desktop"
    }
    New-Item -ItemType Directory -Force -Path $desktop | Out-Null
}
$shortcut = Join-Path $desktop "QT Parking.lnk"
$shell = New-Object -ComObject WScript.Shell
$link = $shell.CreateShortcut($shortcut)
$link.TargetPath = $app
$link.WorkingDirectory = Split-Path $app
$link.Save()

Start-Process -FilePath $app -WorkingDirectory (Split-Path $app)
