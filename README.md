# FreeBSD 使用笔记

## 安装
### ports-mgmt
    portinstall ports-mgmt/pkg
    portinstall ports-mgmt/portmaster
    portinstall ports-mgmt/portupgrade
    portinstall ports-mgmt/pkg_cleanup

### xorg
    portinstall x11/xorg
    portinstall x11-themes/lxappearance x11-wm/obconf x11-wm/obmenu
    portinstall devel/py-xdg x11/tint
    portinstall lang/python3 chinese/ibus-libpinyin

### Desktop Utilities
    portinstall devel/geany
    portinstall x11-fm/xfe
    portinstall graphics/mupdf
    portinstall www/firefox

## 配置
### xterm
    ~/.Xresources.d/uxterm

### fontconfig
    /usr/local/etc/fonts/fonts.conf
    ~/.config/fontconfig/fonts.conf
    ~/.local/share/fonts
