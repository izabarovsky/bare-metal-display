add_executable(example-images
        example-images.cpp
        )

# pull in common dependencies
target_link_libraries(example-images pico_stdlib BareMetalDisplay)

# create map/bin/hex file etc.
pico_add_extra_outputs(example-images)