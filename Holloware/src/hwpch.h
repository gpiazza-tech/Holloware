#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <fstream>
#include <cstring>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <nlohmann/json.hpp>

#include "Holloware/Core/Input.h"
#include "Holloware/Core/KeyCodes.h"
#include "Holloware/Core/MouseButtonCodes.h"

#include "Holloware/Core/Log.h"

#include "Holloware/Debug/Instrumentor.h"

#ifdef HW_PLATFORM_WINDOWS
	#include <Windows.h>
#endif