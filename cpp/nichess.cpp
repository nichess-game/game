#include "nichess.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <tuple>
#include <chrono>

using namespace nichess;

std::string playerToString(Player p) {
  switch(p) {
    case PLAYER_1:
      return "PLAYER_1";
    case PLAYER_2:
      return "PLAYER_2";
    default:
      return "default";
  }
}

std::string pieceTypeToString(PieceType pt) {
  switch(pt) {
    case P1_KING:
      return "K";
    case P1_MAGE:
      return "M";
    case P1_PAWN:
      return "P";
    case P1_WARRIOR:
      return "W";
    case P1_WALL:
      return "L";
    case P1_ASSASSIN:
      return "A";
    case P2_KING:
      return "k";
    case P2_MAGE:
      return "m";
    case P2_PAWN:
      return "p";
    case P2_WARRIOR:
      return "w";
    case P2_WALL:
      return "l";
    case P2_ASSASSIN:
      return "a";
    case NO_PIECE:
      return ".";
    default:
      return "default";
  }
}

bool player1OrEmpty(PieceType pt) {
  switch(pt) {
    case P1_KING:
      return true;
    case P1_MAGE:
      return true;
    case P1_PAWN:
      return true;
    case P1_WARRIOR:
      return true;
    case P1_WALL:
      return true;
    case P1_ASSASSIN:
      return true;
    case P2_KING:
      return false;
    case P2_MAGE:
      return false;
    case P2_PAWN:
      return false;
    case P2_WARRIOR:
      return false;
    case P2_WALL:
      return false;
    case P2_ASSASSIN:
      return false;
    case NO_PIECE:
      return true;
  }
  return false;
}

bool player2OrEmpty(PieceType pt) {
  switch(pt) {
    case P1_KING:
      return false;
    case P1_MAGE:
      return false;
    case P1_PAWN:
      return false;
    case P1_WARRIOR:
      return false;
    case P1_WALL:
      return false;
    case P1_ASSASSIN:
      return false;
    case P2_KING:
      return true;
    case P2_MAGE:
      return true;
    case P2_PAWN:
      return true;
    case P2_WARRIOR:
      return true;
    case P2_WALL:
      return true;
    case P2_ASSASSIN:
      return true;
    case NO_PIECE:
      return true;
  }
  return false;
}

bool pieceBelongsToPlayer(PieceType pt, Player player) {
  switch(pt) {
    case P1_KING:
        return player == PLAYER_1;
      case P1_MAGE:
        return player == PLAYER_1;
      case P1_PAWN:
        return player == PLAYER_1;
      case P1_WARRIOR:
        return player == PLAYER_1;
      case P1_WALL:
        return player == PLAYER_1;
      case P1_ASSASSIN:
        return player == PLAYER_1;
      case P2_KING:
        return player == PLAYER_2;
      case P2_MAGE:
        return player == PLAYER_2;
      case P2_PAWN:
        return player == PLAYER_2;
      case P2_WARRIOR:
        return player == PLAYER_2;
      case P2_WALL:
        return player == PLAYER_2;
      case P2_ASSASSIN:
        return player == PLAYER_2;
      case NO_PIECE:
        return false;
    }
  return false;
}

/*
 * Coordinates are not standard. Bottom left is (0,0) and top right is (7,7)
 */
int coordinatesToBoardIndex(int column, int row) {
  return column + row * NUM_COLUMNS;
}

std::tuple<int, int> boardIndexToCoordinates(int squareIndex) {
  int x = squareIndex -  (floor(squareIndex / NUM_ROWS) * NUM_ROWS);
  int y = floor(squareIndex / NUM_ROWS);
  return std::tuple<int, int>(x, y);
}

PlayerAction::PlayerAction() { }

PlayerAction::PlayerAction(int moveSrcIdx, int moveDstIdx, int abilitySrcIdx, int abilityDstIdx) {
  this->moveSrcIdx = moveSrcIdx;
  this->moveDstIdx = moveDstIdx;
  this->abilitySrcIdx = abilitySrcIdx;
  this->abilityDstIdx = abilityDstIdx;
}
void PlayerAction::print() {
  std::tuple<int,int> srcXY = boardIndexToCoordinates(moveSrcIdx);
  std::tuple<int,int> dstXY = boardIndexToCoordinates(moveDstIdx);
  int mSrcX, mSrcY, mDstX, mDstY;
  // converting board index to coordinates doesn't work for MOVE_SKIP
  if(moveSrcIdx == MOVE_SKIP) { 
    mSrcX = MOVE_SKIP;
    mSrcY = MOVE_SKIP;
    mDstX = MOVE_SKIP;
    mDstY = MOVE_SKIP;
  } else {
    mSrcX = std::get<0>(srcXY);
    mSrcY = std::get<1>(srcXY);
    mDstX = std::get<0>(dstXY);
    mDstY = std::get<1>(dstXY);
  }
  srcXY = boardIndexToCoordinates(abilitySrcIdx);
  dstXY = boardIndexToCoordinates(abilityDstIdx);

  int aSrcX, aSrcY, aDstX, aDstY;
  // converting board index to coordinates doesn't work for ABILITY_SKIP
  if(abilitySrcIdx == ABILITY_SKIP) { 
    aSrcX = ABILITY_SKIP;
    aSrcY = ABILITY_SKIP;
    aDstX = ABILITY_SKIP;
    aDstY = ABILITY_SKIP;
  } else {
    aSrcX = std::get<0>(srcXY);
    aSrcY = std::get<1>(srcXY);
    aDstX = std::get<0>(dstXY);
    aDstY = std::get<1>(dstXY);
  }

  std::cout << "moveSrcX: " << mSrcX << " moveSrcY " << mSrcY << " moveDstX " << mDstX << " moveDstY " << mDstY;
  std::cout << " abilitySrcX: " << aSrcX << " abilitySrcY " << aSrcY << " abilityDstX " << aDstX << " abilityDstY " << aDstY << "\n";
}

PlayerMove::PlayerMove() { }

PlayerMove::PlayerMove(int moveSrcIdx, int moveDstIdx) {
  this->moveSrcIdx = moveSrcIdx;
  this->moveDstIdx = moveDstIdx;
}

void PlayerMove::print() {
  std::tuple<int,int> srcXY = boardIndexToCoordinates(moveSrcIdx);
  std::tuple<int,int> dstXY = boardIndexToCoordinates(moveDstIdx);
  int srcX = std::get<0>(srcXY);
  int srcY = std::get<1>(srcXY);
  int dstX = std::get<0>(dstXY);
  int dstY = std::get<1>(dstXY);
  std::cout << "moveSrcX: " << srcX << " moveSrcY " << srcY << " moveDstX " << dstX << " moveDstY " << dstY << "\n";
}

PlayerAbility::PlayerAbility() { }

PlayerAbility::PlayerAbility(int abilitySrcIdx, int abilityDstIdx) {
  this->abilitySrcIdx = abilitySrcIdx;
  this->abilityDstIdx = abilityDstIdx;
}

void PlayerAbility::print() {
  std::tuple<int,int> srcXY = boardIndexToCoordinates(abilitySrcIdx);
  std::tuple<int,int> dstXY = boardIndexToCoordinates(abilityDstIdx);
  int srcX = std::get<0>(srcXY);
  int srcY = std::get<1>(srcXY);
  int dstX = std::get<0>(dstXY);
  int dstY = std::get<1>(dstXY);
  std::cout << "abilitySrcX: " << srcX << " abilitySrcY " << srcY << " abilityDstX " << dstX << " abilityDstY " << dstY << "\n";
}

Piece::Piece(): type(PieceType::NO_PIECE), healthPoints(0), squareIndex(0) { }

Piece::Piece(PieceType type, int healthPoints, int squareIndex):
  type(type),
  healthPoints(healthPoints),
  squareIndex(squareIndex)
{ }
    
void Piece::print() {
  std::stringstream ss;
  ss << "I am piece " << type << " at idx " << squareIndex << " and I have " << healthPoints << " health points.\n";
  std::cout << ss.str();
}

UndoInfo::UndoInfo() {
  for(int i = 0; i < 9; i++) {
    this->affectedPieces[i] = nullptr;
  }
}

UndoInfo::UndoInfo(int moveSrcIdx, int moveDstIdx, AbilityType abilityType) {
  this->moveSrcIdx = moveSrcIdx;
  this->moveDstIdx = moveDstIdx;
  this->abilityType = abilityType;
  for(int i = 0; i < 9; i++) {
    this->affectedPieces[i] = nullptr;
  }
}

bool isOffBoard(int x, int y) {
  if(x >= NUM_COLUMNS || x < 0 || y >= NUM_ROWS || y < 0)
    return true;
  else
    return false;
}

bool isOffBoard(int squareIndex) {
  if(squareIndex < 0 || squareIndex > NUM_ROWS * NUM_COLUMNS - 1)
    return true;
  else
    return false;
}


/*
 * For each Piece type, for each square, returns legal moves as if there were no other
 * pieces on the board. Elsewhere, occupied squares will be discarded from the legal moves.
 */
