#!/bin/bash
# Script to download and install latest version of graph-analyzer.

##### remove old version #####
cd /home/jsejdak
if [ -e "graph-analyzer" ]
then
	rm -R graph-analyzer
fi

##### download new version #####
git clone https://kuba.sejdak:gS7CK4qK5Xu3@code.google.com/p/graph-analyzer/
cd graph-analyzer
git config --add user.name kuba.sejdak@gmail.com
git config --add user.email kuba.sejdak@gmail.com

##### replace necessary file paths #####
sed -i 's/\/kuba\/projects/\/jsejdak/g' scripts/clearGraphFiles.sh
sed -i 's/\/kuba\/projects/\/jsejdak/g' conf/apache.conf
sed -i 's/\/kuba\/projects/\/jsejdak/g' src/gui/www/www/settings.py

##### prepare environment #####

# GraphAnalyzer.conf
if [ -e /var/www/jsejdak/GraphAnalyzer.conf ]
then
	sudo rm /var/www/jsejdak/GraphAnalyzer.conf
fi
sudo cp conf/GraphAnalyzer.conf /var/www/jsejdak

# GraphAnalyzerTasks.xml
if [ -e /var/www/jsejdak/GraphAnalyzerTasks.xml ]
then
    sudo rm /var/www/jsejdak/GraphAnalyzerTasks.xml
fi
sudo cp conf/GraphAnalyzerTasks.xml /var/www/jsejdak

# clear.sh
if [ -e /var/www/jsejdak/clear.sh ]
then
    sudo rm /var/www/jsejdak/clear.sh
fi
sudo cp scripts/clear.sh /var/www/jsejdak

# graphs/
if [ ! -e /var/www/jsejdak/graphs ]
then
	sudo mkdir /var/www/jsejdak/graphs
fi

# GraphAnalyzer.log
if [ -e /var/www/jsejdak/GraphAnalyer.log ]
then
	cat /dev/null > /var/www/jsejdak/GraphAnalyzer.log
else
	touch /var/www/jsejdak/GraphAnalyzer.log
fi

# copy install.sh itself
cp scripts/install.sh /home/jsejdak

##### build #####
mkdir build
cd build
cmake ..
sudo chmod -R 777 /home/jsejdak/graph-analyzer
sudo chmod -R 777 /var/www/jsejdak

##### install #####
sudo make install
