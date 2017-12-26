#!/bin/bash

# WebRTC install

##############
# Config
##############

# Name of the package you want to download | see all the packages there :
# https://github.com/sourcey/webrtc-precompiled-builds/blob/master/
PACKAGE=webrtc-18252-6294a7e-linux-x64

# Destination
DESTINATION=ExternalLibs/WebRTC

##########################
# Implementation
##########################

set -x && \
cd .. && \
mkdir -p $DESTINATION && \
cd $DESTINATION && \
rm -rf * && \
wget -O $PACKAGE.tar.gz https://github.com/sourcey/webrtc-precompiled-builds/blob/master/$PACKAGE.tar.gz?raw=true
tar -xvzf $PACKAGE.tar.gz && \
rm -rf $PACKAGE.tar.gz
