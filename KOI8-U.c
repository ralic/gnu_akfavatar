#include "avtaddons.h"

static const struct avt_char_map map = {
  .start = 0x80,.end = 0xFF,
  .table = {
	    0x2500,		// 0x80
	    0x2502,		// 0x81
	    0x250C,		// 0x82
	    0x2510,		// 0x83
	    0x2514,		// 0x84
	    0x2518,		// 0x85
	    0x251C,		// 0x86
	    0x2524,		// 0x87
	    0x252C,		// 0x88
	    0x2534,		// 0x89
	    0x253C,		// 0x8A
	    0x2580,		// 0x8B
	    0x2584,		// 0x8C
	    0x2588,		// 0x8D
	    0x258C,		// 0x8E
	    0x2590,		// 0x8F
	    0x2591,		// 0x90
	    0x2592,		// 0x91
	    0x2593,		// 0x92
	    0x2320,		// 0x93
	    0x25A0,		// 0x94
	    0x2219,		// 0x95
	    0x221A,		// 0x96
	    0x2248,		// 0x97
	    0x2264,		// 0x98
	    0x2265,		// 0x99
	    0x00A0,		// 0x9A
	    0x2321,		// 0x9B
	    0x00B0,		// 0x9C
	    0x00B2,		// 0x9D
	    0x00B7,		// 0x9E
	    0x00F7,		// 0x9F
	    0x2550,		// 0xA0
	    0x2551,		// 0xA1
	    0x2552,		// 0xA2
	    0x0451,		// 0xA3
	    0x0454,		// 0xA4
	    0x2554,		// 0xA5
	    0x0456,		// 0xA6
	    0x0457,		// 0xA7
	    0x2557,		// 0xA8
	    0x2558,		// 0xA9
	    0x2559,		// 0xAA
	    0x255A,		// 0xAB
	    0x255B,		// 0xAC
	    0x0491,		// 0xAD
	    0x255D,		// 0xAE
	    0x255E,		// 0xAF
	    0x255F,		// 0xB0
	    0x2560,		// 0xB1
	    0x2561,		// 0xB2
	    0x0401,		// 0xB3
	    0x0404,		// 0xB4
	    0x2563,		// 0xB5
	    0x0406,		// 0xB6
	    0x0407,		// 0xB7
	    0x2566,		// 0xB8
	    0x2567,		// 0xB9
	    0x2568,		// 0xBA
	    0x2569,		// 0xBB
	    0x256A,		// 0xBC
	    0x0490,		// 0xBD
	    0x256C,		// 0xBE
	    0x00A9,		// 0xBF
	    0x044E,		// 0xC0
	    0x0430,		// 0xC1
	    0x0431,		// 0xC2
	    0x0446,		// 0xC3
	    0x0434,		// 0xC4
	    0x0435,		// 0xC5
	    0x0444,		// 0xC6
	    0x0433,		// 0xC7
	    0x0445,		// 0xC8
	    0x0438,		// 0xC9
	    0x0439,		// 0xCA
	    0x043A,		// 0xCB
	    0x043B,		// 0xCC
	    0x043C,		// 0xCD
	    0x043D,		// 0xCE
	    0x043E,		// 0xCF
	    0x043F,		// 0xD0
	    0x044F,		// 0xD1
	    0x0440,		// 0xD2
	    0x0441,		// 0xD3
	    0x0442,		// 0xD4
	    0x0443,		// 0xD5
	    0x0436,		// 0xD6
	    0x0432,		// 0xD7
	    0x044C,		// 0xD8
	    0x044B,		// 0xD9
	    0x0437,		// 0xDA
	    0x0448,		// 0xDB
	    0x044D,		// 0xDC
	    0x0449,		// 0xDD
	    0x0447,		// 0xDE
	    0x044A,		// 0xDF
	    0x042E,		// 0xE0
	    0x0410,		// 0xE1
	    0x0411,		// 0xE2
	    0x0426,		// 0xE3
	    0x0414,		// 0xE4
	    0x0415,		// 0xE5
	    0x0424,		// 0xE6
	    0x0413,		// 0xE7
	    0x0425,		// 0xE8
	    0x0418,		// 0xE9
	    0x0419,		// 0xEA
	    0x041A,		// 0xEB
	    0x041B,		// 0xEC
	    0x041C,		// 0xED
	    0x041D,		// 0xEE
	    0x041E,		// 0xEF
	    0x041F,		// 0xF0
	    0x042F,		// 0xF1
	    0x0420,		// 0xF2
	    0x0421,		// 0xF3
	    0x0422,		// 0xF4
	    0x0423,		// 0xF5
	    0x0416,		// 0xF6
	    0x0412,		// 0xF7
	    0x042C,		// 0xF8
	    0x042B,		// 0xF9
	    0x0417,		// 0xFA
	    0x0428,		// 0xFB
	    0x042D,		// 0xFC
	    0x0429,		// 0xFD
	    0x0427,		// 0xFE
	    0x042A,		// 0xFF
	    }
};


static const struct avt_charenc converter = {
  .data = (void *) &map,
  .to_unicode = map_to_unicode,
  .from_unicode = map_from_unicode
};


extern const struct avt_charenc *
avt_koi8u (void)
{
  return &converter;
}
