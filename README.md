# clamSoup

## Description
clamSoup is a small anti-virus scanner for Linux.
we use Qt5 for the GUI and we used libclamav for the virus engine.
It lets you update and run scans without useing the command line.
it is made to be as quick and as easy as possable.

## Installation

1. Download .deb file [here](https://github.com/Frankmau5/clamSoup/tree/master/bin/deb) 
2. Now run ```sudo dpkg -i  clamSoup_1.0.0.0_x86.deb``` in a terminal that is in the directory you downloaded it to

clamSoup is now installed on your computer.
type ```clamSoup ``` into your terminal to run clamSoup.

**NOTE** a .desktop file will be added soon. That way you don't have to run clamSOup witht the command line

## Source code

1. clone the repo 
2. you will need [QT5 and QT5 creator](https://www.qt.io/download-open-source).
3. Now that you have qt5 installed. open the .pro file with qt creator

## License

clamSoup uses the **GNU GENERAL PUBLIC LICENSE Version 2** [clamSoup license](https://github.com/Frankmau5/clamSoup/blob/master/LICENSE.md)

1. Qt5 - [QT5 and QT5 creator](https://www.qt.io/download-open-source)
2. poco - [poco](https://github.com/pocoproject/poco/blob/master/LICENSE)
3. libclamav - [clamav](https://github.com/Cisco-Talos/clamav-devel)

## FAQ
1. Why use clamsoup? - clamSoup trys to keep scanning for viruses simple so new users can scan easiys 
2. Is there a windows version? - No there is not right now but I have started work on it.