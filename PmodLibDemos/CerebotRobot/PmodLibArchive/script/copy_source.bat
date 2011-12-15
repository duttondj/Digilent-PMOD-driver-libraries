echo "Running" >>  c:\code\test.txt
del /f /q ..\PmodLibSrc\*
xcopy /y /e ..\..\PmodLib\PmodACL\*.* ..\PmodLibSrc\PmodACL\
xcopy /y /e ..\..\PmodLib\PmodCommon\*.* ..\PmodLibSrc\PmodCommon\
xcopy /y /e ..\..\PmodLib\PmodHB5\*.* ..\PmodLibSrc\PmodHB5\
xcopy /y /e ..\..\PmodLib\PmodJSTK\*.* ..\PmodLibSrc\PmodJSTK\
xcopy /y /e ..\..\PmodLib\PmodMic\*.* ..\PmodLibSrc\PmodMic\
xcopy /y /e ..\..\PmodLib\PmodSF\*.* ..\PmodLibSrc\PmodSF\
xcopy /y /e ..\..\PmodLib\PmodDA2\*.* ..\PmodLibSrc\PmodDA2\

del /f /q ..\PmodLib\*
xcopy /y /e ..\..\PmodLib\PmodACL\*.h ..\PmodLib\include\PmodACL\
xcopy /y /e ..\..\PmodLib\PmodCommon\*.h ..\PmodLib\include\PmodCommon\
xcopy /y /e ..\..\PmodLib\PmodHB5\*.h ..\PmodLib\include\PmodHB5\
xcopy /y /e ..\..\PmodLib\PmodJSTK\*.h ..\PmodLib\include\PmodJSTK\
xcopy /y /e ..\..\PmodLib\PmodMic\*.h ..\PmodLib\include\PmodMic\
xcopy /y /e ..\..\PmodLib\PmodSF\*.h ..\PmodLib\include\PmodSF\
xcopy /y /e ..\..\PmodLib\PmodDA2\*.h ..\PmodLib\include\PmodDA2\