/* See LICENSE file for copyright and license details. */

#include "funcions/movestack.c" // функция для циклического перемещения окон

// Доп. слои
#include "layouts/grid.c"
#include "layouts/fibonacci.c"
#include "layouts/bstack.c"
#include "layouts/bstackhoriz.c"

/* appearance */
static const char *fonts[] = {
	"monospace:size=10",
	"Terminus:size=10",
	"FontAwesome:size=10"
};
static const char dmenufont[]       = "monospace:size=12";
static const char normbordercolor[] = "#282c34"; // Цвет рамки неактивного окна
static const char normbgcolor[]     = "#1c1f25"; // Цвет не выделенного фона панели
static const char normfgcolor[]     = "#bbbbbb"; // Цвет не выделенного текста
static const char selbordercolor[]  = "#555555"; // Цвет рамки активного окна
static const char selbgcolor[]      = "#282c34"; // Цвет выделенного фона
static const char selfgcolor[]      = "#eeeeee"; // цвет выделенного текста

static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, 0: display systray on the last monitor*/
static const int showsystray        	 = 1;        /* 0 means no systray */
static const unsigned int borderpx  	 = 1;        /* border pixel of windows */
static const unsigned int snap      	 = 28;       /* snap pixel */
static const int showbar            	 = 1;        /* 0 means no bar */
static const int topbar             	 = 1;        /* 0 means bottom bar */
static const unsigned int mousewrap		 = 1; // 1 - enable mouse warp, 0 - disable mouse warp

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "\uF009",          tile },    /* ◫ first entry is default */ /* \x25EB */
	{ "𝌇",              tile2 },    /* ◫ first entry is default */ /* \x25EB */
	{ "\uF24D",   	     NULL },    /*  no layout function means floating behavior */ /* \xF24D */
	{ "\uF2D0",          monocle },    /* ▇ */
	{ "\uF00A",          grid },    /* ▇ */
	{ "\u259A",          dwindle },    /* ▇ */
	{ "\u259E",          spiral },    /* ▇ */
	{ "\uF0C9",          bstack },    /* ▇ */
	{ "\uF0CA",          bstackhoriz },    /*  */
};

/* tagging */
// icon			default layout
static const Tag tags[] = {
	{ "",		&layouts[7] },
	{ "",		&layouts[3] },
	{ "",		&layouts[0] },
	{ "",		&layouts[3] },
	{ "",		&layouts[1] },
	{ "\uF03E",	&layouts[0] },
	{ "",		&layouts[3] },
	{ "\uF013",	&layouts[0] },
	{ "",		&layouts[0] }
};

#include "funcions/shiftview.c" // Перемещение по тегам вперед/назад

