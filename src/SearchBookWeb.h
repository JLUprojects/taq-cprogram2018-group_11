#pragma once

#include "Library.h"

int SearchBookByKey(OUT LibraryBooks& rBooks, IN CString& rstrKey);
int SearchBookByURI(IN OUT CBook& rBook);
