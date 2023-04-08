add_executable(example-circles
        example-circles.cpp
        )

# pull in common dependencies
target_link_libraries(example-circles pico_stdlib BareMetalDisplay)

# create map/bin/hex file etc.
pico_add_extra_outputs(example-circles)