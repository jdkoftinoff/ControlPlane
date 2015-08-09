#include "ControlPlane/World.hpp"
#include "ControlPlane/RangedValue.hpp"

using namespace ControlPlane;

#define TEST( testname, func, expected )                                                                                       \
    do                                                                                                                         \
    {                                                                                                                          \
        bool e = ( func == expected );                                                                                         \
        r &= e;                                                                                                                \
        std::cout << testname << " : " << #func << " : " << ( e ? "PASS" : "FAIL" ) << std::endl;                              \
    } while ( false )

int main()
{
    bool r = true;

    return r == true ? 0 : 255;
}