std::vector<std::vector<std::vector<PlayerMove>>> generateLegalMovesOnAnEmptyBoard() {
  std::vector<std::vector<std::vector<PlayerMove>>> pieceTypeToSquareToLegalMoves{NUM_PIECE_TYPE};

  // p1 king moves
  std::vector<std::vector<PlayerMove>> squareToP1KingMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      squareToP1KingMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P1_KING] = squareToP1KingMoves;

  // p1 mage moves
  std::vector<std::vector<PlayerMove>> squareToP1MageMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      squareToP1MageMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P1_MAGE] = squareToP1MageMoves;

  // p1 pawn moves
  std::vector<std::vector<PlayerMove>> squareToP1PawnMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      // pawn can also go 2 squares forward
      int move_dst_x = move_column;
      int move_dst_y = move_row + 2;
      if(!isOffBoard(move_dst_x, move_dst_y)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
        playerMoves.push_back(pm);
      }
      squareToP1PawnMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P1_PAWN] = squareToP1PawnMoves;

  // p1 warrior moves
  std::vector<std::vector<PlayerMove>> squareToP1WarriorMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      squareToP1WarriorMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P1_WARRIOR] = squareToP1WarriorMoves;

  // p1 wall moves
  // walls can't move
  std::vector<std::vector<PlayerMove>> squareToP1WallMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      squareToP1WallMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P1_WALL] = squareToP1WallMoves;

  // p1 assassin moves
  std::vector<std::vector<PlayerMove>> squareToP1AssassinMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -2; dx < 3; dx++) {
        for(int dy = -2; dy < 3; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      // 4 extra moves:
      int move_dst_x1 = move_column + 3;
      int move_dst_y1 = move_row + 3;
      int move_dst_x2 = move_column + 3;
      int move_dst_y2 = move_row - 3;
      int move_dst_x3 = move_column - 3;
      int move_dst_y3 = move_row + 3;
      int move_dst_x4 = move_column - 3;
      int move_dst_y4 = move_row - 3;
      if(!isOffBoard(move_dst_x1, move_dst_y1)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x1, move_dst_y1));
        playerMoves.push_back(pm);
      }
      if(!isOffBoard(move_dst_x2, move_dst_y2)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x2, move_dst_y2));
        playerMoves.push_back(pm);
      }
      if(!isOffBoard(move_dst_x3, move_dst_y3)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x3, move_dst_y3));
        playerMoves.push_back(pm);
      }
      if(!isOffBoard(move_dst_x4, move_dst_y4)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x4, move_dst_y4));
        playerMoves.push_back(pm);
      }

      squareToP1AssassinMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P1_ASSASSIN] = squareToP1AssassinMoves;

  // p2 king moves
  std::vector<std::vector<PlayerMove>> squareToP2KingMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      squareToP2KingMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P2_KING] = squareToP2KingMoves;

  // p2 mage moves
  std::vector<std::vector<PlayerMove>> squareToP2MageMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      squareToP2MageMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P2_MAGE] = squareToP2MageMoves;

  // p2 pawn moves
  std::vector<std::vector<PlayerMove>> squareToP2PawnMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      // pawn can also go 2 squares forward(for p2 this means -2 on y axis)
      int move_dst_x = move_column;
      int move_dst_y = move_row - 2;
      if(!isOffBoard(move_dst_x, move_dst_y)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
        playerMoves.push_back(pm);
      }
      squareToP2PawnMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P2_PAWN] = squareToP2PawnMoves;

  // p2 warrior moves
  std::vector<std::vector<PlayerMove>> squareToP2WarriorMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      squareToP2WarriorMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P2_WARRIOR] = squareToP2WarriorMoves;

  // p2 wall moves
  // walls can't move
  std::vector<std::vector<PlayerMove>> squareToP2WallMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      squareToP2WallMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P2_WALL] = squareToP2WallMoves;

  // p2 assassin moves
  std::vector<std::vector<PlayerMove>> squareToP2AssassinMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      for(int dx = -2; dx < 3; dx++) {
        for(int dy = -2; dy < 3; dy++) {
          if(dx == 0 && dy == 0) continue;
          int move_dst_x = move_column + dx;
          int move_dst_y = move_row + dy;
          if(isOffBoard(move_dst_x, move_dst_y)) continue;
          PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x, move_dst_y));
          playerMoves.push_back(pm);
        }
      }
      // 4 extra moves:
      int move_dst_x1 = move_column + 3;
      int move_dst_y1 = move_row + 3;
      int move_dst_x2 = move_column + 3;
      int move_dst_y2 = move_row - 3;
      int move_dst_x3 = move_column - 3;
      int move_dst_y3 = move_row + 3;
      int move_dst_x4 = move_column - 3;
      int move_dst_y4 = move_row - 3;
      if(!isOffBoard(move_dst_x1, move_dst_y1)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x1, move_dst_y1));
        playerMoves.push_back(pm);
      }
      if(!isOffBoard(move_dst_x2, move_dst_y2)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x2, move_dst_y2));
        playerMoves.push_back(pm);
      }
      if(!isOffBoard(move_dst_x3, move_dst_y3)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x3, move_dst_y3));
        playerMoves.push_back(pm);
      }
      if(!isOffBoard(move_dst_x4, move_dst_y4)){
        PlayerMove pm = PlayerMove(srcSquareIndex, coordinatesToBoardIndex(move_dst_x4, move_dst_y4));
        playerMoves.push_back(pm);
      }

      squareToP2AssassinMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[P2_ASSASSIN] = squareToP2AssassinMoves;

  // NO_PIECE moves (shouldn't be used, added for completeness)
  std::vector<std::vector<PlayerMove>> squareToNoPieceMoves{NUM_ROWS*NUM_COLUMNS};
  for(int move_row = 0; move_row < NUM_ROWS; move_row++) {
    for(int move_column = 0; move_column < NUM_COLUMNS; move_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(move_column, move_row);
      std::vector<PlayerMove> playerMoves;
      squareToNoPieceMoves[srcSquareIndex] = playerMoves;
    }
  }
  pieceTypeToSquareToLegalMoves[NO_PIECE] = squareToNoPieceMoves;

  return pieceTypeToSquareToLegalMoves;
}

/*
 * For each Piece type, for each square, returns legal abilities as if there were no other
 * pieces on the board. Elsewhere, abilities will be filtered by the actual board position.
 */
std::vector<std::vector<std::vector<PlayerAbility>>> generateLegalAbilitiesOnAnEmptyBoard() {
  std::vector<std::vector<std::vector<PlayerAbility>>> pieceTypeToSquareToLegalAbilities{NUM_PIECE_TYPE};

  // p1 king abilities
  std::vector<std::vector<PlayerAbility>> squareToP1KingAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP1KingAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P1_KING] = squareToP1KingAbilities;

  // p1 mage abilities
  std::vector<std::vector<PlayerAbility>> squareToP1MageAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -2; dx < 3; dx++) {
        for(int dy = -2; dy < 3; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP1MageAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P1_MAGE] = squareToP1MageAbilities;

  // p1 pawn abilities
  std::vector<std::vector<PlayerAbility>> squareToP1PawnAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP1PawnAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P1_PAWN] = squareToP1PawnAbilities;

  // p1 warrior abilities
  std::vector<std::vector<PlayerAbility>> squareToP1WarriorAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP1WarriorAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P1_WARRIOR] = squareToP1WarriorAbilities;

  // p1 wall abilities
  // walls can't ability
  std::vector<std::vector<PlayerAbility>> squareToP1WallAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      squareToP1WallAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P1_WALL] = squareToP1WallAbilities;

  // p1 assassin abilities
  std::vector<std::vector<PlayerAbility>> squareToP1AssassinAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP1AssassinAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P1_ASSASSIN] = squareToP1AssassinAbilities;

  // p2 king abilities
  std::vector<std::vector<PlayerAbility>> squareToP2KingAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP2KingAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P2_KING] = squareToP2KingAbilities;

  // p2 mage abilities
  std::vector<std::vector<PlayerAbility>> squareToP2MageAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -2; dx < 3; dx++) {
        for(int dy = -2; dy < 3; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP2MageAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P2_MAGE] = squareToP2MageAbilities;

  // p2 pawn abilities
  std::vector<std::vector<PlayerAbility>> squareToP2PawnAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP2PawnAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P2_PAWN] = squareToP2PawnAbilities;

  // p2 warrior abilities
  std::vector<std::vector<PlayerAbility>> squareToP2WarriorAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP2WarriorAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P2_WARRIOR] = squareToP2WarriorAbilities;

  // p2 wall abilities
  // walls can't ability
  std::vector<std::vector<PlayerAbility>> squareToP2WallAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      squareToP2WallAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P2_WALL] = squareToP2WallAbilities;

  // p2 assassin abilities
  std::vector<std::vector<PlayerAbility>> squareToP2AssassinAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      for(int dx = -1; dx < 2; dx++) {
        for(int dy = -1; dy < 2; dy++) {
          if(dx == 0 && dy == 0) continue;
          int ability_dst_x = ability_column + dx;
          int ability_dst_y = ability_row + dy;
          if(isOffBoard(ability_dst_x, ability_dst_y)) continue;
          PlayerAbility pa = PlayerAbility(srcSquareIndex, coordinatesToBoardIndex(ability_dst_x, ability_dst_y));
          playerAbilities.push_back(pa);
        }
      }
      squareToP2AssassinAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[P2_ASSASSIN] = squareToP2AssassinAbilities;

  // NO_PIECE abilities
  std::vector<std::vector<PlayerAbility>> squareToNoPieceAbilities{NUM_ROWS*NUM_COLUMNS};
  for(int ability_row = 0; ability_row < NUM_ROWS; ability_row++) {
    for(int ability_column = 0; ability_column < NUM_COLUMNS; ability_column++) {
      int srcSquareIndex = coordinatesToBoardIndex(ability_column, ability_row);
      std::vector<PlayerAbility> playerAbilities;
      squareToNoPieceAbilities[srcSquareIndex] = playerAbilities;
    }
  }
  pieceTypeToSquareToLegalAbilities[NO_PIECE] = squareToNoPieceAbilities;


  return pieceTypeToSquareToLegalAbilities;
}

/*
 * For index of each square, finds indices of squares that are touching it.
 * Used for mage ability.
 */
std::vector<std::vector<int>> generateSquareToNeighboringSquares() {
  std::vector<std::vector<int>> squareToNeighboringSquares{NUM_ROWS * NUM_COLUMNS};
  for(int srcY = 0; srcY < NUM_ROWS; srcY++) {
    for(int srcX = 0; srcX < NUM_COLUMNS; srcX++) {
      std::vector<int> neighboringSquares;
      int srcIndex = coordinatesToBoardIndex(srcX, srcY);
      for(int k = -1; k < 2; k++) {
        for(int l = -1; l < 2; l++) {
          if(k == 0 && l == 0) continue;
          int newX = srcX + k;
          int newY = srcY + l;
          if(isOffBoard(newX, newY)) continue;
          int newIndex = coordinatesToBoardIndex(newX, newY);
          neighboringSquares.push_back(newIndex);
        }
      }
      squareToNeighboringSquares[srcIndex] = neighboringSquares;  
    }
  }
  return squareToNeighboringSquares;
}

