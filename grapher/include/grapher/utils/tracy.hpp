#pragma once

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#else
/// Placeholder define for Tracy's ZoneScope
#define ZoneScoped
#endif
