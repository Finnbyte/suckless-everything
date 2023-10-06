//Modify this file to change what commands output to your statusbar, and recompile using the make command.

static char city[] = "Turku"; // Weather info

static const Block blocks[] = {
	/*Icon*/	/*Command*/		/*Update Interval*/	/*Update Signal*/
  /* {"CPU: ", "vmstat 1 2 | tail -1 | awk '{print 100-$15}'",	30,		0}, */
	{" ⛅ ", "curl wttr.in/Turku?format='%t(%f)'", 200, 0},
	{"📅 ", "date '+%b %d (%a) %H:%M'",					5,		0},
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim[] = " | ";
static unsigned int delimLen = 5;
