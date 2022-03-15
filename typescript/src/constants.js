export var Player;
(function (Player) {
    Player[Player["PLAYER_1"] = 0] = "PLAYER_1";
    Player[Player["PLAYER_2"] = 1] = "PLAYER_2";
    Player[Player["NO_PLAYER"] = 2] = "NO_PLAYER";
})(Player || (Player = {}));
export var Phase;
(function (Phase) {
    Phase[Phase["MOVE"] = 0] = "MOVE";
    Phase[Phase["ABILITY"] = 1] = "ABILITY";
})(Phase || (Phase = {}));
export var WarriorMoveOffsets = [
    [-1, -1], [-1, 0], [-1, 1],
    [0, -2], [0, -1], [0, 1], [0, 2],
    [1, -1], [1, 0], [1, 1]
];
export var Player1PawnMoveOffsets = [
    [-1, -1], [-1, 0], [-1, 1],
    [0, -1], [0, 1], [0, 2],
    [1, -1], [1, 0], [1, 1]
];
export var Player2PawnMoveOffsets = [
    [-1, -1], [-1, 0], [-1, 1],
    [0, -2], [0, -1], [0, 1],
    [1, -1], [1, 0], [1, 1]
];
export var MageMoveOffsets = [
    [-1, -1], [-1, 0], [-1, 1],
    [0, -1], [0, 1],
    [1, -1], [1, 0], [1, 1]
];
export var AssassinMoveOffsets = [
    [-3, -3], [-3, 3],
    [-2, -2], [-2, -1], [-2, 0], [-2, 1], [-2, 2],
    [-1, -2], [-1, -1], [-1, 0], [-1, 1], [-1, 2],
    [0, -2], [0, -1], [0, 1], [0, 2],
    [1, -2], [1, -1], [1, 0], [1, 1], [1, 2],
    [2, -2], [2, -1], [2, 0], [2, 1], [2, 2],
    [3, -3], [3, 3]
];
export var KingMoveOffsets = [
    [-1, -1], [-1, 0], [-1, 1],
    [0, -1], [0, 1],
    [1, -1], [1, 0], [1, 1]
];
export var WarriorAbilityOffsets = [
    [-1, -1], [-1, 0], [-1, 1],
    [0, -1], [0, 1],
    [1, -1], [1, 0], [1, 1]
];
export var AssassinAbilityOffsets = [
    [-1, -1], [-1, 0], [-1, 1],
    [0, -1], [0, 1],
    [1, -1], [1, 0], [1, 1]
];
export var PawnAbilityOffsets = [
    [-1, -1], [-1, 0], [-1, 1],
    [0, -1], [0, 1],
    [1, -1], [1, 0], [1, 1]
];
export var MageAbilityOffsets = [
    [-2, -2], [-2, -1], [-2, 0], [-2, 1], [-2, 2],
    [-1, -2], [-1, -1], [-1, 0], [-1, 1], [-1, 2],
    [0, -2], [0, -1], [0, 1], [0, 2],
    [1, -2], [1, -1], [1, 0], [1, 1], [1, 2],
    [2, -2], [2, -1], [2, 0], [2, 1], [2, 2]
];
export var KingAbilityOffsets = [
    [-1, -1], [-1, 0], [-1, 1],
    [0, -1], [0, 1],
    [1, -1], [1, 0], [1, 1]
];
