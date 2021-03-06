# ~~~
# Copyright 2020 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ~~~

set(DOXYGEN_PROJECT_NAME "Google Cloud Pub/Sub C++ Client")
set(DOXYGEN_PROJECT_BRIEF "A C++ Client Library for Google Cloud Pub/Sub")
# TODO(#4878) - restore the version number in the documents
set(DOXYGEN_PROJECT_NUMBER "0.0.0 (dev)") # "${GOOGLE_CLOUD_CPP_VERSION}")
set(DOXYGEN_EXAMPLE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/samples)
set(DOXYGEN_EXCLUDE_SYMBOLS "internal" "pubsub_internal" "pubsub_testing")
set(DOXYGEN_PREDEFINED "GOOGLE_CLOUD_CPP_PUBSUB_NS=v1")

include(GoogleCloudCppCommon)

configure_file(version_info.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version_info.h)
add_library(
    pubsub_client # cmake-format: sort
    ack_handler.cc
    ack_handler.h
    connection_options.cc
    connection_options.h
    internal/batching_publisher_connection.cc
    internal/batching_publisher_connection.h
    internal/default_ack_handler_impl.cc
    internal/default_ack_handler_impl.h
    internal/emulator_overrides.cc
    internal/emulator_overrides.h
    internal/ordering_key_publisher_connection.cc
    internal/ordering_key_publisher_connection.h
    internal/publisher_logging.cc
    internal/publisher_logging.h
    internal/publisher_stub.cc
    internal/publisher_stub.h
    internal/subscriber_logging.cc
    internal/subscriber_logging.h
    internal/subscriber_stub.cc
    internal/subscriber_stub.h
    internal/subscription_session.cc
    internal/subscription_session.h
    internal/user_agent_prefix.cc
    internal/user_agent_prefix.h
    message.cc
    message.h
    publisher.cc
    publisher.h
    publisher_connection.cc
    publisher_connection.h
    publisher_options.h
    snapshot.cc
    snapshot.h
    snapshot_mutation_builder.cc
    snapshot_mutation_builder.h
    subscriber.h
    subscriber_connection.cc
    subscriber_connection.h
    subscription.cc
    subscription.h
    subscription_admin_client.cc
    subscription_admin_client.h
    subscription_admin_connection.cc
    subscription_admin_connection.h
    subscription_mutation_builder.cc
    subscription_mutation_builder.h
    subscription_options.h
    topic.cc
    topic.h
    topic_admin_client.cc
    topic_admin_client.h
    topic_admin_connection.cc
    topic_admin_connection.h
    topic_mutation_builder.cc
    topic_mutation_builder.h
    version.h
    version_info.h)
target_include_directories(
    pubsub_client
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
           $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
           $<INSTALL_INTERFACE:include>)
target_link_libraries(
    pubsub_client PUBLIC google_cloud_cpp_grpc_utils google_cloud_cpp_common
                         googleapis-c++::pubsub_protos)
google_cloud_cpp_add_common_options(pubsub_client)
set_target_properties(
    pubsub_client PROPERTIES VERSION "${GOOGLE_CLOUD_CPP_VERSION}"
                             SOVERSION "${GOOGLE_CLOUD_CPP_VERSION_MAJOR}")
target_compile_options(pubsub_client PUBLIC ${GOOGLE_CLOUD_CPP_EXCEPTIONS_FLAG})

add_library(googleapis-c++::pubsub_client ALIAS pubsub_client)

# To avoid maintaining the list of files for the library, export them to a .bzl
# file.
include(CreateBazelConfig)
create_bazel_config(pubsub_client YEAR "2020")

# Create a header-only library for the mocks. We use a CMake `INTERFACE` library
# for these, a regular library would not work on macOS (where the library needs
# at least one .o file). Unfortunately INTERFACE libraries are a bit weird in
# that they need absolute paths for their sources.
add_library(pubsub_client_mocks INTERFACE)
target_sources(
    pubsub_client_mocks
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_ack_handler.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_publisher_connection.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_subscription_admin_connection.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_topic_admin_connection.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_subscriber_connection.h)
target_link_libraries(
    pubsub_client_mocks
    INTERFACE googleapis-c++::pubsub_client google_cloud_cpp_testing
              GTest::gmock_main GTest::gmock GTest::gtest)
create_bazel_config(pubsub_client_mocks YEAR "2020")
target_include_directories(
    pubsub_client_mocks
    INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
              $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
              $<INSTALL_INTERFACE:include>)
target_compile_options(pubsub_client_mocks
                       INTERFACE ${GOOGLE_CLOUD_CPP_EXCEPTIONS_FLAG})

