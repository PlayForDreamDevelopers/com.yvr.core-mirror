set buildTarget= %1

echo %buildTarget%

if %buildTarget% ==build goto BuildDocfx
if %buildTarget% ==clean goto Clean

:BuildDocfx
docfx .\Documentation~\docfx.json --serve
goto End

:Clean
rmdir /s /q .\Documentation~\obj
rmdir /s /q .\Documentation~\_site
goto End

:End