add_executable(example-lines
        example-lines.cpp
        )

# pull in common dependencies
target_link_libraries(example-lines pico_stdlib BareMetalDisplay)

# create map/bin/hex file etc.
pico_add_extra_outputs(example-lines)