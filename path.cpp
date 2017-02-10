#include "path.h"
QString Path::mWorkingDir= QDir::homePath()+"/."+QApplication::applicationName();
bool Path::mCopy=false;
