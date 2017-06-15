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

#### 关于 fontconfig
> 其实 fontconfig 的功能有点令人费解：fontconfig 的用法似乎是在系统中安装足够多的字体，
> 当应用程序提出一个字体要求时，fontconfig 找到那个以某种匹配度来衡量的最合适的那个字体。
> 对大多数应用程序，用户感兴趣的是展现的内容，并依据用户的个人审美和需要以一个统一的方式展现出来，
> 而应用程序不需要也不应该去要求和依赖展现的形式，
> 而 fontconfig 的功能重点似乎是在帮助而不是屏蔽应用程序的这种要求。

## 更新

### 更新 FreeBSD

```bash
#!/bin/csh

# FreeBSD Update
# See http://www.freebsd.org/doc/en_US.ISO8859-1/books/handbook/updating-upgrading-freebsdupdate.html
/usr/sbin/freebsd-update fetch
/usr/sbin/freebsd-update install
```

### 更新 src

```bash
#!/bin/csh

# Updating the Source
# See https://www.freebsd.org/doc/en_US.ISO8859-1/books/handbook/makeworld.html#updating-src-obtaining-src
/usr/bin/svnlite up /usr/src

# Please check /usr/src/UPDATING before updating.
```

### 更新 ports

```bash
#!/bin/csh

# See http://www.freebsd.org/doc/en_US.ISO8859-1/books/handbook/updating-upgrading-portsnap.html

# The first time portsnap is executed, use fetch and extract to install the downloaded files:
# /usr/sbin/portsnap fetch extract
/usr/sbin/portsnap fetch update

( cd /usr/ports; make index )
# or fetch INDEX instead of building it from ports tree
# ( cd /usr/ports; make fetchindex )

/usr/sbin/pkg audit -F
# or, in case of GFWed, :-/
# /usr/local/bin/proxychains /usr/sbin/pkg audit -F

/usr/sbin/pkg autoremove -n

/usr/local/sbin/portsclean -CDL -PP

# Please check /usr/ports/UPDATING before updating.
```
