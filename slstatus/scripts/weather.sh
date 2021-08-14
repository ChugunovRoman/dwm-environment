#!/bin/bash

dwm_weather() {
    DATA=$(curl -s http://wttr.in/%D0%9D%D0%B8%D0%B6%D0%BD%D0%B8%D0%B9+%D0%9D%D0%BE%D0%B2%D0%B3%D0%BE%D1%80%D0%BE%D0%B4?0)
    echo -e "$DATA"
}

dwm_weather