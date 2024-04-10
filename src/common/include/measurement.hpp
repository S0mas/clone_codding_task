#pragma once

struct Measurement
{
    double pressure_ = 0;
    double temperature_ = 0;
    double velocity_ = 0;
};

inline bool operator!=(const Measurement& lhs, const Measurement& rhs)
{
    return !(lhs.pressure_ == rhs.pressure_ && lhs.temperature_ == rhs.temperature_ && lhs.velocity_ == rhs.velocity_);
}
