#!/usr/bin/awk -f

# Convert textfile into C-code (new awk)
#
# Example-Usage:
# ./text2c README > readme.c
# ./text2c name=info_txt README > readme.c
#
# Copyright (c) 2009 Andreas K. Foerster
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

NR == 1 {
  if (name == "" && FILENAME != "-") {
    name = FILENAME
    gsub (/\./, "_", name) # replace . with _
    gsub (/-/, "_", name)  # replace - with _
    }

  if (name == "") name = "text"

  print "const char " name "[] ="
}

{
  # mask backslashes - must be first
  gsub (/\\/, "\\\\")

  # mask quotation marks
  gsub (/\"/, "\\\"")

  # print with quotation marks and \n at the end
  print "\"" $0 "\\n\""
}

END { print ";" }