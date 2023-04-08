#!/usr/bin/python
# SPDX-License-Identifier: GPL-2.0

import sys
import numpy
from PIL import Image


def process_file(infile, outfile):
    img = Image.open(infile)

    if ((img.size[0] != 240) or (img.size[1] != 240)):
        print("This tool only works with 240x240 images (size 57600)")
        sys.exit(1)
    pix = img.load()
    outarray = numpy.zeros((img.size[1] * img.size[0],), dtype=numpy.uint16)
    array_pos = 0
    with open(outfile + '.h', "w") as out_h_file:
        for y_pos in range(img.size[1]):
            for x_pos in range(img.size[0]):
                word = 0
                word = pix[x_pos, y_pos][0] >> 3
                word <<= 6
                word |= pix[x_pos, y_pos][1] >> 2
                word <<= 5
                word |= pix[x_pos, y_pos][2] >> 3
                outarray[array_pos] = word
                array_pos = array_pos + 1
        c_array = 'uint16_t ' + outfile + '[] = {\n' + ',\n'.join("0x%04X" % x for x in outarray) + '\n};'
        macro = '{}_H'.format(outfile.upper())
        out_h_file.write('#include <cstdint>\n\n')
        out_h_file.write('#ifndef {}\n'.format(macro))
        out_h_file.write('#define {}\n\n'.format(macro))
        out_h_file.write(c_array)
        out_h_file.write('\n\n#endif /* {} */\n'.format(macro))
        out_h_file.close()
        print("Output file: " + out_h_file.name)


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage img2bin.py <infile.png> <outfile>")
        sys.exit(1)
    process_file(sys.argv[1], sys.argv[2])
