#!/bin/bash
# Script to get and install latest version of graph-analyzer.

# remove old version
cd /home/jsejdak
if [ -e "graph-analyzer" ] then
	rm -R graph-analyzer
fi

# download new version
git clone https://kuba.sejdak:gS7CK4qK5Xu3@code.google.com/p/graph-analyzer/
cd graph-analyzer
git config --add user.name kuba.sejdak@gmail.com
git config --add user.email kuba.sejdak@gmail.com

# replace necessary file paths
sed -i 's/\/kuba\/projects/\/jsejdak/g' bin/clearGraphFiles.sh
sed -i 's/\/kuba\/projects/\/jsejdak/g' conf/apache.conf
sed -i 's/\/kuba\/projects/\/jsejdak/g' web_gui/web_gui/settings.py

# prepare environment
if [ -e /var/www/jsejdak/analyzer.conf ] then
	sudo rm /var/www/jsejdak/analyzer.conf
fi
sudo cp conf/analyzer.conf /var/www/jsejdak

if [-e /var/www/jsejdak/graphs ] then
	sudo rm -R /var/www/jsejdak/graphs
fi
sudo mkdir /var/www/jsejdak/graphs
sudo chmod 777 /var/www/jsejdak/graphs

if [ -e /var/www/jsejdak/analyer_log ] then
	cat /dev/null > /var/www/jsejdak/analyzer_log
else
	touch /var/www/jsejdak/analyzer_log
	sudo chmod 777 /var/www/jsejdak/analyzer_log
fi

# build
mkdir build
cd build
cmake ..
chmod 777 /home/jsejdak/graph-analyzer/web_gui/static/tmp_graphs

# install
sudo make install
