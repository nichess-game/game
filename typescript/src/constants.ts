export enum Player {
  PLAYER_1 = 0,
  PLAYER_2 = 1,
  NO_PLAYER = 2,
}

export enum Phase {
  MOVE = 0,
  ABILITY = 1
}


export const WarriorMoveOffsets = [
  [-1,-1],[-1,0],[-1,1],
  [0,-1],[0,1],
  [1,-1],[1,0],[1,1]
]
export const Player1PawnMoveOffsets = [
  [-1,-1],[-1,0],[-1,1],
  [0,-1],[0,1],[0,2],
  [1,-1],[1,0],[1,1]
]
export const Player2PawnMoveOffsets = [
  [-1,-1],[-1,0],[-1,1],
  [0,-2],[0,-1],[0,1],
  [1,-1],[1,0],[1,1]
]

export const MageMoveOffsets = [
  [-1,-1],[-1,0],[-1,1],
  [0,-1],[0,1],
  [1,-1],[1,0],[1,1]
]
export const AssassinMoveOffsets = [
  [-3,-3],[-3,3],
  [-2,-2],[-2,-1],[-2,0],[-2,1],[-2,2],
  [-1,-2],[-1,-1],[-1,0],[-1,1],[-1,2],
  [0,-2],[0,-1],[0,1],[0,2],
  [1,-2],[1,-1],[1,0],[1,1],[1,2],
  [2,-2],[2,-1],[2,0],[2,1],[2,2],
  [3,-3],[3,3]
]

export const KingMoveOffsets = [
  [-1,-1],[-1,0],[-1,1],
  [0,-1],[0,1],
  [1,-1],[1,0],[1,1]
]



export const WarriorAbilityOffsets = [
  [-1,-1],[-1,0],[-1,1],
  [0,-1],[0,1],
  [1,-1],[1,0],[1,1]
]
export const AssassinAbilityOffsets = [
  [-1,-1],[-1,0],[-1,1],
  [0,-1],[0,1],
  [1,-1],[1,0],[1,1]
]
export const PawnAbilityOffsets = [
  [-1,-1],[-1,0],[-1,1],
  [0,-1],[0,1],
  [1,-1],[1,0],[1,1]
]
export const MageAbilityOffsets = [
  [-2,-2],[-2,-1],[-2,0],[-2,1],[-2,2],
  [-1,-2],[-1,-1],[-1,0],[-1,1],[-1,2],
  [0,-2],[0,-1],[0,1],[0,2],
  [1,-2],[1,-1],[1,0],[1,1],[1,2],
  [2,-2],[2,-1],[2,0],[2,1],[2,2]
]
export const KingAbilityOffsets = [
  [-1,-1],[-1,0],[-1,1],
  [0,-1],[0,1],
  [1,-1],[1,0],[1,1]
]

