coSetProjectDefaults("pattern")

prebuildcommands{"$(OutputPath)prebuild.exe '../../../src' '../../gen' 'pattern' 'pattern/pch.h''"}

--"J:\CODE\core\src" "J:\CODE\core\build\gen" "pattern" "pattern/pch.h"
--start $(OutputPath)prebuild_d.exe '../../src' '../gen' 'pattern' 'pattern/pch.h'