// Привязка окон к тегам и мониторам
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 * xprop | grep -i class
	 */
	/* class            									instance    title       				tags mask     border width    isfloating   monitor */
	{ "Gnome-terminal", 									NULL,       NULL,       				 1,            borderpx,             0,           0 },

	{ "Code",           									NULL,       NULL,       				 1 << 1,       borderpx,             0,            1 },
	{ "Sublime_text",   									NULL,       NULL,       				 1 << 1,       borderpx,             0,           -1 },
	{ "Atom", 		    									NULL,       NULL,       				 1 << 1,       borderpx,             0,           -1 },
	{ "QtCreator", 											NULL,       NULL,       				 1 << 1,       borderpx,             0,            0 },
	{ "jetbrains-clion", 									NULL,       NULL,       				 1 << 1,       borderpx,             0,            0 },

	{ "Firefox",        									NULL,       NULL,       				 1 << 2,       borderpx,             0,           -1 },
	{ "luakit",         									NULL,       NULL,       				 1 << 2,       borderpx,             0,           -1 },
	{ "Google-chrome",  									NULL,       NULL,       				 1 << 2,       borderpx,             0,           0 },
	{ "Chromium-browser",  									NULL,       NULL,       				 1 << 2,       borderpx,             0,           0 },
	{ "Yandex-browser-beta", 								NULL,       NULL,       				 1 << 2,       borderpx,             0,           -1 },

	{ "pcmanfm-qt",     									NULL,       NULL,       				 1 << 3,       borderpx,             0,           0 },
	{ "Pcmanfm",        									NULL,       NULL,       				 1 << 3,       borderpx,             0,           0 },
	{ "Nautilus",       									NULL,       NULL,       				 1 << 3,       borderpx,             0,           0 },
	{ "Nemo",           									NULL,       NULL,       				 1 << 3,       borderpx,             0,           0 },
	{ "Postman",           									NULL,       NULL,       				 1 << 3,       borderpx,             0,           0 },
	{ "Skype",           									NULL,       NULL,       				 1 << 3,       borderpx,             0,           0 },
	{ "Sqlectron",           								NULL,       NULL,       				 1 << 3,       borderpx,             0,           0 },
	{ "NoSQLBooster for MongoDB",   						NULL,       NULL,       				 1 << 3,       borderpx,             0,           0 },
	{ "Electron",   										NULL,       NULL,       				 1 << 3,       borderpx,             0,           0 },
	{ "zoom",   											NULL,       NULL,       				 1 << 3,       borderpx,             0,           0 },
	{ "FML early loading progress", 						NULL,       NULL,       				 1 << 3,       borderpx,             0,           0 },
	{ "org-tlauncher-tlauncher-rmo-TLauncher",  			NULL,       NULL,       				 1 << 3,       borderpx,             0,           0 },
	{ "net-minecraftforge-installer-SimpleInstaller",   	NULL,       NULL,       				 1 << 3,       borderpx,             0,           0 },

	{ "Audacious",      									NULL,       NULL,       				 1 << 4,       borderpx,             0,           0 },
	{ "TelegramDesktop",									NULL,       NULL,       				 1 << 4,       borderpx,             0,           0 },

	{ "Gimp",           									NULL,       NULL,       				 1 << 5,       borderpx,             0,           -1 },
	{ "Photoshop.exe",  									NULL,       NULL,       				 1 << 5,       borderpx,             0,           -1 },
	{ "Inkscape",       									NULL,       NULL,       				 1 << 5,       borderpx,             0,           -1 },

	{ "Mainwindow.py",  									NULL,       NULL,       				 1 << 6,       borderpx,             0,           -1 },
	{ "Steam",  											NULL,       NULL,       				 1 << 6,       borderpx,             0,           -1 },
	{ "steam_app_109600",  									NULL,       NULL,       				 1 << 6,       0,             	 	 0,           -1 },
	{ "neverwinter.exe",  									NULL,       NULL,       				 1 << 6,       0,             	 	 0,           -1 },
	{ "warfork.x86_64",  									NULL,       NULL,       				 1 << 6,       0,             	 	 0,           -1 },
	{ "csgo_linux64",  										NULL,       NULL,       				 1 << 6,       0,             	 	 0,           -1 },

	{ "VirtualBox",     									NULL,       NULL,       				 1 << 7,       borderpx,             0,           0 },

	{ "qBittorrent",    									NULL,       NULL,       				 1 << 8,       borderpx,             0,           -1 },

	{ "bd",    												NULL,       NULL,       				 1 << 1,       borderpx,             1,           -1 },

	{ "Wine",    											NULL,       NULL,       				 NULL, 	       borderpx,             1,           0 },
	{ "Zenity",    											NULL,       NULL,       				 NULL, 	       borderpx,             1,           0 },
	{ "Gnome-calendar",    									NULL,       NULL,       				 NULL, 	       borderpx,             1,           -1 },
	{ "Gnome-calculator",    								NULL,       NULL,       				 NULL, 	       borderpx,             1,           -1 },
	{ "File-roller",	    								NULL,       NULL,       				 NULL, 	       borderpx,             1,           0 },
	{ "Minecraft 1.7.10",	    							NULL,       NULL,       				 NULL, 	       borderpx,             1,           0 },
	{ "org-tlauncher-tlauncher-rmo-TLauncher",  			NULL,       NULL,       				 NULL, 	       borderpx,             1,           0 },
	{ "dotnet",  											NULL,       "OpenSAGE Big Editor",       NULL, 	       borderpx,             1,           0 },
};

/* layout(s) */
static const float mfact     = 0.60; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

