//MinhNOthelloAI.cpp

#include <ics46/factory/DynamicFactory.hpp>
#include "MinhNOthelloAI.hpp"
#include <iostream>

ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, minhn7::MinhNOthelloAI, "MinhNOthelloAI (Required)");

pair<int, int> minhn7::MinhNOthelloAI::chooseMove(const OthelloGameState& state) {
    pair<int, int> coordinates;
    int score = 0;
    string player = "";

    //get player color and current score
    if(state.isBlackTurn()){
        player = "black";
        score = state.blackScore() - state.whiteScore();
    }

    else{
        player = "white";
        score = state.whiteScore() - state.blackScore();
    }
 
    //make copy of game
    unique_ptr<OthelloGameState> sCopy = state.clone();
 
    //get valid moves
    vector<pair<int, int>> validMoves = getValidMoves(sCopy);
    coordinates=validMoves[0];

    //go through each valid moves
    for(int i=0; i<validMoves.size(); i++){
        //make new copy for each move
        unique_ptr<OthelloGameState> sC = state.clone();
        sC->makeMove(validMoves[i].first, validMoves[i].second);
        int possibleScore = search(sC,player,6);
           
        //use valid move with highest possible score
        if (possibleScore>score){
            score = possibleScore;
            coordinates = validMoves[i];
        }
    }

    return coordinates;
}

int minhn7::MinhNOthelloAI::search(unique_ptr<OthelloGameState>& sCopy, string player, int depth){
    int min=100; 
    int max=0; 
    int score;
    
    //return evaluation of s
    if(depth==0)
        {
            if(player=="black"){
                return(score = sCopy->blackScore()-sCopy->whiteScore());
            }
            else
                return(score = sCopy->whiteScore()-sCopy->blackScore());
        }

    else{
        //generate valid moves
        vector<pair<int, int>> validMoves = getValidMoves(sCopy);

        //if my turn
        if((player=="black" && sCopy->isBlackTurn()) || (player=="white" && sCopy->isWhiteTurn())){
            for(int i=0; i<validMoves.size(); i++){
            sCopy->makeMove(validMoves[i].first, validMoves[i].second);
            score = search(sCopy, player, depth-1);
            if (score>max) 
                max=score;
            return max;
            }
        }

        //if opponent's turn
        else{
            for(int j=0; j<validMoves.size(); j++){
            sCopy->makeMove(validMoves[j].first, validMoves[j].second);
            score = search(sCopy, player, depth-1);
            if (score<min)
                min=score;
            return min;
            }
        }
    }

    return 0;
}

vector<pair<int, int>> minhn7::MinhNOthelloAI::getValidMoves(unique_ptr <OthelloGameState>& s){
    vector<pair<int, int>> validMoves;
    
    for(int x=0; x<s->board().width(); x++){
        for(int y=0; y<s->board().height(); y++){
            if(s->isValidMove(x,y)){
                validMoves.push_back(make_pair(x,y));
            }
        }
    }

    random_shuffle(validMoves.begin(), validMoves.end());
    return validMoves;
}
