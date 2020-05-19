/*
The MIT License
Copyright (c) 2018 Geoffrey Daniels. http://gpdaniels.com/
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
*/

#include <main.tests.hpp>
#include <require.tests.hpp>

#include <debug/access>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

class test_class final {
private:
    /*         storage */ /*volatile*/ /*const*/ /*type*/ /*name*/
    /*            auto */ /*volatile*/ /*const*/    int   variable__a__i = 1;
    /*            auto */   volatile   /*const*/    int   variable__av_i = 1;
    /*            auto */ /*volatile*/   const      int   variable__a_ci = 1;
    /*            auto */   volatile     const      int   variable__avci = 1;
         inline static    /*volatile*/ /*const*/    int   variable__s__i = 1;
         inline static      volatile   /*const*/    int   variable__sv_i = 1;
         inline static    /*volatile*/   const      int   variable__s_ci = 1;
         inline static      volatile     const      int   variable__svci = 1;
               mutable    /*volatile*/ /*const*/    int   variable__m__i = 1;
               mutable      volatile   /*const*/    int   variable__mv_i = 1;
//             mutable    /*volatile*/   const      int   variable__m_ci = 1;
//             mutable      volatile     const      int   variable__mvci = 1;
      constexpr static    /*volatile*/ /*const*/    int   variable_xs__i = 1;
      constexpr static      volatile   /*const*/    int   variable_xsv_i = 1;
      constexpr static    /*volatile*/   const      int   variable_xs_ci = 1;
      constexpr static      volatile     const      int   variable_xsvci = 1;

      void function_a___() {}
      int  function_a__i(/*volatile*/ /*const*/ int) { return 1; }
      int  function_av_i(  volatile   /*const*/ int) { return 1; }
      int  function_a_ci(/*volatile*/   const   int) { return 1; }
      int  function_avci(  volatile     const   int) { return 1; }

      void function_c___() const {}
      int  function_c__i(/*volatile*/ /*const*/ int) const { return 1; }
      int  function_cv_i(  volatile   /*const*/ int) const { return 1; }
      int  function_c_ci(/*volatile*/   const   int) const { return 1; }
      int  function_cvci(  volatile     const   int) const { return 1; }

      static void function_s___() {}
      static int  function_s__i(/*volatile*/ /*const*/ int) { return 1; }
      static int  function_sv_i(  volatile   /*const*/ int) { return 1; }
      static int  function_s_ci(/*volatile*/   const   int) { return 1; }
      static int  function_svci(  volatile     const   int) { return 1; }

public:
      int use() {
          return 0
             + this->variable__a__i
             + this->variable__av_i
             + this->variable__a_ci
             + this->variable__avci
             + test_class::variable__s__i
             + test_class::variable__sv_i
             + test_class::variable__s_ci
             + test_class::variable__svci
             + this->variable__m__i
             + this->variable__mv_i
//           + this->variable__m_ci
//           + this->variable__mvci
             + test_class::variable_xs__i
             + test_class::variable_xsv_i
             + test_class::variable_xs_ci
             + test_class::variable_xsvci;
      }
};

