#pragma once

#ifdef _WIN32
#define GUID_WINDOWS
#endif

#ifdef __linux__
#define GUID_LIBUUID
#endif

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "guid.h"