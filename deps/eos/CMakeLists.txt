project(eos)

add_library(eos SHARED IMPORTED GLOBAL)

if (WIN32)
set_property(TARGET eos PROPERTY
             IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/win/SDK/Bin/EOSSDK-Win64-Shipping.dll")
set_property(TARGET eos PROPERTY
             IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/win/SDK/Lib/EOSSDK-Win64-Shipping.lib")
set_property(TARGET eos PROPERTY
             INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/win/SDK/Include")


elseif(APPLE)
set_property(TARGET eos PROPERTY
             IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/win/SDK/Bin/libEOSSDK-Mac-Shipping.dylib")
set_property(TARGET eos PROPERTY
             INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/win/SDK/Include")
endif()