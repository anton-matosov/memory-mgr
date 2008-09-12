::echo off
setlocal ENABLEEXTENSIONS 

set file_to_check=%cd%\..\memory-mgr\segment_manager.h
set boost_path=C:\Boost\include\boost-1_35

bcp --boost=%boost_path% test utility "%cd%"
::bcp --boost=%boost_path% --scan "%file_to_check%" "%cd%"


pause
endlocal
