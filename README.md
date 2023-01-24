# year-2-gps-co-ordinates-to-graph-plot
Example work from my 2nd Year University C/C++ Module. A program to take GPS plots in the format: "XX.XXXXXX, YY.YYYYYY, DD/MM/YYYY, HH:MM:SS", clean messy data and then generate a graph via gpulot.

Program data is output in the same directory as .exe

## Code Overview:
"main.c"		- Main method is executed through this file and subsequently the entire project. Calls commands to read in configuration file and handle menu choices. Handles command line input

"menu.h"
"menu.c"		- Handles the printing of the menus in 'main.c' and returning of user input.

"configuration.h"
"configuration.c"	- Handles reading and/or creation of configuration settings and file.

"gps_plot.h"
"gps_plot.c"	- Handles creation of .data and .csv files. Creates plot linked list. Calls gnuplot on .data files and removes them.

## Libraries Used:
<stdio.h>		- 'main.c', 'menu.h', 'configuration.h' & 'gps_plot.h'
<stdlib.h>		- 'main.c', 'menu.h', 'configuration.h' & 'gps_plot.h'
<string.h>		- 'main.c',  'configuration.h' & 'gps_plot.h'
<time.h>		- 'gps_plot.h'
<math.h>		- 'gps_plot.h'

Program runs via CMD, "Program.exe" filename <Optional>

[Where 'filename' has preconfigured settings saved within it.]


## License
See LICENSE attached in repository.
