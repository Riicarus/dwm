/* See LICENSE file for copyright and license details. */

/* appearance */
#include <X11/X.h>
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 6;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const int vertpad	    = 10;       /* vertical padding of bar */
static const int sidepad            = 10;	/* horizontal padding of bar */
static const char *fonts[]          = { "SourceCodePro-Regular:size=12", "WenQuanYi Micro Hei:size=12" };	/* Require: nerd-fonts-complete */
static const char dmenufont[]       = "Sauce Code Pro:size=12";	/* Require: nerd-fonts-complete */
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
// static const char col_cyan[]        = "#005577";
static const char col_lime[]        = "#2b333e";
static const char col_moon_white[]  = "#eef7f2";
static const unsigned int baralpha  = 0xd0;
static const unsigned int borderalpha  = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_lime,  col_moon_white },
};

static const unsigned int alphas[][3]      = {
    	/*               fg      bg        border*/
    	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ NULL,       NULL,       NULL,       0,            False,       -1 },
};

/* layout(s) */
static const float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol	arrange function */
	{ "[T]",	tile },
	{ "[F]", 	NULL },
	{ "[M]",	monocle },
};

/* key definitions */
#define MODKEY1 Mod1Mask	// alt
#define MODKEY 	Mod4Mask	// win
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
/* Require: rofi */
static const char *dmenucmd[] = { "rofi", "-combi-modi", "drun,ssh", "-font", "hack 10", "-show", "combi", "-theme", "~/.config/rofi/config.rasi", "-show-icons", NULL };	/* Require: rofi & theme */
static const char *termcmd[]  = { "st", NULL };
static const char *volupcmd[] = { "/home/riicarus/scripts/dwm/vol_up.sh", NULL };	/* Require: alsamixer */
static const char *voldowncmd[] = { "/home/riicarus/scripts/dwm/vol_down.sh", NULL };	/* Require: alsamixer */
static const char *voltogglecmd[] = { "/home/riicarus/scripts/dwm/vol_tog.sh", NULL };	/* Require: alsamixer */

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ControlMask, 		XK_p, 	   spawn,          SHCMD("flameshot gui") },
	{ MODKEY, 		        XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY, 		        XK_F1, 	   spawn,          {.v = voltogglecmd } },
	{ MODKEY, 		        XK_F2,	   spawn,          {.v = voldowncmd } },
	{ MODKEY, 		        XK_F3, 	   spawn,          {.v = volupcmd } },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,             		XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },	/* Require: patch-tagshift */
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,           		XK_comma,  shiftview,      {.i = -1 } },
	{ MODKEY,           		XK_period, shiftview,      {.i = +1 } },
	{ MODKEY|ShiftMask, 		XK_comma,  shifttag,       {.i = -1 } },
	{ MODKEY|ShiftMask, 		XK_period, shifttag,       {.i = +1 } },
	{ MODKEY|ControlMask,           XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask, XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ControlMask|ShiftMask, XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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