/* key definitions */
#define MODKEY Mod4Mask
#define ALT Mod1Mask
#define SHIFT ShiftMask
#define CTRL ControlMask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|CTRL,           		KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|SHIFT,             	KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|CTRL|SHIFT, 			KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]      		 = { "rofi", "-show", "drun", "-theme", "arc-red-dark", NULL };
// static const char *dmenucmd[]      		 = { "dmenu_run", "-m", dmenumon, "-i", "-l", "15", "-H", HOME"/.dmenu_history", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *dmenuSysCmd[]      	 = { "dmenu_sys", "-m", dmenumon, "-i", "-l", "15", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *dmenuClipCmd[]      	 = { "rofi", "-theme", "arc-red-dark", "-modi", "clipboard:greenclip print", "-show", "clipboard", "-run-command", "{cmd}", NULL };
static const char *dmenuRecentCmd[]      = { "rofi", "-show", "run", "-theme", "arc-red-dark", NULL };
// static const char *dmenuRecentCmd[]      = { "dmenu_recent", "-m", dmenumon, "-i", "-l", "15", "-H", HOME"/.dmenu_history", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *dmenuFilesCmd[]       = { "dmenu_files", HOME, HOME, "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *dmenuScriptsCmd[]     = { "dmenu_xdotool", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *rofiSelectedToTabbedCmd[]     = { "rofi_select_to_tabbed.pl", NULL };
static const char *rofiFocusedToTabbedCmd[]     = { "rofi_focused_to_tabbed.pl", NULL };

static const char *termcmd[]       		 = { "gnome-terminal", "--hide-menubar", NULL };
static const char *chromecmd[]     		 = { "chromium-browser", NULL };
static const char *googlechromecmd[]     = { "google-chrome-stable", NULL };
static const char *yandexcmd[]     		 = { "yandex-browser", NULL };
static const char *audaciouscmd[]  		 = { "audacious", NULL };
static const char *sublimecmd[]    		 = { "sublime_text", NULL };
static const char *vscodecmd[]     		 = { "code", NULL };
static const char *atomcmd[]       		 = { "atom", NULL };
static const char *firefoxcmd[]	   		 = { "firefox", NULL };
static const char *pcmanfmcmd[]	   		 = { "pcmanfm-qt", NULL };
static const char *supcmanfmcmd[]  		 = { "gksu", "pcmanfm-qt", NULL };
static const char *virtualboxcmd[] 		 = { "virtualbox", NULL };
static const char *suvirtualboxcmd[] 	 = { "gksu", "virtualbox", NULL };
static const char *photoshopcmd[]  		 = { "/usr/share/playonlinux/playonlinux", "--run", "'Adobe Photoshop CS6'", NULL };
static const char *gimpcmd[]       		 = { "gimp", NULL };
static const char *nemocmd[]       		 = { "nemo", "--no-desktop", NULL };
static const char *nemosucmd[]     		 = { "gksu", "nemo", "--no-desktop", NULL };

static const char *passcmd[]       		 = { "gedit", "/media/ruut/ssd/Soft/Programming/3.txt", NULL };
static const char *hostcmd[]       		 = { "gedit", "/media/ruut/ssd/Soft/Programming/hosts.php", NULL };
static const char *notescmd[]      		 = { "gedit", "/media/ruut/ssd/Soft/Programming/notes.txt", NULL };

static const char *gsacmd[]       		 = { "shutter", "-s", NULL };
static const char *gswcmd[]       		 = { "shutter", "-w", NULL };
static const char *gsfcmd[]       		 = { "shutter", "-f", NULL };
// static const char *gsaccmd[]       = { "gnome-screenshot", "-a", "-c", NULL };
// static const char *gswccmd[]       = { "gnome-screenshot", "-w", "-c", NULL };
static const char *gsfccmd[]       		 = { "gnome-screenshot", "-a", NULL };
// static const char *screenshot_cmd[]		 	 = { "scrot", HOME"/Pictures/Screenshot_from_%F_%T.png", NULL };
// static const char *screenshot_area_cmd[]	 = { "scrot", "-s", "Screenshot_from_%F_%T.png", "-e", "'mv $f ~/Pictures/'", NULL };

