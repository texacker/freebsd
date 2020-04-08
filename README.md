# FreeBSD 使用笔记

## 安装 FreeBSD Base

### [Download](https://www.freebsd.org/where.html) media and [write](https://www.freebsd.org/doc/en_US.ISO8859-1/books/handbook/bsdinstall-pre.html#bsdinstall-usb) image to USB
```bash
wget https://download.freebsd.org/ftp/releases/amd64/amd64/ISO-IMAGES/11.1/FreeBSD-11.1-RELEASE-amd64-mini-memstick.img.xz
wget https://download.freebsd.org/ftp/releases/amd64/amd64/ISO-IMAGES/11.1/CHECKSUM.SHA256-FreeBSD-11.1-RELEASE-amd64
unxz FreeBSD-11.1-RELEASE-amd64-mini-memstick.img.xz
shasum -a 256 -c CHECKSUM.SHA256-FreeBSD-11.1-RELEASE-amd64
dd if=FreeBSD-11.1-RELEASE-amd64-mini-memstick.img of=/dev/da0 bs=1M conv=sync
```

### System-Wide Configuration
    /boot/loader.conf
    /etc/rc.conf
    /etc/make.conf

### Install Ports Collection
```bash
( /usr/sbin/portsnap fetch extract ; cd /usr/ports ; make index )
```

## 安装 Packages

### ports-mgmt
```bash
make -C /usr/ports/ports-mgmt/pkg build install clean
make -C /usr/ports/ports-mgmt/portupgrade build install clean

portinstall ports-mgmt/portmaster
portinstall ports-mgmt/pkg-rmleaf
```

### Root Certificate
```bash
portinstall security/ca_root_nss
```

### xorg
```bash
portinstall x11/xorg
portinstall x11/xlockmore
portinstall x11-wm/obconf x11-wm/obmenu x11-themes/lxappearance
portinstall x11/tint devel/py-xdg
portinstall chinese/ibus-libpinyin
```

### Desktop Utilities
```bash
portinstall devel/geany
portinstall x11-fm/xfe
portinstall graphics/mupdf
portinstall www/firefox
```

## 配置
### xterm
    ~/.Xresources.d/uxterm

### fontconfig
    /usr/local/etc/fonts/fonts.conf
    ~/.config/fontconfig/fonts.conf
    ~/.local/share/fonts

#### 关于 fontconfig
> fontconfig 的功能有点令人费解：fontconfig 的用法似乎是在系统中安装够多的字体，
> 然后当应用程序提出一个字体要求时，fontconfig 找到那个以某种匹配度来衡量的最合适的那个字体。
>
> 对大多数应用程序（除了 InDesign 这类重度字体相关的），用户感兴趣的是展现的内容，
> 并依据用户的个人审美和需要以一个一致的方式展现出来（例如整个桌面环境都统一使用新宋体或者 Noto 字体），
> 而应用程序不需要也不应该去要求和依赖类似于使用哪个字体这样的具体的展现形式。
>
> 但是令人费解的是 fontconfig 的功能重点似乎是在帮助而不是屏蔽应用程序的这种错误要求？

### 配置 xorg
    ~/.Xresources
    ~/.Xresources.d
    ~/.xinitrc

## 运行 xorg
```bash
#!/bin/csh

# AMD Radeon Graphics
startx

# Intel Graphics
#
# In some cases, after loading KMS driver, the system console is blank, even though it is still working.
# sudo /sbin/kldload i915kms && startx
#
# If you want to automatically load intel video driver at boot time,
# we recommend to do it from /etc/rc.conf:
# kld_list="i915kms"
#
# Loading the driver from /boot/loader.conf is not recommended and may not even work.
# Loading i915kms like this is known to cause a hang early in boot with a black screen.
```

## 更新

### 更新 ports tree

```bash
#!/bin/csh

# See http://www.freebsd.org/doc/en_US.ISO8859-1/books/handbook/updating-upgrading-portsnap.html

# The first time portsnap is executed, use fetch and extract to install the downloaded files:
# /usr/sbin/portsnap fetch extract
/usr/sbin/portsnap fetch update

# or using svn :
# # ( cd /usr ; rm -fr ./ports )
# # ( mkdir -p /usr/ports ; /usr/bin/svnlite co https://svn.freebsd.org/ports/head /usr/ports )
# /usr/bin/svnlite up /usr/ports

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

### [重装 Ports](https://www.freebsd.org/cgi/man.cgi?query=portmaster&apropos=0&sektion=0&manpath=FreeBSD+11.1-RELEASE+and+Ports&arch=default&format=html#end)
```bash
#!/bin/csh

# 1. 保留安装记录
portmaster --list-origins > ~/installed-port-list

# 2. Update and delete all ports
# update the ports tree
portmaster -ty --clean-distfiles
portmaster -Faf
pkg delete -afy

# 3. 清理
rm -rf /usr/local/lib/compat/pkg
# back up any files in /usr/local you wish to save, such as configuration files in /usr/local/etc
# manually check /usr/local and /var/db/pkg to make sure that they are really empty

# 4. Reinstall all ports
```

### 更新 src tree

```bash
#!/bin/csh

# Updating the Source
# See :
#   https://www.freebsd.org/doc/en_US.ISO8859-1/books/handbook/makeworld.html#updating-src-obtaining-src
#   https://www.freebsd.org/releng/
#   https://svnweb.freebsd.org/

# The first time :
# ( cd /usr ; rm -fr ./src )
# ( mkdir -p /usr/src ; /usr/bin/svnlite co https://svn.freebsd.org/base/releng/10.3 /usr/src )

# Switch to a different branch :
# /usr/bin/svnlite switch ^/releng/12.1 /usr/src

/usr/bin/svnlite up /usr/src

# Please check /usr/src/UPDATING before updating.
```

### 更新 FreeBSD
```bash
#!/bin/csh

# See: https://www.freebsd.org/doc/en_US.ISO8859-1/books/handbook/makeworld.html

make -C /usr/src cleanworld
make -C /usr/src -j4 buildworld
make -C /usr/src -j4 kernel
echo ready to reboot...

# Log in as single user mode:

# If the system is formatted with UFS:
# fsck -p
# mount -u /
# mount -a -t ufs
# swapon -a

# If the system is instead formatted with ZFS:
# zfs set readonly=off zroot
# zfs mount -a

# Adjust the local time CMOS clock:
# adjkerntz -i

# Install World:
# mergemaster -p
# make -C /usr/src installworld
# mergemaster -FUi
# yes | make -C /usr/src delete-old

# reboot again
```

### 更新 FreeBSD

```bash
#!/bin/csh

# FreeBSD Update
# See http://www.freebsd.org/doc/en_US.ISO8859-1/books/handbook/updating-upgrading-freebsdupdate.html
/usr/sbin/freebsd-update fetch
/usr/sbin/freebsd-update install
```

### 截屏
```bash
portinstall graphics/scrot
( cd /paht/to/save/screenshot ; scrot -c -d 5 ; switch_to_desktop_with_Alt-Tab )
```

## Ports Management

```bash
#!/bin/csh

# 1. List all root and leaf ports
portmaster --list-origins

# 2. Remove leaf packages
pkg-rmleaf

# 3. Upgrade installed packages, and all those packages depending on the given
#    packages as well, even if it is just a reinstall of the same version
portupgrade -fr <pkgname_glob>
```

## References
1. [FreeBSD Handbook](https://www.freebsd.org/doc/en_US.ISO8859-1/books/handbook/).
1. [FreeBSD 简明用户指南](http://bsdelf.github.io/posts/freebsd-brief-user-guide).

