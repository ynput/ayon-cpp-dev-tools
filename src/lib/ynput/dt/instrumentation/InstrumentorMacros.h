#ifndef AYONCPPAPIINSTRUMENTORMACRO_H
#define AYONCPPAPIINSTRUMENTORMACRO_H

#include "Instrumentor.h"

#ifdef AYON_INSTRUMENT
    #define InstrumentFunc(name, cat, ...) InstrumentationTimer timer(name, cat, __VA_ARGS__);
#else
    #define InstrumentFunc(name, cat, ...)
#endif   // DEBUG

#endif   // !AYONCPPAPIINSTRUMENTOR_H