void Game::reset() {
  moveNumber = 0;
  currentPlayer = Player::PLAYER_1;
  // Create starting position
  board[coordinatesToBoardIndex(0,0)] = new Piece(PieceType::P1_KING, KING_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(0,0));
  board[coordinatesToBoardIndex(0,1)] = new Piece(PieceType::P1_PAWN, PAWN_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(0,1));
  board[coordinatesToBoardIndex(1,1)] = new Piece(PieceType::P1_PAWN, PAWN_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(1,1));
  board[coordinatesToBoardIndex(7,0)] = new Piece(PieceType::P1_ASSASSIN, ASSASSIN_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(7,0));
  board[coordinatesToBoardIndex(3,1)] = new Piece(PieceType::P1_WARRIOR, WARRIOR_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(3,1));
  board[coordinatesToBoardIndex(4,1)] = new Piece(PieceType::P1_MAGE, MAGE_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(4,1));
  board[coordinatesToBoardIndex(5,1)] = new Piece(PieceType::P1_PAWN, PAWN_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(5,1));

  board[coordinatesToBoardIndex(7,7)] = new Piece(PieceType::P2_KING, KING_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(7,7));
  board[coordinatesToBoardIndex(7,6)] = new Piece(PieceType::P2_PAWN, PAWN_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(7,6));
  board[coordinatesToBoardIndex(6,6)] = new Piece(PieceType::P2_PAWN, PAWN_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(6,6));
  board[coordinatesToBoardIndex(0,7)] = new Piece(PieceType::P2_ASSASSIN, ASSASSIN_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(0,7));
  board[coordinatesToBoardIndex(4,6)] = new Piece(PieceType::P2_WARRIOR, WARRIOR_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(4,6));
  board[coordinatesToBoardIndex(2,6)] = new Piece(PieceType::P2_MAGE, MAGE_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(2,6));
  board[coordinatesToBoardIndex(3,6)] = new Piece(PieceType::P2_PAWN, PAWN_STARTING_HEALTH_POINTS, coordinatesToBoardIndex(3,6));

  board[coordinatesToBoardIndex(0,2)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(0,2));
  board[coordinatesToBoardIndex(0,3)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(0,3));
  board[coordinatesToBoardIndex(0,4)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(0,4));
  board[coordinatesToBoardIndex(0,5)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(0,5));
  board[coordinatesToBoardIndex(0,6)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(0,6));
  board[coordinatesToBoardIndex(1,0)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(1,0));
  board[coordinatesToBoardIndex(1,2)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(1,2));
  board[coordinatesToBoardIndex(1,3)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(1,3));
  board[coordinatesToBoardIndex(1,4)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(1,4));
  board[coordinatesToBoardIndex(1,5)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(1,5));
  board[coordinatesToBoardIndex(1,6)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(1,6));
  board[coordinatesToBoardIndex(1,7)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(1,7));
  board[coordinatesToBoardIndex(2,0)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(2,0));
  board[coordinatesToBoardIndex(2,1)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(2,1));
  board[coordinatesToBoardIndex(2,2)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(2,2));
  board[coordinatesToBoardIndex(2,3)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(2,3));
  board[coordinatesToBoardIndex(2,4)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(2,4));
  board[coordinatesToBoardIndex(2,5)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(2,5));
  board[coordinatesToBoardIndex(2,7)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(2,7));
  board[coordinatesToBoardIndex(3,0)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(3,3));
  board[coordinatesToBoardIndex(3,2)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(3,2));
  board[coordinatesToBoardIndex(3,3)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(3,3));
  board[coordinatesToBoardIndex(3,4)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(3,4));
  board[coordinatesToBoardIndex(3,5)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(3,5));
  board[coordinatesToBoardIndex(3,7)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(3,7));
  board[coordinatesToBoardIndex(4,0)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(4,0));
  board[coordinatesToBoardIndex(4,2)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(4,2));
  board[coordinatesToBoardIndex(4,3)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(4,3));
  board[coordinatesToBoardIndex(4,4)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(4,4));
  board[coordinatesToBoardIndex(4,5)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(4,5));
  board[coordinatesToBoardIndex(4,7)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(4,7));
  board[coordinatesToBoardIndex(5,0)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(5,0));
  board[coordinatesToBoardIndex(5,2)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(5,2));
  board[coordinatesToBoardIndex(5,3)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(5,3));
  board[coordinatesToBoardIndex(5,4)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(5,4));
  board[coordinatesToBoardIndex(5,5)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(5,5));
  board[coordinatesToBoardIndex(5,6)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(5,6));
  board[coordinatesToBoardIndex(5,7)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(5,7));
  board[coordinatesToBoardIndex(6,0)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(6,0));
  board[coordinatesToBoardIndex(6,1)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(6,1));
  board[coordinatesToBoardIndex(6,2)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(6,2));
  board[coordinatesToBoardIndex(6,3)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(6,3));
  board[coordinatesToBoardIndex(6,4)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(6,4));
  board[coordinatesToBoardIndex(6,5)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(6,5));
  board[coordinatesToBoardIndex(6,7)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(6,7));
  board[coordinatesToBoardIndex(7,1)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(7,1));
  board[coordinatesToBoardIndex(7,2)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(7,2));
  board[coordinatesToBoardIndex(7,3)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(7,3));
  board[coordinatesToBoardIndex(7,4)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(7,4));
  board[coordinatesToBoardIndex(7,5)] = new Piece(PieceType::NO_PIECE, 0, coordinatesToBoardIndex(7,5));

  // Piece pointers are also kept in an array for faster access
  std::vector<Piece*> p1Pieces{NUM_STARTING_PIECES};
  p1Pieces[KING_PIECE_INDEX] = board[coordinatesToBoardIndex(0,0)];
  p1Pieces[PAWN_1_PIECE_INDEX] = board[coordinatesToBoardIndex(0,1)];
  p1Pieces[PAWN_2_PIECE_INDEX] = board[coordinatesToBoardIndex(1,1)];
  p1Pieces[ASSASSIN_PIECE_INDEX] = board[coordinatesToBoardIndex(7,0)];
  p1Pieces[WARRIOR_PIECE_INDEX] = board[coordinatesToBoardIndex(3,1)];
  p1Pieces[MAGE_PIECE_INDEX] = board[coordinatesToBoardIndex(4,1)];
  p1Pieces[PAWN_3_PIECE_INDEX] = board[coordinatesToBoardIndex(5,1)];
  playerToPieces[Player::PLAYER_1] = p1Pieces;

  std::vector<Piece*> p2Pieces{NUM_STARTING_PIECES};
  p2Pieces[KING_PIECE_INDEX] = board[coordinatesToBoardIndex(7,7)];
  p2Pieces[PAWN_1_PIECE_INDEX] = board[coordinatesToBoardIndex(7,6)];
  p2Pieces[PAWN_2_PIECE_INDEX] = board[coordinatesToBoardIndex(6,6)];
  p2Pieces[ASSASSIN_PIECE_INDEX] = board[coordinatesToBoardIndex(0,7)];
  p2Pieces[WARRIOR_PIECE_INDEX] = board[coordinatesToBoardIndex(4,6)];
  p2Pieces[MAGE_PIECE_INDEX] = board[coordinatesToBoardIndex(2,6)];
  p2Pieces[PAWN_3_PIECE_INDEX] = board[coordinatesToBoardIndex(3,6)];
  playerToPieces[Player::PLAYER_2] = p2Pieces;
}


Game::Game() {
  pieceTypeToSquareIndexToLegalMoves = generateLegalMovesOnAnEmptyBoard();
  pieceTypeToSquareIndexToLegalAbilities = generateLegalAbilitiesOnAnEmptyBoard();
  squareToNeighboringSquares = generateSquareToNeighboringSquares();
  reset();
}

/*
 * Assumes that the move and ability are legal.
 * If the ability is not useful (i.e. does not alter the game state), it's converted to
 * AbilityType::NO_ABILITY.
 * Checking whether ability is useful makes the function ~1.5% slower.
 */
