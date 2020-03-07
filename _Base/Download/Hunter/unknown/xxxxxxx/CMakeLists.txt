cmake_minimum_required(VERSION 3.2)
project(HunterDownload LANGUAGES NONE)
include(ExternalProject)
ExternalProject_Add(
    Hunter
    URL
    "x"
    URL_HASH
    SHA1=xxxxxxxx
    DOWNLOAD_DIR
    "/home/janek/sources/CPP/cpp-multilibs/_Base/Download/Hunter/unknown/xxxxxxx"
    TLS_VERIFY
    ON
    SOURCE_DIR
    "/home/janek/sources/CPP/cpp-multilibs/_Base/Download/Hunter/unknown/xxxxxxx/Unpacked"
    CONFIGURE_COMMAND
    ""
    BUILD_COMMAND
    ""
    INSTALL_COMMAND
    ""
)
