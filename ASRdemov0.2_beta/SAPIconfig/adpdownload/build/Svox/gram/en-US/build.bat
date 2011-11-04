@echo off
echo ################################################################
echo # compile adpdownload.gram  
echo ################################################################
cmd /c D:\SAPI\wrokspace\tools\Engine\svox\tools\Scripts\compile.bat adpdownload

echo .
echo ################################################################
echo # copy#
echo ################################################################
copy /v /y /b D:\SAPI\adpdownload\build\Svox\gram\en-US\adpdownload.voc D:\SAPI\adpdownload\build\Svox\resources\ASR\Grammars\en-US\adpdownload.voc
copy /v /y /b D:\SAPI\adpdownload\build\Svox\gram\en-US\adpdownload.grm D:\SAPI\adpdownload\build\Svox\resources\ASR\Grammars\en-US\adpdownload.grm
echo .
echo finish
