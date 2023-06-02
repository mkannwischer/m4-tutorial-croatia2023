# Installation
Be aware that this tutorial will require you to run some kind of Linux.
It was tested with Arch Linux and Ubuntu, but other distributions are likely
fine. MacOS users may be able to make it work, but we cannot offer support
in case it does not.
Windows users are advised to install a virtual machine (e.g., using
[Virtual Box](https://www.virtualbox.org/)) running Ubuntu.


1) Clone this repository recursively:
```
git clone --recurse-submodule https://github.com/mkannwischer/m4-tutorial-croatia2023
```

2) Install [qemu](https://www.qemu.org/) (>= 5.2):
```
# Ubuntu
sudo apt install qemu-system-arm
# Arch
sudo pacman -S qemu-system-arm
```

You can test if you have the right version installed by
```
qemu-system-arm --version
```

3) Install the [Arm gcc toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads).
I recommend using the `11.3.Rel1` version: 
 - Download the `AArch32 bare-metal target (arm-none-eabi)` toolchain for your platform, e.g., `arm-gnu-toolchain-11.3.rel1-x86_64-arm-none-eabi.tar.xz` if you are running Linux on a `x86_64` machine.
 - Unpack the archive, e.g.,
```
sudo tar -xf arm-gnu-toolchain-11.3.rel1-x86_64-arm-none-eabi.tar.xz -C /opt/
```
 - Add binaries to your $PATH, for example append the following to your `~/.bashrc` (e.g., via `gedit ~/.bashrc`)
```
export PATH=/opt/arm-gnu-toolchain-11.3.rel1-x86_64-arm-none-eabi/bin:$PATH
```
  - Reload your `~/.bashrc` via `source ~/.bashrc`

You should now be able to run
```
 arm-none-eabi-gcc --version
```

Note: `arm-none-eabi-gcc` is also available from the `apt`, but is often outdated.
It is recommended to get it directly from the Arm website.

4) For on-board testing, install st-link:
```
# Ubuntu
sudo apt install stlink-tools
# Arch
sudo pacman -S stlink
```

# Pre-tutorial instructions

Please make sure that the 'helloworld' example successfully outputs
```
Hello Croatia 2023!
```
when run in qemu prior to attending the tutorial.
Installation instructions are available above.

You can build and run the example using the following steps:
```
cd helloworld
make
make run-qemu
```

## Running on actual hardware

If you happen to have a STM32F407 discovery board, you can also use that for the
tutorial.
Make sure the board is hooked up as described in https://github.com/mkannwischer/STM32-getting-started/.

You can flash and run the example by
```
cd helloworld
make clean
make PLATFORM=stm32
st-flash write bin/stm32f407-test.bin 0x8000000
```

You should be able to receive the output by running
```
screen /dev/ttyUSB0 38400

# or (better if pyserial is installed)

pyserial-miniterm /dev/ttyUSB0 38400
```
in another terminal and pressing the reset button of the board.

# Tutorial instructions

Follow the instructions on the slides (XXX: will be posted before the tutorial)
