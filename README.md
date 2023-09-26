# go_into_the_mist
A take on Gomoku with some extra game mechanics, built using C
This assignment was completed for COMP2017 - Systems Programming in my 4th year. It is a program that runs the game Gomoku, however with an added mechanic of Mist, which means that at any time, the player is only able to view a 7x7 square of the otherwise 15x15 game board. This 7x7 hole can be centred anywhere on the board, and so will sometimes be partially off the edge of the board.

To run the game, complete the following commands
```
$ gcc gitm.c -o gitm
$ ./gitm
```

The game is 2 player, with `o` (white) and `#` (black). Black always goes first. To win, a player must get 5 stones in a row horizontally, vertically of diagonally.

## Commands
Once in the game, you can run any of the following commands (case sensitive)
|command|use|
|-|-|
|`who`|Displays which player's turn it is|
|`term`|Terminates the program with exit code 1|
|`resign`|The current player resigns, causing the other player to win and the game to end|
|`view`|Shows the current board. The first string shows the coordinate of the centre of the 7x7 square not covered by mist. The second string shows the contents of this 7x7 'hole' in the mist|
|`place <col><row>`|Allows a player to complete their turn. If the move is successful, there will be no output. You can play in the mist.|
|`history`|Displays all of moves so far.|


## `view` output symbols
|Symbol|Meaning|
|-|-|
|`#`|black stone|
|`o`|white stone|
|`.`|blank square|
|`x`|off board|

## Tests
- Some e2e tests were written, and are in the form of `.in` and `.out` files in the [tests](/tests) folder.