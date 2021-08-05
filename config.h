#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */

/* 窗口边框像素 */
static const unsigned int borderpx  = 2;        

/* 捕捉像素 */
static const unsigned int snap      = 100;       

/* 0: sloppy systray follows selected monitor, 
 * >0: pin systray to monitor X 
 * */
static const unsigned int systraypinning = 0;   

/* 系统托盘间距*/
static const unsigned int systrayspacing = 1;   

/* 1: if pinning fails, 
 * display systray on the first monitor, 
 * False: display systray on the last monitor
 * */
static const int systraypinningfailfirst = 1;   

/* 0 means no systray */
static const int showsystray        = 1;     

/* horiz inner gap between windows */
static const unsigned int gappih    = 6;       

/* vert inner gap between windows */
static const unsigned int gappiv    = 6;       

/* horiz outer gap between windows and screen edge */
static const unsigned int gappoh    = 6;       

/* vert outer gap between windows and screen edge */
static const unsigned int gappov    = 6;       

/* 1 means no outer gap when there is only one window */
static const int smartgaps          = 1;        

/* The value to be subtracted based on the height of the font: bh = drw->fonts->h - besubfh; */
static const int besubfh            = 5;        

/* 0 means no bar */
static const int showbar            = 1;        

/* 0 means bottom bar */
static const int topbar             = 1;        

/* Switch view on tag switch */
static const Bool viewontag         = True;     

/* 字体列表 */
static const char *fonts[]          = { "Noto Sans Devanagari Black:size=14" };

/* 是否显示状态栏窗口管理方式 */
static const int showlayout         = 0; 

/* 是否在状态栏显示客户端 */ 
static const int showclient         = 0;

/*static const char norm_fg_color[]     = "#f0fcff"; */
static const char norm_fg_color[]     = "#f0fcff"; /*这一行指的是xsetroot得到的状态栏文字颜色*/
/*static const char norm_bg_color[]     = "#282a36"; */
static const char norm_bg_color[]     = "#312520"; /*这一行指的是状态栏背景颜色*/
/*static const char norm_border_color[] = "#282a36"; */
static const char norm_border_color[] = "#312520"; 
static const char sel_fg_color[]      = "#f0fcff"; 
static const char sel_bg_color[]      = "#312520"; 
static const char sel_border_color[]  = "#312520"; 
static const char hid_fg_color[]      = "#f0fcff"; 
static const char hid_bg_color[]      = "#312520"; 
static const char hid_border_color[]  = "#312520"; 

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm]    = { norm_fg_color, norm_bg_color, norm_border_color },
	[SchemeSel]     = { sel_fg_color,  sel_bg_color,  sel_border_color  },
	[SchemeHid]     = { hid_fg_color,  hid_bg_color,  hid_border_color  },
};

static const unsigned int norm_fg_alpha     = 0xD2;
static const unsigned int norm_bg_alpha     = 0xD2;
static const unsigned int norm_border_alpha = 0xD2;
static const unsigned int sel_fg_alpha      = 0xD2;
static const unsigned int sel_bg_alpha      = 0xD2;
static const unsigned int sel_border_alpha  = 0x10;
static const unsigned int hid_fg_alpha      = 0xD2;
static const unsigned int hid_bg_alpha      = 0xD2;
static const unsigned int hid_border_alpha  = 0xD2;
static const unsigned int baralpha          = 0xD2;

static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { norm_fg_alpha,     norm_bg_alpha, norm_border_alpha },
	[SchemeSel]  = { sel_fg_alpha,      sel_bg_alpha,  sel_border_alpha  },
	[SchemeHid]     = { hid_fg_alpha,   hid_bg_alpha,  hid_border_alpha  },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",               NULL,       NULL,       0,            1,           -1 },
	{ "Android Emulator",   NULL,       NULL,       0,            1,           -1 },
	{ "Emulator",           NULL,       NULL,       0,            1,           -1 },
	{ "quemu-system-i386",  NULL,       NULL,       0,            1,           -1 },
	{ "google-chrome",      NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* 主区域大小系数 [0.05..0.95] */
static const int nmaster     = 1;    /* 主区域客户端的数量 */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "﫳",      tile },    /* first entry is default */
	{ "ﳠ",      NULL },    /* no layout function means floating behavior */
	{ "",      monocle },
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

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]           = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[]            = { "st", NULL };
static const char *browsercmd[]         = { "google-chrome-stable", NULL };
static const char *changelightupcmd[]   = { "xbacklight", "-inc", "10", NULL };
static const char *changelightdowncmd[] = { "xbacklight", "-dec", "10", NULL };
static const char scratchpadname[]      = "scratchpad";
static const char *scratchpadcmd[]      = { "st", "-t", scratchpadname, "-g", "80x24", NULL };
static const char *screenshotcmd[]      = { "flameshot", "gui", NULL };
static const char *slockcmd[]           = {"slock", NULL};