void Game::makeAction(int moveSrcIdx, int moveDstIdx, int abilitySrcIdx, int abilityDstIdx) {
  UndoInfo undoInfo = UndoInfo();
  undoInfo.moveSrcIdx = moveSrcIdx;
  undoInfo.moveDstIdx = moveDstIdx;
  if(moveSrcIdx != MOVE_SKIP) {
    board[moveDstIdx] = board[moveSrcIdx];
    board[moveDstIdx]->squareIndex = moveDstIdx;
    board[moveSrcIdx] = new Piece(PieceType::NO_PIECE, 0, moveSrcIdx);
  }
  if(abilitySrcIdx != ABILITY_SKIP) {
    Piece* abilitySrcPiece = board[abilitySrcIdx];
    Piece* abilityDstPiece = board[abilityDstIdx];
    Piece* neighboringPiece;
    int neighboringSquare;
    switch(abilitySrcPiece->type) {
      // king does single target damage
      case P1_KING:
        if(player1OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= KING_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::KING_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        break;
      // mage damages attacked piece and all enemy pieces that are touching it
      case P1_MAGE:
        if(player1OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= MAGE_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::MAGE_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        for(int i = 0; i < squareToNeighboringSquares[abilityDstIdx].size(); i++) {
          neighboringSquare = squareToNeighboringSquares[abilityDstIdx][i];
          neighboringPiece = board[neighboringSquare];
          if(player1OrEmpty(neighboringPiece->type)) continue;  // don't damage your own pieces
          neighboringPiece->healthPoints -= MAGE_ABILITY_POINTS;
          // i+1 because 0 is for abilityDstPiece
          undoInfo.affectedPieces[i+1] = neighboringPiece;
          if(neighboringPiece->healthPoints <= 0) {
            board[neighboringSquare] = new Piece(NO_PIECE, 0, neighboringSquare);
          }
        }
        break;
      case P1_PAWN:
        if(abilityDstPiece->type == P1_WALL || abilityDstPiece->type == P2_WALL) {
          undoInfo.abilityType = AbilityType::PAWN_DESTROY_WALL;
          undoInfo.affectedPieces[0] = abilityDstPiece;
          board[abilityDstIdx] = new Piece(NO_PIECE, 0, abilityDstIdx); // destroy wall
        } else if(abilityDstPiece->type == NO_PIECE) {
          undoInfo.abilityType = AbilityType::PAWN_MAKE_WALL;
          undoInfo.affectedPieces[0] = abilityDstPiece;
          board[abilityDstIdx] = new Piece(P1_WALL, WALL_STARTING_HEALTH_POINTS, abilityDstIdx); // make wall
        } else {
          abilityDstPiece->healthPoints -= PAWN_ABILITY_POINTS; // damage piece
          undoInfo.abilityType = AbilityType::PAWN_DAMAGE;
          undoInfo.affectedPieces[0] = abilityDstPiece;
          if(abilityDstPiece->healthPoints <= 0) {
            board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
          }
        }
        break;
      case P1_WARRIOR:
        if(player1OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= WARRIOR_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::WARRIOR_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        break;
      case P1_WALL:
        break;
      case P1_ASSASSIN:
        if(player1OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= ASSASSIN_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::ASSASSIN_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        break;
      case P2_KING:
        if(player2OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= KING_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::KING_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        break;
      case P2_MAGE:
        if(player2OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= MAGE_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::MAGE_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        for(int i = 0; i < squareToNeighboringSquares[abilityDstIdx].size(); i++) {
          neighboringSquare = squareToNeighboringSquares[abilityDstIdx][i];
          neighboringPiece = board[neighboringSquare];
          if(player2OrEmpty(neighboringPiece->type)) continue;  // don't damage your own pieces
          neighboringPiece->healthPoints -= MAGE_ABILITY_POINTS;
          // i+1 because 0 is for abilityDstPiece
          undoInfo.affectedPieces[i+1] = neighboringPiece;
          if(neighboringPiece->healthPoints <= 0) {
            board[neighboringSquare] = new Piece(NO_PIECE, 0, neighboringSquare);
          }
        }
        break;
      case P2_PAWN:
        if(abilityDstPiece->type == P1_WALL || abilityDstPiece->type == P2_WALL) {
          undoInfo.abilityType = AbilityType::PAWN_DESTROY_WALL;
          undoInfo.affectedPieces[0] = abilityDstPiece;
          board[abilityDstIdx] = new Piece(NO_PIECE, 0, abilityDstIdx); // destroy wall
        } else if(abilityDstPiece->type == NO_PIECE) {
          undoInfo.abilityType = AbilityType::PAWN_MAKE_WALL;
          undoInfo.affectedPieces[0] = abilityDstPiece;
          board[abilityDstIdx] = new Piece(P2_WALL, WALL_STARTING_HEALTH_POINTS, abilityDstIdx); // make wall
        } else {
          abilityDstPiece->healthPoints -= PAWN_ABILITY_POINTS; // damage piece
          undoInfo.abilityType = AbilityType::PAWN_DAMAGE;
          undoInfo.affectedPieces[0] = abilityDstPiece;
          if(abilityDstPiece->healthPoints <= 0) {
            board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
          }
        }
        break;
      case P2_WARRIOR:
        if(player2OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= WARRIOR_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::WARRIOR_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        break;
      case P2_WALL:
        break;
      case P2_ASSASSIN:
        if(player2OrEmpty(abilityDstPiece->type)) {
          undoInfo.abilityType = AbilityType::NO_ABILITY;
          break;
        }
        abilityDstPiece->healthPoints -= ASSASSIN_ABILITY_POINTS;
        undoInfo.abilityType = AbilityType::ASSASSIN_DAMAGE;
        undoInfo.affectedPieces[0] = abilityDstPiece;
        if(abilityDstPiece->healthPoints <= 0) {
          board[abilityDstIdx] = new Piece(PieceType::NO_PIECE, 0, abilityDstIdx);
        }
        break;
    }
  } else {
    undoInfo.abilityType = AbilityType::NO_ABILITY;
  }
  this->moveNumberToUndoInfo[this->moveNumber] = undoInfo;
  this->moveNumber += 1;
  this->currentPlayer = ~currentPlayer;
  return;
}

void Game::undoLastAction() {
  UndoInfo undoInfo = this->moveNumberToUndoInfo[this->moveNumber - 1];
  // undo ability
  switch(undoInfo.abilityType) {
    Piece* affectedPiece;
    case KING_DAMAGE:
      affectedPiece = undoInfo.affectedPieces[0];
      affectedPiece->healthPoints += KING_ABILITY_POINTS;
      this->board[affectedPiece->squareIndex] = affectedPiece;
      break;
    case MAGE_DAMAGE:
      for(int i = 0; i < 9; i++){ // 1 attacked square and 8 neighboring
        affectedPiece = undoInfo.affectedPieces[i];
        if(affectedPiece == nullptr) continue;
        affectedPiece->healthPoints += MAGE_ABILITY_POINTS;
        this->board[affectedPiece->squareIndex] = affectedPiece;
      }
      break;
    case WARRIOR_DAMAGE:
      affectedPiece = undoInfo.affectedPieces[0];
      affectedPiece->healthPoints += WARRIOR_ABILITY_POINTS;
      this->board[affectedPiece->squareIndex] = affectedPiece;
      break;
    case ASSASSIN_DAMAGE:
      affectedPiece = undoInfo.affectedPieces[0];
      affectedPiece->healthPoints += ASSASSIN_ABILITY_POINTS;
      this->board[affectedPiece->squareIndex] = affectedPiece;
      break;
    case PAWN_DAMAGE:
      affectedPiece = undoInfo.affectedPieces[0];
      affectedPiece->healthPoints += PAWN_ABILITY_POINTS;
      this->board[affectedPiece->squareIndex] = affectedPiece;
      break;
    case PAWN_MAKE_WALL:
      affectedPiece = undoInfo.affectedPieces[0];
      this->board[affectedPiece->squareIndex] = new Piece(PieceType::NO_PIECE, 0, affectedPiece->squareIndex);
      break;
    case PAWN_DESTROY_WALL:
      affectedPiece = undoInfo.affectedPieces[0];
      this->board[affectedPiece->squareIndex] = affectedPiece;
      break;
    case NO_ABILITY:
      break;
    default:
      break;
  }
  // undo move
  if(undoInfo.moveSrcIdx != MOVE_SKIP) {
    undoMove(undoInfo.moveSrcIdx, undoInfo.moveDstIdx);
  }

  this->moveNumber -= 1;
  this->currentPlayer = ~currentPlayer;

  return;
}

void Game::print() {
  std::cout << "------------------------------------------\n";
  for(int i = NUM_ROWS-1; i >= 0; i--) {
    std::cout << i << "   ";
    for(int j = 0; j < NUM_COLUMNS; j++) {
      if(board[coordinatesToBoardIndex(j, i)]->type != PieceType::NO_PIECE) {
        std::cout << pieceTypeToString(board[coordinatesToBoardIndex(j, i)]->type) << board[coordinatesToBoardIndex(j, i)]->healthPoints << " ";
      } else {
        std::cout << pieceTypeToString(board[coordinatesToBoardIndex(j, i)]->type) << "   " << " ";
      }
    }
    std::cout << "\n";
  } 
  std::cout << "\n";
  std::cout << "   ";
  for(int i = 0; i < NUM_COLUMNS; i++) {
    std::cout << " " << i << "   ";
  }
  std::cout << "\n------------------------------------------\n";
}

void Game::makeMove(int moveSrcIdx, int moveDstIdx) {
  board[moveDstIdx] = board[moveSrcIdx];
  board[moveDstIdx]->squareIndex = moveDstIdx;
  board[moveSrcIdx] = new Piece(PieceType::NO_PIECE, 0, moveSrcIdx);
  return;
}

/*
 * Since move is being reverted, goal here is to move from "destination" to "source".
 */
void Game::undoMove(int moveSrcIdx, int moveDstIdx) {
  board[moveSrcIdx] = board[moveDstIdx];
  board[moveSrcIdx]->squareIndex = moveSrcIdx;
  board[moveDstIdx] = new Piece(PieceType::NO_PIECE, 0, moveDstIdx);
  return;
}

/*
 * Assumes that the game is not over.
 */
std::vector<PlayerMove> Game::legalMovesByPiece(int srcSquareIdx) {
  std::vector<PlayerMove> retval;
  Piece* piece = board[srcSquareIdx];
  if((!pieceBelongsToPlayer(piece->type, currentPlayer)) ||
      piece->healthPoints <= 0) {
    return retval;
  }
  auto legalMovesOnEmptyBoard = pieceTypeToSquareIndexToLegalMoves[piece->type][piece->squareIndex];
  for(int i = 0; i < legalMovesOnEmptyBoard.size(); i++) {
    if(board[legalMovesOnEmptyBoard[i].moveDstIdx]->type != NO_PIECE) continue;
    retval.push_back(legalMovesOnEmptyBoard[i]);
  }
  return retval;
}

/*
 * Assumes that the game is not over.
 * Useful abilities are those that change the game state.
 * For example, warrior attacking an empty square is legal but doesn't change the game state.
 */
std::vector<PlayerAbility> Game::usefulLegalAbilitiesByPiece(int srcSquareIdx) {
  std::vector<PlayerAbility> retval;
  Piece* piece = board[srcSquareIdx];
  if((!pieceBelongsToPlayer(piece->type, currentPlayer)) ||
      piece->healthPoints <= 0) {
    return retval;
  }
  auto legalAbilitiesOnEmptyBoard = pieceTypeToSquareIndexToLegalAbilities[piece->type][piece->squareIndex];
  for(int l = 0; l < legalAbilitiesOnEmptyBoard.size(); l++) {
    PlayerAbility currentAbility = legalAbilitiesOnEmptyBoard[l];
    Piece* destinationSquarePiece = board[currentAbility.abilityDstIdx];
    // exclude useless abilities, e.g. warrior attacking empty square
    switch(piece->type) {
      // king can only use abilities on enemy pieces
      case P1_KING:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            continue;
          case P1_MAGE:
            continue;
          case P1_PAWN:
            continue;
          case P1_WARRIOR:
            continue;
          case P1_WALL:
            continue;
          case P1_ASSASSIN:
            continue;
          case P2_KING:
            break;
          case P2_MAGE:
            break;
          case P2_PAWN:
            break;
          case P2_WARRIOR:
            break;
          case P2_WALL:
            break;
          case P2_ASSASSIN:
            break;
          case NO_PIECE:
            continue;
          default:
            break;
        }
      // mage can only use abilities on enemy pieces
      case P1_MAGE:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            continue;
          case P1_MAGE:
            continue;
          case P1_PAWN:
            continue;
          case P1_WARRIOR:
            continue;
          case P1_WALL:
            continue;
          case P1_ASSASSIN:
            continue;
          case P2_KING:
            break;
          case P2_MAGE:
            break;
          case P2_PAWN:
            break;
          case P2_WARRIOR:
            break;
          case P2_WALL:
            break;
          case P2_ASSASSIN:
            break;
          case NO_PIECE:
            continue;
          default:
            break;
        }
        break;
      // pawn can use abilities on enemy pieces, on allied walls and on empty squares
      case P1_PAWN:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            continue;
          case P1_MAGE:
            continue;
          case P1_PAWN:
            continue;
          case P1_WARRIOR:
            continue;
          case P1_WALL:
            break;
          case P1_ASSASSIN:
            continue;
          case P2_KING:
            break;
          case P2_MAGE:
            break;
          case P2_PAWN:
            break;
          case P2_WARRIOR:
            break;
          case P2_WALL:
            break;
          case P2_ASSASSIN:
            break;
          case NO_PIECE:
            break;
          default:
            break;
        }
        break;
      // warrior can only use abilities on enemy pieces
      case P1_WARRIOR:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            continue;
          case P1_MAGE:
            continue;
          case P1_PAWN:
            continue;
          case P1_WARRIOR:
            continue;
          case P1_WALL:
            continue;
          case P1_ASSASSIN:
            continue;
          case P2_KING:
            break;
          case P2_MAGE:
            break;
          case P2_PAWN:
            break;
          case P2_WARRIOR:
            break;
          case P2_WALL:
            break;
          case P2_ASSASSIN:
            break;
          case NO_PIECE:
            continue;
          default:
            break;
        }
        break;
      // wall can't use abilities
      case P1_WALL:
        break;
      // assassin can only use abilities on enemy pieces
      case P1_ASSASSIN:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            continue;
          case P1_MAGE:
            continue;
          case P1_PAWN:
            continue;
          case P1_WARRIOR:
            continue;
          case P1_WALL:
            continue;
          case P1_ASSASSIN:
            continue;
          case P2_KING:
            break;
          case P2_MAGE:
            break;
          case P2_PAWN:
            break;
          case P2_WARRIOR:
            break;
          case P2_WALL:
            break;
          case P2_ASSASSIN:
            break;
          case NO_PIECE:
            continue;
          default:
            break;
        }
        break;

      // king can only use abilities on enemy pieces
      case P2_KING:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            break;
          case P1_MAGE:
            break;
          case P1_PAWN:
            break;
          case P1_WARRIOR:
            break;
          case P1_WALL:
            break;
          case P1_ASSASSIN:
            break;
          case P2_KING:
            continue;
          case P2_MAGE:
            continue;
          case P2_PAWN:
            continue;
          case P2_WARRIOR:
            continue;
          case P2_WALL:
            continue;
          case P2_ASSASSIN:
            continue;
          case NO_PIECE:
            continue;
          default:
            break;
        }
      // mage can only use abilities on enemy pieces
      case P2_MAGE:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            break;
          case P1_MAGE:
            break;
          case P1_PAWN:
            break;
          case P1_WARRIOR:
            break;
          case P1_WALL:
            break;
          case P1_ASSASSIN:
            break;
          case P2_KING:
            continue;
          case P2_MAGE:
            continue;
          case P2_PAWN:
            continue;
          case P2_WARRIOR:
            continue;
          case P2_WALL:
            continue;
          case P2_ASSASSIN:
            continue;
          case NO_PIECE:
            continue;
          default:
            break;
        }
        break;
      // pawn can use abilities on enemy pieces, on allied walls and on empty squares
      case P2_PAWN:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            break;
          case P1_MAGE:
            break;
          case P1_PAWN:
            break;
          case P1_WARRIOR:
            break;
          case P1_WALL:
            break;
          case P1_ASSASSIN:
            break;
          case P2_KING:
            continue;
          case P2_MAGE:
            continue;
          case P2_PAWN:
            continue;
          case P2_WARRIOR:
            continue;
          case P2_WALL:
            break;
          case P2_ASSASSIN:
            continue;
          case NO_PIECE:
            break;
          default:
            break;
        }
        break;
      // warrior can only use abilities on enemy pieces
      case P2_WARRIOR:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            break;
          case P1_MAGE:
            break;
          case P1_PAWN:
            break;
          case P1_WARRIOR:
            break;
          case P1_WALL:
            break;
          case P1_ASSASSIN:
            break;
          case P2_KING:
            continue;
          case P2_MAGE:
            continue;
          case P2_PAWN:
            continue;
          case P2_WARRIOR:
            continue;
          case P2_WALL:
            continue;
          case P2_ASSASSIN:
            continue;
          case NO_PIECE:
            continue;
          default:
            break;
        }
        break;
      // wall can't use abilities
      case P2_WALL:
        break;
      // assassin can only use abilities on enemy pieces
      case P2_ASSASSIN:
        switch(destinationSquarePiece->type) {
          case P1_KING:
            break;
          case P1_MAGE:
            break;
          case P1_PAWN:
            break;
          case P1_WARRIOR:
            break;
          case P1_WALL:
            break;
          case P1_ASSASSIN:
            break;
          case P2_KING:
            continue;
          case P2_MAGE:
            continue;
          case P2_PAWN:
            continue;
          case P2_WARRIOR:
            continue;
          case P2_WALL:
            continue;
          case P2_ASSASSIN:
            continue;
          case NO_PIECE:
            continue;
          default:
            break;
        }
        break;
      case NO_PIECE:
        break;
      default:
        break;
    }
    retval.push_back(currentAbility);
  }
  return retval;
}

