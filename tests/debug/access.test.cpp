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
#include <testbench/require.tests.hpp>

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
    /*            auto */ /*volatile*/ /*const*/    int   variable_XaXXi = 1;
    /*            auto */   volatile   /*const*/    int   variable_XavXi = 1;
    /*            auto */ /*volatile*/   const      int   variable_XaXci = 1;
    /*            auto */   volatile     const      int   variable_Xavci = 1;
         static inline    /*volatile*/ /*const*/    int   variable_XsXXi = 1;
         static inline      volatile   /*const*/    int   variable_XsvXi = 1;
         static inline    /*volatile*/   const      int   variable_XsXci = 1;
         static inline      volatile     const      int   variable_Xsvci = 1;
               mutable    /*volatile*/ /*const*/    int   variable_XmXXi = 1;
               mutable      volatile   /*const*/    int   variable_XmvXi = 1;
//             mutable    /*volatile*/   const      int   variable_XmXci = 1;
//             mutable      volatile     const      int   variable_Xmvci = 1;
      constexpr static    /*volatile*/ /*const*/    int   variable_xsXXi = 1;
      constexpr static      volatile   /*const*/    int   variable_xsvXi = 1;
      constexpr static    /*volatile*/   const      int   variable_xsXci = 1;
      constexpr static      volatile     const      int   variable_xsvci = 1;

      void function_aXXX() {}
      int  function_aXXi(/*volatile*/ /*const*/ int) { return 1; }
      int  function_avXi(  volatile   /*const*/ int) { return 1; }
      int  function_aXci(/*volatile*/   const   int) { return 1; }
      int  function_avci(  volatile     const   int) { return 1; }

      void function_cXXX() const {}
      int  function_cXXi(/*volatile*/ /*const*/ int) const { return 1; }
      int  function_cvXi(  volatile   /*const*/ int) const { return 1; }
      int  function_cXci(/*volatile*/   const   int) const { return 1; }
      int  function_cvci(  volatile     const   int) const { return 1; }

      static void function_sXXX() {}
      static int  function_sXXi(/*volatile*/ /*const*/ int) { return 1; }
      static int  function_svXi(  volatile   /*const*/ int) { return 1; }
      static int  function_sXci(/*volatile*/   const   int) { return 1; }
      static int  function_svci(  volatile     const   int) { return 1; }

public:
      int use() {
          return 0
             + this->variable_XaXXi
             + this->variable_XavXi
             + this->variable_XaXci
             + this->variable_Xavci
             + test_class::variable_XsXXi
             + test_class::variable_XsvXi
             + test_class::variable_XsXci
             + test_class::variable_Xsvci
             + this->variable_XmXXi
             + this->variable_XmvXi
//           + this->variable_XmXci
//           + this->variable_Xmvci
             + test_class::variable_xsXXi
             + test_class::variable_xsvXi
             + test_class::variable_xsXci
             + test_class::variable_xsvci;
      }
};

