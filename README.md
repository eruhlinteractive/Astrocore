**NOTE: THIS LIBRARY IS NOT INTENDED FOR PRODUCTION USE, AND IS CURRENTLY LACKING MANY FEATURES. USE AT YOUR OWN RISK**

# Astrocore

## What is it
Astrocore is a game-focused C++ library built on [Raylib](https://www.raylib.com/) that aims to provided generalized management systems. This includes systems such as transformation inheritance, scene and asset management, etc.

## Other Features:
- TMX tilemap loading
- Composition/node based entity structure


## Installation

**Prerequisites**
- [vcpkg](https://github.com/microsoft/vcpkg)

1. Install the following libraries with vcpkg: [Raylib](https://www.raylib.com/), [Box2D](https://box2d.org/)
2. Build and install the following libraries to your system: [tmxlite](https://github.com/fallahn/tmxlite)
3. Add astrocore to the Cmake build config


## Libraries Used
- [Raylib](https://www.raylib.com/) (base, rendering)
- [Box2D](https://box2d.org/) (physics)
- [tmxlite](https://github.com/fallahn/tmxlite) (tilemap parsing)
- [nlohmann json](https://github.com/nlohmann/json) (JSON parsing)
