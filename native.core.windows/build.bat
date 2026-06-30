devenv "./native.core.windows/native.core.windows.sln" /Rebuild Release /Out buildlog.log

copy .\native.core.windows\x64\Release\native.core.windows.dll ..\com.yvr.core\Plugins\Windows\yvrplugin.dll

pause