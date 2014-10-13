NVR
===

# Step 1 - Update your system
sudo apt-get update
sudo apt-get install git-core curl build-essential openssl libssl-dev

# if you’re on a Debian-based distribution like Ubuntu, try apt-get:
sudo apt-get install git

git clone --recursive git://github.com/dprotopopov/NVR.git
cd ~/NVR

# http://nodejs.org/download/
apt-get install curl
curl -sL https://deb.nodesource.com/setup | bash -
apt-get install -y nodejs build-essential npm

# Installing bower
npm install -g bower
bower update --allow-root

cd ~/NVR/boost
./bootstrap.sh
./b2

cd ~/NVR/bower_components/tinyxml_2_6_2
make clean && make

cd ~/NVR/bower_components/tinyxpath_1_3_1
apt-get install -y automake
#In order to build TinyXPath under Linux:
autoreconf --force --install
make -f Makefile.configure
./configure 
make clean && make

cd ~/NVR
make clean && make
./nvr
