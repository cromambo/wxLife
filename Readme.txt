This program was made to learn wxWidgets and refresh my c++ knowledge. I was happy with my understanding of wxWidgets and the concept of the Game of Life, so I've moved on to try some other tech.

Work that could still be done:
Add an interface that lets the user choose the behaviour for cell evolution. Right now it has some interesting presets, like the Conway pattern and the Maze pattern. This interface would be like 16 checkboxs in a grid, for 0 to 8 neighbors living and 0-8 neighbors dead, and the checkbox would be whether, in that state, the cell should be alive or dead after an evolution. Additionally, there could be a textbox or two that lets the user put in a notation for the rule. For example, the Conway notation is B3/S23, which means dead cells become alive (B = birth) with 3 living cells around them, and living cells stay alive (S = survive) with 2 or 3 living cells around them.

Brush GUI for a 3x3 solid brush, 3x3 empty brush, or gliders for 4 different directions. Right now, shift+left click makes a glider that goes down and right. I put in a way to use the other brushes but there is no way to select which is used.

The board evolution algorithm could be rewritten to use the Hashlife algorithm, which as I understand it, finds patterns on the board and hashes them into their eventual result. Lots of math, I presume.

Doesn't have an installer/deployment method. Other computers will need some wxWidget binaries and any dlls that Visual Studio programs need. I am most likely to come back to this once I've tried out some installer packages on other work.

General UI testing and bugfixing. Code is obviously not production code, though I tried to comment the confusing stuff well.

Things that I feel aren't that great:
The GUI is poor, I don't care to make it pretty.

Unit testing module is sketchy, it was excellent for getting the board up and running but after that there was lots of GUI work and trying of new things in wxWidgets and I learned by messing around with them.

Exception handling is bleh.




MISCELLANEOUS STUFF
Manifest generation is off in debug to fix an incremental linking problem I was having(and didn't really understand).

The 2.9.4 vc10.0 precompiled wxWidgets binaries have a wx.h file (which is just a ton of includes) that didn't have includes for all the controls I wanted to use. If you get link errors like not being able to find the header for a wxStaticLine, find that class in the wxWidgets install and make sure the header for it is in the wx.h header. I put a copy of my header in, it might be useful, it was originally at C:\WX\2.9.4\include\wx

The intellisense fix header is really strange. Apparently, wxWidgets uses a lot of preprocessor and macro code in their include structure to support multiple platforms, and Visual Studio gets lost as to what headers intellisense should be looking at, so if I just include all those headers in a file(that isn't even included in any part of my program), intellisense can find them.