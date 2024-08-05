#pragma once

#include <QtCore/qglobal.h>

#ifdef MEMORY_VISUALIZER_LIB
# define MEMORY_VISUALIZER_EXPORT Q_DECL_EXPORT
#else
# define MEMORY_VISUALIZER_EXPORT Q_DECL_IMPORT
#endif
