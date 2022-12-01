Learning C++ as I go.

Using 8x8 array representation, also known as "mailbox". Once baseline implementation is
finished, I'll try to improve it with bitboards.

**Important**: *std::vector<PlayerAction> legalActions* currently does **not** return actions with useless
abilities, e.g. warrior attacking an empty square, despite them being technically legal. This
implementation is made for AI training, where useless actions should never be considered. I might
split *legalActions* into *allLegalActions* and *usefulLegalActions*.
