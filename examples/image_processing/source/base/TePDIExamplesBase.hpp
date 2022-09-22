#ifndef TEPDIEXAMPLESBASE_HPP
  #define TEPDIEXAMPLESBASE_HPP

  #include "TeDefines.h"

  #if TePLATFORM == TePLATFORMCODE_MSWINDOWS
    #define TEPDIEXAMPLESRESPATH "..\\..\\resources\\"
    #define TEPDIEXAMPLESBINPATH "..\\..\\bin\\"
  #elif TePLATFORM == TePLATFORMCODE_LINUX
    #define TEPDIEXAMPLESRESPATH "../resources/"
    #define TEPDIEXAMPLESBINPATH "../bin/"
  #else
    #error "ERROR: Unsupported platform"
  #endif

#endif
