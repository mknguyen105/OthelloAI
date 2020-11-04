//MinhNOthelloAI.hpp

#ifndef MINHNOTHELLOAI_HPP
#define MINHNOTHELLOAI_HPP

#include "OthelloAI.hpp"
#include "OthelloBoard.hpp"
#include "OthelloGameState.hpp"
#include <vector>
#include <utility>
#include <string>

using namespace std;

namespace minhn7 {
    class MinhNOthelloAI : public OthelloAI {
    public:
        pair<int,int> chooseMove(const OthelloGameState& state);
        static int search(unique_ptr<OthelloGameState>& sCopy, string player, int depth);
        static vector<pair<int, int>> getValidMoves(unique_ptr <OthelloGameState>& s);
    };
}

#endif