GTL_ACCESS_GENERATE(test_class_access, test_class,
    GTL_ACCESS_TYPE,    auto,                int, variable__a__i,
    GTL_ACCESS_TYPE,    auto,       volatile int, variable__av_i,
    GTL_ACCESS_TYPE,    auto,          const int, variable__a_ci,
    GTL_ACCESS_TYPE,    auto, volatile const int, variable__avci,
    GTL_ACCESS_TYPE,  static,                int, variable__s__i,
    GTL_ACCESS_TYPE,  static,       volatile int, variable__sv_i,
    GTL_ACCESS_TYPE,  static,          const int, variable__s_ci,
    GTL_ACCESS_TYPE,  static, volatile const int, variable__svci,
    GTL_ACCESS_TYPE, mutable,                int, variable__m__i,
    GTL_ACCESS_TYPE, mutable,       volatile int, variable__mv_i,
//  GTL_ACCESS_TYPE, mutable,          const int, variable__m_ci,
//  GTL_ACCESS_TYPE, mutable, volatile const int, variable__mvci,
    GTL_ACCESS_TYPE,  static,          const int, variable_xs__i,
    GTL_ACCESS_TYPE,  static, volatile const int, variable_xsv_i,
    GTL_ACCESS_TYPE,  static,          const int, variable_xs_ci,
    GTL_ACCESS_TYPE,  static, volatile const int, variable_xsvci,

    GTL_ACCESS_FUNC,    auto, void(void),                    function_a___,
    GTL_ACCESS_FUNC,    auto, int(int),                      function_a__i,
    GTL_ACCESS_FUNC,    auto, int(volatile int),             function_av_i,
    GTL_ACCESS_FUNC,    auto, int(const int),                function_a_ci,
    GTL_ACCESS_FUNC,    auto, int(volatile const int),       function_avci,
    GTL_ACCESS_FUNC,    auto, void(void) const,              function_c___,
    GTL_ACCESS_FUNC,    auto, int(int) const,                function_c__i,
    GTL_ACCESS_FUNC,    auto, int(volatile int) const,       function_cv_i,
    GTL_ACCESS_FUNC,    auto, int(const int) const,          function_c_ci,
    GTL_ACCESS_FUNC,    auto, int(volatile const int) const, function_cvci,
    GTL_ACCESS_FUNC,  static, void(void),                    function_s___,
    GTL_ACCESS_FUNC,  static, int(int),                      function_s__i,
    GTL_ACCESS_FUNC,  static, int(volatile int),             function_sv_i,
    GTL_ACCESS_FUNC,  static, int(const int),                function_s_ci,
    GTL_ACCESS_FUNC,  static, int(volatile const int),       function_svci
);

TEST(access, traits, standard) {
    REQUIRE(sizeof(test_class) <= sizeof(test_class_access), "Expecting sizeof test_class (%ld) to smaller than or equal to the sizeof test_class_access (%ld)", sizeof(test_class), sizeof(test_class_access));

    REQUIRE(std::is_pod<test_class>::value == false);
    REQUIRE(std::is_pod<test_class_access>::value == false);

    REQUIRE(std::is_trivial<test_class>::value == false);
    REQUIRE(std::is_trivial<test_class_access>::value == false);

    #if defined(__clang__)
        REQUIRE(std::is_trivially_copyable<test_class>::value == true);
    #elif (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__INTEL_COMPILER))
        REQUIRE(std::is_trivially_copyable<test_class>::value == true);
    #elif defined(_MSC_VER)
        REQUIRE(std::is_trivially_copyable<test_class>::value == false);
    #endif

    REQUIRE(std::is_trivially_copyable<test_class_access>::value == false);

    REQUIRE(std::is_standard_layout<test_class>::value == true);
    REQUIRE(std::is_standard_layout<test_class_access>::value == false);
}

TEST(access, evaluate, access) {
    test_class test;
    test_class_access access(test);

    REQUIRE(access.variable__a__i == 1);
    REQUIRE(access.variable__av_i == 1);
    REQUIRE(access.variable__a_ci == 1);
    REQUIRE(access.variable__avci == 1);
    REQUIRE(access.variable__s__i == 1);
    REQUIRE(access.variable__sv_i == 1);
    REQUIRE(access.variable__s_ci == 1);
    REQUIRE(access.variable__svci == 1);
    REQUIRE(access.variable__m__i == 1);
    REQUIRE(access.variable__mv_i == 1);
//  REQUIRE(access.variable__m_ci == 1);
//  REQUIRE(access.variable__mvci == 1);
    REQUIRE(access.variable_xs__i == 1);
    REQUIRE(access.variable_xsv_i == 1);
    REQUIRE(access.variable_xs_ci == 1);
    REQUIRE(access.variable_xsvci == 1);

    access.function_a___();
    REQUIRE(access.function_a__i(1) == 1);
    REQUIRE(access.function_av_i(1) == 1);
    REQUIRE(access.function_a_ci(1) == 1);
    REQUIRE(access.function_avci(1) == 1);
    access.function_c___();
    REQUIRE(access.function_c__i(1) == 1);
    REQUIRE(access.function_cv_i(1) == 1);
    REQUIRE(access.function_c_ci(1) == 1);
    REQUIRE(access.function_cvci(1) == 1);
    access.function_s___();
    REQUIRE(access.function_s__i(1) == 1);
    REQUIRE(access.function_sv_i(1) == 1);
    REQUIRE(access.function_s_ci(1) == 1);
    REQUIRE(access.function_svci(1) == 1);
}
