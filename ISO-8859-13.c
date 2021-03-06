/*
 * ISO-8859-13 (ISO Latin-7) support for AKFAvatar
 * Copyright (c) 2013 Andreas K. Foerster <akf@akfoerster.de>
 *
 * This file is part of AKFAvatar
 *
 * AKFAvatar is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AKFAvatar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "avtaddons.h"

static const struct avt_char_map map = {
  .start = 0xA1,.end = 0xFF,
  .table = {
	    0x201D,		// 0xA1
	    0x00A2,		// 0xA2
	    0x00A3,		// 0xA3
	    0x00A4,		// 0xA4
	    0x201E,		// 0xA5
	    0x00A6,		// 0xA6
	    0x00A7,		// 0xA7
	    0x00D8,		// 0xA8
	    0x00A9,		// 0xA9
	    0x0156,		// 0xAA
	    0x00AB,		// 0xAB
	    0x00AC,		// 0xAC
	    0x00AD,		// 0xAD
	    0x00AE,		// 0xAE
	    0x00C6,		// 0xAF
	    0x00B0,		// 0xB0
	    0x00B1,		// 0xB1
	    0x00B2,		// 0xB2
	    0x00B3,		// 0xB3
	    0x201C,		// 0xB4
	    0x00B5,		// 0xB5
	    0x00B6,		// 0xB6
	    0x00B7,		// 0xB7
	    0x00F8,		// 0xB8
	    0x00B9,		// 0xB9
	    0x0157,		// 0xBA
	    0x00BB,		// 0xBB
	    0x00BC,		// 0xBC
	    0x00BD,		// 0xBD
	    0x00BE,		// 0xBE
	    0x00E6,		// 0xBF
	    0x0104,		// 0xC0
	    0x012E,		// 0xC1
	    0x0100,		// 0xC2
	    0x0106,		// 0xC3
	    0x00C4,		// 0xC4
	    0x00C5,		// 0xC5
	    0x0118,		// 0xC6
	    0x0112,		// 0xC7
	    0x010C,		// 0xC8
	    0x00C9,		// 0xC9
	    0x0179,		// 0xCA
	    0x0116,		// 0xCB
	    0x0122,		// 0xCC
	    0x0136,		// 0xCD
	    0x012A,		// 0xCE
	    0x013B,		// 0xCF
	    0x0160,		// 0xD0
	    0x0143,		// 0xD1
	    0x0145,		// 0xD2
	    0x00D3,		// 0xD3
	    0x014C,		// 0xD4
	    0x00D5,		// 0xD5
	    0x00D6,		// 0xD6
	    0x00D7,		// 0xD7
	    0x0172,		// 0xD8
	    0x0141,		// 0xD9
	    0x015A,		// 0xDA
	    0x016A,		// 0xDB
	    0x00DC,		// 0xDC
	    0x017B,		// 0xDD
	    0x017D,		// 0xDE
	    0x00DF,		// 0xDF
	    0x0105,		// 0xE0
	    0x012F,		// 0xE1
	    0x0101,		// 0xE2
	    0x0107,		// 0xE3
	    0x00E4,		// 0xE4
	    0x00E5,		// 0xE5
	    0x0119,		// 0xE6
	    0x0113,		// 0xE7
	    0x010D,		// 0xE8
	    0x00E9,		// 0xE9
	    0x017A,		// 0xEA
	    0x0117,		// 0xEB
	    0x0123,		// 0xEC
	    0x0137,		// 0xED
	    0x012B,		// 0xEE
	    0x013C,		// 0xEF
	    0x0161,		// 0xF0
	    0x0144,		// 0xF1
	    0x0146,		// 0xF2
	    0x00F3,		// 0xF3
	    0x014D,		// 0xF4
	    0x00F5,		// 0xF5
	    0x00F6,		// 0xF6
	    0x00F7,		// 0xF7
	    0x0173,		// 0xF8
	    0x0142,		// 0xF9
	    0x015B,		// 0xFA
	    0x016B,		// 0xFB
	    0x00FC,		// 0xFC
	    0x017C,		// 0xFD
	    0x017E,		// 0xFE
	    0x2019		// 0xFF
	    }
};


static const struct avt_charenc converter = {
  .data = (void *) &map,
  .decode = map_to_unicode,
  .encode = map_from_unicode
};


extern const struct avt_charenc *
avt_iso8859_13 (void)
{
  return &converter;
}
