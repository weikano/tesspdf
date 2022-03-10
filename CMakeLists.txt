cmake_minimum_required(VERSION 3.5)

project(tesspdf VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Threads REQUIRED)
set(ENV{PDFium_DIR} /home/weikano/下载/pdfium-linux-x64/)
message("variable is $ENV{PDFium_DIR}")

set(ENV{PKG_CONFIG_PATH} /home/weikano/git/out/djvu/lib/pkgconfig)
message("djvu pkgconfig path is $ENV{PKG_CONFIG_PATH}")
find_package(PkgConfig)
pkg_check_modules(djvulibre REQUIRED ddjvuapi)
message(STATUS "==ddjvuapi libs:${djvulibre_LIBRARIES}")
message(STATUS "==ddjvuapi include:${djvulibre_INCLUDE_DIRS}")
message(STATUS "==ddjvuapi include:${djvulibre_LIBDIR}")

set(PROJECT_SOURCES
        main.cpp        
        tesssource.cpp
        ocr.cpp
        imagefiletesssource.cpp
        pdftesssource.cpp
        tesssourcefactory.h
        djvutesssource.cpp
)

include_directories(
    /usr/include/tesseract
)
include_directories(${PDFium_DIR}/include)
include_directories(${djvulibre_INCLUDE_DIRS})


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(tesspdf
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET tesspdf APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(tesspdf SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(tesspdf
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(tesspdf PRIVATE
    tesseract
    lept

    PRIVATE Threads::Threads
    ${PDFium_DIR}/lib/libabsl.so
    ${PDFium_DIR}/lib/libc++.so
    ${PDFium_DIR}/lib/libchrome_zlib.so
    ${PDFium_DIR}/lib/libicui18n.so
    ${PDFium_DIR}/lib/libicuuc.so
    ${PDFium_DIR}/lib/libpdfium.so

    ${djvulibre_LIBDIR}/libdjvulibre.so
    jpeg
    pthread
    )

set_target_properties(tesspdf PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(tesspdf)
endif()
