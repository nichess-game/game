import { Board } from './board'
import { Player, Phase } from './constants'
import { GameObject, Empty, Piece, Mage, King, Pawn, Warrior, Wall, getMoveOffsets, getAbilityOffsets } from './game-object'
import { Move } from './move'
import { Ability } from './ability'

export class Game {
  private _board: Board
  private _gameOver: boolean 
  private _winner: Player
  private _history: Array<[Move, Ability]>
  private _lastMove: Move

  constructor() {
    this._board = new Board()
    this._gameOver = false
    this._winner = null
    this._history = []
    this._lastMove = null
  }
  
  move(pieceX: number, pieceY: number, destinationX: number, destinationY: number): boolean {
    if(this._board.phase() != Phase.MOVE)
      return false
    var piece = this._board.getGameObjectByCoordinates(pieceX, pieceY) as Piece
    if(piece.player != this._board.playerTurn()) {
      return false
    }
    if(!this._board.isOffBoard(destinationX, destinationY) && this._board.isEmpty(destinationX, destinationY)) {
      let legalMoves: Move[] = this.legalMoves(pieceX, pieceY)
      for(let i = 0; i < legalMoves.length; i++) {
        if(legalMoves[i].dstX == destinationX && legalMoves[i].dstY == destinationY) {
          this._board.movePiece(piece, destinationX, destinationY)
          this._board.setPhase(Phase.ABILITY)
          this._lastMove = new Move(pieceX, pieceY, destinationX, destinationY)
          return true
        }
      }
    }
    return false
  }

  reset(): void {
    this._board = new Board()
    this._gameOver = false
    this._winner = null
    this._history = []
  }

  _pawnAbility(pawn: Pawn, destinationX: number, destinationY: number): boolean {
    var destinationPiece = this._board.getGameObjectByCoordinates(destinationX, destinationY) as Piece
    let legalAbilities: Ability[] = this.legalAbilities(pawn.x, pawn.y)

    if(destinationPiece instanceof Empty) {
      this._board.addGameObject(destinationX, destinationY, new Wall(destinationX, destinationY, pawn.player))
      return true
    }
    if(destinationPiece instanceof Wall) {
      this._board.addGameObject(destinationX, destinationY, new Empty(destinationX, destinationY))
      return true
    }

    if(destinationPiece instanceof Piece && destinationPiece.player != pawn.player) {
      destinationPiece.healthPoints -= pawn.abilityPoints
      this._board.removeIfDead(destinationPiece)
      return true
    }

    return false
    
  }
    
  _mageAbility(sourcePiece: Piece, destinationX: number, destinationY: number): boolean {
    var enemyPiece = this._board.getGameObjectByCoordinates(destinationX, destinationY) as Piece
    if(enemyPiece instanceof Empty)
      return true
    if(!(enemyPiece instanceof Piece) || enemyPiece.player == sourcePiece.player) {
      return false
    }
    var neighbourEnemyPieces: Piece[] = this._board.getNeighbourEnemyPieces(enemyPiece)
    for(let i = 0; i < neighbourEnemyPieces.length; i++) {
      let piece: Piece = neighbourEnemyPieces[i]
      piece.healthPoints -= sourcePiece.abilityPoints
      this._board.removeIfDead(piece)
    }
    return true
  }


  _defaultAbility(sourcePiece: Piece, destinationX: number, destinationY: number): boolean {
    var destinationPiece = this._board.getGameObjectByCoordinates(destinationX, destinationY) as Piece
    if(destinationPiece instanceof Empty)
      return true
    if(!(destinationPiece instanceof Piece) || destinationPiece.player == sourcePiece.player) {
      return false
    }
    if(sourcePiece.player != destinationPiece.player) {
      destinationPiece.healthPoints -= sourcePiece.abilityPoints
      this._board.removeIfDead(destinationPiece)
    }
    return true
  }

