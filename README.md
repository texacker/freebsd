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
/usr/sbin/portsnap fetch extract && make -C /usr/ports index
```

## 安装 Packages

### Preparation
```bash
make -C /usr/ports/security/sudo fetch-recursive build install clean
sudo make -C /usr/ports/sysutils/tmux fetch-recursive build install clean
```

### ports-mgmt
```bash
sudo make -C /usr/ports/ports-mgmt/portupgrade build install clean

sudo portinstall ports-mgmt/pkg
sudo portinstall ports-mgmt/portmaster
sudo portinstall ports-mgmt/pkg-rmleaf
```

### Root Certificate
```bash
sudo portinstall security/ca_root_nss
```

### CLI Utilities
```bash
sudo portinstall editors/vim-console
sudo portinstall sysutils/htop
sudo portinstall archivers/unrar
```

### xorg
```bash
sudo portinstall x11/xorg
sudo portinstall x11/xlockmore
sudo portinstall x11-wm/obconf x11-wm/obmenu x11-themes/lxappearance
sudo portinstall x11/tint devel/py-xdg
sudo portinstall chinese/ibus-libpinyin
```

### Desktop Utilities
```bash
sudo portinstall devel/geany
sudo portinstall x11-fm/xfe
sudo portinstall graphics/mupdf
sudo portinstall www/firefox
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
>
> 参考：[Font Configuration](https://wiki.archlinux.org/index.php/Font_configuration)

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

# Update the ports tree

# 1. 保留安装记录
pkg autoremove && portmaster --list-origins | sort > ~/.portmaster/installed-port-list

# 2. Update and delete all ports
portmaster -ty --clean-distfiles
portmaster -Faf
pkg delete -afy

# 3. 清理
#
# a. rm -rf /usr/local/lib/compat/pkg

# b. back up any files in /usr/local you wish to save, such as configuration files in :
#    /usr/local/etc

# c. manually check to make sure that they are really EMPTY :
#    /usr/local
#    /var/db/pkg

# 4. Reinstall :
#    ports-mgmt/pkg
#    ports-mgmt/portmaster

#    reinstall others :
#    portmaster --no-confirm `cat ~/.portmaster/installed-port-list`
```

### 更新 src tree

```bash
#!/bin/csh

# Download the Latest Source Code
#   https://riptutorial.com/freebsd/example/23734/download-the-latest-source-code

# SVN
#   https://www.freebsd.org/doc/en_US.ISO8859-1/books/handbook/makeworld.html#updating-src-obtaining-src
#   https://www.freebsd.org/releng/
#   https://svnweb.freebsd.org/

# The first time :
# ( cd /usr ; rm -fr ./src )
# ( mkdir -p /usr/src ; /usr/bin/svnlite co https://svn.freebsd.org/base/releng/10.3 /usr/src )

# Switch to a different branch :
# /usr/bin/svnlite switch ^/releng/12.2 /usr/src

/usr/bin/svnlite up /usr/src

# TARBALL
# wget http://ftp.freebsd.org/pub/FreeBSD/releases/amd64/12.2-RELEASE/src.txz
# tar -C /usr/src -Jxvf src.txz

# Please check /usr/src/UPDATING before updating.
```

### 更新 FreeBSD（旧）
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

# See: https://www.freebsd.org/doc/en_US.ISO8859-1/books/handbook/makeworld.html

# === Build ===

sudo make -C /usr/src cleanworld
sudo mergemaster -p

sudo make -j 4 -C /usr/src buildworld
sudo make -j 4 -C /usr/src buildkernel

# === Install ===

# a complete cleaning of all ports, if desired.

sudo make -C /usr/src installkernel

sudo shutdown -r now

sudo make -C /usr/src installworld
sudo mergemaster -FUi

# === Restart ===

sudo shutdown -r now

# make -C /usr/src check-old
sudo make BATCH_DELETE_OLD_FILES=yes -C /usr/src delete-old

# make -C /usr/src check-old-libs
sudo make BATCH_DELETE_OLD_FILES=yes -C /usr/src delete-old-libs

# The CMOS clock keeps local time, rather than UTC time.
sudo touch /etc/wall_cmos_clock

sudo tzsetup && sudo ntpdate 0.pool.ntp.org
```

### 更新 FreeBSD

```bash
#!/bin/csh

# FreeBSD Update
# See http://www.freebsd.org/doc/en_US.ISO8859-1/books/handbook/updating-upgrading-freebsdupdate.html
sudo /usr/sbin/freebsd-update fetch
sudo /usr/sbin/freebsd-update install
```

## Ports Management

```bash
#!/bin/csh

# 1. List all installed ports
pkg info

# 2. List all installed ports by category
portmaster -L

# 3. List all root and leaf ports
portmaster --list-origins

# 4. Remove leaf packages
pkg-rmleaf

# 5. Upgrade installed packages, and all those packages depending on the given
#    packages as well, even if it is just a reinstall of the same version
portupgrade -fr <pkgname_glob>

# 6. Only fetch recursively
portinstall -FR <pkgname_glob>

# 7. portsnap: snapshot corrupt
#rm /var/db/portsnap/tag && portsnap fetch extract
sudo rm -fr /var/db/portsnap/* && sudo portsnap auto
```

## Utilities

### 截屏
```bash
sudo portinstall graphics/scrot
( cd /paht/to/save/screenshot ; scrot -c -d 5 ; switch_to_desktop_with_Alt-Tab )
```

### ImageMagick - Command-line Tools: Convert
```bash
# See: https://imagemagick.org/script/convert.php
#      https://legacy.imagemagick.org/Usage/

sudo portinstall graphics/ImageMagick7-nox11
```

## References
1. [FreeBSD Handbook](https://www.freebsd.org/doc/en_US.ISO8859-1/books/handbook/).
1. [FreeBSD 简明用户指南](http://bsdelf.github.io/posts/freebsd-brief-user-guide).
1. [使用 BSD 作为桌面系统](https://dieken.gitlab.io/posts/bsd-as-desktop-system/).
