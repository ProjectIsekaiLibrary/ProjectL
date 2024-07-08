# Automatically generated by scripts/boost/generate-ports.ps1

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/tti
    REF boost-1.82.0
    SHA512 2ec45de7457df78febec14d64850b91b7956036d4b3fc859ce95314e5af183b179960fa36cf2a1b8d6345f513d2c8357e3b1838ee3b61d10795cdacf678eca82
    HEAD_REF master
)

include(${CURRENT_INSTALLED_DIR}/share/boost-vcpkg-helpers/boost-modular-headers.cmake)
boost_modular_headers(SOURCE_PATH ${SOURCE_PATH})