/*
 * Assumes that the game is not over.
 */
std::vector<PlayerAbility> Game::allLegalAbilitiesByPiece(int srcSquareIdx) {
  std::vector<PlayerAbility> retval;
  Piece* piece = board[srcSquareIdx];
  if((!pieceBelongsToPlayer(piece->type, currentPlayer)) ||
      piece->healthPoints <= 0) {
    return retval;
  }
  retval = pieceTypeToSquareIndexToLegalAbilities[piece->type][piece->squareIndex];
  return retval;
}

/*
 * Useful actions are those whose abilities change the game state.
 * For example, warrior attacking an empty square is legal but doesn't change the game state.
 */
std::vector<PlayerAction> Game::usefulLegalActions() {
  std::vector<PlayerAction> retval;
  // If King is dead, game is over and there are no legal actions
  if(playerToPieces[currentPlayer][KING_PIECE_INDEX]->healthPoints <= 0) {
    return retval;
  }
  for(int i = 0; i < NUM_STARTING_PIECES; i++) {
    Piece* currentPiece = playerToPieces[currentPlayer][i];
    if(currentPiece->healthPoints <= 0) continue; // dead pieces don't move

    auto legalMoves = pieceTypeToSquareIndexToLegalMoves[currentPiece->type][currentPiece->squareIndex];
    for(int j = 0; j < legalMoves.size(); j++) {
      if(board[legalMoves[j].moveDstIdx]->type != NO_PIECE) continue;
      makeMove(legalMoves[j].moveSrcIdx, legalMoves[j].moveDstIdx);
      for(int k = 0; k < NUM_STARTING_PIECES; k++) {
        Piece* cp2 = playerToPieces[currentPlayer][k];
        if(cp2->healthPoints <= 0) continue; // no abilities for dead pieces
        auto legalAbilities = pieceTypeToSquareIndexToLegalAbilities[cp2->type][cp2->squareIndex];
        for(int l = 0; l < legalAbilities.size(); l++) {
          PlayerAbility currentAbility = legalAbilities[l];
          Piece* destinationSquarePiece = board[currentAbility.abilityDstIdx];
          // exclude useless abilities, e.g. warrior attacking empty square
          switch(cp2->type) {
            // king can only use abilities on enemy pieces
            case P1_KING:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  continue;
                case P1_MAGE:
                  continue;
                case P1_PAWN:
                  continue;
                case P1_WARRIOR:
                  continue;
                case P1_WALL:
                  continue;
                case P1_ASSASSIN:
                  continue;
                case P2_KING:
                  break;
                case P2_MAGE:
                  break;
                case P2_PAWN:
                  break;
                case P2_WARRIOR:
                  break;
                case P2_WALL:
                  break;
                case P2_ASSASSIN:
                  break;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
            // mage can only use abilities on enemy pieces
            case P1_MAGE:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  continue;
                case P1_MAGE:
                  continue;
                case P1_PAWN:
                  continue;
                case P1_WARRIOR:
                  continue;
                case P1_WALL:
                  continue;
                case P1_ASSASSIN:
                  continue;
                case P2_KING:
                  break;
                case P2_MAGE:
                  break;
                case P2_PAWN:
                  break;
                case P2_WARRIOR:
                  break;
                case P2_WALL:
                  break;
                case P2_ASSASSIN:
                  break;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
              break;
            // pawn can use abilities on enemy pieces, on allied walls and on empty squares
            case P1_PAWN:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  continue;
                case P1_MAGE:
                  continue;
                case P1_PAWN:
                  continue;
                case P1_WARRIOR:
                  continue;
                case P1_WALL:
                  break;
                case P1_ASSASSIN:
                  continue;
                case P2_KING:
                  break;
                case P2_MAGE:
                  break;
                case P2_PAWN:
                  break;
                case P2_WARRIOR:
                  break;
                case P2_WALL:
                  break;
                case P2_ASSASSIN:
                  break;
                case NO_PIECE:
                  break;
                default:
                  break;
              }
              break;
            // warrior can only use abilities on enemy pieces
            case P1_WARRIOR:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  continue;
                case P1_MAGE:
                  continue;
                case P1_PAWN:
                  continue;
                case P1_WARRIOR:
                  continue;
                case P1_WALL:
                  continue;
                case P1_ASSASSIN:
                  continue;
                case P2_KING:
                  break;
                case P2_MAGE:
                  break;
                case P2_PAWN:
                  break;
                case P2_WARRIOR:
                  break;
                case P2_WALL:
                  break;
                case P2_ASSASSIN:
                  break;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
              break;
            // wall can't use abilities
            case P1_WALL:
              break;
            // assassin can only use abilities on enemy pieces
            case P1_ASSASSIN:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  continue;
                case P1_MAGE:
                  continue;
                case P1_PAWN:
                  continue;
                case P1_WARRIOR:
                  continue;
                case P1_WALL:
                  continue;
                case P1_ASSASSIN:
                  continue;
                case P2_KING:
                  break;
                case P2_MAGE:
                  break;
                case P2_PAWN:
                  break;
                case P2_WARRIOR:
                  break;
                case P2_WALL:
                  break;
                case P2_ASSASSIN:
                  break;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
              break;

            // king can only use abilities on enemy pieces
            case P2_KING:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  break;
                case P1_MAGE:
                  break;
                case P1_PAWN:
                  break;
                case P1_WARRIOR:
                  break;
                case P1_WALL:
                  break;
                case P1_ASSASSIN:
                  break;
                case P2_KING:
                  continue;
                case P2_MAGE:
                  continue;
                case P2_PAWN:
                  continue;
                case P2_WARRIOR:
                  continue;
                case P2_WALL:
                  continue;
                case P2_ASSASSIN:
                  continue;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
            // mage can only use abilities on enemy pieces
            case P2_MAGE:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  break;
                case P1_MAGE:
                  break;
                case P1_PAWN:
                  break;
                case P1_WARRIOR:
                  break;
                case P1_WALL:
                  break;
                case P1_ASSASSIN:
                  break;
                case P2_KING:
                  continue;
                case P2_MAGE:
                  continue;
                case P2_PAWN:
                  continue;
                case P2_WARRIOR:
                  continue;
                case P2_WALL:
                  continue;
                case P2_ASSASSIN:
                  continue;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
              break;
            // pawn can use abilities on enemy pieces, on allied walls and on empty squares
            case P2_PAWN:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  break;
                case P1_MAGE:
                  break;
                case P1_PAWN:
                  break;
                case P1_WARRIOR:
                  break;
                case P1_WALL:
                  break;
                case P1_ASSASSIN:
                  break;
                case P2_KING:
                  continue;
                case P2_MAGE:
                  continue;
                case P2_PAWN:
                  continue;
                case P2_WARRIOR:
                  continue;
                case P2_WALL:
                  break;
                case P2_ASSASSIN:
                  continue;
                case NO_PIECE:
                  break;
                default:
                  break;
              }
              break;
            // warrior can only use abilities on enemy pieces
            case P2_WARRIOR:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  break;
                case P1_MAGE:
                  break;
                case P1_PAWN:
                  break;
                case P1_WARRIOR:
                  break;
                case P1_WALL:
                  break;
                case P1_ASSASSIN:
                  break;
                case P2_KING:
                  continue;
                case P2_MAGE:
                  continue;
                case P2_PAWN:
                  continue;
                case P2_WARRIOR:
                  continue;
                case P2_WALL:
                  continue;
                case P2_ASSASSIN:
                  continue;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
              break;
            // wall can't use abilities
            case P2_WALL:
              break;
            // assassin can only use abilities on enemy pieces
            case P2_ASSASSIN:
              switch(destinationSquarePiece->type) {
                case P1_KING:
                  break;
                case P1_MAGE:
                  break;
                case P1_PAWN:
                  break;
                case P1_WARRIOR:
                  break;
                case P1_WALL:
                  break;
                case P1_ASSASSIN:
                  break;
                case P2_KING:
                  continue;
                case P2_MAGE:
                  continue;
                case P2_PAWN:
                  continue;
                case P2_WARRIOR:
                  continue;
                case P2_WALL:
                  continue;
                case P2_ASSASSIN:
                  continue;
                case NO_PIECE:
                  continue;
                default:
                  break;
              }
              break;
            case NO_PIECE:
              break;
            default:
              break;
          }
          PlayerAction p = PlayerAction(legalMoves[j].moveSrcIdx, legalMoves[j].moveDstIdx, currentAbility.abilitySrcIdx, currentAbility.abilityDstIdx);
          retval.push_back(p);
        }
      }
      // player can skip the ability
      PlayerAction p = PlayerAction(legalMoves[j].moveSrcIdx, legalMoves[j].moveDstIdx, ABILITY_SKIP, ABILITY_SKIP);
      retval.push_back(p);

      undoMove(legalMoves[j].moveSrcIdx, legalMoves[j].moveDstIdx);
    }
  }
  // player can skip the move
  for(int k = 0; k < NUM_STARTING_PIECES; k++) {
    Piece* cp2 = playerToPieces[currentPlayer][k];
    if(cp2->healthPoints <= 0) continue; // no abilities for dead pieces
    auto legalAbilities = pieceTypeToSquareIndexToLegalAbilities[cp2->type][cp2->squareIndex];
    for(int l = 0; l < legalAbilities.size(); l++) {
      Piece* destinationSquarePiece = board[legalAbilities[l].abilityDstIdx];
      // exclude useless abilities
      switch(cp2->type) {
        // king can only use abilities on enemy pieces
        case P1_KING:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              continue;
            case P1_MAGE:
              continue;
            case P1_PAWN:
              continue;
            case P1_WARRIOR:
              continue;
            case P1_WALL:
              continue;
            case P1_ASSASSIN:
              continue;
            case P2_KING:
              break;
            case P2_MAGE:
              break;
            case P2_PAWN:
              break;
            case P2_WARRIOR:
              break;
            case P2_WALL:
              break;
            case P2_ASSASSIN:
              break;
            case NO_PIECE:
              continue;
            default:
              break;
          }
        // mage can only use abilities on enemy pieces
        case P1_MAGE:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              continue;
            case P1_MAGE:
              continue;
            case P1_PAWN:
              continue;
            case P1_WARRIOR:
              continue;
            case P1_WALL:
              continue;
            case P1_ASSASSIN:
              continue;
            case P2_KING:
              break;
            case P2_MAGE:
              break;
            case P2_PAWN:
              break;
            case P2_WARRIOR:
              break;
            case P2_WALL:
              break;
            case P2_ASSASSIN:
              break;
            case NO_PIECE:
              continue;
            default:
              break;
          }
          break;
        // pawn can use abilities on enemy pieces, on allied walls and on empty squares
        case P1_PAWN:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              continue;
            case P1_MAGE:
              continue;
            case P1_PAWN:
              continue;
            case P1_WARRIOR:
              continue;
            case P1_WALL:
              break;
            case P1_ASSASSIN:
              continue;
            case P2_KING:
              break;
            case P2_MAGE:
              break;
            case P2_PAWN:
              break;
            case P2_WARRIOR:
              break;
            case P2_WALL:
              break;
            case P2_ASSASSIN:
              break;
            case NO_PIECE:
              break;
            default:
              break;
          }
          break;
        // warrior can only use abilities on enemy pieces
        case P1_WARRIOR:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              continue;
            case P1_MAGE:
              continue;
            case P1_PAWN:
              continue;
            case P1_WARRIOR:
              continue;
            case P1_WALL:
              continue;
            case P1_ASSASSIN:
              continue;
            case P2_KING:
              break;
            case P2_MAGE:
              break;
            case P2_PAWN:
              break;
            case P2_WARRIOR:
              break;
            case P2_WALL:
              break;
            case P2_ASSASSIN:
              break;
            case NO_PIECE:
              continue;
            default:
              break;
          }
          break;
        // wall can't use abilities
        case P1_WALL:
          break;
        // assassin can only use abilities on enemy pieces
        case P1_ASSASSIN:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              continue;
            case P1_MAGE:
              continue;
            case P1_PAWN:
              continue;
            case P1_WARRIOR:
              continue;
            case P1_WALL:
              continue;
            case P1_ASSASSIN:
              continue;
            case P2_KING:
              break;
            case P2_MAGE:
              break;
            case P2_PAWN:
              break;
            case P2_WARRIOR:
              break;
            case P2_WALL:
              break;
            case P2_ASSASSIN:
              break;
            case NO_PIECE:
              continue;
            default:
              break;
          }
          break;

        // king can only use abilities on enemy pieces
        case P2_KING:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              break;
            case P1_MAGE:
              break;
            case P1_PAWN:
              break;
            case P1_WARRIOR:
              break;
            case P1_WALL:
              break;
            case P1_ASSASSIN:
              break;
            case P2_KING:
              continue;
            case P2_MAGE:
              continue;
            case P2_PAWN:
              continue;
            case P2_WARRIOR:
              continue;
            case P2_WALL:
              continue;
            case P2_ASSASSIN:
              continue;
            case NO_PIECE:
              continue;
            default:
              break;
          }
        // mage can only use abilities on enemy pieces
        case P2_MAGE:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              break;
            case P1_MAGE:
              break;
            case P1_PAWN:
              break;
            case P1_WARRIOR:
              break;
            case P1_WALL:
              break;
            case P1_ASSASSIN:
              break;
            case P2_KING:
              continue;
            case P2_MAGE:
              continue;
            case P2_PAWN:
              continue;
            case P2_WARRIOR:
              continue;
            case P2_WALL:
              continue;
            case P2_ASSASSIN:
              continue;
            case NO_PIECE:
              continue;
            default:
              break;
          }
          break;
        // pawn can use abilities on enemy pieces, on allied walls and on empty squares
        case P2_PAWN:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              break;
            case P1_MAGE:
              break;
            case P1_PAWN:
              break;
            case P1_WARRIOR:
              break;
            case P1_WALL:
              break;
            case P1_ASSASSIN:
              break;
            case P2_KING:
              continue;
            case P2_MAGE:
              continue;
            case P2_PAWN:
              continue;
            case P2_WARRIOR:
              continue;
            case P2_WALL:
              break;
            case P2_ASSASSIN:
              continue;
            case NO_PIECE:
              break;
            default:
              break;
          }
          break;
        // warrior can only use abilities on enemy pieces
        case P2_WARRIOR:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              break;
            case P1_MAGE:
              break;
            case P1_PAWN:
              break;
            case P1_WARRIOR:
              break;
            case P1_WALL:
              break;
            case P1_ASSASSIN:
              break;
            case P2_KING:
              continue;
            case P2_MAGE:
              continue;
            case P2_PAWN:
              continue;
            case P2_WARRIOR:
              continue;
            case P2_WALL:
              continue;
            case P2_ASSASSIN:
              continue;
            case NO_PIECE:
              continue;
            default:
              break;
          }
          break;
        // wall can't use abilities
        case P2_WALL:
          break;
        // assassin can only use abilities on enemy pieces
        case P2_ASSASSIN:
          switch(destinationSquarePiece->type) {
            case P1_KING:
              break;
            case P1_MAGE:
              break;
            case P1_PAWN:
              break;
            case P1_WARRIOR:
              break;
            case P1_WALL:
              break;
            case P1_ASSASSIN:
              break;
            case P2_KING:
              continue;
            case P2_MAGE:
              continue;
            case P2_PAWN:
              continue;
            case P2_WARRIOR:
              continue;
            case P2_WALL:
              continue;
            case P2_ASSASSIN:
              continue;
            case NO_PIECE:
              continue;
            default:
              break;
          }
          break;
        case NO_PIECE:
          break;
        default:
          break;
      }

      PlayerAction p = PlayerAction(MOVE_SKIP, MOVE_SKIP, legalAbilities[l].abilitySrcIdx, legalAbilities[l].abilityDstIdx);
      retval.push_back(p);
    }
  }
  // player can skip both move and ability
  PlayerAction p = PlayerAction(MOVE_SKIP, MOVE_SKIP, ABILITY_SKIP, ABILITY_SKIP);
  retval.push_back(p);
  return retval;
}

