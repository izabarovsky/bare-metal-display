/*
    <Generate from ${template_file_path}>
    generate font bitmap
    bpp : ${bpp}
    font: ${font}
    size: ${size}
    encoding method: ${encoding_method}

    width: ${width}
    height: ${height}
*/


static const uint16_t ${font_lowercase}${size}_bmp[] = {
====split====
    // ${charname}
    ${bmpdata},
====split====
};

####split####

typedef struct
{
    char symbol;
    uint8_t width;
    uint8_t height;
    uint16_t bmp_index;
}font_table_t;

static const font_table_t ${font_lowercase}${size}_table[] = {
====split====
    {'${symbol}', ${width}, ${height}, ${bmpidx}},  // ${charname}
====split====
};

