#pragma once

#include <Eigen/Dense>

#include <vector>
#include <cstdint>
#include <span>
#include <array>
#include <type_traits>
#include <iostream>
#include <algorithm>

#include <Fastor/Fastor.h>
#include <cblas.h>

#ifndef COESIUS_LIB_H
#define COESIUS_LIB_H

#define NAMESPACE_COESIUS Coesius

#include <coesius/dev/general.ipp>

namespace NAMESPACE_COESIUS { // Start namespace scope

namespace Internal { 
    #include <coesius/dev/internal.ipp> // There's so much I'm not stuffing it all into this header}
}

} // Namespace end scope

#endif // No more Coesius_lib