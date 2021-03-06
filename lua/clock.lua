#!/usr/bin/env lua-akfavatar

--[[-------------------------------------------------------------------
Clock for AKFAvatar
Copyright (c) 2011,2012,2013 Andreas K. Foerster <akf@akfoerster.de>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
--]]-------------------------------------------------------------------

local avt = require "lua-akfavatar"
local graphic = require "akfavatar-graphic"

local resized = 0xE800

avt.title("Clock")
avt.start()

local function draw_clockface(gr, radius, color)
  local clockface = gr:duplicate()
  local width, height = gr:size()

  clockface:home()
  clockface:color("floral white")
  clockface:disc(radius-3)
  clockface:color(color)
  clockface:thickness(3)
  clockface:circle(radius-3)

  -- background image
  avt.set_bitmap_color("dark blue")
  clockface:put_file(avt.search("gnu-head.xbm"),
    width / 2 - 268 / 2,
    height / 4 - (253/2) + 20)

  -- draw minute points
  for minute=1,60 do
    clockface:home()
    clockface:heading(minute * 360/60)
    clockface:move(radius - 18)
    clockface:disc(2)
  end

  -- draw hours
  local textdist = 40
  clockface:textalign("center", "center")
  for hour=1,12 do
    clockface:home()
    clockface:heading(hour * 360/12)
    clockface:move(radius - textdist)
    clockface:text(hour)
    clockface:move(textdist - 18)
    clockface:disc(7)
  end

  -- middle dot
  clockface:home()
  clockface:disc(10)

  -- show date
  local fwidth, fheight = gr:font_size()
  os.setlocale("", "time") --> for the formatting of the date
  local date = os.date("%x", timestamp)
  local date_plate, plate_width, plate_height
      = graphic.new(#date*fwidth + 15, fheight + 15, "grey90")

  date_plate:border3d(true)
  date_plate:center()
  date_plate:textalign("center", "center")
  date_plate:color("black")
  date_plate:text(date)
  clockface:put(date_plate,
    width / 2 - plate_width / 2,
    height * 3/4 - plate_height / 2)

  return clockface
end -- draw clockface


local function clock()
  ::start::
  local color = "saddle brown"
  local s = math.min(graphic.fullsize())
  local gr, width, height = graphic.new(s, s)
  local radius = s / 2
  local pointerlength = radius - 35
  local timestamp, oldtime
  local time = os.time
  local date = os.date

  gr:color(color)
  local clockface = draw_clockface(gr, radius, color)
  -- FIXME: date doesn't change at midnight

  timestamp = time()

  repeat

    -- wait until timestamp actually changes (every second)
    while timestamp == oldtime do
      avt.wait(0.1)
      timestamp = time()
    end

    oldtime = timestamp

    local time = date("*t", timestamp)

    -- at midnight update the date
    if time.hour == 0 and time.min == 0 and time.sec == 0 then
      clockface = draw_clockface(gr, radius, color)
    end

    gr:put(clockface) --> overwrites everything

    -- hours pointer
    gr:home()
    gr:heading((time.hour*60/12 + time.min/12) * 360/60)
    gr:thickness(6)
    gr:draw(pointerlength/2)

    -- minutes pointer
    gr:home()
    gr:heading(time.min * 360/60)
    gr:thickness(3)
    gr:draw(pointerlength)

    -- seconds pointer
    gr:home()
    gr:heading(time.sec * 360/60)
    gr:thickness(1)
    gr:draw(pointerlength)

    gr:show()
  until avt.key_pressed()

  if resized==avt.get_key() then goto start end
end

if graphic.set_resize_key then
  graphic.set_resize_key(resized)
end

clock()
