#pragma once

#include <half.hpp>

struct State
{
    half_float::half pressure_{123.4};
    half_float::half temperature_{567.8};
    half_float::half velocity_{999.9};
};
