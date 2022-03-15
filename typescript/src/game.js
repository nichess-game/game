import { Board } from './board';
import { Player, Phase } from './constants';
import { Empty, Piece, Mage, King, Pawn, Warrior, Wall, getMoveOffsets, getAbilityOffsets } from './game-object';
import { Move } from './move';
import { Ability } from './ability';
var Game = /** @class */ (function () {
    function Game() {
        this._board = new Board();
        this._gameOver = false;
        this._winner = null;
    }
    Game.prototype.move = function (pieceX, pieceY, destinationX, destinationY) {
        if (this._board.phase() != Phase.MOVE)
            return false;
        var piece = this._board.getGameObjectByCoordinates(pieceX, pieceY);
        if (piece.player != this._board.playerTurn()) {
            return false;
        }
        if (!this._board.isOffBoard(destinationX, destinationY) && this._board.isEmpty(destinationX, destinationY)) {
            var legalMoves = this.legalMoves(pieceX, pieceY);
            for (var i = 0; i < legalMoves.length; i++) {
                if (legalMoves[i].x == destinationX && legalMoves[i].y == destinationY) {
                    this._board.movePiece(piece, destinationX, destinationY);
                    this._board.setPhase(Phase.ABILITY);
                    return true;
                }
            }
        }
        return false;
    };
    Game.prototype.reset = function () {
        this._board = new Board();
        this._gameOver = false;
        this._winner = null;
    };
    Game.prototype._pawnAbility = function (pawn, destinationX, destinationY) {
        var destinationPiece = this._board.getGameObjectByCoordinates(destinationX, destinationY);
        var legalAbilities = this.legalAbilities(pawn.x, pawn.y);
        if (destinationPiece instanceof Empty) {
            this._board.addGameObject(destinationX, destinationY, new Wall(destinationX, destinationY, pawn.player));
            return true;
        }
        if (destinationPiece instanceof Wall) {
            this._board.addGameObject(destinationX, destinationY, new Empty(destinationX, destinationY));
            return true;
        }
        return false;
    };
    Game.prototype._mageAbility = function (sourcePiece, destinationX, destinationY) {
        var enemyPiece = this._board.getGameObjectByCoordinates(destinationX, destinationY);
        if (enemyPiece instanceof Empty)
            return true;
        if (!(enemyPiece instanceof Piece) || enemyPiece.player == sourcePiece.player) {
            return false;
        }
        var neighbourEnemyPieces = this._board.getNeighbourEnemyPieces(enemyPiece);
        for (var i = 0; i < neighbourEnemyPieces.length; i++) {
            var piece = neighbourEnemyPieces[i];
            piece.healthPoints -= sourcePiece.abilityPoints;
            this._board.removeIfDead(piece);
        }
        return true;
    };
    Game.prototype._defaultAbility = function (sourcePiece, destinationX, destinationY) {
        var destinationPiece = this._board.getGameObjectByCoordinates(destinationX, destinationY);
        if (destinationPiece instanceof Empty)
            return true;
        if (!(destinationPiece instanceof Piece) || destinationPiece.player == sourcePiece.player) {
            return false;
        }
        if (sourcePiece.player != destinationPiece.player) {
            destinationPiece.healthPoints -= sourcePiece.abilityPoints;
            this._board.removeIfDead(destinationPiece);
        }
        return true;
    };
    Game.prototype.ability = function (pieceX, pieceY, destinationX, destinationY) {
        if (this._board.phase() != Phase.ABILITY)
            return false;
        var sourcePiece = this._board.getGameObjectByCoordinates(pieceX, pieceY);
        if (sourcePiece.player != this._board.playerTurn()) {
            return false;
        }
        var legalAbilities = this.legalAbilities(sourcePiece.x, sourcePiece.y);
        var success = false;
        for (var i = 0; i < legalAbilities.length; i++) {
            if (legalAbilities[i].x == destinationX && legalAbilities[i].y == destinationY) {
                if (sourcePiece instanceof Pawn) {
                    success = this._pawnAbility(sourcePiece, destinationX, destinationY);
                }
                else if (sourcePiece instanceof Mage) {
                    success = this._mageAbility(sourcePiece, destinationX, destinationY);
                }
                else {
                    success = this._defaultAbility(sourcePiece, destinationX, destinationY);
                }
                break;
            }
        }
        if (success) {
            this._updateGameOver();
            this._board.changePlayerTurn();
            this._board.setPhase(Phase.MOVE);
        }
        return success;
    };
    Game.prototype.legalAbilities = function (pieceX, pieceY) {
        if (this._board.phase() != Phase.ABILITY)
            return [];
        var retval = [];
        var sourcePiece = this._board.getGameObjectByCoordinates(pieceX, pieceY);
        if (sourcePiece.player != this._board.playerTurn()) {
            return [];
        }
        var abilityOffsets = getAbilityOffsets(sourcePiece);
        for (var i = 0; i < abilityOffsets.length; i++) {
            var x = sourcePiece.x + abilityOffsets[i][0];
            var y = sourcePiece.y + abilityOffsets[i][1];
            retval.push(new Ability(x, y));
        }
        return retval;
    };
    Game.prototype.board = function () {
        var retval = [];
        var row = [];
        for (var i = this._board.height - 1; i >= 0; i--) {
            for (var j = 0; j < this._board.width; j++) {
                row.push(this._board.getGameObjectByCoordinates(j, i));
            }
            retval.push(row);
            row = [];
        }
        return retval;
    };
    Game.prototype.getGameObjectByCoordinates = function (x, y) {
        return this._board.getGameObjectByCoordinates(x, y);
    };
    Game.prototype.legalMoves = function (pieceX, pieceY) {
        if (this._board.phase() != Phase.MOVE)
            return [];
        var retval = [];
        var sourcePiece = this._board.getGameObjectByCoordinates(pieceX, pieceY);
        if (sourcePiece.player != this._board.playerTurn()) {
            return [];
        }
        var moveOffsets = getMoveOffsets(sourcePiece);
        for (var i = 0; i < moveOffsets.length; i++) {
            var x = sourcePiece.x + moveOffsets[i][0];
            var y = sourcePiece.y + moveOffsets[i][1];
            // check that warrior/pawn can't jump over pieces
            // this is bad, but move generation in general is tricky, so it will do for now
            if ((sourcePiece instanceof Pawn) || (sourcePiece instanceof Warrior)) {
                if (moveOffsets[i][1] == 2) {
                    if (!this._board.isEmpty(x, y - 1))
                        continue;
                }
                if (moveOffsets[i][1] == -2) {
                    if (!this._board.isEmpty(x, y + 1))
                        continue;
                }
            }
            if (this._board.getGameObjectByCoordinates(x, y) instanceof Empty) {
                retval.push(new Move(x, y));
            }
        }
        return retval;
    };
    Game.prototype._updateGameOver = function () {
        var player1KingAlive = false;
        var player2KingAlive = false;
        for (var i = 0; i < this._board.height; i++) {
            for (var j = 0; j < this._board.width; j++) {
                var piece = this._board.getGameObjectByCoordinates(j, i);
                if (piece instanceof King) {
                    if (piece.player == Player.PLAYER_1)
                        player1KingAlive = true;
                    else
                        player2KingAlive = true;
                }
            }
        }
        if (!player1KingAlive) {
            this._gameOver = true;
            this._winner = Player.PLAYER_2;
        }
        if (!player2KingAlive) {
            this._gameOver = true;
            this._winner = Player.PLAYER_1;
        }
    };
    Game.prototype.gameOver = function () {
        return [this._gameOver, this._winner];
    };
    Game.prototype.boardToString = function () {
        return this._board.toString();
    };
    Game.prototype.boardFromString = function (encodedBoard) {
        this._board.fromString(encodedBoard);
    };
    return Game;
}());
export { Game };
