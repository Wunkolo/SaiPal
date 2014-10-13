#pragma once

//config options varying between sai versions;

#if defined(SAI120)
#define VersionStr "1.2.0"
#define SESSION 0x49098C
#elif defined(SAI110)
#define VersionStr "1.1.0"
#define SESSION 0x4BB810
#else
#define VersionStr "1.1.0"
#define SESSION 0x4BB810
#endif
