import { __extends } from "tslib";
import { Player, WarriorMoveOffsets, Player1PawnMoveOffsets, Player2PawnMoveOffsets, KingMoveOffsets, MageMoveOffsets, AssassinMoveOffsets, WarriorAbilityOffsets, PawnAbilityOffsets, KingAbilityOffsets, MageAbilityOffsets, AssassinAbilityOffsets } from './constants';
var GameObject = /** @class */ (function () {
    function GameObject(x, y, player, type) {
        this.x = x;
        this.y = y;
        this.player = player;
        this.type = type;
    }
    return GameObject;
}());
export { GameObject };
var Piece = /** @class */ (function (_super) {
    __extends(Piece, _super);
    function Piece(x, y, player, type, healthPoints, abilityPoints) {
        var _this = _super.call(this, x, y, player, type) || this;
        _this.healthPoints = healthPoints;
        _this.abilityPoints = abilityPoints;
        return _this;
    }
    return Piece;
}(GameObject));
export { Piece };
var Mage = /** @class */ (function (_super) {
    __extends(Mage, _super);
    function Mage(x, y, player) {
        return _super.call(this, x, y, player, 'mage', 300, 80) || this;
    }
    return Mage;
}(Piece));
export { Mage };
var King = /** @class */ (function (_super) {
    __extends(King, _super);
    function King(x, y, player) {
        return _super.call(this, x, y, player, 'king', 200, 30) || this;
    }
    return King;
}(Piece));
export { King };
var Warrior = /** @class */ (function (_super) {
    __extends(Warrior, _super);
    function Warrior(x, y, player) {
        return _super.call(this, x, y, player, 'warrior', 600, 100) || this;
    }
    return Warrior;
}(Piece));
export { Warrior };
var Pawn = /** @class */ (function (_super) {
    __extends(Pawn, _super);
    function Pawn(x, y, player) {
        return _super.call(this, x, y, player, 'pawn', 300, 0) || this;
    }
    return Pawn;
}(Piece));
export { Pawn };
var Assassin = /** @class */ (function (_super) {
    __extends(Assassin, _super);
    function Assassin(x, y, player) {
        return _super.call(this, x, y, player, 'assassin', 20, 200) || this;
    }
    return Assassin;
}(Piece));
export { Assassin };
var Wall = /** @class */ (function (_super) {
    __extends(Wall, _super);
    function Wall(x, y, player) {
        return _super.call(this, x, y, player, 'wall', 100, 0) || this;
    }
    return Wall;
}(Piece));
export { Wall };
var Empty = /** @class */ (function (_super) {
    __extends(Empty, _super);
    function Empty(x, y) {
        return _super.call(this, x, y, Player.NO_PLAYER, 'empty') || this;
    }
    return Empty;
}(GameObject));
export { Empty };
export function getMoveOffsets(piece) {
    if (piece instanceof Pawn) {
        if (piece.player == Player.PLAYER_1) {
            return Player1PawnMoveOffsets;
        }
        else {
            return Player2PawnMoveOffsets;
        }
    }
    if (piece instanceof Warrior) {
        return WarriorMoveOffsets;
    }
    if (piece instanceof Assassin) {
        return AssassinMoveOffsets;
    }
    if (piece instanceof Mage) {
        return MageMoveOffsets;
    }
    if (piece instanceof King) {
        return KingMoveOffsets;
    }
    return null;
}
export function getAbilityOffsets(piece) {
    if (piece instanceof Pawn) {
        return PawnAbilityOffsets;
    }
    if (piece instanceof Warrior) {
        return WarriorAbilityOffsets;
    }
    if (piece instanceof Assassin) {
        return AssassinAbilityOffsets;
    }
    if (piece instanceof Mage) {
        return MageAbilityOffsets;
    }
    if (piece instanceof King) {
        return KingAbilityOffsets;
    }
    return [];
}
