import {
  Player,
  WarriorMoveOffsets, Player1PawnMoveOffsets, Player2PawnMoveOffsets,
  KingMoveOffsets, MageMoveOffsets, AssassinMoveOffsets,
  WarriorAbilityOffsets, PawnAbilityOffsets, KingAbilityOffsets,
  MageAbilityOffsets, AssassinAbilityOffsets
} from './constants'

export class GameObject {
  public x: number
  public y: number
  public player: Player
  public type: string
  constructor(x: number, y: number, player: Player, type: string) {
    this.x = x
    this.y = y
    this.player = player
    this.type = type
  } 
}

export abstract class Piece extends GameObject {
  public healthPoints: number
  public abilityPoints: number

  constructor(x: number, y: number, player: Player, type: string, healthPoints: number, abilityPoints: number) {
    super(x, y, player, type)
    this.healthPoints = healthPoints
    this.abilityPoints = abilityPoints
  }
}

export class Mage extends Piece {
  constructor(x: number, y: number, player: Player) {
    super(x, y, player, 'mage', 230, 80)
  }
}

export class King extends Piece {
  constructor(x: number, y: number, player: Player) {
    super(x, y, player, 'king', 200, 60)
  }
}

export class Warrior extends Piece {
  constructor(x: number, y: number, player: Player) {
    super(x, y, player, 'warrior', 500, 100)
  }
}

export class Pawn extends Piece {
  constructor(x: number, y: number, player: Player) {
    super(x, y, player, 'pawn', 300, 30)
  }
}

export class Assassin extends Piece {
  constructor(x: number, y: number, player: Player) {
    super(x, y, player, 'assassin', 110, 120)
  }
}

export class Wall extends Piece {
  constructor(x: number, y: number, player: Player) {
    super(x, y, player, 'wall', 100, 0)
  }
}


export class Empty extends GameObject {
  constructor(x: number, y: number) {
    super(x, y, Player.NO_PLAYER, 'empty')
  }
}


export function getMoveOffsets(piece: Piece): number[][] {
  if(piece instanceof Pawn) {
    if(piece.player == Player.PLAYER_1) {
      return Player1PawnMoveOffsets
    } else {
      return Player2PawnMoveOffsets
    }
  }
  if(piece instanceof Warrior) {
    return WarriorMoveOffsets
  }
  if(piece instanceof Assassin) {
    return AssassinMoveOffsets
  }
  if(piece instanceof Mage) {
    return MageMoveOffsets
  }
  if(piece instanceof King) {
    return KingMoveOffsets
  }
  return null
}

export function getAbilityOffsets(piece: Piece): number[][] {
  if(piece instanceof Pawn) {
    return PawnAbilityOffsets
  }
  if(piece instanceof Warrior) {
    return WarriorAbilityOffsets
  }
  if(piece instanceof Assassin) {
    return AssassinAbilityOffsets
  }
  if(piece instanceof Mage) {
    return MageAbilityOffsets
  }
  if(piece instanceof King) {
    return KingAbilityOffsets
  }
  return []
}
