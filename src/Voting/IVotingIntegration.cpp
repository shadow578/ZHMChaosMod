#include "IVotingIntegration.h"

#include <typeinfo>

std::string IVotingIntegration::GetName() const
{
    std::string s_Name = typeid(*this).name();

    if (s_Name.find("class ") != std::string::npos)
    {
        s_Name.erase(0, 6 /* "class ".length() */);
    }

    return s_Name;
}