// Scripts
static const char *toggleTouchpad[]  = { "bash", HOME"/sh/toggleTouchPad.sh", NULL };

// Change sound
static const char *upVolume5pre[]      = { "amixer", "set", "Master", "5%+", NULL };
static const char *upVolume2pre[]      = { "amixer", "set", "Master", "2%+", NULL };
static const char *downVolume5pre[]    = { "amixer", "set", "Master", "5%-", NULL };
static const char *downVolume2pre[]    = { "amixer", "set", "Master", "2%-", NULL };
// static const char *toggleVolume[]  = { "amixer", "set", "Master", "toggle", NULL }; // for debian
static const char *toggleVolume[]  = { "amixer", "-D", "pulse", "set", "Master", "1+", "toggle", NULL }; // for ubuntu
// static const char *upVolume[]      = { "amixer", "-D", "pulse", "sset", "Master", "5%+", NULL }; // for ubuntu
// static const char *downVolume[]    = { "amixer", "-D", "pulse", "sset", "Master", "5%-", NULL }; // for ubuntu
// static const char *toggleVolume[]  = { "amixer", "-D", "pulse", "sset", "Master", "toggle", NULL }; // for ubuntu

static const char *toggleAudacious[]  = { "audacious", "-t", NULL };
static const char *nextAudacious[]    = { "audacious", "-f", NULL };
static const char *rewAudacious[]     = { "audacious", "-r", NULL };

static const char *xkillcmd[]	   = { "xkill", NULL };
static const char *shutdowncmd[]   = { "shutdown", "-h", "+0", NULL };
static const char *rebootcmd[]	   = { "reboot", NULL };

static const char *slock[]	  	   = { "slock", NULL };

