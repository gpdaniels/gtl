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

#include <main.tests.hpp>
#include <comparison.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#define GTL_API_EXPORT
#include <vision/api>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <string>
#include <type_traits>
#include <chrono>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

// Mock of the API.

extern "C" struct gtl_vision_system {
    std::string configuration = "{}";
    long long int timestamp_start = 0;

    ~gtl_vision_system() {

    }

    gtl_vision_system() {
        gtl_vision_get_timestamp(this, &timestamp_start);
    }
};

#if defined(_MSC_VER)
    #if defined(GTL_API_EXPORT)
        #define GTL_API_VISIBILITY __declspec(dllexport)
    #else
        #define GTL_API_VISIBILITY __declspec(dllimport)
    #endif

    #define GTL_API_CALL __stdcall

    #define GTL_API_STATIC_ASSERT(ASSERTION, MESSAGE) static_assert(ASSERTION, MESSAGE);
#else
    #if defined(GTL_API_EXPORT)
        #define GTL_API_VISIBILITY __attribute__((visibility("default")))
    #else
        #define GTL_API_VISIBILITY __attribute__((visibility("default")))
    #endif

    #define GTL_API_CALL

    #if defined(__cplusplus)
        #define GTL_API_STATIC_ASSERT(ASSERTION, MESSAGE) static_assert(ASSERTION, MESSAGE);
    #else
        #define GTL_API_STATIC_ASSERT(ASSERTION, MESSAGE) _Static_assert(ASSERTION, MESSAGE);
    #endif
#endif

GTL_API_STATIC_ASSERT(sizeof(char) == 1,            "For ABI compatibility the sizeof(char) must be 1 byte.")
GTL_API_STATIC_ASSERT(sizeof(short int) == 2,       "For ABI compatibility the sizeof(short int) must be 2 bytes.")
GTL_API_STATIC_ASSERT(sizeof(int) == 4,             "For ABI compatibility the sizeof(int) must be 4 bytes.")
GTL_API_STATIC_ASSERT(sizeof(long long int) == 8,   "For ABI compatibility the sizeof(long long int) must be 8 bytes.")
GTL_API_STATIC_ASSERT(sizeof(float) == 4,           "For ABI compatibility the sizeof(float) must be 4 bytes.")

extern "C" GTL_API_VISIBILITY gtl_vision_return_enum GTL_API_CALL gtl_vision_create(gtl_vision_system** system) {
    if (!system) {
        return gtl_vision_return_failure_invalid_system;
    }
    *system = new gtl_vision_system();
    return gtl_vision_return_success;
}

extern "C" GTL_API_VISIBILITY gtl_vision_return_enum GTL_API_CALL gtl_vision_destroy(gtl_vision_system** system) {
    if (!system) {
        return gtl_vision_return_failure_invalid_system;
    }
    if (!*system) {
        return gtl_vision_return_failure_invalid_system;
    }
    delete *system;
    *system = nullptr;
    return gtl_vision_return_success;
}

extern "C" GTL_API_VISIBILITY gtl_vision_return_enum GTL_API_CALL gtl_vision_get_timestamp(gtl_vision_system* system, long long int* timestamp) {
    if (!system) {
        return gtl_vision_return_failure_invalid_system;
    }
    if (!timestamp) {
        return gtl_vision_return_failure_invalid_argument;
    }
    *timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    return gtl_vision_return_success;
}

extern "C" GTL_API_VISIBILITY gtl_vision_return_enum GTL_API_CALL gtl_vision_get_configuration(gtl_vision_system* system, char* configuration, int* length) {
    if (!system) {
        return gtl_vision_return_failure_invalid_system;
    }
    if ((!length) || ((!configuration) && (*length != 0))) {
        return gtl_vision_return_failure_invalid_argument;
    }
    if (*length < 0) {
        return gtl_vision_return_failure_invalid_argument;
    }

    // TODO: This probably should return a generated configuration from settings (and/or license information).

    if (static_cast<std::string::size_type>(*length) < system->configuration.size() + 1) {
        *length = static_cast<int>(system->configuration.size() + 1);
        return gtl_vision_return_failure_insufficient_data_length;
    }
    *length = static_cast<int>(system->configuration.size() + 1);
    for (char c : system->configuration) {
        *configuration++ = c;
    }
    *configuration++ = 0;
    return gtl_vision_return_success;
}

