AgarPlusPlus
============
*Agar version in C++*

About :
-------
AgarPlusPlus is a perfect game for everyone

Requirements :
--------------
- C++14 (MSVC (>=v141) or GCC (>=6.2.0))
- CMake (>=3.10) and FindBoost module version with support for Boost 1.66.0
- libraries ( [Installation] )
  - Boost (>=1.66.0)
  - RapidJSON (>=1.1.0)
  - WebRTC (>=M63)

Installation :
-------------
There are 2 platforms, where you can start server : Linux, Windows.
* ### Linux :
    - #### Libraries:
        All the libraries should be in folder ExternalLibs. For Boost and WebRTC we have installation scripts
        - Boost : `./scripts/install_boost.sh`
        - WebRTC : `./scripts/install_webrtc.sh`
