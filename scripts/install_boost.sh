#!/bin/bash

# Boost install

##############
# Config
##############

# Boost version
VERSION="1_66_0"

# In this folder there will be folders include, lib, src and boost archive
DESTINATION=ExternalLibs/boost_$VERSION

# Comment next line to install everything
LIBS="system program_options"

##########################
# Implementation
##########################
DOTVERSION=$(echo $VERSION | sed -e 's/_/./g')
for LIB in $LIBS; do
    WITH="$WITH --with-$LIB"
done

set -x && \
cd .. && \
mkdir -p $DESTINATION && \
cd $DESTINATION && \
rm -rf * && \
wget https://downloads.sourceforge.net/project/boost/boost/$DOTVERSION/boost_$VERSION.tar.gz && \
tar -xvzf boost_$VERSION.tar.gz && \
mv boost_$VERSION src && \
cd src && \
mkdir build && \
./bootstrap.sh --with-toolset=gcc define="_GLIBCXX_USE_CXX11_ABI=0" --with-libraries=program_options,system,test && \
./b2 --prefix=.. --build-dir=build install -j 4 --toolset=gcc architecture=x86 address-model=64 define="_GLIBCXX_USE_CXX11_ABI=0" --stagedir=".\stage64" threading=multi runtime-link=static link=static --layout=tagged --with-program_options --with-system --with-test release debug stage && \
cd .. 
rm -rf src/ boost_$VERSION.tar.gz