/*
 * Includes actions with useless abilities (i.e. those that don't alter the game state)
 */
std::vector<PlayerAction> Game::allLegalActions() {
  std::vector<PlayerAction> retval;
  // If King is dead, game is over and there are no legal actions
  if(playerToPieces[currentPlayer][KING_PIECE_INDEX]->healthPoints <= 0) {
    return retval;
  }
  for(int i = 0; i < NUM_STARTING_PIECES; i++) {
    Piece* currentPiece = playerToPieces[currentPlayer][i];
    if(currentPiece->healthPoints <= 0) continue; // dead pieces don't move

    auto legalMoves = pieceTypeToSquareIndexToLegalMoves[currentPiece->type][currentPiece->squareIndex];
    for(int j = 0; j < legalMoves.size(); j++) {
      if(board[legalMoves[j].moveDstIdx]->type != NO_PIECE) continue;
      makeMove(legalMoves[j].moveSrcIdx, legalMoves[j].moveDstIdx);
      for(int k = 0; k < NUM_STARTING_PIECES; k++) {
        Piece* cp2 = playerToPieces[currentPlayer][k];
        if(cp2->healthPoints <= 0) continue; // no abilities for dead pieces
        auto legalAbilities = pieceTypeToSquareIndexToLegalAbilities[cp2->type][cp2->squareIndex];
        for(int l = 0; l < legalAbilities.size(); l++) {
          PlayerAbility currentAbility = legalAbilities[l];
          Piece* destinationSquarePiece = board[currentAbility.abilityDstIdx];

          PlayerAction p = PlayerAction(legalMoves[j].moveSrcIdx, legalMoves[j].moveDstIdx, currentAbility.abilitySrcIdx, currentAbility.abilityDstIdx);
          retval.push_back(p);
        }
      }
      // player can skip the ability
      PlayerAction p = PlayerAction(legalMoves[j].moveSrcIdx, legalMoves[j].moveDstIdx, ABILITY_SKIP, ABILITY_SKIP);
      retval.push_back(p);

      undoMove(legalMoves[j].moveSrcIdx, legalMoves[j].moveDstIdx);
    }
  }
  // player can skip the move
  for(int k = 0; k < NUM_STARTING_PIECES; k++) {
    Piece* cp2 = playerToPieces[currentPlayer][k];
    if(cp2->healthPoints <= 0) continue; // no abilities for dead pieces
    auto legalAbilities = pieceTypeToSquareIndexToLegalAbilities[cp2->type][cp2->squareIndex];
    for(int l = 0; l < legalAbilities.size(); l++) {
      Piece* destinationSquarePiece = board[legalAbilities[l].abilityDstIdx];
      PlayerAction p = PlayerAction(MOVE_SKIP, MOVE_SKIP, legalAbilities[l].abilitySrcIdx, legalAbilities[l].abilityDstIdx);
      retval.push_back(p);
    }
  }
  // player can skip both move and ability
  PlayerAction p = PlayerAction(MOVE_SKIP, MOVE_SKIP, ABILITY_SKIP, ABILITY_SKIP);
  retval.push_back(p);
  return retval;
}

