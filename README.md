# shGame
Attempt to implement the card game "Secret Hitler". The official webpage of the game can be found [here](https://www.secrethitler.com/).

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
 ```

 ## GameMasterLocal
 Console application for a game master moderating a match. The application basically tells the game master what to do next,
 monitors the game state and automatically performs actions like drawing and shuffling cards. Requires the 
 SecretHitlerGameLogic-library to run. Build using the supplied CMake-file.
