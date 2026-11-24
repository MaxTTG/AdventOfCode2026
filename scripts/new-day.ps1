param(
    [Parameter(Mandatory = $true)]
    [ValidateRange(1, 25)]
    [int]$Day
)

$ErrorActionPreference = 'Stop'

$root = Split-Path -Parent $PSScriptRoot
$dayName = '{0:D2}' -f $Day
$sourceRelativePath = "days/day_$dayName.cpp"
$inputRelativePath = "inputs/day_$dayName.txt"
$sourcePath = Join-Path $root $sourceRelativePath
$inputPath = Join-Path $root $inputRelativePath
$cmakePath = Join-Path $root 'CMakeLists.txt'

if (Test-Path $sourcePath) {
    throw "Solver already exists: $sourceRelativePath"
}
if (Test-Path $inputPath) {
    throw "Input file already exists: $inputRelativePath"
}

$cmakeContent = [System.IO.File]::ReadAllText($cmakePath)
$marker = '# END AOC DAY SOURCES'
if (-not $cmakeContent.Contains($marker)) {
    throw "Could not find the CMake day-source marker in CMakeLists.txt."
}

$template = @'
#include <istream>

#include "aoc/registry.hpp"
#include "aoc/solution.hpp"

namespace {

std::string part1(std::istream& input) {
    (void)input;
    return "TODO";
}

std::string part2(std::istream& input) {
    (void)input;
    return "TODO";
}

aoc::Solution solve_day___DAY__(std::istream& input) {
    return aoc::run_parts(input, part1, part2);
}

AOC_REGISTER_DAY(__DAY_NUMBER__, solve_day___DAY__)

}  // namespace
'@
$sourceContent = $template.Replace('__DAY__', $dayName).Replace('__DAY_NUMBER__', [string]$Day)

$daysDirectory = Join-Path $root 'days'
$inputsDirectory = Join-Path $root 'inputs'
New-Item -ItemType Directory -Force -Path $daysDirectory, $inputsDirectory | Out-Null
[System.IO.File]::WriteAllText(
    $sourcePath,
    $sourceContent,
    [System.Text.UTF8Encoding]::new($false)
)
[System.IO.File]::WriteAllText(
    $inputPath,
    '',
    [System.Text.UTF8Encoding]::new($false)
)

$updatedCmakeContent = $cmakeContent.Replace(
    $marker,
    "$sourceRelativePath`r`n$marker"
)
[System.IO.File]::WriteAllText(
    $cmakePath,
    $updatedCmakeContent,
    [System.Text.UTF8Encoding]::new($false)
)

Write-Output "Created $sourceRelativePath and $inputRelativePath."
Write-Output 'Reconfigure and build with: cmake -S . -B build; cmake --build build'
