#!/bin/sh

[ -r $HOME/.Xresources ] && xrdb $HOME/.Xresources

## XIM: fcitx
#export XMODIFIERS='@im=fcitx'
#export GTK_IM_MODULE=fcitx
#export GTK3_IM_MODULE=xim
#exec /usr/local/bin/fcitx &

## XIM: ibus, see https://github.com/ibus/ibus/wiki/ReadMe
export GTK_IM_MODULE=ibus
export GTK3_IM_MODULE=xim
export QT_IM_MODULE=ibus
export XMODIFIERS='@im=ibus'

## Start ibus:

## In GNOME
#export XIM=ibus
#export XIM_PROGRAM="ibus-daemon"
#export XIM_ARGS="--daemonize --xim"

## In other WM: openbox, dwm
exec /usr/local/bin/ibus-daemon --daemonize --xim --replace &

## Status bar (in dwm)
#while true; do
#  xsetroot -name "$(date "+%m-%d %u %R")"
#  sleep 15
#done &

xset fp= /usr/local/share/fonts/misc/
#xset fp+ /usr/local/share/fonts/TTF/
xset fp rehash
xset b off

## Launch WM
#exec /usr/local/bin/gnome-session
#exec ck-launch-session gnome-session
#exec /usr/local/bin/mate-session
#exec /usr/local/bin/startxfce4
#exec /usr/local/bin/startlxde
exec /usr/local/bin/openbox-session
#exec /usr/local/bin/dwm