static const char *upvol[]   = { "/home/ash/scripts/vol-up.sh",  NULL };
static const char *downvol[] = { "/home/ash/scripts/vol-down.sh",  NULL };
static const char *mutevol[] = { "/home/ash/scripts/vol-toggle.sh",  NULL };

static const char *wpcmd[]  = { "/home/ash/scripts/wp-change.sh", NULL };

static const char *rofi[] = { "rofi", "-show", "drun", "-theme", "onedark", NULL };
static const char *edge[] = { "microsoft-edge-dev", NULL };

static Key keys[] = {
	/* modifier            key                      function        argument */
	{ MODKEY,              XK_p,                    spawn,          {.v = dmenucmd } },
	{ MODKEY,              XK_Return,               spawn,          {.v = termcmd } },
	{ MODKEY,              XK_n,                    spawn,          {.v = rofi } },
	{ MODKEY,              XK_c,                    spawn,          {.v = browsercmd } },
	{ MODKEY|ShiftMask,    XK_c,                    spawn,          {.v = edge } },          
	{ MODKEY,              XK_F11,                  spawn,          {.v = downvol } },
	{ MODKEY|ControlMask,  XK_F10,                  spawn,          {.v = mutevol } },
	{ MODKEY,              XK_F12,                  spawn,          {.v = upvol   } },
	{ MODKEY|ShiftMask,    XK_F6,                   spawn,          {.v = changelightupcmd } },
	{ MODKEY|ShiftMask,    XK_F5,                   spawn,          {.v = changelightdowncmd } },
	{ MODKEY,              XK_b,                    spawn,          {.v = wpcmd } },
	{ 0,                   XK_Print,                spawn,          {.v = screenshotcmd } },
	{ MODKEY,              XK_grave,                togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY|ShiftMask,    XK_j,                    rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_k,                    rotatestack,    {.i = -1 } },
	{ MODKEY,              XK_j,                    focusstack,     {.i = +1 } },
	{ MODKEY,              XK_k,                    focusstack,     {.i = -1 } },
	{ MODKEY,              XK_h,                    viewtoleft,     {0} },
	{ MODKEY,              XK_l,                    viewtoright,    {0} },
	{ MODKEY|ShiftMask,    XK_h,                    tagtoleft,      {0} },
	{ MODKEY|ShiftMask,    XK_l,                    tagtoright,     {0} },
	{ MODKEY,              XK_t,                    incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_t,                    incnmaster,     {.i = -1 } },
	{ MODKEY|ControlMask,  XK_h,                    setmfact,       {.f = -0.05} },
	{ MODKEY|ControlMask,  XK_l,                    setmfact,       {.f = +0.05} },
	{ MODKEY,              XK_m,                    hidewin,        {0} },
	{ MODKEY|ShiftMask,    XK_m,                    restorewin,     {0} },
	{ MODKEY,              XK_o,                    hideotherwins,  {0}},
	{ MODKEY|ShiftMask,    XK_o,                    restoreotherwins, {0}},
	{ MODKEY|ShiftMask,    XK_Return,               zoom,           {0} },
	{ MODKEY,              XK_Tab,                  view,           {0} },
	{ MODKEY|ShiftMask,    XK_q,                    killclient,     {0} },
	{ MODKEY|ShiftMask,    XK_f,                    fullscreen,     {0} },
	{ MODKEY,              XK_apostrophe,           togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY|ShiftMask,    XK_n,                    togglealttag,   {0} },
	TAGKEYS(               XK_1,                      0)
	TAGKEYS(               XK_2,                      1)
	TAGKEYS(               XK_3,                      2)
	TAGKEYS(               XK_4,                      3)
	TAGKEYS(               XK_5,                      4)
	TAGKEYS(               XK_6,                      5)
	TAGKEYS(               XK_7,                      6)
	TAGKEYS(               XK_8,                      7)
	TAGKEYS(               XK_9,                      8)
	{ MODKEY|ControlMask,  XK_c,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
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

