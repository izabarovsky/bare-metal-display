add_executable(example-fonts
        example-fonts.cpp
        )

# pull in common dependencies
target_link_libraries(example-fonts pico_stdlib BareMetalDisplay)

# create map/bin/hex file etc.
pico_add_extra_outputs(example-fonts)
