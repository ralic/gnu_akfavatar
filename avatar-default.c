/*
 * AKFAvatar - library for showing an avatar who says things in a balloon
 * This file imports the default avatar image
 * Copyright (c) 2007,2009,2010,2012 Andreas K. Foerster <akf@akfoerster.de>
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

#include "data/male_user.xpm"
#include "akfavatar.h"

extern int
avt_avatar_image_default (void)
{
  return avt_avatar_image_xpm (male_user_xpm);
}
