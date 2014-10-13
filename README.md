NVR
===

# Step 1 - Update your system
sudo apt-get update
sudo apt-get install git-core curl build-essential openssl libssl-dev

# if you’re on a Debian-based distribution like Ubuntu, try apt-get:
sudo apt-get install git

git clone --recursive git://github.com/dprotopopov/NVR.git
cd NVR


# http://nodejs.org/download/
sudo apt-get update
sudo apt-get install nodejs

# Installing npm
curl http://npmjs.org/install.sh | sh

# Installing bower
npm install -g bower
bower


cd boost
./bootstrap.sh
./b2

cd ..

cd bower_components/tinyxml_2_6_2
make clean && make

cd ../..

cd bower_components/tinyxpath_1_3_1
#In order to build TinyXPath under Linux:
make -f Makefile.configure
./configure 
make clean && make

cd ../..

make clean && make

./nvr