extern "C" GTL_API_VISIBILITY gtl_vision_return_enum GTL_API_CALL gtl_vision_set_configuration(gtl_vision_system* system, const char* configuration, int length) {
    if (!system) {
        return gtl_vision_return_failure_invalid_system;
    }
    if (!configuration) {
        return gtl_vision_return_failure_invalid_argument;
    }
    system->configuration.clear();
    system->configuration.reserve(static_cast<std::string::size_type>(length));
    for (int i = 0; i < length; ++i) {
        system->configuration.push_back(*configuration++);
    }

    // TODO: Configuration validation, and enabling/disabling of features.

    return gtl_vision_return_success;
}

extern "C" GTL_API_VISIBILITY gtl_vision_return_enum GTL_API_CALL gtl_vision_set_sensor_rig(gtl_vision_system* system, const gtl_vision_sensor_rig_struct* rig, int length) {
    if (!system) {
        return gtl_vision_return_failure_invalid_system;
    }
    if (!rig) {
        return gtl_vision_return_failure_invalid_argument;
    }

    // TODO: Save sensor rig information.
    static_cast<void>(rig);
    static_cast<void>(length);

    return gtl_vision_return_success;
}

extern "C" GTL_API_VISIBILITY gtl_vision_return_enum GTL_API_CALL gtl_vision_set_sensor_data(gtl_vision_system* system, const gtl_vision_sensor_data_struct* data, int length) {
    if (!system) {
        return gtl_vision_return_failure_invalid_system;
    }
    if (!data) {
        return gtl_vision_return_failure_invalid_argument;
    }

    // TODO: Process sensor data.
    static_cast<void>(data);
    static_cast<void>(length);

    return gtl_vision_return_success;
}

extern "C" GTL_API_VISIBILITY gtl_vision_return_enum GTL_API_CALL gtl_vision_get_pose(gtl_vision_system* system, gtl_vision_pose_struct* pose) {
    if (!system) {
        return gtl_vision_return_failure_invalid_system;
    }
    if (!pose) {
        return gtl_vision_return_failure_invalid_argument;
    }

    // TODO: Get system pose.
    static_cast<void>(pose);

    return gtl_vision_return_success;
}

extern "C" GTL_API_VISIBILITY gtl_vision_return_enum GTL_API_CALL gtl_vision_get_pose_at_timestamp(gtl_vision_system* system, gtl_vision_pose_struct* pose, long long int timestamp) {
    if (!system) {
        return gtl_vision_return_failure_invalid_system;
    }
    if (!pose) {
        return gtl_vision_return_failure_invalid_argument;
    }
    if (timestamp < system->timestamp_start) {
        return gtl_vision_return_failure_invalid_argument;
    }

    // TODO: Get system pose at a time, interpolated or extrapolated.
    static_cast<void>(pose);
    static_cast<void>(timestamp);

    return gtl_vision_return_success;
}

extern "C" GTL_API_VISIBILITY gtl_vision_return_enum GTL_API_CALL gtl_vision_get_map_chunk(gtl_vision_system* system, float x, float y, float z, gtl_vision_map_chunk_struct* chunk) {
    if (!system) {
        return gtl_vision_return_failure_invalid_system;
    }
    if (!chunk) {
        return gtl_vision_return_failure_invalid_argument;
    }

    // TODO: Return a chunk of the map.
    static_cast<void>(x);
    static_cast<void>(y);
    static_cast<void>(z);
    static_cast<void>(chunk);

    return gtl_vision_return_success;
}

// Tests for the enum to sting functions.

