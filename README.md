# Geometry Dash Gamesave Cryptor
## Download
- The latest Windows binary (.exe) can be downloaded [here](https://github.com/xypwn/gd-gamesave-cryptor/releases)

## Usage
- `Shift`  + `Right Click` in the folder you dropped the .exe file into
- Select `Open PowerShell window here`
- In the PowerShell window, type one of the following:
	- `.\gd_crypt -d` to decrypt and store the result in the current folder
	- `.\gd_crypt -e` to encrypt and store the result in the current folder
	- `.\gd_crypt -es` to encrypt and store the result in the Geometry Dash folder

## Building from source on Linux
#### Building for Linux
- If you are using Proton to play GD, do not use Wine for the Windows version. Use this version instead as it automatically uses the correct path for Proton.
- Clone the repository and cd into it

	`$ git clone https://github.com/xypwn/gd-gamesave-cryptor.git`

	`$ cd gd-gamesave-cryptor`
- Build the binary:

	`$ make`
- The executable is named `gd_crypt` and can be run with `./gd_crypt`
#### Building for Windows
- You will need to install your distro's version of `mingw-w64`
	- `sudo apt install mingw-w64` on Ubuntu
	- Install the AUR package `mingw-w64-gcc` on Arch Linux
- Clone the repository and cd into it

	`$ git clone https://github.com/xypwn/gd-gamesave-cryptor.git`

	`$ cd gd-gamesave-cryptor`
- Build the Windows binary:

	`$ make xcomp_linux_win`
-  The executable is named `gd_crypt.exe`