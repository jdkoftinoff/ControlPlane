#include "ControlPlane/World.hpp"
#include "ControlPlane/AvdeccString.hpp"

using namespace ControlPlane;

#define TEST( testname, func, expected )                                                                                       \
    do                                                                                                                         \
    {                                                                                                                          \
        bool e = ( func == expected );                                                                                         \
        r &= e;                                                                                                                \
        std::cout << ( e ? "PASS" : "FAIL" ) << " : " << testname << " : " << #func << std::endl;                              \
    } while ( false )

///
/// \brief test_AvdeccNameString_Constructor
///
/// Test the default constructor of AvdeccNameString
///
/// \return true on pass
///
bool test_AvdeccNameString_Constructor()
{
    bool r;
    AvdeccNameString nm;

    std::cout << nm.get() << std::endl;

    std::string s = nm.get();

    if ( s == std::string( "" ) )
    {
        r = true;
    }
    else
    {
        std::cout << s.length() << std::endl;
        r = false;
    }
    return r;
}

///
/// \brief test_AvdeccNameString_StringConstructor
///
/// Test the string constructor of AvdeccNameString
///
/// \return true on pass
///
bool test_AvdeccNameString_StringConstructor()
{
    bool r;
    AvdeccNameString nm( "Abcdefghijklmnop" );

    std::cout << nm.get() << std::endl;

    std::string s = nm.get();

    if ( s == std::string( "Abcdefghijklmnop" ) )
    {
        r = true;
    }
    else
    {
        std::cout << s.length() << std::endl;
        r = false;
    }
    return r;
}

///
/// \brief test_AvdeccNameString_CopyConstuctor
///
/// Test the copy constructor of the AvdeccNameString
///
/// \return true on pass
///
bool test_AvdeccNameString_CopyConstuctor()
{
    bool r;
    AvdeccNameString nm( "Abcdefghijklmnop" );

    AvdeccNameString nm1 = nm;

    if ( nm1.get() == std::string( "Abcdefghijklmnop" ) )
    {
        r = true;
    }
    else
    {
        r = false;
    }
    return r;
}

///
/// \brief test_AvdeccNameString_Assignment
///
/// Test the assignment operator assigning from another AvdeccNameString
///
/// \return true on pass
///
bool test_AvdeccNameString_Assignment()
{
    bool r;
    AvdeccNameString nm( "Abcdefghijklmnop" );

    AvdeccNameString nm1;

    nm1 = nm;

    if ( nm1.get() == std::string( "Abcdefghijklmnop" ) )
    {
        r = true;
    }
    else
    {
        r = false;
    }
    return r;
}

///
/// \brief test_AvdeccNameString_Assignment_64
///
/// Test the assignment operator with 64 characters.
///
/// \return true on pass
///
bool test_AvdeccNameString_Assignment_64()
{
    bool r;
    AvdeccNameString nm( "1234567890123456789012345678901234567890123456789012345678901234" );

    AvdeccNameString nm1;

    nm1 = nm;

    if ( nm1.get() == std::string( "1234567890123456789012345678901234567890123456789012345678901234" ) )
    {
        r = true;
    }
    else
    {
        r = false;
    }
    return r;
}

///
/// \brief test_AvdeccNameString_Assignment_65
///
/// Test the assignment operator with 65 characters
///
/// \return true on pass with 64 characters copied.
///
bool test_AvdeccNameString_Assignment_65()
{
    bool r;
    AvdeccNameString nm( "12345678901234567890123456789012345678901234567890123456789012345" );

    AvdeccNameString nm1;

    nm1 = nm;

    if ( nm1.get() == std::string( "1234567890123456789012345678901234567890123456789012345678901234" ) )
    {
        r = true;
    }
    else
    {
        r = false;
    }
    return r;
}

int main()
{
    bool r = true;

    TEST( "constructor", test_AvdeccNameString_Constructor(), true );
    TEST( "constructor", test_AvdeccNameString_StringConstructor(), true );
    TEST( "constructor", test_AvdeccNameString_CopyConstuctor(), true );
    TEST( "assignment", test_AvdeccNameString_Assignment(), true );
    TEST( "assignment", test_AvdeccNameString_Assignment_64(), true );
    TEST( "assignment", test_AvdeccNameString_Assignment_65(), true );

    return r == true ? 0 : 255;
}
