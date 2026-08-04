pushd ..\
if exist "Perplex.sln" del "Perplex.sln"
set /p gameDir="Enter game directory: "
call vendor\premake\premake5.exe vs2022 --gameDir=%gameDir%
popd
PAUSE