# Define the tests in a function so we have a new scope for variable names.
function (google_cloud_cpp_pubsub_client_define_tests)
    # The tests require googletest to be installed. Force CMake to use the
    # config file for googletest (that is, the CMake file installed by
    # googletest itself), because the generic `FindGTest` module does not define
    # the GTest::gmock target, and the target names are also weird.
    find_package(GTest CONFIG REQUIRED)

    add_library(
        pubsub_client_testing # cmake-format: sort
        testing/mock_publisher_stub.h testing/mock_subscriber_stub.h
        testing/random_names.cc testing/random_names.h)
    target_link_libraries(
        pubsub_client_testing
        PUBLIC pubsub_client google_cloud_cpp_grpc_utils
               google_cloud_cpp_common googleapis-c++::pubsub_protos)
    google_cloud_cpp_add_common_options(pubsub_client_testing)
    target_compile_options(pubsub_client_testing
                           PUBLIC ${GOOGLE_CLOUD_CPP_EXCEPTIONS_FLAG})
    create_bazel_config(pubsub_client_testing YEAR "2020")

    set(pubsub_client_unit_tests
        # cmake-format: sort
        ack_handler_test.cc
        internal/batching_publisher_connection_test.cc
        internal/default_ack_handler_impl_test.cc
        internal/emulator_overrides_test.cc
        internal/ordering_key_publisher_connection_test.cc
        internal/publisher_logging_test.cc
        internal/subscriber_logging_test.cc
        internal/subscription_session_test.cc
        internal/user_agent_prefix_test.cc
        message_test.cc
        publisher_connection_test.cc
        publisher_option_test.cc
        publisher_test.cc
        snapshot_mutation_builder_test.cc
        snapshot_test.cc
        subscriber_connection_test.cc
        subscriber_test.cc
        subscription_admin_client_test.cc
        subscription_admin_connection_test.cc
        subscription_mutation_builder_test.cc
        subscription_test.cc
        topic_admin_client_test.cc
        topic_admin_connection_test.cc
        topic_mutation_builder_test.cc
        topic_test.cc)

    # Export the list of unit tests to a .bzl file so we do not need to maintain
    # the list in two places.
    export_list_to_bazel("pubsub_client_unit_tests.bzl"
                         "pubsub_client_unit_tests" YEAR "2020")

    # Generate a target for each unit test.
    foreach (fname ${pubsub_client_unit_tests})
        google_cloud_cpp_add_executable(target "pubsub" "${fname}")
        target_link_libraries(
            ${target}
            PRIVATE pubsub_client_mocks
                    pubsub_client_testing
                    googleapis-c++::pubsub_client
                    google_cloud_cpp_testing
                    google_cloud_cpp_testing_grpc
                    GTest::gmock_main
                    GTest::gmock
                    GTest::gtest)
        google_cloud_cpp_add_common_options(${target})

        # With googletest it is relatively easy to exceed the default number of
        # sections (~65,000) in a single .obj file. Add the /bigobj option to
        # all the tests, even if it is not needed.
        if (MSVC)
            target_compile_options(${target} PRIVATE "/bigobj")
        endif ()
        add_test(NAME ${target} COMMAND ${target})
    endforeach ()
endfunction ()

# Only define the tests if testing is enabled. Package maintainers may not want
# to build all the tests everytime they create a new package or when the package
# is installed from source.
if (BUILD_TESTING)
    google_cloud_cpp_pubsub_client_define_tests()
endif (BUILD_TESTING)

add_subdirectory(integration_tests)

# Only compile the samples if we're building with exceptions enabled. They
# require exceptions to keep them simple and idiomatic.
if (GOOGLE_CLOUD_CPP_ENABLE_CXX_EXCEPTIONS)
    add_subdirectory(samples)
endif (GOOGLE_CLOUD_CPP_ENABLE_CXX_EXCEPTIONS)

# Get the destination directories based on the GNU recommendations.
include(GNUInstallDirs)

# Export the CMake targets to make it easy to create configuration files.
install(EXPORT pubsub-targets
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/pubsub_client")

# Install the libraries and headers in the locations determined by
# GNUInstallDirs
install(
    TARGETS pubsub_client
    EXPORT pubsub-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT google_cloud_cpp_pubsub_runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT google_cloud_cpp_pubsub_runtime
            NAMELINK_SKIP
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT google_cloud_cpp_pubsub_development)
# With CMake-3.12 and higher we could avoid this separate command (and the
# duplication).
install(
    TARGETS pubsub_client
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT google_cloud_cpp_pubsub_development
            NAMELINK_ONLY
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT google_cloud_cpp_pubsub_development)

google_cloud_cpp_install_headers("pubsub_client" "include/google/cloud/pubsub")
google_cloud_cpp_install_headers("pubsub_client_mocks"
                                 "include/google/cloud/pubsub")

# Setup global variables used in the following *.in files.
set(GOOGLE_CLOUD_CONFIG_VERSION_MAJOR ${GOOGLE_CLOUD_CPP_VERSION_MAJOR})
set(GOOGLE_CLOUD_CONFIG_VERSION_MINOR ${GOOGLE_CLOUD_CPP_VERSION_MINOR})
set(GOOGLE_CLOUD_CONFIG_VERSION_PATCH ${GOOGLE_CLOUD_CPP_VERSION_PATCH})
set(GOOGLE_CLOUD_PC_NAME "The Google Cloud Pubsub C++ Client Library")
set(GOOGLE_CLOUD_PC_DESCRIPTION
    "Provides C++ APIs to access Google Cloud Pubsub.")
set(GOOGLE_CLOUD_PC_REQUIRES
    "google_cloud_cpp_grpc_utils google_cloud_cpp_common googleapis_cpp_pubsub_protos"
)
set(GOOGLE_CLOUD_PC_LIBS "-lpubsub_client")

# Create and install the pkg-config files.
configure_file("${PROJECT_SOURCE_DIR}/google/cloud/pubsub/config.pc.in"
               "pubsub_client.pc" @ONLY)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/pubsub_client.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
    COMPONENT google_cloud_cpp_pubsub_development)

# Create and install the CMake configuration files.
configure_file("config.cmake.in" "pubsub_client-config.cmake" @ONLY)
configure_file(
    "${PROJECT_SOURCE_DIR}/google/cloud/pubsub/config-version.cmake.in"
    "pubsub_client-config-version.cmake" @ONLY)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/pubsub_client-config.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/pubsub_client-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/pubsub_client"
    COMPONENT google_cloud_cpp_pubsub_development)
