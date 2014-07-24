#!/bin/sh

cd ..

#Assuming that you have gcc 4.x.x and wget on your machine
#LATEST_PIN=pin-2.3-16358-gcc.4.0.0-ia32-linux.tar.gz
#LATEST_PIN=pin-2.2-15865-gcc.4.0.0-ia32-linux.tar.gz
#LATEST_PIN=pin-2.2-15113-gcc.4.0.0-ia32-linux.tar.gz
#LATEST_PIN=pin-2.2-14297-gcc.4.0.0-ia32-linux.tar.gz
LATEST_PIN=pin-2.2-13635-gcc.4.0.0-ia32-linux.tar.gz
DOWNLOAD_URL=http://rogue.colorado.edu/cgi-bin/Pin/download.pl?file=$LATEST_PIN
echo "Download pin " $LATEST_PIN
wget -qc $DOWNLOAD_URL

echo "Untarring it in " $PWD
tar xzf $LATEST_PIN
#remove pin symlink if present
rm -f pin
echo "Creating a simlink pin"
ln -s `basename $LATEST_PIN .tar.gz` pin


