# Automatically generated by scripts/boost/generate-ports.ps1

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/local_function
    REF boost-1.82.0
    SHA512 31e08d116f2619ee6071fab9db33d1f338acd2b6a01f2d1b18aef9bdee901a8bdb2407afa93a49558ea816555fc9f333c223d22b118c5fd07984704e8ebc0ddb
    HEAD_REF master
)

include(${CURRENT_INSTALLED_DIR}/share/boost-vcpkg-helpers/boost-modular-headers.cmake)
boost_modular_headers(SOURCE_PATH ${SOURCE_PATH})