  ability(pieceX: number, pieceY: number, destinationX: number, destinationY: number): boolean {
    if(this._board.phase() != Phase.ABILITY)
      return false

    var sourcePiece = this._board.getGameObjectByCoordinates(pieceX, pieceY) as Piece
    if(sourcePiece.player != this._board.playerTurn()) {
      return false
    }
    let legalAbilities: Ability[] = this.legalAbilities(sourcePiece.x, sourcePiece.y)
    var success: boolean = false

    for(let i = 0; i < legalAbilities.length; i++) {
      if(legalAbilities[i].dstX == destinationX && legalAbilities[i].dstY == destinationY) {
        if(sourcePiece instanceof Pawn) {
          success = this._pawnAbility(sourcePiece, destinationX, destinationY)
        }
        else if(sourcePiece instanceof Mage) {
          success = this._mageAbility(sourcePiece, destinationX, destinationY)
        }
        else {
         success = this._defaultAbility(sourcePiece, destinationX, destinationY)
        }
        break
      }
    }
    if(success) {
      this._updateGameOver()
      var a = new Ability(pieceX, pieceY, destinationX, destinationY)
      this._history.push([this._lastMove, a])
      this._board.changePlayerTurn()
      this._board.setPhase(Phase.MOVE)
    }
    return success
  }

  legalAbilities(pieceX: number, pieceY: number): Ability[] {
    if(this._board.phase() != Phase.ABILITY)
      return []
    var retval: Ability[] = []
    var sourcePiece = this._board.getGameObjectByCoordinates(pieceX, pieceY) as Piece
    if((sourcePiece.player != this._board.playerTurn()) ||
      sourcePiece instanceof Wall) {
      return []
    }
    var abilityOffsets = getAbilityOffsets(sourcePiece)
    for(let i = 0; i < abilityOffsets.length; i++) {
      const x = sourcePiece.x + abilityOffsets[i][0]
      const y = sourcePiece.y + abilityOffsets[i][1]
      if(y < 0 || y >= this._board.height || x < 0 || x >= this._board.height)
        continue
      retval.push(new Ability(pieceX, pieceY, x, y))
    }
    return retval
  }


  board(): any[] {
    var retval = []
    var row = []
    for(var i = this._board.height-1; i >= 0; i--) {
      for(var j = 0; j < this._board.width; j++) {
        row.push(this._board.getGameObjectByCoordinates(j, i))
      }
      retval.push(row)
      row = []
    }
    return retval
  }

  getGameObjectByCoordinates(x: number, y: number) {
    return this._board.getGameObjectByCoordinates(x, y)
  }

  legalMoves(pieceX: number, pieceY: number): Move[] {
    if(this._board.phase() != Phase.MOVE)
      return []
    var retval: Move[] = []
    var sourcePiece = this._board.getGameObjectByCoordinates(pieceX, pieceY) as Piece
    if((sourcePiece.player != this._board.playerTurn()) ||
      sourcePiece instanceof Wall) {
      return []
    }
    var moveOffsets = getMoveOffsets(sourcePiece)
    for(let i = 0; i < moveOffsets.length; i++) {
      const x = sourcePiece.x + moveOffsets[i][0]
      const y = sourcePiece.y + moveOffsets[i][1]
      if(y < 0 || y >= this._board.height || x < 0 || x >= this._board.height)
        continue
      // check that pawn can't jump over pieces
      if((sourcePiece instanceof Pawn) || (sourcePiece instanceof Warrior)) {
        if(moveOffsets[i][1] == 2) {
          if(!this._board.isEmpty(x, y-1))
            continue
        }
        if(moveOffsets[i][1] == -2) {
          if(!this._board.isEmpty(x, y+1))
            continue
        }

      }
      if(this._board.getGameObjectByCoordinates(x,y) instanceof Empty) {
        retval.push(new Move(pieceX, pieceY, x, y))
      }
    }
    
    return retval
  }

  _updateGameOver(): void {
    var player1KingAlive: boolean = false
    var player2KingAlive: boolean = false
    for(let i = 0; i < this._board.height; i++) {
      for(let j = 0; j < this._board.width; j++) {
        let piece = this._board.getGameObjectByCoordinates(j,i)
        if(piece instanceof King) {
          if(piece.player == Player.PLAYER_1)
            player1KingAlive = true
          else
            player2KingAlive = true
        }
      }
    }
    if(!player1KingAlive) {
      this._gameOver = true
      this._winner = Player.PLAYER_2
    }
    if(!player2KingAlive) {
      this._gameOver = true
      this._winner = Player.PLAYER_1
    }
  }

  gameOver(): [boolean, Player] {
    return [this._gameOver, this._winner]
  }

  boardToString(): string {
    return this._board.toString()
  }

  boardFromString(encodedBoard: string): void {
    this._board.fromString(encodedBoard)
  }

  history(): Array<[Move, Ability]> {
    return this._history
  }

}
