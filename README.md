# **AgarPlusPlus**

*Agar version in C++*

## ***About*** :

AgarPlusPlus is a perfect game for everyone.
It's about our lives, how easy to get everything you ever want and loose it in one moment.
It's about confidence to strangers, making coalitions with them to reach the top of the world.
And then loosing everything in the case of lost battle or betrayal. Agar - game of life.

Here you can find backed and frontend.

* Frontend is a web-page, which runs simple JS to play.
* Backend is a server written in C++.


## ***Minimal requirements*** :

* `C++14*` ( `Visual C++ 14.1*` or `GCC 6.2.0*`
* `CMake 3.9*` ( + FindBoost module for `Boost 1.66.0*`
* libraries
  - `Boost 1.66.0*`
  - `RapidJSON 1.1.0*`
  - `WebRTC M63*`
* Web-brower:
  - Mozilla Firefox 58
  - Google Chrome or Chromium (version 64)

`*` - 100 % it will work with current version, but not guarantee in work with higher versions.

## ***Installation*** :

Currently server can be started on 2 platforms: Linux, Windows.

### Libraries:
The libraries you need to run could be downloaded and stored by our scripts.
If you have all the libraries installed you can [GO](#server) to the next step.

* **Linux** :
  - Install all libraries: `./scripts/install_all_libs.sh` (TODO)

  or

  - Boost 1.66.0: `./scripts/install_boost.sh`
  - WebRTC M63: `./scripts/install_webrtc.sh`
  - RapidJSON 1.1.0: `./scripts/install_rapidjson.sh` (TODO)

    *In the case of running this scripts, you should add some routs in* `LocalConfig.cmake`.

    *Example: *`set(BOOST_ROOT "/home/user666/AgarPlusPlus/ExternalLibs/boost_1_66_0/)`

* **Windows** : (TODO)
    In progress.

Hurray! Libraries have been installed.

### Server:
The best way to run build server is:
```bash
mkdir build
cd built
cmake ../
make
```
Server has been built. Niiiiiiice.

### GUI:
Frontend is ready for work. You can place wherever you want on your computer.

## ***Run***:
All preparations have been made. Now lets run the game.
### Server:
In `build/` folder run `./agar`
### GUI:
To run game frontend you have to open `/frontend/Main.html` in the browser and press START!



## ***Authors***:
* shakandrew
* m-sobieszek
