import { Position } from './position';
import { Mage, Wall, Pawn, Warrior, King, Empty, Piece, Assassin } from './game-object';
import { Player, Phase } from './constants';
var Board = /** @class */ (function () {
    function Board() {
        this.board = new Map();
        this.height = 10;
        this.width = 10;
        this._phase = Phase.MOVE;
        this._playerTurn = Player.PLAYER_1;
        for (var i = 0; i < this.height; i++) {
            for (var j = 0; j < this.width; j++) {
                var p = new Position(i, j);
                var go = new Empty(i, j);
                this.board.set(JSON.stringify(p), go);
            }
            // player_1
            this.board.set(JSON.stringify(new Position(0, 0)), new Assassin(0, 0, Player.PLAYER_1));
            this.board.set(JSON.stringify(new Position(1, 0)), new Warrior(1, 0, Player.PLAYER_1));
            this.board.set(JSON.stringify(new Position(3, 0)), new Mage(3, 0, Player.PLAYER_1));
            this.board.set(JSON.stringify(new Position(4, 0)), new King(4, 0, Player.PLAYER_1));
            this.board.set(JSON.stringify(new Position(5, 0)), new Mage(5, 0, Player.PLAYER_1));
            this.board.set(JSON.stringify(new Position(8, 0)), new Warrior(8, 0, Player.PLAYER_1));
            this.board.set(JSON.stringify(new Position(9, 0)), new Assassin(9, 0, Player.PLAYER_1));
            this.board.set(JSON.stringify(new Position(2, 1)), new Pawn(2, 1, Player.PLAYER_1));
            this.board.set(JSON.stringify(new Position(3, 1)), new Pawn(3, 1, Player.PLAYER_1));
            this.board.set(JSON.stringify(new Position(4, 1)), new Pawn(4, 1, Player.PLAYER_1));
            this.board.set(JSON.stringify(new Position(5, 1)), new Pawn(5, 1, Player.PLAYER_1));
            this.board.set(JSON.stringify(new Position(6, 1)), new Pawn(6, 1, Player.PLAYER_1));
            // player_2
            this.board.set(JSON.stringify(new Position(0, 9)), new Assassin(0, 9, Player.PLAYER_2));
            this.board.set(JSON.stringify(new Position(1, 9)), new Warrior(1, 9, Player.PLAYER_2));
            this.board.set(JSON.stringify(new Position(3, 9)), new Mage(3, 9, Player.PLAYER_2));
            this.board.set(JSON.stringify(new Position(4, 9)), new King(4, 9, Player.PLAYER_2));
            this.board.set(JSON.stringify(new Position(5, 9)), new Mage(5, 9, Player.PLAYER_2));
            this.board.set(JSON.stringify(new Position(8, 9)), new Warrior(8, 9, Player.PLAYER_2));
            this.board.set(JSON.stringify(new Position(9, 9)), new Assassin(9, 9, Player.PLAYER_2));
            this.board.set(JSON.stringify(new Position(2, 8)), new Pawn(2, 8, Player.PLAYER_2));
            this.board.set(JSON.stringify(new Position(3, 8)), new Pawn(3, 8, Player.PLAYER_2));
            this.board.set(JSON.stringify(new Position(4, 8)), new Pawn(4, 8, Player.PLAYER_2));
            this.board.set(JSON.stringify(new Position(5, 8)), new Pawn(5, 8, Player.PLAYER_2));
            this.board.set(JSON.stringify(new Position(6, 8)), new Pawn(6, 8, Player.PLAYER_2));
        }
    }
    Board.prototype.toString = function () {
        var retval = this._playerTurn + '|' + this._phase + '|';
        for (var i = 0; i < this.height; i++) {
            for (var j = 0; j < this.width; j++) {
                var go = this.getGameObjectByCoordinates(j, i);
                if (go.type == 'empty') {
                    retval += go.type + ',';
                }
                else {
                    var piece = go;
                    retval += piece.player + '-' + piece.type + '-' + piece.healthPoints + ',';
                }
            }
        }
        return retval;
    };
    Board.prototype.fromString = function (encodedBoard) {
        var newBoard = new Map();
        var ss = encodedBoard.split('|');
        if (ss[0] == '0')
            this._playerTurn = Player.PLAYER_1;
        else
            this._playerTurn = Player.PLAYER_2;
        if (ss[1] == '0')
            this._phase = Phase.MOVE;
        else
            this._phase = Phase.ABILITY;
        var s = ss[2].split(',');
        for (var i = 0; i < this.height; i++) {
            for (var j = 0; j < this.width; j++) {
                var currentObject = s[i * 10 + j];
                if (currentObject == 'empty') {
                    newBoard.set(JSON.stringify(new Position(j, i)), new Empty(j, i));
                }
                else {
                    var currentObjectSplit = currentObject.split('-');
                    var player = void 0;
                    var piece = void 0;
                    if (currentObjectSplit[0] == '0')
                        player = Player.PLAYER_1;
                    else
                        player = Player.PLAYER_2;
                    var type = currentObjectSplit[1];
                    var healthPoints = parseInt(currentObjectSplit[2]);
                    if (type == 'pawn') {
                        piece = new Pawn(j, i, player);
                    }
                    else if (type == 'mage') {
                        piece = new Mage(j, i, player);
                    }
                    else if (type == 'assassin') {
                        piece = new Assassin(j, i, player);
                    }
                    else if (type == 'warrior') {
                        piece = new Warrior(j, i, player);
                    }
                    else if (type == 'wall') {
                        piece = new Wall(j, i, player);
                    }
                    else {
                        piece = new King(j, i, player);
                    }
                    piece.healthPoints = healthPoints;
                    newBoard.set(JSON.stringify(new Position(j, i)), piece);
                }
            }
        }
        this.board = newBoard;
    };
    Board.prototype.isOffBoard = function (x, y) {
        if (x > (this.width - 1) || x < 0 || y > (this.height - 1) || y < 0)
            return true;
        return false;
    };
    Board.prototype.isEmpty = function (x, y) {
        var gameObject = this.getGameObjectByCoordinates(x, y);
        return gameObject instanceof Empty;
    };
    Board.prototype.getNeighbourEnemyPieces = function (piece) {
        var retval = [];
        for (var i = -1; i <= 1; i++) {
            for (var j = -1; j <= 1; j++) {
                var newX = piece.x + i;
                var newY = piece.y + j;
                var newPiece = this.getGameObjectByCoordinates(newX, newY);
                if (!(newPiece instanceof Piece)) {
                    continue;
                }
                if (newPiece.player == piece.player && newPiece.player != Player.NO_PLAYER) {
                    retval.push(newPiece);
                }
            }
        }
        return retval;
    };
    Board.prototype.getGameObjectByCoordinates = function (x, y) {
        return this.board.get(JSON.stringify(new Position(x, y)));
    };
    Board.prototype.movePiece = function (piece, newX, newY) {
        var oldPosition = new Position(piece.x, piece.y);
        var newPosition = new Position(newX, newY);
        this.board.set(JSON.stringify(oldPosition), new Empty(piece.x, piece.y));
        this.board.set(JSON.stringify(newPosition), piece);
        piece.x = newX;
        piece.y = newY;
    };
    Board.prototype.removeIfDead = function (piece) {
        if (piece.healthPoints <= 0) {
            var p = new Position(piece.x, piece.y);
            this.board.set(JSON.stringify(p), new Empty(piece.x, piece.y));
        }
    };
    Board.prototype.addGameObject = function (x, y, gameObject) {
        var p = new Position(x, y);
        this.board.set(JSON.stringify(p), gameObject);
    };
    Board.prototype.playerTurn = function () {
        return this._playerTurn;
    };
    Board.prototype.changePlayerTurn = function () {
        if (this._playerTurn == Player.PLAYER_1)
            this._playerTurn = Player.PLAYER_2;
        else
            this._playerTurn = Player.PLAYER_1;
    };
    Board.prototype.phase = function () {
        return this._phase;
    };
    Board.prototype.setPhase = function (p) {
        this._phase = p;
    };
    return Board;
}());
export { Board };
