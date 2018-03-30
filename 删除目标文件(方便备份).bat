rd /Q /S Project\MDK\NorFlash
rd /Q /S Project\MDK\SDRAM
del /Q Project\MDK\*.bak
del /Q Project\MDK\*.dep
del /Q Project\MDK\JLink*
del /Q Project\MDK\*.uvgui.*

::rd 删除目录
::del 删除文件
::参数:
::/S      除目录本身外，还将删除指定目录下的所有子目录和
::       文件。用于删除目录树。
::/Q      安静模式，带 /S 删除目录树时不要求确认

::	--舒来