// Биндинги
static Key keys[] = {
	/* modifier                     key        C function        bash script */
	{ ALT,                          XK_d,      spawn,          {.v = dmenucmd } },
	{ ALT,                  		XK_r,      spawn,          {.v = dmenuRecentCmd } },
	{ MODKEY,                  		XK_r,      spawn,          {.v = dmenuSysCmd } },
	{ ALT,                  		XK_c,      spawn,          {.v = dmenuClipCmd } },
	{ ALT,                  		XK_f,      spawn,          {.v = dmenuFilesCmd } },
	{ ALT,                  		XK_s,      spawn,          {.v = dmenuScriptsCmd } },
	{ MODKEY,                  		XK_q,      spawn,          {.v = rofiSelectedToTabbedCmd } },
	{ MODKEY|SHIFT,                 XK_q,      spawn,          {.v = rofiFocusedToTabbedCmd } },

	{ ALT,                          XK_t,      spawn,          {.v = termcmd } },
	{ CTRL|ALT,                     XK_w,      spawn,          {.v = chromecmd } },
	{ ALT,                          XK_w,      spawn,          {.v = googlechromecmd } },
	{ ALT,                          XK_a,      spawn,          {.v = audaciouscmd } },
	{ ALT|SHIFT,                    XK_x,      spawn,          {.v = atomcmd } },
	{ ALT,		                    XK_x,      spawn,          {.v = vscodecmd } },
	{ MODKEY|SHIFT,                 XK_x,      spawn,          {.v = sublimecmd } },
	// { ALT,                          XK_f,	   spawn,	       {.v = firefoxcmd } },
	{ ALT,                          XK_y,	   spawn,	       {.v = yandexcmd } },
	{ ALT,                          XK_e,	   spawn,	       {.v = nemocmd } },
	{ ALT|SHIFT,                    XK_e,	   spawn,	       {.v = nemosucmd } },
	{ ALT,                          XK_p,	   spawn,	       {.v = pcmanfmcmd } },
	{ ALT|SHIFT,                    XK_e,	   spawn,	       {.v = supcmanfmcmd } },
	{ ALT,                          XK_v,	   spawn,	       {.v = virtualboxcmd } },
	{ ALT|SHIFT,                    XK_v,	   spawn,	       {.v = suvirtualboxcmd } },
	{ ALT,		                    XK_p,	   spawn,	       {.v = photoshopcmd } },
	{ ALT,      		            XK_g,	   spawn,	       {.v = gimpcmd } },

	{ SHIFT,      		            XK_Print,  spawn,	       {.v = gsfccmd } },
	// { SHIFT,      		            XK_Print,  spawn,	       {.v = gsacmd } },
	// { ALT,                          XK_Print,  spawn,	       {.v = gswcmd } },
	// { NULL,      		            XK_Print,  spawn,	       {.v = gsfcmd } },
	// { CTRL|SHIFT,      		        XK_Print,  spawn,	       {.v = gsaccmd } },
	// { CTRL|ALT|SHIFT,  		        XK_Print,  spawn,	       {.v = gswccmd } },
	{ NULL,         		        XK_Print,  spawn,	       {.v = gsfccmd } },

	{ MODKEY|ALT,      		            XK_1,	   spawn,	       {.v = passcmd } },
	{ MODKEY|ALT,      		            XK_2,      spawn,	       {.v = hostcmd } },
	{ MODKEY|ALT,      		            XK_3,      spawn,	       {.v = notescmd } },

	// { MODKEY,                       XK_r,	   spawn,	       {.v = rebootcmd } },
	// { MODKEY,                       XK_h,	   spawn,	       {.v = shutdowncmd } },
	{ MODKEY,                       XK_x,	   spawn,	       {.v = xkillcmd } },

	{ MODKEY,                       XK_F7,	   spawn,	       {.v = toggleTouchpad } },
	{ MODKEY,                       XK_F1,	   spawn,	       {.v = toggleVolume } },
	{ MODKEY,                       XK_F2,	   spawn,	       {.v = downVolume2pre } },
	{ MODKEY|ALT,                   XK_F2,	   spawn,	       {.v = downVolume5pre } },
	{ MODKEY,                   	XK_F3,	   spawn,	       {.v = upVolume2pre } },
	{ MODKEY|ALT,                   XK_F3,	   spawn,	       {.v = upVolume5pre } },

	{ MODKEY,                       XK_F5,	   spawn,	       {.v = toggleAudacious } },
	{ MODKEY,                       XK_F6,	   spawn,	       {.v = nextAudacious } },
	{ MODKEY,                       XK_F4,	   spawn,	       {.v = rewAudacious } },

	{ MODKEY,                       XK_l,	   spawn,	       {.v = slock } },

	{ MODKEY,                       XK_space,  togglebar,      {0} },
	{ ALT,                          XK_space,  showScratchPad, {0} },
	{ MODKEY,                       XK_s,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_w,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_a,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_d,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|SHIFT,                 XK_g,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|SHIFT,                 XK_d,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|SHIFT,                 XK_s,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY|SHIFT,                 XK_c,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY|SHIFT,                 XK_h,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY,                       XK_b,      setlayout,      {0} },
	{ MODKEY|SHIFT,                 XK_b,      togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|SHIFT,                 XK_0,      tag,            {.ui = ~0 } },
	// { MODKEY,                       XK_q,      focusmon,       {.i = -1 } },
	// { MODKEY,                       XK_e,      focusmon,       {.i = +1 } },
	// { MODKEY|ALT,                   XK_q,      tagmon,         {.i = -1 } },
	// { MODKEY|ALT,                   XK_e,      tagmon,         {.i = +1 } },
	{ MODKEY|ALT,                   XK_w,      movestack,      {.i = -1 } },
	{ MODKEY|ALT,                   XK_s,      movestack,      {.i = +1 } },
	{ MODKEY|ALT,                   XK_a,      shiftview,      {.i = -1 } },
	{ MODKEY|ALT,                   XK_d,      shiftview,      {.i = +1 } },
	{ MODKEY,                       XK_Left,   shiftview,      {.i = -1 } },
	{ MODKEY,                       XK_Right,  shiftview,      {.i = +1 } },
	// { MODKEY,                       XK_Left,   viewtoleft,     {0} },
	// { MODKEY,                       XK_Right,  viewtoright,    {0} },
	{ MODKEY|SHIFT,		            XK_Left,   tagtoleft,      {0} },
	{ MODKEY|SHIFT,		            XK_Right,  tagtoright,     {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|CTRL|SHIFT,                 XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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