TEST(api, function, return_enum_to_string) {
    REQUIRE(testbench::is_string_same(gtl_vision_return_enum_to_string(gtl_vision_return_success),                          "success"));
    REQUIRE(testbench::is_string_same(gtl_vision_return_enum_to_string(gtl_vision_return_failure_insufficient_data_length), "failure: insufficient data length"));
    REQUIRE(testbench::is_string_same(gtl_vision_return_enum_to_string(gtl_vision_return_failure_invalid_system),           "failure: invalid system"));
    REQUIRE(testbench::is_string_same(gtl_vision_return_enum_to_string(gtl_vision_return_failure_invalid_argument),         "failure: invalid argument"));
    REQUIRE(testbench::is_string_same(gtl_vision_return_enum_to_string(gtl_vision_return_failure_invalid_configuration),    "failure: invalid configuration"));
    REQUIRE(testbench::is_string_same(gtl_vision_return_enum_to_string(gtl_vision_return_failure_invalid_rig_sensor),       "failure: invalid rig sensor"));
    REQUIRE(testbench::is_string_same(gtl_vision_return_enum_to_string(gtl_vision_return_failure_invalid_sensor_data),      "failure: invalid sensor_data"));
    REQUIRE(testbench::is_string_same(gtl_vision_return_enum_to_string(gtl_vision_return_invalid),                          "invalid"));
    REQUIRE(testbench::is_string_same(gtl_vision_return_enum_to_string(static_cast<gtl_vision_return_enum>(0xFFFFFFFE)),    "unknown"));
}

TEST(api, function, sensor_emum_to_string) {
    REQUIRE(testbench::is_string_same(gtl_vision_sensor_emum_to_string(gtl_vision_sensor_empty),                            "empty"));
    REQUIRE(testbench::is_string_same(gtl_vision_sensor_emum_to_string(gtl_vision_sensor_map_chunk),                        "map chunk"));
    REQUIRE(testbench::is_string_same(gtl_vision_sensor_emum_to_string(gtl_vision_sensor_local_scale),                      "local scale"));
    REQUIRE(testbench::is_string_same(gtl_vision_sensor_emum_to_string(gtl_vision_sensor_local_linear),                     "local linear"));
    REQUIRE(testbench::is_string_same(gtl_vision_sensor_emum_to_string(gtl_vision_sensor_local_angular),                    "local augular"));
    REQUIRE(testbench::is_string_same(gtl_vision_sensor_emum_to_string(gtl_vision_sensor_remote_range),                     "remote range"));
    REQUIRE(testbench::is_string_same(gtl_vision_sensor_emum_to_string(gtl_vision_sensor_remote_bearing),                   "remote bearing"));
    REQUIRE(testbench::is_string_same(gtl_vision_sensor_emum_to_string(gtl_vision_sensor_remote_description),               "remote description"));
    REQUIRE(testbench::is_string_same(gtl_vision_sensor_emum_to_string(gtl_vision_sensor_invalid),                          "invalid"));
    REQUIRE(testbench::is_string_same(gtl_vision_sensor_emum_to_string(static_cast<gtl_vision_sensor_emum>(0xFFFFFFFE)),    "unknown"));
}

// Tests for the API functionality.

TEST(api, function, create_destroy) {
    gtl_vision_system* system = nullptr;
    REQUIRE(gtl_vision_create(nullptr) == gtl_vision_return_failure_invalid_system);
    REQUIRE(gtl_vision_create(&system) == gtl_vision_return_success);
    REQUIRE(gtl_vision_destroy(nullptr) == gtl_vision_return_failure_invalid_system);
    REQUIRE(gtl_vision_destroy(&system) == gtl_vision_return_success);
    REQUIRE(system == nullptr);
}

TEST(api, function, get_timestamp) {
    gtl_vision_system* system = nullptr;
    long long int timestamp = 0;
    long long int start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    REQUIRE(gtl_vision_create(&system) == gtl_vision_return_success);
    REQUIRE(gtl_vision_get_timestamp(nullptr, nullptr) == gtl_vision_return_failure_invalid_system);
    REQUIRE(gtl_vision_get_timestamp(nullptr, &timestamp) == gtl_vision_return_failure_invalid_system);
    REQUIRE(gtl_vision_get_timestamp(system, nullptr) == gtl_vision_return_failure_invalid_argument);
    REQUIRE(gtl_vision_get_timestamp(system, &timestamp) == gtl_vision_return_success);
    REQUIRE(gtl_vision_destroy(&system) == gtl_vision_return_success);
    REQUIRE(system == nullptr);
    long long int stop = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    REQUIRE(start < timestamp);
    REQUIRE(stop > timestamp);
}

