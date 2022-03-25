#include "util.hh"

namespace misc
{
    bool isNumber(std::string& str)
    {
        for (auto c : str)
        {
            if (!isdigit(c))
                return false;
        }
        return true;
    }
} // namespace misc