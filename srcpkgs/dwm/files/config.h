#include "push.c"
#include "X11/XF86keysym.h"

/* appearance */
static const char *fonts[] = {
	"terminus:pixelsize=16:antialias=false:autohint:false"
};
static const char dmenufont[]       = "terminus:pixelsize=16:antialias=false:autohint:false";
static const char normbordercolor[] = "black";
static const char normbgcolor[]     = "#000044";
static const char normfgcolor[]     = "gray90";
static const char selbordercolor[]  = "gray90";
static const char selbgcolor[]      = "gray90";
static const char selfgcolor[]      = "#000044";
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class           instance    title       tags mask     isfloating   monitor */
	{ "Gimp",          NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",       NULL,       NULL,       1 << 8,       0,           -1 },
        {  NULL,           NULL,      "irssi",     1 << 7,       0,           -1 },
	{ "GIMP",          NULL,       NULL,       1 << 5,       0,           -1 },
	{ "Rawtherapee",   NULL,       NULL,       1 << 6,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* application preferences */
#define TERM "st"
#define BROWSER "firefox"
#define MAIL "mutt"
#define IRC "weechat"

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */

/* launchers */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *term[]  = { TERM, NULL };
static const char *termwd[]  = { "pwd|st", NULL };
static const char *mail[]  = { TERM, "-t", MAIL, "-e",  MAIL, NULL};
static const char *editor[] = {"vim", "-t", "vim", "-e", "-vim", NULL};
static const char *browser[]  = { BROWSER, NULL};
static const char *lock[]  = { "slock", NULL };
static const char *irc[] = {"st", "-t", IRC, "-e", IRC, NULL};
static const char *raw[] = {"rawtherapee", NULL};
static const char *gimp[] = {"gimp", NULL};

/* media keys */
static const char *vold[] = { "amixer", "-q", "set", "Master", "5%-", "unmute", NULL };
static const char *volu[] = { "amixer", "-q", "set", "Master", "5%+", "unmute", NULL };
static const char *mute[] = { "amixer", "-q", "set", "Master", "toggle", NULL };
static const char *play[] = { "mpc", "toggle", NULL };
static const char *next[] = { "mpc", "next", NULL };
static const char *prev[] = { "mpc", "prev", NULL };
static const char *stop[] = { "mpc", "stop", NULL };
static const char *backlu[] = { "xbacklight", "-steps", "1", "-time", "0", "-inc", "5%", NULL };
static const char *backld[] = { "xbacklight", "-steps", "1", "-time", "0", "-dec", "5%", NULL };
static const char *prtscr[] = { "scrot", "~/temp/screenshot.png", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */

        /* launchers */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = term } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termwd } },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          {.v = mail } },
	{ MODKEY|ShiftMask,             XK_b,      spawn,          {.v = browser } },
	{ MODKEY,                       XK_c,      spawn,          {.v = irc } },
	{ MODKEY,                       XK_r,      spawn,          {.v = raw } },
	{ MODKEY,                       XK_g,      spawn,          {.v = gimp } },
	{ MODKEY,                       XK_v,      spawn,          {.v = editor } },

        /* window management */
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      pushdown,       {0} },
	{ MODKEY|ShiftMask,             XK_k,      pushup,         {0} },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = +1 } }, 
	{ MODKEY,                       XK_p,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ControlMask|ShiftMask, XK_l,      spawn,          {.v = lock } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_x,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

        /* media control */
        {0,                             XF86XK_AudioLowerVolume,    spawn,          {.v = vold } },
        {0,                             XF86XK_AudioRaiseVolume,    spawn,          {.v = volu } },
        {0,                             XF86XK_AudioMute,           spawn,          {.v = mute } },
        {0,                             XF86XK_AudioPlay,           spawn,          {.v = play } },
        {0,                             XF86XK_AudioNext,           spawn,          {.v = next } },
        {0,                             XF86XK_AudioPrev,           spawn,          {.v = prev } },
        {0,                             XF86XK_AudioStop,           spawn,          {.v = stop } },
        {0,                             XF86XK_MonBrightnessUp,     spawn,          {.v = backlu } },
        {0,                             XF86XK_MonBrightnessDown,   spawn,          {.v = backld } },
        /* OTHER */
        { MODKEY|ControlMask|ShiftMask, XK_p,                       spawn,          {.v = prtscr } },

        /* tagkeys */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,          {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = term } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

