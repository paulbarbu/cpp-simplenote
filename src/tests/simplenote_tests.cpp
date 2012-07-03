#include <cxxtest/TestSuite.h>

#include "../includes/simplenote.hpp"

class SimpleNoteTester : public CxxTest::TestSuite{
public:
    void testM(){
        TS_ASSERT_THROWS(Simplenote a("", ""), AuthenticationError);
    }
};