#pragma once

#include <cassert>

#define CONNECT_CHECK(connection) assert(static_cast<bool>(connection));
