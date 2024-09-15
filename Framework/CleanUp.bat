@echo off
del /s /q Client\Intermediate\*.*
del /s /q Engine\Intermediate\*.*
del /s /q ToolAnimNotifyExtractor\Intermediate\*.*
del /s /q ToolEffect\Intermediate\*.*
del /s /q ToolFbxBinary\Intermediate\*.*
del /s /q ToolMap\Intermediate\*.*
del /s /q ToolUI\Intermediate\*.*
rmdir /s /q .vs