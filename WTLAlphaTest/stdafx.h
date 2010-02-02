// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <atlbase.h>
#include <atlwin.h>
#include <atltypes.h>

#include <uianimation.h>
#include <d2d1.h>
#include <dwrite.h>
#include <Wincodec.h>
#include <gdiplus.h>

#include <algorithm>
#undef min
#undef max
#define max(a,b) std::max(a,b)
#define min(a,b) std::min(a,b)

#define _SECURE_ATL 1
#define _WTL_NO_AUTOMATIC_NAMESPACE
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_USE_DDX_FLOAT

#include "atlbase.h"
#include "atlapp.h"
#include "atlwin.h"
#include "atlcrack.h"
#include "atlmisc.h"
#include "atlgdi.h"
#include "atlframe.h"
#include "atlctrls.h"
#include "atlddx.h"

#undef min
#undef max


#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include <string>
#include <vector>
#include <stdlib.h>