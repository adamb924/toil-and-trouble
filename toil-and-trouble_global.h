#ifndef TOILANDTROUBLE_GLOBAL_H
#define TOILANDTROUBLE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TOILANDTROUBLE_LIBRARY)
#  define TOILANDTROUBLE_EXPORT Q_DECL_EXPORT
#else
#  define TOILANDTROUBLE_EXPORT Q_DECL_IMPORT
#endif

#endif // TOILANDTROUBLE_GLOBAL_H