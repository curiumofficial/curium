
Debian
====================
This directory contains files used to package curiumnd/curiumn-qt
for Debian-based Linux systems. If you compile curiumnd/curiumn-qt yourself, there are some useful files here.

## curium: URI support ##


curiumn-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install curiumn-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your curiumqt binary to `/usr/bin`
and the `../../share/pixmaps/curium128.png` to `/usr/share/pixmaps`

curiumn-qt.protocol (KDE)

