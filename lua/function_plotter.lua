#!/usr/bin/env lua-akfavatar

--[[-------------------------------------------------------------------
Function Plotter for AKFAvatar
Copyright (c) 2011 Andreas K. Foerster <info@akfoerster.de>

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

require "lua-akfavatar"
require "akfavatar-canvas"

-- scale means: so many pixels for the value 1
local scale = 40

deg = math.deg
rad = math.rad
sqrt = math.sqrt
exp = math.exp
log = math.log
log10 = math.log10
pi = math.pi
sin = math.sin
sinh = math.sinh
asin = math.asin
cos = math.cos
cosh = math.cosh
tan = math.tan
tanh = math.tanh
atan = math.atan

-- add your own functions and constants here

-------------------------------------------------------------------------------
avt.initialize {
  title = "Function Plotter",
  avatar = require "akfavatar.teacher",
  encoding = "UTF-8"
  }

local c, width, height = canvas.new()
local halfwidth, halfheight = width/2, height/2

local function px(x, scale) -- physical x
  return x * scale + halfwidth
end

local function lx (x, scale) -- logical x
  return (x - halfwidth) / scale
end

local function py (y, scale) -- physical y
  return y * -scale + halfheight
end

local function ly (y, scale) -- logical y
  return -(y - halfheight) / scale
end

local function cross(scale)
  local s = 5 --> size of the marks
  local step

  if scale > 10 then
    step = 1
  elseif scale > 1 then
    step = 10
  else
    step = 0
  end

  c:thickness(1)
  c:color "black"
  c:clear()

  c:line (1, halfheight, width, halfheight)
  c:line (halfwidth, 1, halfwidth, height)

  if step > 0 then
    for x = step, lx(width, scale), step do
      c:line (px(x, scale), halfheight - s,
              px(x, scale), halfheight + s)
    end

    for x = -step, lx(1, scale), -step do
      c:line (px(x, scale), halfheight - s,
              px(x, scale), halfheight + s)
    end

    for y = step, ly(1, scale), step do
      c:line (halfwidth - s, py(y, scale),
              halfwidth + s, py(y, scale))
    end

    for y = -step, ly(height, scale), -step do
      c:line (halfwidth - s, py(y, scale),
              halfwidth + s, py(y, scale))
    end
  end
end

local function function_error()
  error("The function was not in a correct notation", 0)
end

local function plot(f)
  local choice
  local animate = true
  local scale = scale

  repeat
    cross(scale)

    c:thickness(2)
    c:color "royal blue"  --> don't tell my old teacher what pen I use ;-)

    local old_y = math.huge --> something offscreen

    for x=1,width do
      local y = py(f(lx(x, scale)), scale)

      if math.abs(old_y - y) < height
        then c:lineto (x, y)
        else c:moveto (x, y) --> don't draw huge jumps
      end

      -- show each time we reach a full value and it's visible
      if animate and x % scale == 0 and y >= 1 and y <= height then
        c:show()
        avt.wait(0.2)
      end

      old_y = y
    end

    c:show() --> show final result
    animate = false --> only animate the first time

    choice = avt.navigate "+-x"
    if "+"==choice then scale = scale * 1.5
    elseif "-"==choice then scale = scale / 1.5
    end
  until "x"==choice
end

local function plot_string(s)
  local fs = loadstring("function f(x) return "..s.." end")
  if fs and pcall(fs) then
    -- check if it produces a number
    local okay, v = pcall(f, 1)
    if not okay or type(v)~="number" then function_error() end
    plot(f)
  else
    function_error()
  end
end

local funcstring = arg[1]
if not funcstring then
  avt.set_balloon_height(2)
  avt.say "Enter the function:\n"
  funcstring = avt.ask "f(x)="
end

if funcstring and funcstring ~= "" then
  plot_string(funcstring)
end

