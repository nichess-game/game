## About

This implementation is not optimised for AI training.

## Compile

Download and install Node.js and npm https://docs.npmjs.com/downloading-and-installing-node-js-and-npm

To install typescript and tslib run:

```
npm install
```

Position yourself inside src directory and run:

```
tsc
```

## Example code


```
import { Game } from './game.js'

const game = new Game()

let moveSuccess = false
let abilitySuccess = false
let [gameOver, winner] = game.gameOver()

while(!gameOver) {
  while(!moveSuccess) {
    let [srcX, srcY, dstX, dstY] = getMoveInput()
    moveSuccess = game.move(srcX, srcY, dstX, dstY)
  }
  while(!abilitySuccess) {
    let [srcX, srcY, dstX, dstY] = getAbilityInput()
    abilitySuccess = game.ability(srcX, srcY, dstX, dstY)
  }

}

function getMoveInput() {
  ...
}

function getAbilityInput() {
  ...
}

```

## API

### move(pieceX: number, pieceY: number, destinationX: number, destinationY: number): boolean

Returns true if the move succeeded or false if it failed.

### reset(): void

Resets the game.

### ability(pieceX: number, pieceY: number, destinationX: number, destinationY: number): boolean

Returns true if the ability succeeded or false if it failed.

### legalAbilities(pieceX: number, pieceY: number): Ability[]

Returns an array of legal destination coordinates. 

### board(): GameObject[][]

2D array representation of the current position.

### getGameObjectByCoordinates(x: number, y: number): GameObject

### legalMoves(pieceX: number, pieceY: number): Move[]

Returns an array of legal destination coordinates. 

### gameOver(): [boolean, Player]

First value returned is true if the game is over and false otherwise.

Second value shows who won the game. 0 if PLAYER_1, 1 if PLAYER_2, otherwise null.

### boardToString(): string

Encodes the board in the following way: Turn|Phase|game-object1,game-object2,game-object3,...

Turn - 0 if PLAYER_1, 1 if PLAYER_2

Phase - 0 if MOVE, 1 if ABILITY. Reminder: 1 turn consists of move + ability, where move is
performed first and ability second.

game-object - There are 2 cases here. Game object is either 'empty' or
'player-pieceType-healthPoints'

Example of an encoded board:

0|0|0-assassin-20,0-warrior-600,empty,0-mage-300,0-king-200,0-mage-300,empty,empty,0-warrior-600,0-assassin-20,empty,empty,0-pawn-300,0-pawn-300,0-pawn-300,0-pawn-300,0-pawn-300,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,empty,1-pawn-300,1-pawn-300,1-pawn-300,1-pawn-300,1-pawn-300,empty,empty,empty,1-assassin-20,1-warrior-600,empty,1-mage-300,1-king-200,1-mage-300,empty,empty,1-warrior-600,1-assassin-20,

### boardFromString(encodedBoard: string): void

Creates a new game with the same state as encodedBoard.
