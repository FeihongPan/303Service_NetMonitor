#ifndef COMMONDLL_GLOBAL_H
#define COMMONDLL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMMONDLL_LIBRARY)
#  define COMMONDLL_EXPORT Q_DECL_EXPORT
#else
#  define COMMONDLL_EXPORT Q_DECL_IMPORT
#endif

#endif // COMMONDLL_GLOBAL_H
