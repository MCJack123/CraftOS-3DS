cmake_minimum_required(VERSION 3.10)

project(craftos-3ds C CXX)

# Set C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Add your source files here
add_executable(craftos-3ds
    # Add your source files
    main.cpp
    hbkb/source/hbkb.cpp
    hbkb/source/hw_controls.cpp
    hbkb/source/hw_graphics.cpp
)

dkp_add_embedded_binary_library(hbkb-data
    hbkb/data/hbkb_abc_lower.bin
    hbkb/data/hbkb_abc_upper.bin
    hbkb/data/hbkb_back_sel.bin
    hbkb/data/hbkb_caps_sel.bin
    hbkb/data/hbkb_normal_sel.bin
    hbkb/data/hbkb_shift_sel.bin
    hbkb/data/hbkb_space_sel.bin
    hbkb/data/hbkb_special_sel.bin
    hbkb/data/hbkb_special.bin
)

file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/bios.lua)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/bios.lua
    COMMAND python -c "\"data=open('${CMAKE_CURRENT_SOURCE_DIR}/craftos2-rom/bios.lua','rb').read();open('${CMAKE_CURRENT_BINARY_DIR}/bios.lua','wb').write(data+b'\\0')\""
)
dkp_add_embedded_binary_library(bios ${CMAKE_CURRENT_BINARY_DIR}/bios.lua)

# Include craftos-base
add_subdirectory(craftos-base)

# Link craftos-base library
target_link_libraries(craftos-3ds
    craftos-base
    hbkb-data
    bios
)

target_include_directories(craftos-3ds PRIVATE hbkb/source)

ctr_generate_smdh(craftos-3ds
    OUTPUT craftos.smdh
    NAME CraftOS-3DS
    DESCRIPTION "CraftOS for the 3DS"
    AUTHOR JackMacWindows
    ICON icon.png
)

ctr_create_3dsx(craftos-3ds
    OUTPUT craftos.3dsx
    SMDH craftos.smdh
    ROMFS craftos2-rom/rom
)