@echo on
color 2f
mode con: cols=80 lines=25
@echo 正在清理VS中间文件，请稍候...
@echo Off 
del /s /a *.txt *.suo *.ncb *.user *.pdb *.idb *.netmodule *.obj *.sdf *.log *.exp *.lastbuildstate *.v11.suo *.tlog *.aps *.ilk 2>nul 
FOR /R . %%d IN (.) DO rd /s /q "%%d\Debug" 2>nul 
FOR /R . %%d IN (.) DO rd /s /q "%%d\Release" 2>nul 
FOR /R . %%d IN (.) DO rd /s /q "%%d\Obj" 2>nul

rem If the Properties directory is empty, remove it 
FOR /R . %%d in (.) do rd /q "%%d\Properties" 2> nul

@echo 清理完毕！！！
@pause