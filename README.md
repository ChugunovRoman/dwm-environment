DWM environment
---------------
![](https://raw.githubusercontent.com/ChugunovRoman/dwm-environment/master/screenshots/screenshot_2.png)

Includes:
---------------
* dwm 6.1
* dmenu 4.6
* slock 1.4
* Rust status bar (need rust and cargo on target platform for build)

Install binary
------------
- git clone git@github.com:ChugunovRoman/dwm-environment.git
- cd dwm-environment
- sduo ./install_precompiled

Build and Install
------------
- git clone git@github.com:ChugunovRoman/dwm-environment.git
- cd dwm-environment
- ./build
- sudo ./install

Usage
-----
Either run directly or add to your .sessionrc:
`rust-dwm-status &` <br>
Choose the dwm in gdm and login in your system

My default keybindings:
-----


Exec application: <br>
* **Alt+d** - exec dmenu <br>
* **Alt+w** - exec google-chrome <br>
* **Alt+a** - exec audacious <br>
* **Alt+x** - exec code <br>
* **Alt+t** - exec gnome-terminal <br>
* **Alt+e** - exec neno --no-desktop <br>

Change layouts: <br>
* **Shift+ModKey+t** - set "tile" layout <br>
* **Shift+ModKey+f** - set "float" layout <br>
* **Shift+ModKey+m** - set "monocle" layout <br>
* **Shift+ModKey+g** - set "grid" layout <br>
* **Shift+ModKey+d** - set "dwindle" layout <br>
* **Shift+ModKey+s** - set "spiral" layout <br>
* **Shift+ModKey+c** - set "bstack" layout <br>
* **Shift+ModKey+h** - set "bstackhoriz" layout <br>

All keybinding is in the dwm/config.h fiile

Note
-----
* Mod key is Super (win key) by default
* By default the bar is hidden. For show bar use **ModKey+Space** keybinding
* All keybinding you can change in dwm/config.h file