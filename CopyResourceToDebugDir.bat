@echo on
color 2f
mode con: cols=80 lines=25
@echo ����׼��������Դ�ļ���DebugĿ¼�����Ժ�...
@echo off
set CUR_DIR=%cd%
set DEST_DIR=%cd%\proj.win32\Debug.win32
set SRC_DIR=%cd%\Resources
if not exist %DEST_DIR% md %DEST_DIR%
xcopy %SRC_DIR% %DEST_DIR% /c/e/q 

@echo ������ϣ�����
pause