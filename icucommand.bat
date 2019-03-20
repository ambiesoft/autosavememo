@echo off
set PATH=%~dp0icu\icu4c\bin;%PATH%

subst I: %~dp0icu
echo directory 'icu' is mouted to 'I:\' because the build may be failed due to long path.
echo run 'subst I: /D' to unmount.

cmd /k
