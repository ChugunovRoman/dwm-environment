#!/bin/bash
# xhost +local:ruut
# xhost +name:ruut
# Xnest :1 -geometry 800x600
# Xephyr -screen 800x600 :3 &
# xinit ~/.xinitrc -- /usr/bin/Xephyr -ac -screen 800x600 :1 &
startx -- /usr/bin/Xephyr -ac -screen 800x600 :1 &
# /usr/bin/Xephyr -ac -screen 800x600 -reset :3 &


# export DISPLAY=:1
# xinit ~/.xinitrc -- `which Xnest` :1 -geometry 1920x1080

# Xnest -geometry "1024x768+64+0" -name "Xnest - DWM" :3 &

# DISPLAY=:3.0
# export DISPLAY

# echo "Starting DWM on $DISPLAY"

# DISPLAY=:3.0 ./dwm
