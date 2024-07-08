if("${FONT_CONFIGURATION}" STREQUAL "fontconfig")
    # Poppler uses different variable names than CMake.
    find_package(Fontconfig REQUIRED)
    set(FONTCONFIG_DEFINITIONS "")
    set(FONTCONFIG_INCLUDE_DIR "${Fontconfig_INCLUDE_DIRS}")
    set(FONTCONFIG_LIBRARIES "Fontconfig::Fontconfig")
endif()

# Poppler uses different variable names than CMake,
# plus ICONV_SECOND_ARGUMENT_IS_CONST
find_package(Iconv REQUIRED)
set(ICONV_INCLUDE_DIR "${Iconv_INCLUDE_DIR}")
set(ICONV_LIBRARIES "${Iconv_LIBRARIES}")

# Create helper file for iconv usage requirement
set(poppler_iconv [[
Name: poppler-vcpkg-iconv
Description: iconv linking requirements for poppler
Version: 0
Libs:]])
string(TOLOWER "${Iconv_LIBRARIES}" iconv_libraries)
if(iconv_libraries MATCHES "iconv")
    string(APPEND poppler_iconv " -liconv")
endif()
if(iconv_libraries MATCHES "charset")
    string(APPEND poppler_iconv " -lcharset")
endif()
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/poppler-vcpkg-iconv.pc" "${poppler_iconv}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/poppler-vcpkg-iconv.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
