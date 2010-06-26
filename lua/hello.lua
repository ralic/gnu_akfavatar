#!/usr/bin/env lua-akfavatar

-- Copyright (c) 2009,2010 Andreas K. Foerster <info@akfoerster.de>
-- License: GPL version 3 or later

-- Note: Since this is a trivial example, you don't have to keep my
-- Copyright notice, but you may replace it with your own.

require "lua-akfavatar" --> makes sure Lua-AKFAvatar is used

avt.initialize{title="Hello World", avatar="default", mode=avt.window_mode}
avt.encoding("UTF-8") --> the strings in this program are in UTF-8
avt.set_text_delay() --> activate the slowprint mode (optional)
avt.move_in() --> move the avatar in (optional)
avt.set_balloon_size(7, 20) --> set the size of the balloon (optional)

-- say something:
avt.say [[
Hello world
Bonjour le monde
Hallo Welt
Hej Världen
Καλημέρα κόσμε
]]

avt.newline()
avt.say("π≈", math.pi) --> avt.say accepts strings and numbers
avt.wait_button() --> wait for a button to be pressed
avt.move_out() --> move the avatar out (optional)
