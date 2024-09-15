xcopy		/y		.\Engine\Public\*.h			.\Import\Engine\Header\

xcopy		/y		.\Engine\Binary\debug\Engine.lib	.\Import\Engine\Library\debug\
xcopy		/y		.\Engine\Binary\debug\Engine.dll	.\Client\Binary\debug\
xcopy		/y		.\Engine\Binary\debug\Engine.dll	.\ToolEffect\Binary\
xcopy		/y		.\Engine\Binary\debug\Engine.dll	.\ToolUI\Binary\
xcopy		/y		.\Engine\Binary\debug\Engine.dll	.\ToolMap\Binary\
xcopy		/y		.\Engine\Binary\debug\Engine.dll	.\ToolAnimation\Binary\

xcopy		/y		.\Engine\Binary\release\Engine.lib	.\Import\Engine\Library\release\
xcopy		/y		.\Engine\Binary\release\Engine.dll	.\Client\Binary\release\