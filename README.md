# CampoLampone2025
This is repo for Firmware, PCBs, Images, schemes and stuff

## Setup

1) Clone with all the submodules
```
	git clone <repository-name-ssh-or-https> --recursive
```

2) Add TinyUSB to SDK
```
	cd pico-sdk && git submodule update --init
```

3) Create build dir and build da project!
```
	mkdir build && cd build
	cmake ..
	make -j<number of jobs>
```

## TODO

- [ ] Check when to move from pico-sdk develop branch to main branch