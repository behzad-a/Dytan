set PINTOOL=%USERPROFILE%\Projects\Research\ConfigAPI\pin-2.10+dytan32\pin-2.10-45467-msvc8-ia32_intel64-windows
set DYTAN=%PINTOOL%\source\tools\dytan
%PINTOOL%\ia32\bin\pin -t %DYTAN%\obj-ia32\dytan.dll -- %1 %2 %3 %4 %5


