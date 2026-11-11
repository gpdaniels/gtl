/*
Copyright (C) 2018-2024 Geoffrey Daniels. https://gpdaniels.com/

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 of the License only.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <testbench/main.tests.hpp>

#include <testbench/comparison.tests.hpp>
#include <testbench/optimise.tests.hpp>
#include <testbench/require.tests.hpp>
#include <testbench/template.tests.hpp>

#include <container/static_variant>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(static_variant, traits, standard) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type_1) -> void {
            using type1 = typename decltype(test_type_1)::type;
            REQUIRE((std::is_pod<gtl::static_variant<type1>>::value == false), "Expected std::is_pod to be false.");
            REQUIRE((std::is_trivial<gtl::static_variant<type1>>::value == false), "Expected std::is_trivial to be false.");
            REQUIRE((std::is_trivially_copyable<gtl::static_variant<type1>>::value == false), "Expected std::is_trivially_copyable to be false.");
            REQUIRE((std::is_standard_layout<gtl::static_variant<type1>>::value == true), "Expected std::is_standard_layout to be true.");
            testbench::test_template<testbench::test_types>(
                [](auto test_type_2) -> void {
                    using type2 = typename decltype(test_type_2)::type;
                    REQUIRE((std::is_pod<gtl::static_variant<type1, type2>>::value == false), "Expected std::is_pod to be false.");
                    REQUIRE((std::is_trivial<gtl::static_variant<type1, type2>>::value == false), "Expected std::is_trivial to be false.");
                    REQUIRE((std::is_trivially_copyable<gtl::static_variant<type1, type2>>::value == false), "Expected std::is_trivially_copyable to be false.");
                    REQUIRE((std::is_standard_layout<gtl::static_variant<type1, type2>>::value == true), "Expected std::is_standard_layout to be true.");
                }
            );
        }
    );
}

TEST(static_variant, constructor, empty) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type_1) -> void {
            using type1 = typename decltype(test_type_1)::type;
            gtl::static_variant<type1> variant1;
            testbench::do_not_optimise_away(variant1);
            testbench::test_template<testbench::test_types>(
                [](auto test_type_2) -> void {
                    using type2 = typename decltype(test_type_2)::type;
                    gtl::static_variant<type1, type2> variant2;
                    testbench::do_not_optimise_away(variant2);
                }
            );
        }
    );
}

TEST(static_variant, constructor, value) {
    gtl::static_variant<int, float> static_variant_e;
    gtl::static_variant<int, float> static_variant_i(1234);
    gtl::static_variant<int, float> static_variant_f(1.0f);
}

TEST(static_variant, function, empty) {
    gtl::static_variant<int, float> static_variant_e;
    gtl::static_variant<int, float> static_variant_i(1234);
    gtl::static_variant<int, float> static_variant_f(1.0f);

    REQUIRE(static_variant_e.empty() == true);
    REQUIRE(static_variant_i.empty() == false);
    REQUIRE(static_variant_f.empty() == false);
}

TEST(static_variant, function, is) {
    gtl::static_variant<int, float> static_variant_e;
    gtl::static_variant<int, float> static_variant_i(1234);
    gtl::static_variant<int, float> static_variant_f(1.0f);

    REQUIRE(static_variant_e.is<int>() == false);
    REQUIRE(static_variant_e.is<float>() == false);
    REQUIRE(static_variant_i.is<int>() == true);
    REQUIRE(static_variant_i.is<float>() == false);
    REQUIRE(static_variant_f.is<int>() == false);
    REQUIRE(static_variant_f.is<float>() == true);

    static_variant_i = 1.0f;
    static_variant_f = 1234;

    REQUIRE(static_variant_e.is<int>() == false);
    REQUIRE(static_variant_e.is<float>() == false);
    REQUIRE(static_variant_i.is<int>() == false);
    REQUIRE(static_variant_i.is<float>() == true);
    REQUIRE(static_variant_f.is<int>() == true);
    REQUIRE(static_variant_f.is<float>() == false);
}

TEST(static_variant, function, as) {
    gtl::static_variant<int, float> static_variant_i(1234);
    gtl::static_variant<int, float> static_variant_f(1.0f);

    REQUIRE(testbench::is_value_equal(static_variant_i.as<int>(), 1234));
    REQUIRE(testbench::is_value_equal(static_variant_i.as<float>(), 1234.0f));
    REQUIRE(testbench::is_value_equal(static_variant_f.as<int>(), 1));
    REQUIRE(testbench::is_value_equal(static_variant_f.as<float>(), 1.0f));
}

TEST(static_variant, function, get) {
    gtl::static_variant<int, float> static_variant_i(1234);
    gtl::static_variant<int, float> static_variant_f(1.0f);

    REQUIRE(testbench::is_value_equal(static_variant_i.get<int>(), 1234));
    REQUIRE(testbench::is_value_equal(static_variant_f.get<float>(), 1.0f));
}

TEST(static_variant, function, set) {
    gtl::static_variant<int, float> static_variant_i(1234);
    gtl::static_variant<int, float> static_variant_f(1.0f);

    REQUIRE(testbench::is_value_equal(static_variant_i.get<int>(), 1234));
    REQUIRE(testbench::is_value_equal(static_variant_f.get<float>(), 1.0f));

    static_variant_i.set<int>(4321);
    static_variant_f.set<float>(0.1f);

    REQUIRE(testbench::is_value_equal(static_variant_i.get<int>(), 4321));
    REQUIRE(testbench::is_value_equal(static_variant_f.get<float>(), 0.1f));
}

namespace {
    int tracked_constructed = 0;
    int tracked_copy_constructed = 0;
    int tracked_move_constructed = 0;
    int tracked_copy_assigned = 0;
    int tracked_move_assigned = 0;
    int tracked_destructed = 0;
    int tracked_invalid = 0;

    // A type that tracks how it is constructed, assigned, and destructed, and knows if it is used without being constructed.
    struct tracked_type {
        const tracked_type* self;
        int value;

        tracked_type(int initial_value)
            : self(this)
            , value(initial_value) {
            ++tracked_constructed;
        }

        tracked_type(const tracked_type& other)
            : self(this)
            , value(other.value) {
            tracked_invalid += (other.self != &other);
            ++tracked_copy_constructed;
        }

        tracked_type(tracked_type&& other)
            : self(this)
            , value(other.value) {
            tracked_invalid += (other.self != &other);
            other.value = -1;
            ++tracked_move_constructed;
        }

        tracked_type& operator=(const tracked_type& other) {
            tracked_invalid += (this->self != this);
            tracked_invalid += (other.self != &other);
            this->value = other.value;
            ++tracked_copy_assigned;
            return *this;
        }

        tracked_type& operator=(tracked_type&& other) {
            tracked_invalid += (this->self != this);
            tracked_invalid += (other.self != &other);
            this->value = other.value;
            other.value = -1;
            ++tracked_move_assigned;
            return *this;
        }

        ~tracked_type() {
            tracked_invalid += (this->self != this);
            this->self = nullptr;
            ++tracked_destructed;
        }
    };

    void tracked_reset() {
        tracked_constructed = 0;
        tracked_copy_constructed = 0;
        tracked_move_constructed = 0;
        tracked_copy_assigned = 0;
        tracked_move_assigned = 0;
        tracked_destructed = 0;
        tracked_invalid = 0;
    }

    bool tracked_balanced() {
        return (tracked_invalid == 0) && ((tracked_constructed + tracked_copy_constructed + tracked_move_constructed) == tracked_destructed);
    }
}

TEST(static_variant, constructor, copying) {
    tracked_reset();
    {
        gtl::static_variant<int, tracked_type> static_variant_e;
        gtl::static_variant<int, tracked_type> static_variant_i(1234);
        gtl::static_variant<int, tracked_type> static_variant_t(tracked_type(5678));
        const gtl::static_variant<int, tracked_type>& static_variant_t_const = static_variant_t;

        gtl::static_variant<int, tracked_type> static_variant_copy_e(static_variant_e);
        REQUIRE(static_variant_copy_e.empty() == true);

        gtl::static_variant<int, tracked_type> static_variant_copy_i(static_variant_i);
        REQUIRE(static_variant_copy_i.is<int>() == true);
        REQUIRE(static_variant_copy_i.get<int>() == 1234);
        REQUIRE(static_variant_i.get<int>() == 1234);

        gtl::static_variant<int, tracked_type> static_variant_copy_t1(static_variant_t);
        REQUIRE(static_variant_copy_t1.is<tracked_type>() == true);
        REQUIRE(static_variant_copy_t1.get<tracked_type>().value == 5678);
        REQUIRE(static_variant_t.get<tracked_type>().value == 5678);

        gtl::static_variant<int, tracked_type> static_variant_copy_t2(static_variant_t_const);
        REQUIRE(static_variant_copy_t2.is<tracked_type>() == true);
        REQUIRE(static_variant_copy_t2.get<tracked_type>().value == 5678);
        REQUIRE(static_variant_t.get<tracked_type>().value == 5678);

        REQUIRE(tracked_copy_constructed == 3, "Expected 3 not %d", tracked_copy_constructed);
        REQUIRE(tracked_copy_assigned == 0);
        REQUIRE(tracked_move_assigned == 0);
    }
    REQUIRE(tracked_balanced(), "Invalid: %d, Constructed: %d, Destructed: %d", tracked_invalid, tracked_constructed + tracked_copy_constructed + tracked_move_constructed, tracked_destructed);
}

TEST(static_variant, constructor, moving) {
    tracked_reset();
    {
        gtl::static_variant<int, tracked_type> static_variant_e;
        gtl::static_variant<int, tracked_type> static_variant_i(1234);
        gtl::static_variant<int, tracked_type> static_variant_t(tracked_type(5678));
        const int move_constructed = tracked_move_constructed;

        gtl::static_variant<int, tracked_type> static_variant_move_e(static_cast<gtl::static_variant<int, tracked_type>&&>(static_variant_e));
        REQUIRE(static_variant_move_e.empty() == true);

        gtl::static_variant<int, tracked_type> static_variant_move_i(static_cast<gtl::static_variant<int, tracked_type>&&>(static_variant_i));
        REQUIRE(static_variant_move_i.is<int>() == true);
        REQUIRE(static_variant_move_i.get<int>() == 1234);

        gtl::static_variant<int, tracked_type> static_variant_move_t(static_cast<gtl::static_variant<int, tracked_type>&&>(static_variant_t));
        REQUIRE(static_variant_move_t.is<tracked_type>() == true);
        REQUIRE(static_variant_move_t.get<tracked_type>().value == 5678);

        REQUIRE(tracked_move_constructed == move_constructed + 1);
        REQUIRE(tracked_copy_assigned == 0);
        REQUIRE(tracked_move_assigned == 0);
    }
    REQUIRE(tracked_balanced(), "Invalid: %d, Constructed: %d, Destructed: %d", tracked_invalid, tracked_constructed + tracked_copy_constructed + tracked_move_constructed, tracked_destructed);
}

TEST(static_variant, operator, copying) {
    tracked_reset();
    {
        const gtl::static_variant<int, tracked_type> static_variant_e;
        const gtl::static_variant<int, tracked_type> static_variant_i(1234);
        const gtl::static_variant<int, tracked_type> static_variant_t1(tracked_type(5678));
        const gtl::static_variant<int, tracked_type> static_variant_t2(tracked_type(8765));

        gtl::static_variant<int, tracked_type> static_variant;

        // Empty to empty.
        static_variant = static_variant_e;
        REQUIRE(static_variant.empty() == true);

        // Empty to a non-trivial type.
        static_variant = static_variant_t1;
        REQUIRE(static_variant.is<tracked_type>() == true);
        REQUIRE(static_variant.get<tracked_type>().value == 5678);
        REQUIRE(tracked_copy_assigned == 0);

        // A non-trivial type to the same type.
        static_variant = static_variant_t2;
        REQUIRE(static_variant.is<tracked_type>() == true);
        REQUIRE(static_variant.get<tracked_type>().value == 8765);
        REQUIRE(tracked_copy_assigned == 1);

        // A non-trivial type to itself.
        const gtl::static_variant<int, tracked_type>& static_variant_const = static_variant;
        static_variant = static_variant_const;
        REQUIRE(static_variant.is<tracked_type>() == true);
        REQUIRE(static_variant.get<tracked_type>().value == 8765);

        // A non-trivial type to a different type.
        static_variant = static_variant_i;
        REQUIRE(static_variant.is<int>() == true);
        REQUIRE(static_variant.get<int>() == 1234);

        // A different type to a non-trivial type.
        static_variant = static_variant_t1;
        REQUIRE(static_variant.is<tracked_type>() == true);
        REQUIRE(static_variant.get<tracked_type>().value == 5678);
        REQUIRE(tracked_copy_assigned == 1);

        // A non-trivial type to empty.
        static_variant = static_variant_e;
        REQUIRE(static_variant.empty() == true);

        // The sources must not have been modified.
        REQUIRE(static_variant_i.get<int>() == 1234);
        REQUIRE(static_variant_t1.get<tracked_type>().value == 5678);
        REQUIRE(static_variant_t2.get<tracked_type>().value == 8765);
        REQUIRE(tracked_move_assigned == 0);
    }
    REQUIRE(tracked_balanced(), "Invalid: %d, Constructed: %d, Destructed: %d", tracked_invalid, tracked_constructed + tracked_copy_constructed + tracked_move_constructed, tracked_destructed);
}

TEST(static_variant, operator, moving) {
    tracked_reset();
    {
        using static_variant_type = gtl::static_variant<int, tracked_type>;

        static_variant_type static_variant;

        // Empty to empty.
        static_variant = static_variant_type();
        REQUIRE(static_variant.empty() == true);

        // Empty to a non-trivial type.
        static_variant = static_variant_type(tracked_type(5678));
        REQUIRE(static_variant.is<tracked_type>() == true);
        REQUIRE(static_variant.get<tracked_type>().value == 5678);
        REQUIRE(tracked_move_assigned == 0);

        // A non-trivial type to the same type.
        static_variant = static_variant_type(tracked_type(8765));
        REQUIRE(static_variant.is<tracked_type>() == true);
        REQUIRE(static_variant.get<tracked_type>().value == 8765);
        REQUIRE(tracked_move_assigned == 1);

        // A non-trivial type to itself.
        static_variant_type& static_variant_reference = static_variant;
        static_variant = static_cast<static_variant_type&&>(static_variant_reference);
        REQUIRE(static_variant.is<tracked_type>() == true);
        REQUIRE(static_variant.get<tracked_type>().value == 8765);

        // A non-trivial type to a different type.
        static_variant = static_variant_type(1234);
        REQUIRE(static_variant.is<int>() == true);
        REQUIRE(static_variant.get<int>() == 1234);

        // A different type to a non-trivial type.
        static_variant = static_variant_type(tracked_type(5678));
        REQUIRE(static_variant.is<tracked_type>() == true);
        REQUIRE(static_variant.get<tracked_type>().value == 5678);
        REQUIRE(tracked_move_assigned == 1);

        // A non-trivial type to empty.
        static_variant = static_variant_type();
        REQUIRE(static_variant.empty() == true);

        REQUIRE(tracked_copy_assigned == 0);
    }
    REQUIRE(tracked_balanced(), "Invalid: %d, Constructed: %d, Destructed: %d", tracked_invalid, tracked_constructed + tracked_copy_constructed + tracked_move_constructed, tracked_destructed);
}
