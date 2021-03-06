///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) DreamWorks Animation LLC
//
// All rights reserved. This software is distributed under the
// Mozilla Public License 2.0 ( http://www.mozilla.org/MPL/2.0/ )
//
// Redistributions of source code must retain the above copyright
// and license notice and the following restrictions and disclaimer.
//
// *     Neither the name of DreamWorks Animation nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// IN NO EVENT SHALL THE COPYRIGHT HOLDERS' AND CONTRIBUTORS' AGGREGATE
// LIABILITY FOR ALL CLAIMS REGARDLESS OF THEIR BASIS EXCEED US$250.00.
//
///////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <openvdb/Metadata.h>
#include <openvdb/Types.h>
#include <iostream>
#include <sstream>

// CPPUNIT_TEST_SUITE() invokes CPPUNIT_TESTNAMER_DECL() to generate a suite name
// from the FixtureType.  But if FixtureType is a templated type, the generated name
// can become long and messy.  This macro overrides the normal naming logic,
// instead invoking FixtureType::testSuiteName(), which should be a static member
// function that returns a std::string containing the suite name for the specific
// template instantiation.
#undef CPPUNIT_TESTNAMER_DECL
#define CPPUNIT_TESTNAMER_DECL( variableName, FixtureType ) \
    CPPUNIT_NS::TestNamer variableName( FixtureType::testSuiteName() )

template<typename T>
class TestMetadataIO: public CppUnit::TestCase
{
public:
    static std::string testSuiteName()
    {
        std::string name = openvdb::typeNameAsString<T>();
        if (!name.empty()) name[0] = static_cast<char>(::toupper(name[0]));
        return "TestMetadataIO" + name;
    }

    CPPUNIT_TEST_SUITE(TestMetadataIO);
    CPPUNIT_TEST(test);
    CPPUNIT_TEST(testMultiple);
    CPPUNIT_TEST_SUITE_END();

    void test();
    void testMultiple();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestMetadataIO<int>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestMetadataIO<int64_t>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestMetadataIO<float>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestMetadataIO<double>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestMetadataIO<std::string>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestMetadataIO<openvdb::Vec3R>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestMetadataIO<openvdb::Vec2i>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestMetadataIO<openvdb::Vec4d>);


namespace {

template<typename T> struct Value { static T create(int i) { return T(i); } };

template<> struct Value<std::string> {
    static std::string create(int i) { return "test" + std::to_string(i); }
};

template<typename T> struct Value<openvdb::math::Vec2<T>> {
    using ValueType = openvdb::math::Vec2<T>;
    static ValueType create(int i) { return ValueType(i, i+1); }
};
template<typename T> struct Value<openvdb::math::Vec3<T>> {
    using ValueType = openvdb::math::Vec3<T>;
    static ValueType create(int i) { return ValueType(i, i+1, i+2); }
};
template<typename T> struct Value<openvdb::math::Vec4<T>> {
    using ValueType = openvdb::math::Vec4<T>;
    static ValueType create(int i) { return ValueType(i, i+1, i+2, i+3); }
};

}


template<typename T>
void
TestMetadataIO<T>::test()
{
    using namespace openvdb;

    const T val = Value<T>::create(1);
    TypedMetadata<T> m(val);

    std::ostringstream ostr(std::ios_base::binary);

    m.write(ostr);

    std::istringstream istr(ostr.str(), std::ios_base::binary);

    TypedMetadata<T> tm;
    tm.read(istr);

    OPENVDB_NO_FP_EQUALITY_WARNING_BEGIN

    CPPUNIT_ASSERT_EQUAL(val, tm.value());

    OPENVDB_NO_FP_EQUALITY_WARNING_END
}


template<typename T>
void
TestMetadataIO<T>::testMultiple()
{
    using namespace openvdb;

    const T val1 = Value<T>::create(1), val2 = Value<T>::create(2);
    TypedMetadata<T> m1(val1);
    TypedMetadata<T> m2(val2);

    std::ostringstream ostr(std::ios_base::binary);

    m1.write(ostr);
    m2.write(ostr);

    std::istringstream istr(ostr.str(), std::ios_base::binary);

    TypedMetadata<T> tm1, tm2;
    tm1.read(istr);
    tm2.read(istr);

    OPENVDB_NO_FP_EQUALITY_WARNING_BEGIN

    CPPUNIT_ASSERT_EQUAL(val1, tm1.value());
    CPPUNIT_ASSERT_EQUAL(val2, tm2.value());

    OPENVDB_NO_FP_EQUALITY_WARNING_END
}

// Copyright (c) DreamWorks Animation LLC
// All rights reserved. This software is distributed under the
// Mozilla Public License 2.0 ( http://www.mozilla.org/MPL/2.0/ )
