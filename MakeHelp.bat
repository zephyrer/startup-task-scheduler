@echo off
REM -- 最初に Microsoft Visual C++ で作成した resource.h からマップ ファイルを作成します
echo // MAKEHELP.BAT generated Help Map file.  Used by STASK.HPJ. >"hlp\sTask.hm"
echo. >>"hlp\sTask.hm"
echo // コマンド (ID_* , IDM_*) >>"hlp\sTask.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\sTask.hm"
echo. >>"hlp\sTask.hm"
echo // プロンプト (IDP_*) >>"hlp\sTask.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\sTask.hm"
echo. >>"hlp\sTask.hm"
echo // リソース (IDR_*) >>"hlp\sTask.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\sTask.hm"
echo. >>"hlp\sTask.hm"
echo // ダイアログ (IDD_*) >>"hlp\sTask.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\sTask.hm"
echo. >>"hlp\sTask.hm"
echo // フレーム コントロール (IDW_*) >>"hlp\sTask.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\sTask.hm"
REM -- プロジェクト STASK のヘルプを作成


echo Win32 ﾍﾙﾌﾟ ﾌｧｲﾙのﾋﾞﾙﾄﾞ中
start /wait hcw /C /E /M "hlp\sTask.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\sTask.hlp" goto :Error
if not exist "hlp\sTask.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\sTask.hlp" Debug
if exist Debug\nul copy "hlp\sTask.cnt" Debug
if exist Release\nul copy "hlp\sTask.hlp" Release
if exist Release\nul copy "hlp\sTask.cnt" Release
echo.
goto :done

:Error
echo hlp\sTask.hpj(1) : error: ヘルプファイルを作成する時に問題が発生しました。

:done
echo.
