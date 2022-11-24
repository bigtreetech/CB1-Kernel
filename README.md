# Debian Build for AllWinner H616


## Tested Hardwares

* [Yuzuki Chameleon](https://github.com/YuzukiHD/YuzukiChameleon)
  - XR829 wireless module driver is not added yet. We are working on this.
- [BigTreeTech CB1](https://github.com/bigtreetech/CB1)

## build prerequisites

On Ubuntu22.04

``` zsh
sudo apt-get install ccache debian-archive-keyring debootstrap device-tree-compiler dwarves gcc-arm-linux-gnueabihf jq libbison-dev libc6-dev-armhf-cross libelf-dev libfl-dev liblz4-tool libpython2.7-dev libusb-1.0-0-dev pigz pixz pv swig pkg-config python3-distutils qemu-user-static u-boot-tools distcc uuid-dev lib32ncurses-dev lib32stdc++6 apt-cacher-ng aptly aria2 libfdt-dev libssl-dev
```

## Versions Included

Part   | Version
-------|--------
kernel | 5.16.17
uboot  | 2021.10

> uboot config: Allwinner-H616/u-boot/configs/h616_defconfig

## Build

In the project's root directory, run:

``` bash
sudo ./build.sh
```