TEST(api, function, get_set_configuration) {
    gtl_vision_system* system = nullptr;
    char configuration[100]{};
    int length = 0;

    REQUIRE(gtl_vision_create(&system) == gtl_vision_return_success);

    REQUIRE(gtl_vision_get_configuration(nullptr, nullptr, nullptr) == gtl_vision_return_failure_invalid_system);
    REQUIRE(gtl_vision_get_configuration(nullptr, &configuration[0], nullptr) == gtl_vision_return_failure_invalid_system);
    REQUIRE(gtl_vision_get_configuration(nullptr, nullptr, &length) == gtl_vision_return_failure_invalid_system);
    REQUIRE(gtl_vision_get_configuration(nullptr, &configuration[0], &length) == gtl_vision_return_failure_invalid_system);
    REQUIRE(gtl_vision_get_configuration(system, nullptr, nullptr) == gtl_vision_return_failure_invalid_argument);
    REQUIRE(gtl_vision_get_configuration(system, &configuration[0], nullptr) == gtl_vision_return_failure_invalid_argument);

    length = 0;
    REQUIRE(gtl_vision_get_configuration(system, nullptr, &length) == gtl_vision_return_failure_insufficient_data_length);
    REQUIRE(length == 3);

    length = 0;
    REQUIRE(gtl_vision_get_configuration(system, &configuration[0], &length) == gtl_vision_return_failure_insufficient_data_length);
    REQUIRE(length == 3);

    length = 2;
    REQUIRE(gtl_vision_get_configuration(system, &configuration[0], &length) == gtl_vision_return_failure_insufficient_data_length);
    REQUIRE(length == 3);

    length = 4;
    REQUIRE(gtl_vision_get_configuration(system, &configuration[0], &length) == gtl_vision_return_success);
    REQUIRE(length == 3);
    REQUIRE(testbench::is_string_same("{}", &configuration[0]));

    length = 100;
    REQUIRE(gtl_vision_get_configuration(system, &configuration[0], &length) == gtl_vision_return_success);
    REQUIRE(length == 3);
    REQUIRE(testbench::is_string_same("{}", &configuration[0]));

    REQUIRE(gtl_vision_set_configuration(nullptr, nullptr, 0) == gtl_vision_return_failure_invalid_system);
    REQUIRE(gtl_vision_set_configuration(nullptr, R"({"setting":"something"})", 0) == gtl_vision_return_failure_invalid_system);
    REQUIRE(gtl_vision_set_configuration(nullptr, R"({"setting":"something"})", 24) == gtl_vision_return_failure_invalid_system);
    REQUIRE(gtl_vision_set_configuration(system, nullptr, 0) == gtl_vision_return_failure_invalid_argument);
    REQUIRE(gtl_vision_set_configuration(system, R"({"setting":"something"})", 0) == gtl_vision_return_success);

    length = 100;
    REQUIRE(gtl_vision_get_configuration(system, &configuration[0], &length) == gtl_vision_return_success);
    REQUIRE(length == 1);
    REQUIRE(testbench::is_string_same("", &configuration[0]));

    REQUIRE(gtl_vision_set_configuration(system, R"({"setting":"something"})", 24) == gtl_vision_return_success);

    length = 100;
    REQUIRE(gtl_vision_get_configuration(system, &configuration[0], &length) == gtl_vision_return_success);
    REQUIRE(length == 25);
    REQUIRE(testbench::is_string_same(R"({"setting":"something"})", &configuration[0]));

    REQUIRE(gtl_vision_destroy(&system) == gtl_vision_return_success);
    REQUIRE(system == nullptr);
}

TEST(api, function, set_sensor_rig) {
    gtl_vision_system* system = nullptr;

    REQUIRE(gtl_vision_create(&system) == gtl_vision_return_success);
    REQUIRE(gtl_vision_set_sensor_rig(nullptr, nullptr, 0) == gtl_vision_return_failure_invalid_system);

    // TODO: ...

    // TODO: Sensors must have a unique id.

    REQUIRE(gtl_vision_destroy(&system) == gtl_vision_return_success);
    REQUIRE(system == nullptr);
}

TEST(api, function, set_sensor_data) {
    gtl_vision_system* system = nullptr;

    REQUIRE(gtl_vision_create(&system) == gtl_vision_return_success);
    REQUIRE(gtl_vision_set_sensor_data(nullptr, nullptr, 0) == gtl_vision_return_failure_invalid_system);

    // TODO: ...

    // TODO: Sensors data must have a id that matches one of the rig sensors.

    REQUIRE(gtl_vision_destroy(&system) == gtl_vision_return_success);
    REQUIRE(system == nullptr);
}

