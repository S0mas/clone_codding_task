#pragma once

#include <half.hpp>

#include <sstream>

struct State
{
    half_float::half pressure_{123.4};
    half_float::half temperature_{567.8};
    half_float::half velocity_{999.9};
};

inline auto stateToString(const State& state) -> std::string
{
    std::stringstream stream;
    stream << '$';
    stream << state.pressure_;
    stream << ",";
    stream << state.temperature_;
    stream << ",";
    stream << state.velocity_;
    return stream.str();
}
