param(
    [string]$OutputName = "QT-Parking-Setup.exe"
)

$ErrorActionPreference = "Stop"

$workspace = Split-Path $PSScriptRoot -Parent
$dist = Join-Path $workspace "dist"
$payload = Join-Path $dist "payload"
$installerInput = Join-Path $dist "installer-input"
$qtBin = "C:\msys64\ucrt64\bin"
$windeployqt = Join-Path $qtBin "windeployqt.exe"
$objdump = Join-Path $qtBin "objdump.exe"
$iexpress = "C:\Windows\System32\iexpress.exe"
$builtExe = Join-Path $workspace "qt-parking\build\release\qt_parking.exe"
$model = Join-Path $workspace "qt-parking\model\mnist_model.onnx"
$installer = Join-Path $dist $OutputName

foreach ($required in @($windeployqt, $objdump, $iexpress, $builtExe, $model)) {
    if (-not (Test-Path -LiteralPath $required)) {
        throw "Required file not found: $required"
    }
}

New-Item -ItemType Directory -Force -Path $dist | Out-Null

function Reset-DistDirectory([string]$path) {
    $distPrefix = [IO.Path]::GetFullPath($dist).TrimEnd('\') + '\'
    $fullPath = [IO.Path]::GetFullPath($path)
    if (-not $fullPath.StartsWith($distPrefix, [StringComparison]::OrdinalIgnoreCase)) {
        throw "Refusing to reset a path outside dist: $fullPath"
    }
    if (Test-Path -LiteralPath $fullPath) {
        Remove-Item -LiteralPath $fullPath -Recurse -Force
    }
    New-Item -ItemType Directory -Force -Path $fullPath | Out-Null
}

Reset-DistDirectory $payload
Reset-DistDirectory $installerInput

$releaseDir = Join-Path $payload "qt-parking\build\release"
$modelDir = Join-Path $payload "qt-parking\model"
New-Item -ItemType Directory -Force -Path $releaseDir, $modelDir | Out-Null

$stagedExe = Join-Path $releaseDir "qt_parking.exe"
Copy-Item -LiteralPath $builtExe -Destination $stagedExe
Copy-Item -LiteralPath $model -Destination (Join-Path $modelDir "mnist_model.onnx")

& $windeployqt --release --compiler-runtime --no-translations --dir $releaseDir $stagedExe
if ($LASTEXITCODE -ne 0) {
    throw "windeployqt failed with exit code $LASTEXITCODE"
}

function Get-ImportedDlls([string]$file) {
    & $objdump -p $file 2>$null | ForEach-Object {
        if ($_ -match '^\s*DLL Name:\s*(.+?)\s*$') {
            $matches[1]
        }
    }
}

# windeployqt covers Qt. Recursively copy only the MSYS2/OpenCV DLLs that
# the executable, Qt plugins, and their newly copied dependencies import.
$processed = [Collections.Generic.HashSet[string]]::new(
    [StringComparer]::OrdinalIgnoreCase)
do {
    $copied = $false
    $peFiles = Get-ChildItem -LiteralPath $releaseDir -Recurse -File |
        Where-Object { $_.Extension -in '.exe', '.dll' }

    foreach ($file in $peFiles) {
        if (-not $processed.Add($file.FullName)) {
            continue
        }
        foreach ($dll in Get-ImportedDlls $file.FullName) {
            $destination = Join-Path $releaseDir $dll
            $source = Join-Path $qtBin $dll
            if (-not (Test-Path -LiteralPath $destination) -and
                (Test-Path -LiteralPath $source)) {
                Copy-Item -LiteralPath $source -Destination $destination
                $copied = $true
            }
        }
    }
} while ($copied)

$missing = [Collections.Generic.HashSet[string]]::new(
    [StringComparer]::OrdinalIgnoreCase)
$peFiles = Get-ChildItem -LiteralPath $releaseDir -Recurse -File |
    Where-Object { $_.Extension -in '.exe', '.dll' }
foreach ($file in $peFiles) {
    foreach ($dll in Get-ImportedDlls $file.FullName) {
        $source = Join-Path $qtBin $dll
        $deployed = Join-Path $releaseDir $dll
        if ((Test-Path -LiteralPath $source) -and
            -not (Test-Path -LiteralPath $deployed)) {
            [void]$missing.Add($dll)
        }
    }
}
if ($missing.Count -ne 0) {
    throw "Missing deployed DLLs: $($missing -join ', ')"
}

$payloadZip = Join-Path $installerInput "payload.zip"
Compress-Archive -Path (Join-Path $payload '*') -DestinationPath $payloadZip `
    -CompressionLevel Optimal -Force
Copy-Item -LiteralPath (Join-Path $PSScriptRoot "install.cmd") `
    -Destination $installerInput
Copy-Item -LiteralPath (Join-Path $PSScriptRoot "install.ps1") `
    -Destination $installerInput

$sedPath = Join-Path $installerInput "installer.sed"
$sourceDir = $installerInput.TrimEnd('\') + '\'
$sed = @"
[Version]
Class=IEXPRESS
SEDVersion=3

[Options]
PackagePurpose=InstallApp
ShowInstallProgramWindow=1
HideExtractAnimation=0
UseLongFileName=1
InsideCompressed=0
CAB_FixedSize=0
CAB_ResvCodeSigning=0
RebootMode=N
InstallPrompt=%InstallPrompt%
DisplayLicense=%DisplayLicense%
FinishMessage=%FinishMessage%
TargetName=%TargetName%
FriendlyName=%FriendlyName%
AppLaunched=%AppLaunched%
PostInstallCmd=<None>
AdminQuietInstCmd=%AppLaunched%
UserQuietInstCmd=%AppLaunched%
SourceFiles=SourceFiles

[Strings]
InstallPrompt=
DisplayLicense=
FinishMessage=
TargetName="$installer"
FriendlyName="QT Parking Setup"
AppLaunched="install.cmd"
FILE0="payload.zip"
FILE1="install.cmd"
FILE2="install.ps1"

[SourceFiles]
SourceFiles0="$sourceDir"

[SourceFiles0]
%FILE0%=
%FILE1%=
%FILE2%=
"@
Set-Content -LiteralPath $sedPath -Value $sed -Encoding Default

if (Test-Path -LiteralPath $installer) {
    Remove-Item -LiteralPath $installer -Force
}
& $iexpress /N /Q $sedPath
for ($attempt = 0; $attempt -lt 10 -and -not (Test-Path -LiteralPath $installer); $attempt++) {
    Start-Sleep -Seconds 1
}
if (-not (Test-Path -LiteralPath $installer) -or (Get-Item -LiteralPath $installer).Length -eq 0) {
    throw "IExpress failed to create $installer"
}

$payloadSize = (Get-ChildItem -LiteralPath $payload -Recurse -File |
    Measure-Object -Property Length -Sum).Sum
$installerInfo = Get-Item -LiteralPath $installer
Write-Host "Payload bytes: $payloadSize"
Write-Host "Installer: $($installerInfo.FullName)"
Write-Host "Installer bytes: $($installerInfo.Length)"