TEST(api, function, get_pose) {
    gtl_vision_system* system = nullptr;
    gtl_vision_pose_struct pose{};

    REQUIRE(gtl_vision_create(&system) == gtl_vision_return_success);
    REQUIRE(gtl_vision_get_pose(nullptr, nullptr) == gtl_vision_return_failure_invalid_system);
    REQUIRE(gtl_vision_get_pose(nullptr, &pose) == gtl_vision_return_failure_invalid_system);

    REQUIRE(gtl_vision_get_pose(system, nullptr) == gtl_vision_return_failure_invalid_argument);

    REQUIRE(gtl_vision_get_pose(system, &pose) == gtl_vision_return_success);

    // TODO: ...

    // TODO: Check validity of returned pose.

    // TODO: Load some poses.

    // TODO: Check validity of returned pose.

    REQUIRE(gtl_vision_destroy(&system) == gtl_vision_return_success);
    REQUIRE(system == nullptr);
}

TEST(api, function, get_pose_at_timestamp) {
    gtl_vision_system* system = nullptr;
    gtl_vision_pose_struct pose{};
    long long int timestamp = 0;

    REQUIRE(gtl_vision_create(&system) == gtl_vision_return_success);

    REQUIRE(gtl_vision_get_timestamp(system, &timestamp) == gtl_vision_return_success);

    REQUIRE(gtl_vision_get_pose_at_timestamp(nullptr, nullptr, 0) == gtl_vision_return_failure_invalid_system);
    REQUIRE(gtl_vision_get_pose_at_timestamp(nullptr, &pose, 0) == gtl_vision_return_failure_invalid_system);
    REQUIRE(gtl_vision_get_pose_at_timestamp(nullptr, &pose, timestamp) == gtl_vision_return_failure_invalid_system);

    REQUIRE(gtl_vision_get_pose_at_timestamp(system, nullptr, 0) == gtl_vision_return_failure_invalid_argument);
    REQUIRE(gtl_vision_get_pose_at_timestamp(system, &pose, 0) == gtl_vision_return_failure_invalid_argument);

    REQUIRE(gtl_vision_get_pose_at_timestamp(system, &pose, timestamp) == gtl_vision_return_success);

    // TODO: ...

    // TODO: Check validity of returned pose.

    // TODO: Load some poses.

    // TODO: Check validity of returned interpolated and extrapolated poses.

    REQUIRE(gtl_vision_destroy(&system) == gtl_vision_return_success);
    REQUIRE(system == nullptr);
}

TEST(api, function, get_map_chunk) {
    gtl_vision_system* system = nullptr;
    gtl_vision_map_chunk_struct map{};

    REQUIRE(gtl_vision_create(&system) == gtl_vision_return_success);
    REQUIRE(gtl_vision_get_map_chunk(nullptr, 0.0f, 0.0f, 0.0f, nullptr) == gtl_vision_return_failure_invalid_system);
    REQUIRE(gtl_vision_get_map_chunk(nullptr, 0.0f, 0.0f, 0.0f,  &map) == gtl_vision_return_failure_invalid_system);

    REQUIRE(gtl_vision_get_map_chunk(system, 0.0f, 0.0f, 0.0f, nullptr) == gtl_vision_return_failure_invalid_argument);

    REQUIRE(gtl_vision_get_map_chunk(system, 0.0f, 0.0f, 0.0f, &map) == gtl_vision_return_success);

    // TODO: ...

    // TODO: Load a map.

    // TODO: Check that I can get the map chunk when coordinates are within.

    // TODO: Check that I can't get the map chunk when coordinates are without.

    // TODO: Load a multi-chunk map.

    // TODO: Check that I can get the map chunk when coordinates are within.

    // TODO: Check that I can't get the map chunk when coordinates are without.

    // TODO: Check that I can get the multiple map chunks when coordinates are within each one.

    REQUIRE(gtl_vision_destroy(&system) == gtl_vision_return_success);
    REQUIRE(system == nullptr);
}

