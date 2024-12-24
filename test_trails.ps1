
function Write-Yellow($Message) {
    Write-Host $Message -ForegroundColor Yellow
}
function Write-Green($Message) {
    Write-Host $Message -ForegroundColor Green
}
function Write-Red($Message) {
    Write-Host $Message -ForegroundColor Red
}

# Test files
$file1 = "082.txt"
$file2 = "081.txt"
$tempFile1 = "temp_082.txt"
$tempFile2 = "temp_081.txt"

# Ensure trails.exe exists
if (!(Test-Path "./trails.exe")) {
    Write-Red "Error: trails.exe not found. Please build the project before running this script."
    exit 1
}

# Ensure test files exist
if (!(Test-Path $file1) -or !(Test-Path $file2)) {
    Write-Red "Error: Test files $file1 or $file2 are missing!"
    exit 1
}

Write-Yellow "Starting ./trails automated tests..."

# Test 1: Valid input with FILE1
Write-Yellow "Running test 1: Valid input with $file1..."
Start-Process -NoNewWindow -Wait -FilePath "./trails.exe" -ArgumentList "--input=$file1"
Start-Sleep -Seconds 2

# Test 2: Valid input with FILE2
Write-Yellow "Running test 2: Valid input with $file2..."
Start-Process -NoNewWindow -Wait -FilePath "./trails.exe" -ArgumentList "--input=$file2"
Start-Sleep -Seconds 2

# Test 3: Prompt = interactive with FILE1
Write-Yellow "Running test 3: Interactive prompt with $file1..."
"2.5" | Start-Process -NoNewWindow -Wait -FilePath "./trails.exe" -ArgumentList "--input=$file1 --prompt=interactive"
Start-Sleep -Seconds 2

# Test 4: Prompt = interactive with FILE2
Write-Yellow "Running test 4: Interactive prompt with $file2..."
"1.5" | Start-Process -NoNewWindow -Wait -FilePath "./trails.exe" -ArgumentList "--input=$file2 --prompt=interactive"
Start-Sleep -Seconds 2

# Test 5: Prompt disabled with FILE1
Write-Yellow "Running test 5: Disabled prompt with $file1..."
Start-Process -NoNewWindow -Wait -FilePath "./trails.exe" -ArgumentList "--input=$file1 --prompt=disabled"
Start-Sleep -Seconds 2

# Test 6: Prompt disabled with FILE2
Write-Yellow "Running test 6: Disabled prompt with $file2..."
Start-Process -NoNewWindow -Wait -FilePath "./trails.exe" -ArgumentList "--input=$file2 --prompt=disabled"
Start-Sleep -Seconds 2

# Test 7: Mismatched count with FILE1
Write-Yellow "Running test 7: Mismatched count with $file1..."
Copy-Item $file1 $tempFile1
(Get-Content $tempFile1) -replace '^.*$', '4' | Set-Content $tempFile1 
Start-Process -NoNewWindow -Wait -FilePath "./trails.exe" -ArgumentList "--input=$tempFile1"
Remove-Item $tempFile1
Start-Sleep -Seconds 2

# Test 8: Mismatched count with FILE2
Write-Yellow "Running test 8: Mismatched count with $file2..."
Copy-Item $file2 $tempFile2
(Get-Content $tempFile2) -replace '^.*$', '3' | Set-Content $tempFile2 
Start-Process -NoNewWindow -Wait -FilePath "./trails.exe" -ArgumentList "--input=$tempFile2"
Remove-Item $tempFile2
Start-Sleep -Seconds 2

# Test 9: Invalid file structure (missing or invalid trail count)
Write-Yellow "Running test 9: Invalid file structure test..."
"invalid`nRada2 6.19`n" | Out-File -FilePath "invalid_test.txt"
Start-Process -NoNewWindow -Wait -FilePath "./trails.exe" -ArgumentList "--input=invalid_test.txt"
Remove-Item "invalid_test.txt"
Start-Sleep -Seconds 2

# Test 10: Invalid trail data (negative trail length)
Write-Yellow "Running test 10: Invalid trail data test..."
"2`nRada2 -6.19`nMets 4.92`n" | Out-File -FilePath "invalid_trail_data.txt"
Start-Process -NoNewWindow -Wait -FilePath "./trails.exe" -ArgumentList "--input=invalid_trail_data.txt"
Remove-Item "invalid_trail_data.txt"
Start-Sleep -Seconds 2

# Test 11: Empty file
Write-Yellow "Running test 11: Empty file test..."
New-Item -ItemType File -Path "empty_test.txt" | Out-Null
Start-Process -NoNewWindow -Wait -FilePath "./trails.exe" -ArgumentList "--input=empty_test.txt"
Remove-Item "empty_test.txt"
Start-Sleep -Seconds 2

# Test 12: Display help manual --help
Write-Yellow "Running test 12: Displaying help manual (--help)..."
Start-Process -NoNewWindow -Wait -FilePath "./trails.exe" -ArgumentList "--help"
Start-Sleep -Seconds 2

# Test 13: Display help manual -h
Write-Yellow "Running test 13: Displaying help manual (-h)..."
Start-Process -NoNewWindow -Wait -FilePath "./trails.exe" -ArgumentList "-h"
Start-Sleep -Seconds 2

Write-Green "All tests completed successfully."
