//
// Created by Orik on 20/10/2025.
//

#include <motor/components/Component.h>

using namespace Motor;

bool Component::isEnabled() { return enabled; }
void Component::setEnabled(bool enabled) { this->enabled = enabled; }