/*
 * Checks whether values are in the right range.
 */
bool isActionValid(int moveSrcIdx, int moveDstIdx, int abilitySrcIdx, int abilityDstIdx) {
  bool moveValid = false;
  bool abilityValid = false;
  if(moveSrcIdx == MOVE_SKIP && moveDstIdx == MOVE_SKIP) {
    moveValid = true;
  } else if((!isOffBoard(moveSrcIdx)) && (!isOffBoard(moveDstIdx))) {
    moveValid = true;
  }
  if(abilitySrcIdx == ABILITY_SKIP && abilityDstIdx == ABILITY_SKIP) {
    abilityValid = true;
  } else if((!isOffBoard(abilitySrcIdx)) && (!isOffBoard(abilityDstIdx))) {
    abilityValid = true;
  }
  return moveValid && abilityValid;
}

bool Game::isActionLegal(int moveSrcIdx, int moveDstIdx, int abilitySrcIdx, int abilityDstIdx) {
  // It's important for this method to not have many exit points because it's altering the game
  // state. If a return statement is between makeMove and undoMove, game state will remain changed
  // which shouldn't happen in a method that checks action legality.
  bool validInput = isActionValid(moveSrcIdx, moveDstIdx, abilitySrcIdx, abilityDstIdx);
  if(!validInput) return false;

  bool moveLegal = false;
  bool abilityLegal = false;
  bool movePieceBelongsToCurrentPlayerOrMoveSkip = false;
  bool abilityPieceBelongsToCurrentPlayerOrAbilitySkip = false;
  bool movePieceIsAliveOrMoveSkip = false;
  bool abilityPieceIsAliveOrAbilitySkip = false;
  bool currentPlayersKingIsAlive = false;
  Piece* movePiece;
  Piece* abilityPiece;

  if(moveSrcIdx == MOVE_SKIP && moveDstIdx == MOVE_SKIP) {
    moveLegal = true;
    movePieceBelongsToCurrentPlayerOrMoveSkip = true;
    movePieceIsAliveOrMoveSkip = true;
  } else {
    movePiece = board[moveSrcIdx];
    if(pieceBelongsToPlayer(movePiece->type, currentPlayer)) {
      movePieceBelongsToCurrentPlayerOrMoveSkip = true;
    }
    if(movePiece->healthPoints > 0) {
      movePieceIsAliveOrMoveSkip = true;
    }
    auto legalMovesOnEmptyBoard = pieceTypeToSquareIndexToLegalMoves[movePiece->type][movePiece->squareIndex];
    for(int i = 0; i < legalMovesOnEmptyBoard.size(); i++) {
      PlayerMove currentMove = legalMovesOnEmptyBoard[i];
      if(board[currentMove.moveDstIdx]->type == NO_PIECE && 
          currentMove.moveDstIdx == moveDstIdx) {
        moveLegal = true;
        makeMove(moveSrcIdx, moveDstIdx);
        break;
      }
    }
  }
  if(abilitySrcIdx == ABILITY_SKIP && abilityDstIdx == ABILITY_SKIP) {
    abilityLegal = true;
    abilityPieceBelongsToCurrentPlayerOrAbilitySkip = true;
    abilityPieceIsAliveOrAbilitySkip = true;
  } else {
    abilityPiece = board[abilitySrcIdx];
    if(pieceBelongsToPlayer(abilityPiece->type, currentPlayer)) {
      abilityPieceBelongsToCurrentPlayerOrAbilitySkip = true;
    }
    if(abilityPiece->healthPoints > 0) {
      abilityPieceIsAliveOrAbilitySkip = true;
    }
    auto legalAbilitiesOnEmptyBoard = pieceTypeToSquareIndexToLegalAbilities[abilityPiece->type][abilityPiece->squareIndex];
    for(int i = 0; i < legalAbilitiesOnEmptyBoard.size(); i++) {
      PlayerAbility currentAbility = legalAbilitiesOnEmptyBoard[i];
      if(currentAbility.abilityDstIdx == abilityDstIdx) {
        abilityLegal = true;
        break;
      }
    }
  }

  if(moveSrcIdx != MOVE_SKIP) {
    undoMove(moveSrcIdx, moveDstIdx);
  }

  currentPlayersKingIsAlive = playerToPieces[currentPlayer][KING_PIECE_INDEX]->healthPoints > 0;
  
  if(moveLegal && abilityLegal && movePieceBelongsToCurrentPlayerOrMoveSkip &&
      abilityPieceBelongsToCurrentPlayerOrAbilitySkip && movePieceIsAliveOrMoveSkip &&
      abilityPieceIsAliveOrAbilitySkip && currentPlayersKingIsAlive) {
    return true;
  } else {
    return false;
  }
}

Player Game::getCurrentPlayer() {
  return currentPlayer;
}

Piece Game::getPieceByCoordinates(int x, int y) {
  return *board[coordinatesToBoardIndex(x, y)]; 
}

Piece Game::getPieceBySquareIndex(int squareIndex) {
  return *board[squareIndex]; 
}

bool Game::gameOver() {
  Piece* p1King = playerToPieces[PLAYER_1][KING_PIECE_INDEX];
  Piece* p2King = playerToPieces[PLAYER_2][KING_PIECE_INDEX];
  if(p1King->healthPoints <= 0 || p2King->healthPoints <= 0) {
    return true;
  } else {
    return false;
  }
}

std::optional<Player> Game::winner() {
  Piece* p1King = playerToPieces[PLAYER_1][KING_PIECE_INDEX];
  Piece* p2King = playerToPieces[PLAYER_2][KING_PIECE_INDEX];
  if(p1King->healthPoints <= 0) {
    return PLAYER_2;
  } else if(p2King->healthPoints <= 0) {
    return PLAYER_1;
  }
  return std::nullopt;
}

std::string Game::boardToString() {
  std::stringstream retval;
  retval << currentPlayer << "|";
  Piece* currentPiece;
  for(int i = 0; i < NUM_ROWS*NUM_COLUMNS; i++) {
    currentPiece = board[i];
    if(currentPiece->type == NO_PIECE) {
      retval << "empty,";
    } else {
      switch(currentPiece->type) {
        case P1_KING:
          retval << "0-king-";
          break;
        case P1_MAGE:
          retval << "0-mage-";
          break;
        case P1_PAWN:
          retval << "0-pawn-";
          break;
        case P1_WARRIOR:
          retval << "0-warrior-";
          break;
        case P1_WALL:
          retval << "0-wall-";
          break;
        case P1_ASSASSIN:
          retval << "0-assassin-";
          break;
        case P2_KING:
          retval << "1-king-";
          break;
        case P2_MAGE:
          retval << "1-mage-";
          break;
        case P2_PAWN:
          retval << "1-pawn-";
          break;
        case P2_WARRIOR:
          retval << "1-warrior-";
          break;
        case P2_WALL:
          retval << "1-wall-";
          break;
        case P2_ASSASSIN:
          retval << "1-assassin-";
          break;
      }        
      retval << currentPiece->healthPoints << ",";
    }
  }
  return retval.str();
}