GTL_ACCESS_GENERATE(test_class_access, test_class,
    GTL_ACCESS_TYPE,    auto,                int, variable_XaXXi,
    GTL_ACCESS_TYPE,    auto,       volatile int, variable_XavXi,
    GTL_ACCESS_TYPE,    auto,          const int, variable_XaXci,
    GTL_ACCESS_TYPE,    auto, volatile const int, variable_Xavci,
    GTL_ACCESS_TYPE,  static,                int, variable_XsXXi,
    GTL_ACCESS_TYPE,  static,       volatile int, variable_XsvXi,
    GTL_ACCESS_TYPE,  static,          const int, variable_XsXci,
    GTL_ACCESS_TYPE,  static, volatile const int, variable_Xsvci,
    GTL_ACCESS_TYPE, mutable,                int, variable_XmXXi,
    GTL_ACCESS_TYPE, mutable,       volatile int, variable_XmvXi,
//  GTL_ACCESS_TYPE, mutable,          const int, variable_XmXci,
//  GTL_ACCESS_TYPE, mutable, volatile const int, variable_Xmvci,
    GTL_ACCESS_TYPE,  static,          const int, variable_xsXXi,
    GTL_ACCESS_TYPE,  static, volatile const int, variable_xsvXi,
    GTL_ACCESS_TYPE,  static,          const int, variable_xsXci,
    GTL_ACCESS_TYPE,  static, volatile const int, variable_xsvci,

    GTL_ACCESS_FUNC,    auto, void(void),                    function_aXXX,
    GTL_ACCESS_FUNC,    auto, int(int),                      function_aXXi,
    GTL_ACCESS_FUNC,    auto, int(volatile int),             function_avXi,
    GTL_ACCESS_FUNC,    auto, int(const int),                function_aXci,
    GTL_ACCESS_FUNC,    auto, int(volatile const int),       function_avci,
    GTL_ACCESS_FUNC,    auto, void(void) const,              function_cXXX,
    GTL_ACCESS_FUNC,    auto, int(int) const,                function_cXXi,
    GTL_ACCESS_FUNC,    auto, int(volatile int) const,       function_cvXi,
    GTL_ACCESS_FUNC,    auto, int(const int) const,          function_cXci,
    GTL_ACCESS_FUNC,    auto, int(volatile const int) const, function_cvci,
    GTL_ACCESS_FUNC,  static, void(void),                    function_sXXX,
    GTL_ACCESS_FUNC,  static, int(int),                      function_sXXi,
    GTL_ACCESS_FUNC,  static, int(volatile int),             function_svXi,
    GTL_ACCESS_FUNC,  static, int(const int),                function_sXci,
    GTL_ACCESS_FUNC,  static, int(volatile const int),       function_svci
);

TEST(access, traits, standard) {
    REQUIRE(std::is_pod<test_class>::value == false);
    REQUIRE(std::is_pod<test_class_access>::value == false);

    REQUIRE(std::is_trivial<test_class>::value == false);
    REQUIRE(std::is_trivial<test_class_access>::value == false);

    #if defined(__clang__)
        REQUIRE(std::is_trivially_copyable<test_class>::value == true);
    #elif (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__clang__) && (!defined(__INTEL_COMPILER)))
        REQUIRE(std::is_trivially_copyable<test_class>::value == true);
    #elif defined(_MSC_VER)
        #if 0
            // Not reliable across compiler / os.
            REQUIRE(std::is_trivially_copyable<test_class>::value == false);
        #endif
    #endif

    REQUIRE(std::is_trivially_copyable<test_class_access>::value == false);

    REQUIRE(std::is_standard_layout<test_class>::value == true);
    REQUIRE(std::is_standard_layout<test_class_access>::value == false);
}

TEST(access, evaluate, access) {
    test_class test;
    test_class_access access(test);

    REQUIRE(access.variable_XaXXi == 1);
    REQUIRE(access.variable_XavXi == 1);
    REQUIRE(access.variable_XaXci == 1);
    REQUIRE(access.variable_Xavci == 1);
    REQUIRE(access.variable_XsXXi == 1);
    REQUIRE(access.variable_XsvXi == 1);
    REQUIRE(access.variable_XsXci == 1);
    REQUIRE(access.variable_Xsvci == 1);
    REQUIRE(access.variable_XmXXi == 1);
    REQUIRE(access.variable_XmvXi == 1);
//  REQUIRE(access.variable_XmXci == 1);
//  REQUIRE(access.variable_Xmvci == 1);
    REQUIRE(access.variable_xsXXi == 1);
    REQUIRE(access.variable_xsvXi == 1);
    REQUIRE(access.variable_xsXci == 1);
    REQUIRE(access.variable_xsvci == 1);

    access.function_aXXX();
    REQUIRE(access.function_aXXi(1) == 1);
    REQUIRE(access.function_avXi(1) == 1);
    REQUIRE(access.function_aXci(1) == 1);
    REQUIRE(access.function_avci(1) == 1);
    access.function_cXXX();
    REQUIRE(access.function_cXXi(1) == 1);
    REQUIRE(access.function_cvXi(1) == 1);
    REQUIRE(access.function_cXci(1) == 1);
    REQUIRE(access.function_cvci(1) == 1);
    access.function_sXXX();
    REQUIRE(access.function_sXXi(1) == 1);
    REQUIRE(access.function_svXi(1) == 1);
    REQUIRE(access.function_sXci(1) == 1);
    REQUIRE(access.function_svci(1) == 1);
}
