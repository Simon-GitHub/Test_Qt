@echo off
setlocal enabledelayedexpansion  

REM 删除当前文件夹及子文件夹下所有的Debug文件夹（跳过./depend文件夹）
for /d %%a in (./*) do (
	set var=%%a
	REM 由于depend下boost/include中有以debug命名的源文件夹，因此要排除depend目录
	if not !var!==depend (
		REM echo !var!
		for /r ./!var! %%a in (Debug) do (  
			REM 在循环中排除含有depend的路径，此种方法会使得循环把每个路径都遍历一次，耗费时间
			REM set var2=false
			REM echo %%a|findstr "depend" >nul && set var2=true
			REM if !var2!==false (
			if exist %%a (
				echo "delete" %%a
				rd /s /q "%%a" 
			)
			REM )
		)
	)
	REM 删除是debug的目录(/i不区分大小写)
	if /i !var!==debug (
		echo "delete" !var!
		rd /s /q !var!
	)
)

for /r . %%a in (Release) do (  
	if exist %%a (
		echo "delete" %%a
		rd /s /q "%%a" 
	)
)

for /r . %%a in (ipch) do (  
	if exist %%a (
		echo "delete" %%a
		rd /s /q "%%a" 
	)
)

for /r . %%a in (GeneratedFiles) do (  
	if exist %%a (
		echo "delete" %%a
		rd /s /q "%%a" 
	)
)

REM 下面的for循环删除不了隐藏文件，因此隐藏的*.suo文件删除不了
for /r . %%a in (*.sln) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (*.suo) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (*.sdf) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (*.vcxproj.user) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (*.vcxproj.filters) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (*.vcxproj) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (*.stash) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)


pause