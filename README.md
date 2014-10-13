NVR
===
git clone --recursive https://github.com/dprotopopov/NVR.git
cd NVR


# http://nodejs.org/download/

#Installing npm
curl http://npmjs.org/install.sh | sh

npm install -g bower
bower

cd boost
./bootstrap.sh
./b2


cd bower_components/tinyxml_2_6_2
make

cd bower_components/tinyxpath_1_3_1
#In order to build TinyXPath under Linux:
make -f Makefile.configure
./configure 
make


cd NVR
make
