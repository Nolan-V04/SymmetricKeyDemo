# SymmetricKeyDemo

If you want to compile this code first you must install CryptoPP Library.
I will instruct you to install the library step by step.
Step 1: Downloading Crypto++ library in this link https://github.com/weidai11/cryptopp
Step 2: Downloading Cryptopp-PEM in this link https://github.com/noloader/cryptopp-pem
Step 3: Copying Cryptopp-PEM files into Crypto++ folder

  You have to copy all the files in cryptopp-pem folder except '/git' and move it to cryptopp folder
![image](https://github.com/user-attachments/assets/5542c193-48a3-4657-83cc-35448c839135)

Step 4: Opening and compiling Crypto++ in MVS
  - Opening file 'cryptlib.vcxproj' in cryptopp folder to open MVS
  ![image](https://github.com/user-attachments/assets/fbeb77c6-095d-4174-8e56-b096ce338b4c)
  - Adding file pem.h, pem-common.h, into header folder
  ![image](https://github.com/user-attachments/assets/f5476422-0da7-4cc5-98a3-1786083e78e9)
  - Adding file pem-common.cpp, pem-write.cpp, pem-read.cpp into sources
  ![image](https://github.com/user-attachments/assets/7862d492-b625-413f-99a1-d11fbc9d337b)
  - Adjust the configuration accordingly (Release-x64), then proceed to build the Crypto++ solution.
Step 5: Configuring our project to use the complied Crypto++ library
  - Right-click on the property you need to configure, then select Properties.
  - Modify the configuration and platform:
  ![image](https://github.com/user-attachments/assets/dab72e30-2ac1-4bef-85cf-4b3c1e70b159)
  - Go to C/C++, select General, and modify the Additional Include Directories to the path where the CryptoPP library was downloaded
  ![image](https://github.com/user-attachments/assets/31194b32-43a2-4a0a-9c23-a10f9e8a132d)
  - In C/C++, go to Code Generation, then change the Runtime Library to Multi-threaded (/MT)
  ![image](https://github.com/user-attachments/assets/d96914e5-5fc1-4498-ba4c-2dde30e2501c)
  - Go to Linker -> General, then set Additional Library Directories to the path where the release output files are stored.
  ![image](https://github.com/user-attachments/assets/f60743e8-9858-4abd-82d7-bba80b38a099)
  - Go to Linker -> Input, then set Additional Dependencies to cryptlib.lib
  ![image](https://github.com/user-attachments/assets/a181f14f-44ad-4604-a94e-f2ab0bdccd7c)
Note: If you find it too difficult to follow the above steps, please refer to the following video: https://www.youtube.com/watch?v=5XE4zEN-WKg
Step 6: Open properties of solution then choose multiple start up and remember to run server first.
![image](https://github.com/user-attachments/assets/18689e75-c2a7-4ccf-a614-0946132b552f)
Finally: You can run project just remember to change this
![image](https://github.com/user-attachments/assets/362d0b9e-2770-4ed6-acab-e0666d1b81e9)










