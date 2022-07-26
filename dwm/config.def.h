/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int snap      = 15;       /* snap pixel */

static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
 
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 5;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "hack:size=12" }; 
static const char dmenufont[]       = "hack:size=12";
static const char border_active[]   = "#f0f0f0";
static const char border_inactive[] = "#444444";
static const char col_red[]         = "#ff5454";
static const char col_gray1[]       = "#222222";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
  /*               fg         bg         border   */
  [SchemeNorm] = { col_gray3, col_gray1, border_inactive }, // Inactive window
  [SchemeSel]  = { col_gray4, col_cyan,  border_active }, // Selected window
  [SchemeStatus]  = { col_gray3, col_gray1,  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
  [SchemeTagsSel]  = { col_gray4, "#38aabd",  "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
  [SchemeTagsNorm]  = { col_gray3, col_gray1,  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
  [SchemeInfoSel]  = { col_gray4, col_gray1,  "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
  [SchemeInfoNorm]  = { col_gray3, col_gray1,  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

static const char *const autostart[] = {
  /* "sxhkd", NULL, */
  "dwmblocks", NULL,
  "emacs", "--daemon", NULL,
  "hsetroot", "-fill", "/home/bytz/.config/wallpapers/0042.jpg", NULL,
  NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII" };

static const Rule rules[] = {
  /* xprop(1):
   *	WM_CLASS(STRING) = instance, class
   *	WM_NAME(STRING) = title
   */
  /* class      instance    title       tags mask     isfloating   monitor */
  { "Gimp",     NULL,       NULL,       0,            1,           -1 },
  { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },

};

/* layout(s) */
static const float mfact     = 0.57; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "[]=",      tile },    /* first entry is default */
  { "><>",      NULL },    /* no layout function means floating behavior */
  { "[F]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
/* tag keys rewritten with keychords patch */
#define TAGKEYS(KEY,TAG)												\
	{1, {{MODKEY, KEY}},								view,           {.ui = 1 << TAG} },	\
	{1, {{MODKEY|ControlMask, KEY}},					toggleview,     {.ui = 1 << TAG} }, \
	{1, {{MODKEY|ShiftMask, KEY}},						tag,            {.ui = 1 << TAG} }, \
	{1, {{MODKEY|ControlMask|ShiftMask, KEY}},			toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* Keybinds and commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */

static const char *dmenucmd[] = {"dmenu_run", NULL};
static const char *dmenudesktopcmd[] = {"dmenu_desktop_file", NULL};
/* static const char *shutdowncmd[]  = { "sudo", "openrc-shutdown", "-p", "now", NULL }; */
/* static const char *rebootcmd[]  = { "sudo", "reboot", NULL }; */
static const char *termcmd[]  = { "st", NULL };
/* Emacs commands */
static const char *emacsclient[] = { "emacsclient", "-c", NULL };
static const char *emacsdired[] = { "emacsclient", "-c", "--eval", "(dired nil)", NULL };
static const char *emacsemms[] = { "emacsclient", "-c", "--eval", "(emms-play-directory-tree)", NULL };

/* Allow dwm to restart in place */
#include "selfrestart.c"

/* Explaining a bit of Keychord keychords[]:
   Keychords look a lot different than regular keybinds, but are pretty much the same, except for
   the "keys" part.

   Keychords start with a numeral, in this case 1 or 2, which specify if dwm treats the keychord as a regular
   keybind, or a keychord. 1 = regular keybind, 2 = keychord.

   Keychords are made of braces inside of braces. There are two outer braces, which are mandatory for everything.
   But inside these outer braces, you can have keys grouped by single braces. These are treated as keychords.

   It's easy to grasp this with a few examples:
   --------------------------------------------
   
  This line here is a regular keybind, which spawns terminal, when user presses Super + Shift + Return
  {1, {{MODKEY|ShiftMask, XK_Return}},     spawn,                  {.v = termcmd } },

  This line here is a keychord, which spawns an emacsclient, when user presses Super + e AND THEN e 
  {2, {{MODKEY, XK_e}, {0, XK_e}},         spawn,                  {.v = emacscmd } }, (0, XK_e means it will wait for a normal key, and not a MODKEY + a key)

  Keychords allow for more fluid keyboard control, since you can just release whatever was before the final part of the keychord.
  and because the second is a keychord, you can do another command with a keychord Super + e + m,
  which spawns a different thing than an emacsclient, for example a music player inside emacs!
*/

/* For moving windows up or down a stack with j and k */
#include "movestack.c"

static Keychord keychords[] = {
  /* keys                                  function                argument */
  /* Spawning commands */
  {1, {{MODKEY|ShiftMask, XK_Return}},     spawn,                  {.v = termcmd } },
  {1, {{MODKEY, XK_r}},                    spawn,                  {.v = dmenucmd } },
  {1, {{MODKEY, XK_d}},                    spawn,                  {.v = dmenudesktopcmd } },
  /* Emacs */
  {2, {{MODKEY, XK_e}, {0, XK_e}},         spawn,                  {.v = emacsclient } },
  {2, {{MODKEY, XK_e}, {0, XK_m}},         spawn,                  {.v = emacsemms } },
  {2, {{MODKEY, XK_e}, {0, XK_d}},         spawn,                  {.v = emacsdired } },
  /* Controlling windows */
  {1, {{MODKEY, XK_b}},                    togglebar,              {0} },
  {1, {{MODKEY, XK_j}},                    focusstack,             {.i = +1 } },
  {1, {{MODKEY, XK_k}},                    focusstack,             {.i = -1 } },
  {1, {{MODKEY, XK_i}},                    incnmaster,             {.i = +1 } },
  {1, {{MODKEY, XK_p}},                    incnmaster,             {.i = -1 } },
  {1, {{MODKEY, XK_h}},                    setmfact,               {.f = -0.05} },
  {1, {{MODKEY, XK_l}},                    setmfact,               {.f = +0.05} },
  {1, {{MODKEY|ShiftMask, XK_j}},          movestack,              {.i = +1 } },
  {1, {{MODKEY|ShiftMask, XK_k}},          movestack,              {.i = -1 } },
  {1, {{MODKEY, XK_Return}},               zoom,                   {0} },
  {1, {{MODKEY, XK_Tab}},                  view,                   {0} },
  {1, {{MODKEY, XK_x}},                    killclient,             {0} },
  {1, {{MODKEY|ShiftMask, XK_q}},          quit,                   {0} },
  {1, {{MODKEY, XK_t}},                    setlayout,              {.v = &layouts[0]} },
  {1, {{MODKEY, XK_f}},                    setlayout,              {.v = &layouts[1]} },
  {1, {{MODKEY, XK_m}},                    setlayout,              {.v = &layouts[2]} },
  {1, {{MODKEY, XK_space}},                setlayout,              {0} },
  {1, {{MODKEY|ShiftMask, XK_space}},      togglefloating,         {0} },
  {1, {{MODKEY|ShiftMask, XK_f}},          togglefullscr,          {0} },
  {1, {{MODKEY, XK_0}},                    view,                   {.ui = ~0 } },
  {1, {{MODKEY|ShiftMask, XK_0}},          tag,                    {.ui = ~0 } },
  {1, {{MODKEY, XK_comma}},                focusmon,               {.i = -1 } },
  {1, {{MODKEY, XK_period}},               focusmon,               {.i = +1 } },
  {1, {{MODKEY|ShiftMask, XK_comma}},      tagmon,                 {.i = -1 } },
  {1, {{MODKEY|ShiftMask, XK_period}},     tagmon,                 {.i = +1 } },
  TAGKEYS(                        XK_1,                      0)
  TAGKEYS(                        XK_2,                      1)
  TAGKEYS(                        XK_3,                      2)
  TAGKEYS(                        XK_4,                      3)
  TAGKEYS(                        XK_5,                      4)
  TAGKEYS(                        XK_6,                      5)
  TAGKEYS(                        XK_7,                      6)
  TAGKEYS(                        XK_8,                      7)
  {1, {{MODKEY|ShiftMask, XK_r}},              self_restart,   {0} },
  {1, {{MODKEY|ShiftMask, XK_q}},              quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
  /* click                event mask      button          function        argument */
  { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
  { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkWinTitle,          0,              Button2,        zoom,           {0} },
  { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
  { ClkTagBar,            0,              Button1,        view,           {0} },
  { ClkTagBar,            0,              Button3,        toggleview,     {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
