# shGame
Attempt to implement the card game "Secret Hitler".

 ## Game logic
 Static library to simulate the game board. Install by:
 ```
 cd shGame/src
 mkdir build && cd build
 cmake ..
 sudo make install
 ```
 
 Use by:
 ```
 #include <SecretHitlerGameLogic/Filename>
 ```
 Link with
 ```
 -lSecretHitlerGameLogic
