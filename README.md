# p-Vex 
This bundle contains the 4.2 version of [p-Vex](http://rvex.ewi.tudelft.nl/).

## Getting Started
To run p-Vex you'll need [Ubuntu 14.04](http://releases.ubuntu.com/14.04/) Modelsim and [Vivado 2015.4](https://www.xilinx.com/member/forms/download/xef.html?filename=Xilinx_Vivado_SDK_2015.4_1118_2_Lin64.bin&akdm=1).

## Modelsim install

First, unzip the file with the command:<br>
`tar -xvzf setup.tar.gz`<br>

Then open the order file to follow the orders to install.<br>
Orders:<br>
  1- Edit the $user in set_mentor.sh file<br>
  2- Execute the set_mentor.sh file with 'sudo ./set_mentor.sh'. (you may change the permission with chmod +x with needed.)<br>
  3- Execute the ./deps.sh<br>
  4- Then ./modelsim-iso.sh<br>
  5- Inside the installation, the target can be your home/user/modelsim. The modelsim folder must be created.<br>
  6- Select all the files and wait to the installation to finish.<br>
  7- Execute the crack, which will open a wine window.<br>
  8- Open by hand the license.dat inside the modelsim folder and comment the first two lines following the order file.<br>
  9- Execute the libfreetype as the last instruction of the order file.<br>
  10- All set.<br>


## Vivado install

Orders:<br>
  1- Download the Vivado 2015-4.<br>
  2- Inside the installer don't update to the latest version.<br>
  3- Install the Vivado HL System Edition.<br>
  4- Select the target folder to be your home/user/ (because the settings.sh file inside the cache-test needs this path).<br>
  5- If the license manager opens you can close it.<br>


## Setting up after installing

Now you need to enter the installation folder of the Vivado and search for the settings64.sh file. You must copy the file to the folder(platform/cache-test) which containing the begin.sh file.
