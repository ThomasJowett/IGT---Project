#include "iComponents.h"

const std::size_t Component::Type = std::hash<std::string>()(TO_STRING(Component));

CLASS_DEFINITION(Component, iRenderable)
CLASS_DEFINITION(Component, iUpdateable)