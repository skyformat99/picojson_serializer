#include <catch.hpp>
#include <limits>

#include "../picojson_serializer.h"
#include "test_helpers.h"

namespace picojson {
    namespace convert {

        template<typename T>
        struct value_converter<T,
                               typename enable_if<std::numeric_limits<T>::is_iec559>::type> {
            static value to_value(T v) {
                return value_converter<double>::to_value(v);
            }
            static void from_value(value const& ov, T& v) {
                double v2(0.);
                value_converter<double>::from_value(ov, v2);
                v = static_cast<T>(v2);
            }
        };

    }
}

TEST_CASE("Non-class type") {

    SECTION("serialization") {

        float f(1.2f);
        picojson::value pv = picojson::convert::to_value(f);
        CHECK(is<double>(pv, 1.2f));

        SECTION("deserialization from value") {
            float f_(0.f);
            picojson::convert::from_value(pv, f_);
            CHECK(f_ == f);

            SECTION("deserialization from string") {
                std::string fs = picojson::convert::to_string(f);
                float f__(0.f);
                picojson::convert::from_string(fs, f__);
                CHECK(f__ == f);
            }
        }

	SECTION("const data") {
            float const fc(2.3f);
            picojson::value pv = picojson::convert::to_value(fc);
            CHECK(is<double>(pv, 2.3f));
        }

    }

    SECTION("should not compile from predicate") {
        wchar_t c(1);
        //picojson::convert::to_value(c);
    }

}
