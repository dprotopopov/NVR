NVR
===

# Step 1 - Update your system
apt-get update && apt-get install git-core curl build-essential openssl libssl-dev
# if you’re on a Debian-based distribution like Ubuntu, try apt-get:
apt-get install git && git clone --recursive git://github.com/dprotopopov/NVR.git
# http://nodejs.org/download/
cd ~/NVR && apt-get install curl && curl -sL https://deb.nodesource.com/setup | bash - && apt-get install -y nodejs build-essential npm
# Installing bower
npm install -g bower && bower update --allow-root
# to build boost under Linux:
cd ~/NVR/boost && ./bootstrap.sh && ./b2
# to build TinyXml under Linux:
cd ~/NVR/bower_components/tinyxml_2_6_2 && make clean && make
# to build TinyXPath under Linux:
cd ~/NVR/bower_components/tinyxpath_1_3_1 && apt-get install -y automake && autoreconf --force --install && make -f Makefile.configure && ./configure && make clean && make
# build and run
cd ~/NVR && git pull && make clean && make && ./nvr
