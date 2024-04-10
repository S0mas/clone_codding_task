#pragma once

struct Configuration
{
    int frequency_ = 0;
    bool debug_ = false;
};

inline bool operator!=(const Configuration& lhs, const Configuration& rhs)
{
    return !(lhs.frequency_ == rhs.frequency_ && lhs.debug_ == rhs.debug_);
}
