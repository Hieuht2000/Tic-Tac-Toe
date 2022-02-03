# Tic-Tac-Toe
Final RTOS course work 

#Course final work : Tic-Tac-Toe
Create a simple Tic-Tac-Tow - game.

Game board 3x3 or greater (Use Global memory)

In program create four tasks:

player 1 (X)

Player 2 (O)

Draw board

Analyze game status (if either have won) (Optional)


Game board is created as shared memory 

Game logic can be made with:
Semaphores (Might require many different),
MBoxes - Use MBox like token,
Event Flags,
OSFlagCreate(),
OSFlagPend(),
OSFlagPost().

![tic_Tac_toe (1)](https://user-images.githubusercontent.com/63698805/152349039-a5d3871d-cd23-4846-9c3c-033272b0d5dd.png)


Unzip the files into correct directory structure:(source code in rtos_1 file)

![uCOS_dir_structure (1)](https://user-images.githubusercontent.com/63698805/152349210-16ef7ca1-d13e-4c06-a53f-153fcb802b8f.png)

