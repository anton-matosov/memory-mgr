::echo off
setlocal ENABLEEXTENSIONS 

set file_to_check=%cd%\..\detail\iterator.hpp
set boost_path=E:\projects\Library\boost_1_37_0

::bcp --boost=%boost_path% --scan "%file_to_check%" "%cd%"

::bcp --boost=%boost_path% bind test mpl tuple concept_check config type_traits "%cd%"
bcp --boost=%boost_path% iterator test foreach bind "%cd%"


pause
endlocal
