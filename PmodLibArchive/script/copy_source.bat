REM Copies source files from PmodLibTestDev to PmodLibSrc and PmodLib folder, typical use if post development and test.

del /f /q ..\PmodLibSrc\*
xcopy /y /e ..\PmodLibDevTest\PmodACL\*.* ..\PmodLibSrc\PmodACL\
xcopy /y /e ..\PmodLibDevTest\PmodCommon\*.* ..\PmodLibSrc\PmodCommon\
xcopy /y /e ..\PmodLibDevTest\PmodHB5\*.* ..\PmodLibSrc\PmodHB5\
xcopy /y /e ..\PmodLibDevTest\PmodJSTK\*.* ..\PmodLibSrc\PmodJSTK\
xcopy /y /e ..\PmodLibDevTest\PmodMic\*.* ..\PmodLibSrc\PmodMic\
xcopy /y /e ..\PmodLibDevTest\PmodSF\*.* ..\PmodLibSrc\PmodSF\
xcopy /y /e ..\PmodLibDevTest\PmodDA2\*.* ..\PmodLibSrc\PmodDA2\
xcopy /y /e ..\PmodLibDevTest\pmodlib.h ..\PmodLibSrc\

del /f /q ..\PmodLib\*
xcopy /y /e ..\PmodLibDevTest\PmodACL\*.h ..\PmodLib\include\PmodACL\
xcopy /y /e ..\PmodLibDevTest\PmodCommon\*.h ..\PmodLib\include\PmodCommon\
xcopy /y /e ..\PmodLibDevTest\PmodHB5\*.h ..\PmodLib\include\PmodHB5\
xcopy /y /e ..\PmodLibDevTest\PmodJSTK\*.h ..\PmodLib\include\PmodJSTK\
xcopy /y /e ..\PmodLibDevTest\PmodMic\*.h ..\PmodLib\include\PmodMic\
xcopy /y /e ..\PmodLibDevTest\PmodSF\*.h ..\PmodLib\include\PmodSF\
xcopy /y /e ..\PmodLibDevTest\PmodDA2\*.h ..\PmodLib\include\PmodDA2\
xcopy /y /e ..\PmodLibDevTest\pmodlib.h ..\PmodLibSrc\include\