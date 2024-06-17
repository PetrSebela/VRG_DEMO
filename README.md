# Simple simulation of projectiles using external balistics
This project was made as part of job interview for VR Group, a.s. 

# Usage
`./sim sx sy sz tx ty tz projectile_velocity time_step`
- `s[xyz]` - shooter position 
- `t[xyz]` - target position 
- `projectile_velocity` - initial projectile velocity
- `time_step` - how much time passes between each simulation step

**NOTE** : `y` axis is considered to point upwards


# Build
All dependencies are resolved resolved during first build
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Tests
```
$ cd tests
$ cmake .
$ make
```

# Dependencies
- [EnTT](https://github.com/skypjack/entt) 
    - A fast and reliable entity component system (ECS)
    - Automaticaly fetched during first build
- [Catch2](https://github.com/catchorg/Catch2) 
    - A modern, C++-native, test framework for unit-tests
    - Automaticaly fetched during first build of `tests.cpp`