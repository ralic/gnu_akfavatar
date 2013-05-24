#include "avtaddons.h"

static const struct avt_char_map map = {
  .start = 0xA0,.end = 0xFF,
  .table = {
	    0x00A0,		// 0xA0
	    0x0104,		// 0xA1
	    0x02D8,		// 0xA2
	    0x0141,		// 0xA3
	    0x00A4,		// 0xA4
	    0x013D,		// 0xA5
	    0x015A,		// 0xA6
	    0x00A7,		// 0xA7
	    0x00A8,		// 0xA8
	    0x0160,		// 0xA9
	    0x015E,		// 0xAA
	    0x0164,		// 0xAB
	    0x0179,		// 0xAC
	    0x00AD,		// 0xAD
	    0x017D,		// 0xAE
	    0x017B,		// 0xAF
	    0x00B0,		// 0xB0
	    0x0105,		// 0xB1
	    0x02DB,		// 0xB2
	    0x0142,		// 0xB3
	    0x00B4,		// 0xB4
	    0x013E,		// 0xB5
	    0x015B,		// 0xB6
	    0x02C7,		// 0xB7
	    0x00B8,		// 0xB8
	    0x0161,		// 0xB9
	    0x015F,		// 0xBA
	    0x0165,		// 0xBB
	    0x017A,		// 0xBC
	    0x02DD,		// 0xBD
	    0x017E,		// 0xBE
	    0x017C,		// 0xBF
	    0x0154,		// 0xC0
	    0x00C1,		// 0xC1
	    0x00C2,		// 0xC2
	    0x0102,		// 0xC3
	    0x00C4,		// 0xC4
	    0x0139,		// 0xC5
	    0x0106,		// 0xC6
	    0x00C7,		// 0xC7
	    0x010C,		// 0xC8
	    0x00C9,		// 0xC9
	    0x0118,		// 0xCA
	    0x00CB,		// 0xCB
	    0x011A,		// 0xCC
	    0x00CD,		// 0xCD
	    0x00CE,		// 0xCE
	    0x010E,		// 0xCF
	    0x0110,		// 0xD0
	    0x0143,		// 0xD1
	    0x0147,		// 0xD2
	    0x00D3,		// 0xD3
	    0x00D4,		// 0xD4
	    0x0150,		// 0xD5
	    0x00D6,		// 0xD6
	    0x00D7,		// 0xD7
	    0x0158,		// 0xD8
	    0x016E,		// 0xD9
	    0x00DA,		// 0xDA
	    0x0170,		// 0xDB
	    0x00DC,		// 0xDC
	    0x00DD,		// 0xDD
	    0x0162,		// 0xDE
	    0x00DF,		// 0xDF
	    0x0155,		// 0xE0
	    0x00E1,		// 0xE1
	    0x00E2,		// 0xE2
	    0x0103,		// 0xE3
	    0x00E4,		// 0xE4
	    0x013A,		// 0xE5
	    0x0107,		// 0xE6
	    0x00E7,		// 0xE7
	    0x010D,		// 0xE8
	    0x00E9,		// 0xE9
	    0x0119,		// 0xEA
	    0x00EB,		// 0xEB
	    0x011B,		// 0xEC
	    0x00ED,		// 0xED
	    0x00EE,		// 0xEE
	    0x010F,		// 0xEF
	    0x0111,		// 0xF0
	    0x0144,		// 0xF1
	    0x0148,		// 0xF2
	    0x00F3,		// 0xF3
	    0x00F4,		// 0xF4
	    0x0151,		// 0xF5
	    0x00F6,		// 0xF6
	    0x00F7,		// 0xF7
	    0x0159,		// 0xF8
	    0x016F,		// 0xF9
	    0x00FA,		// 0xFA
	    0x0171,		// 0xFB
	    0x00FC,		// 0xFC
	    0x00FD,		// 0xFD
	    0x0163,		// 0xFE
	    0x02D9		// 0xFF
	    }
};


static const struct avt_charenc converter = {
  .data = (void *) &map,
  .to_unicode = map_to_unicode,
  .from_unicode = map_from_unicode
};


extern const struct avt_charenc *
avt_iso8859_2 (void)
{
  return &converter;
}