void Game::boardFromString(std::string encodedBoard) {
  currentPlayer = (Player)(std::stoi(encodedBoard.substr(0, encodedBoard.find("|"))));
  moveNumber = 0;
  // pieces need to exist in the piece array even if they're dead
  // first all pieces are initialized as dead, then they're replaced if found in the encodedBoard
  std::vector<Piece*> p1Pieces(NUM_STARTING_PIECES);
  p1Pieces[KING_PIECE_INDEX] = new Piece(PieceType::P1_KING, 0, 0);
  p1Pieces[PAWN_1_PIECE_INDEX] = new Piece(PieceType::P1_PAWN, 0, 0);
  p1Pieces[PAWN_2_PIECE_INDEX] = new Piece(PieceType::P1_PAWN, 0, 0);
  p1Pieces[PAWN_3_PIECE_INDEX] = new Piece(PieceType::P1_PAWN, 0, 0);
  p1Pieces[ASSASSIN_PIECE_INDEX] = new Piece(PieceType::P1_ASSASSIN, 0, 0);
  p1Pieces[MAGE_PIECE_INDEX] = new Piece(PieceType::P1_MAGE, 0, 0);
  p1Pieces[WARRIOR_PIECE_INDEX] = new Piece(PieceType::P1_WARRIOR, 0, 0);

  std::vector<Piece*> p2Pieces(NUM_STARTING_PIECES);
  p2Pieces[KING_PIECE_INDEX] = new Piece(PieceType::P2_KING, 0, 0);
  p2Pieces[PAWN_1_PIECE_INDEX] = new Piece(PieceType::P2_PAWN, 0, 0);
  p2Pieces[PAWN_2_PIECE_INDEX] = new Piece(PieceType::P2_PAWN, 0, 0);
  p2Pieces[PAWN_3_PIECE_INDEX] = new Piece(PieceType::P2_PAWN, 0, 0);
  p2Pieces[ASSASSIN_PIECE_INDEX] = new Piece(PieceType::P2_ASSASSIN, 0, 0);
  p2Pieces[MAGE_PIECE_INDEX] = new Piece(PieceType::P2_MAGE, 0, 0);
  p2Pieces[WARRIOR_PIECE_INDEX] = new Piece(PieceType::P2_WARRIOR, 0, 0);


  std::string b1 = encodedBoard.substr(2);
  std::string delimiter1 = ",";
  std::string delimiter2 = "-";
  std::string token1;
  std::string s, tmp;
  size_t pos = 0;
  int boardIdx = 0;
  while((pos = b1.find(delimiter1)) != std::string::npos) {
    token1 = b1.substr(0, pos);
    if(token1 == "empty") {
      board[boardIdx] = new Piece(PieceType::NO_PIECE, 0, boardIdx);
    } else {
      std::stringstream ss(token1);
      std::vector<std::string> words;
      while(std::getline(ss, tmp, '-')) {
        words.push_back(tmp);
      }
      int healthPoints = std::stoi(words[2]);
      s = words[0] + words[1];
      if(s == "0king") {
        board[boardIdx] = new Piece(PieceType::P1_KING, healthPoints, boardIdx);
        p1Pieces[KING_PIECE_INDEX] = board[boardIdx];
      } else if(s == "0pawn") {
        board[boardIdx] = new Piece(PieceType::P1_PAWN, healthPoints, boardIdx);
        if(p1Pieces[PAWN_1_PIECE_INDEX]->healthPoints <= 0) {
          p1Pieces[PAWN_1_PIECE_INDEX] = board[boardIdx];
        } else if(p1Pieces[PAWN_2_PIECE_INDEX]->healthPoints <= 0) {
          p1Pieces[PAWN_2_PIECE_INDEX] = board[boardIdx];
        } else if(p1Pieces[PAWN_3_PIECE_INDEX]->healthPoints <= 0) {
          p1Pieces[PAWN_3_PIECE_INDEX] = board[boardIdx];
        } else {
          throw "Already found 3 living PLAYER_1 Pawns";
        }
      } else if(s == "0mage") {
        board[boardIdx] = new Piece(PieceType::P1_MAGE, healthPoints, boardIdx);
        p1Pieces[MAGE_PIECE_INDEX] = board[boardIdx];
      } else if(s == "0assassin") {
        board[boardIdx] = new Piece(PieceType::P1_ASSASSIN, healthPoints, boardIdx);
        p1Pieces[ASSASSIN_PIECE_INDEX] = board[boardIdx];
      } else if(s == "0warrior") {
        board[boardIdx] = new Piece(PieceType::P1_WARRIOR, healthPoints, boardIdx);
        p1Pieces[WARRIOR_PIECE_INDEX] = board[boardIdx];
      } else if(s == "0wall") {
        board[boardIdx] = new Piece(PieceType::P1_WALL, healthPoints, boardIdx);
      } else if(s == "1king") {
        board[boardIdx] = new Piece(PieceType::P2_KING, healthPoints, boardIdx);
        p2Pieces[KING_PIECE_INDEX] = board[boardIdx];
      } else if(s == "1pawn") {
        board[boardIdx] = new Piece(PieceType::P2_PAWN, healthPoints, boardIdx);
        if(p2Pieces[PAWN_1_PIECE_INDEX]->healthPoints <= 0) {
          p2Pieces[PAWN_1_PIECE_INDEX] = board[boardIdx];
        } else if(p2Pieces[PAWN_2_PIECE_INDEX]->healthPoints <= 0) {
          p2Pieces[PAWN_2_PIECE_INDEX] = board[boardIdx];
        } else if(p2Pieces[PAWN_3_PIECE_INDEX]->healthPoints <= 0) {
          p2Pieces[PAWN_3_PIECE_INDEX] = board[boardIdx];
        } else {
          throw "Already found 3 living PLAYER_2 Pawns";
        }
      } else if(s == "1mage") {
        board[boardIdx] = new Piece(PieceType::P2_MAGE, healthPoints, boardIdx);
        p2Pieces[MAGE_PIECE_INDEX] = board[boardIdx];
      } else if(s == "1assassin") {
        board[boardIdx] = new Piece(PieceType::P2_ASSASSIN, healthPoints, boardIdx);
        p2Pieces[ASSASSIN_PIECE_INDEX] = board[boardIdx];
      } else if(s == "1warrior") {
        board[boardIdx] = new Piece(PieceType::P2_WARRIOR, healthPoints, boardIdx);
        p2Pieces[WARRIOR_PIECE_INDEX] = board[boardIdx];
      } else if(s == "1wall") {
        board[boardIdx] = new Piece(PieceType::P2_WALL, healthPoints, boardIdx);
      }
    }
    b1.erase(0, pos + delimiter1.length());
    boardIdx += 1;
  }

  playerToPieces[PLAYER_1] = p1Pieces;
  playerToPieces[PLAYER_2] = p2Pieces;
}

/* 
 * performance test - https://www.chessprogramming.org/Perft
 * with bulk counting
 */
unsigned long long perft(Game& game, int depth) {
  std::vector<PlayerAction> legalActions;
  unsigned long long nodes = 0;
  legalActions = game.usefulLegalActions();
  int numLegalActions = legalActions.size();
  if(depth == 1) {
    return (unsigned long long) numLegalActions;
  }
  for(int i = 0; i < numLegalActions; i++) {
    if(depth == 3)
      std::cout << i << " / " << numLegalActions <<  "\n";
    PlayerAction pa = legalActions[i];
    game.makeAction(pa.moveSrcIdx, pa.moveDstIdx, pa.abilitySrcIdx, pa.abilityDstIdx);
    nodes += perft(game, depth-1);
    game.undoLastAction();
  }

  return nodes;
}

int main() {
  Game g = Game();
  /*
  while(true) {
    g.print();
    auto allLegalActions = g.allLegalActions();
    auto usefulLegalActions = g.usefulLegalActions();
    std::cout << "Number of all legal actions: " << allLegalActions.size() << "\n";
    std::cout << "Number of useful legal actions: " << usefulLegalActions.size() << "\n";
    int pieceX, pieceY;
    std::cout << "Enter piece's x coordinate\n";
    std::cin >> pieceX;
    std::cout << "Enter piece's y coordinate:\n";
    std::cin >> pieceY;
    Piece p = g.getPieceByCoordinates(pieceX, pieceY);
    p.print();
    auto legalAbilities = g.allLegalAbilitiesByPiece(coordinatesToBoardIndex(pieceX, pieceY));
    std::cout << "all legal abilities:\n";
    for(int i = 0; i < legalAbilities.size(); i++) {
      legalAbilities[i].print();
    }
    bool gameOver = g.gameOver();
    std::optional<Player> winner = g.winner();
    std::cout << "gameOver: " << gameOver << "\n";
    if(winner)
      std::cout << "winner: " << *winner << "\n";
  }
  */

  /*
  std::cout << "Calculating number of nodes at depth 3\n";
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  unsigned long long nodes = perft(g, 3);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Total number of nodes at depth 3: " << nodes << "\n";
  std::cout << "Calculating time: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
  std::cout << "Nodes per second: " << nodes / std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << std::endl;
  */

  std::vector<PlayerAction> legalActions;
  while(true) {
    legalActions = g.usefulLegalActions();
     //std::cout << "Total number of legal actions: " << legalActions.size() << "\n";
    std::string boardString = g.boardToString();
    std::cout << boardString << "\n";
    g.boardFromString(boardString);
    g.print();
    if(g.getCurrentPlayer() == PLAYER_1) {
      std::cout << "Player to move: PLAYER_1 (upper-case letters)\n";
    } else {
      std::cout << "Player to move: PLAYER_2 (lower-case letters)\n";
    }
    int x1, y1, x2, y2, x3, y3, x4, y4;
    std::cout << "Input assumed valid\n";
    std::cout << "Enter move's source x coordinate:";
    std::cin >> x1;
    std::cout << "Enter move's source y coordinate:";
    std::cin >> y1;
    std::cout << "Enter move's destination x coordinate:";
    std::cin >> x2;
    std::cout << "Enter move's destination y coordinate:";
    std::cin >> y2;
    std::cout << "Enter ability's source x coordinate:";
    std::cin >> x3;
    std::cout << "Enter ability's source y coordinate:";
    std::cin >> y3;
    std::cout << "Enter ability's destination x coordinate:";
    std::cin >> x4;
    std::cout << "Enter ability's destination y coordinate:";
    std::cin >> y4;

    int moveSrcIdx;
    int moveDstIdx;
    int abilitySrcIdx;
    int abilityDstIdx;
    if(x1 == MOVE_SKIP || x2 == MOVE_SKIP || y1 == MOVE_SKIP || y2 == MOVE_SKIP) {
      moveSrcIdx = MOVE_SKIP;
      moveDstIdx = MOVE_SKIP;
    } else {
      moveSrcIdx = coordinatesToBoardIndex(x1, y1);
      moveDstIdx = coordinatesToBoardIndex(x2, y2);
    }
    if(x3 == ABILITY_SKIP || x4 == ABILITY_SKIP || y3 == ABILITY_SKIP || y4 == ABILITY_SKIP) {
      abilitySrcIdx = ABILITY_SKIP;
      abilityDstIdx = ABILITY_SKIP;
    } else {
      abilitySrcIdx = coordinatesToBoardIndex(x3, y3);
      abilityDstIdx = coordinatesToBoardIndex(x4, y4);
    }

    if(x1 == -2 || x2 == -2 || x3 == -2 || x4 == -2 
        || y1 == -2 || y2 == -2 || y3 == -2 || y4 == -2) { // TODO: testing, delete when done.
      g.undoLastAction();
    } else {
      g.makeAction(moveSrcIdx, moveDstIdx, abilitySrcIdx, abilityDstIdx);
    }
    std::cout << "--------------------\n";
  }
  return 0;
}
