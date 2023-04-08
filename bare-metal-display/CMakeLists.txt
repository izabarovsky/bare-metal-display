file(GLOB FILES *.cpp *.h)

add_library(BareMetalDisplay ${FILES}
        BaseFont.h
        fonts/dejavu-sans-bold-16/DejaVuSansBold16.cpp
        fonts/dejavu-sans-bold-16/DejaVuSansBold16.h
        fonts/basic-8x8/Basic8x8.cpp
        fonts/basic-8x8/Basic8x8.h
        drivers/GC9A01.h
        drivers/GC9A01.cpp
        BareMetalDisplay.cpp
        BareMetalDisplay.h
        drivers/DisplayDriver.h
        drivers/GC9A01_Driver.cpp
        drivers/GC9A01_Driver.h
        Color.h
        )

target_link_libraries(BareMetalDisplay pico_stdlib hardware_spi hardware_dma)
