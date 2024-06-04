#pragma once

#pragma warning (disable : 4251)
#pragma error (disable : E0410)

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <list>
#include <map>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <unordered_set>
#include "mutex"
#include "unordered_map"
using namespace std;

#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_TypeDef.h"
#include "Engine_Function.h"
using namespace Engine;


#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

