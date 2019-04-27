if not exist %~dp0prepare.bat (
  echo prepare.bat not exist. Copy prepare.bat.sample and edit it.
  goto :error
)
call %~dp0prepare.bat


cd /D %QTROOT%\Tools\%QTMINGBIN%
cmd /k