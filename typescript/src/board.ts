import { Position } from './position'
import { GameObject, Mage, Wall, Pawn, Warrior, King, Empty, Piece, Assassin } from './game-object'
import { Player, Phase } from './constants'

export class Board {
  public board: Map<string, GameObject>
  public height: number
  public width: number
  private _phase: Phase // One turn is move+ability. Phase indicates whether move or ability is next.
  private _playerTurn: Player

  constructor() {
    this.board = new Map<string, GameObject>()
    this.height = 8
    this.width = 8
    this._phase = Phase.MOVE
    this._playerTurn = Player.PLAYER_1
    for(var i = 0; i < this.height; i++) {
      for(var j = 0; j < this.width; j++) {
        var p = new Position(i,j)
        var go = new Empty(i, j)
        this.board.set(JSON.stringify(p), go)
      }
      // player_1
      this.board.set(JSON.stringify(new Position(0, 0)), new King(0, 0, Player.PLAYER_1));
      this.board.set(JSON.stringify(new Position(0, 1)), new Pawn(0, 1, Player.PLAYER_1));
      this.board.set(JSON.stringify(new Position(1, 1)), new Pawn(1, 1, Player.PLAYER_1));
      this.board.set(JSON.stringify(new Position(7, 0)), new Assassin(7, 0, Player.PLAYER_1));
      this.board.set(JSON.stringify(new Position(3, 1)), new Warrior(3, 1, Player.PLAYER_1));
      this.board.set(JSON.stringify(new Position(4, 1)), new Mage(4, 1, Player.PLAYER_1));
      this.board.set(JSON.stringify(new Position(5, 1)), new Pawn(5, 1, Player.PLAYER_1));
      // player_2
      this.board.set(JSON.stringify(new Position(7, 7)), new King(7, 7, Player.PLAYER_2));
      this.board.set(JSON.stringify(new Position(7, 6)), new Pawn(7, 6, Player.PLAYER_2));
      this.board.set(JSON.stringify(new Position(6, 6)), new Pawn(6, 6, Player.PLAYER_2));
      this.board.set(JSON.stringify(new Position(0, 7)), new Assassin(0, 7, Player.PLAYER_2));
      this.board.set(JSON.stringify(new Position(4, 6)), new Warrior(4, 6, Player.PLAYER_2));
      this.board.set(JSON.stringify(new Position(2, 6)), new Pawn(2, 6, Player.PLAYER_2));
      this.board.set(JSON.stringify(new Position(3, 6)), new Mage(3, 6, Player.PLAYER_2));

    }
  }

  toString(): string {
    var retval: string = this._playerTurn + '|' + this._phase + '|'
    for(let i = 0; i < this.height; i++) {
      for(let j = 0; j < this.width; j++) {
        let go: GameObject = this.getGameObjectByCoordinates(j,i)
        if(go.type == 'empty') {
         retval += go.type + ',' 
        } else {
          let piece = go as Piece
          retval += piece.player + '-' + piece.type + '-' + piece.healthPoints + ','
        }
      }
    }
    return retval
  }


  fromString(encodedBoard: string): void{
    let newBoard: Map<string, GameObject> = new Map<string, GameObject>()
    const ss = encodedBoard.split('|')
    if(ss[0] == '0')
      this._playerTurn = Player.PLAYER_1
    else 
      this._playerTurn = Player.PLAYER_2
    if(ss[1] == '0')
      this._phase = Phase.MOVE
    else
      this._phase = Phase.ABILITY 
    const s = ss[2].split(',')
    for(let i = 0; i < this.height; i++) {
      for(let j = 0; j < this.width; j++) {
        let currentObject = s[i*10 + j]
        if(currentObject == 'empty') {
          newBoard.set(JSON.stringify(new Position(j,i)), new Empty(j,i))
        } else {
          const currentObjectSplit: string[] = currentObject.split('-')
          let player: Player
          let piece: Piece

          if(currentObjectSplit[0] == '0')
            player = Player.PLAYER_1
          else
            player = Player.PLAYER_2
          const type: string = currentObjectSplit[1]
          const healthPoints: number = parseInt(currentObjectSplit[2])
          if(type == 'pawn') {
            piece = new Pawn(j, i, player)
          } else if(type == 'mage') {
            piece = new Mage(j, i, player)
          } else if(type == 'assassin') {
            piece = new Assassin(j, i, player)
          } else if(type == 'warrior') {
            piece = new Warrior(j, i, player)
          } else if(type == 'wall') {
            piece = new Wall(j, i, player)
          } else {
            piece = new King(j, i, player)
          }
          piece.healthPoints = healthPoints
          newBoard.set(JSON.stringify(new Position(j,i)), piece)
        }
      }
    }
    this.board = newBoard
  }

  isOffBoard(x: number, y: number): boolean {
    if(x > (this.width-1) || x < 0 || y > (this.height - 1) || y < 0)
      return true

    return false
  }

  isEmpty(x: number, y: number): boolean {
    var gameObject = this.getGameObjectByCoordinates(x, y)
    return gameObject instanceof Empty
  }

  getNeighbourEnemyPieces(piece: Piece): Piece[] {
    var retval: Piece[] = [] 
    for(let i = -1; i <= 1; i++) {
      for(let j = -1; j <= 1; j++) {
        let newX = piece.x + i
        let newY = piece.y + j
        let newPiece = this.getGameObjectByCoordinates(newX, newY) as Piece
        if(!(newPiece instanceof Piece)) {
          continue
        }
        if(newPiece.player == piece.player && newPiece.player != Player.NO_PLAYER) {
          retval.push(newPiece)
        }
      }
    }
    return retval
  }
  
  getGameObjectByCoordinates(x: number, y: number): GameObject {
    return this.board.get(JSON.stringify(new Position(x, y)))
  }

  movePiece(piece: Piece, newX: number, newY: number): void {
    var oldPosition = new Position(piece.x, piece.y)
    var newPosition = new Position(newX, newY)
    this.board.set(JSON.stringify(oldPosition), new Empty(piece.x, piece.y))
    this.board.set(JSON.stringify(newPosition), piece)
    piece.x = newX
    piece.y = newY
  }

  removeIfDead(piece: Piece): void {
    if(piece.healthPoints <= 0) {
      var p = new Position(piece.x, piece.y)
      this.board.set(JSON.stringify(p), new Empty(piece.x, piece.y))
    }
  }

  addGameObject(x: number, y: number, gameObject: GameObject): void{
    var p = new Position(x, y)
    this.board.set(JSON.stringify(p), gameObject)
  }

  playerTurn(): Player {
    return this._playerTurn
  }

  changePlayerTurn(): void {
    if(this._playerTurn == Player.PLAYER_1)
      this._playerTurn = Player.PLAYER_2
    else 
      this._playerTurn = Player.PLAYER_1
  }

  phase(): Phase {
    return this._phase
  }

  setPhase(p: Phase): void {
    this._phase = p
  }

}
