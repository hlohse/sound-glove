#include "Error.h"

Error& Error::instance()
{
    static Error instance;
    return instance;
}

Error::Error()
:   type_(TYPE_NONE),
    info_(no_info)
{
}

bool Error::isNone() const
{
    return type() == TYPE_NONE;
}

void Error::set(const Type type, const int info)
{
    type_ = type;
    info_ = info;
}

Error::Type Error::type() const
{
    return type_;
}

int Error::info() const
{
    return info_;
}
