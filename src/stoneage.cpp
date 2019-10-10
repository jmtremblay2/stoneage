#include "h/stoneage.h"
#include "h/csv.h"
#include "h/deck.hpp"
#include "h/utils.h"


#include <iostream>
#include <string>

using std::string;
using std::cout;

std::ostream& operator<<(std::ostream& os, const SA::PlayerID p){
    switch(p){
        case SA::PlayerID::ONE:  os << "player 1";    break;
        case SA::PlayerID::TWO:  os << "player 2";     break;
        case SA::PlayerID::NONE: os << "player OH OH"; break;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const SA::Building& b){
    os << "wood: " << b.wood << ",  " <<
        "brick: " << b.brick << ",  " <<
        "stone: " << b.stone << ",  " <<
        "gold: " << b.gold << ",  " <<
        "nress: " << b.nressources << ",  " <<
        "nkinds: " << b.nkinds << ",  " <<
        "any7: " << b.any << "\n";
    return os;
}

SA::StoneAge::StoneAge() :
        wood(20),
        brick(16),
        stone(12),
        gold(10),
        food(1000){

    populateBuildings();
    populateCivCards();
    populateActions();
}


void SA::StoneAge::populateBuildings(){
    CSV data("data/buildings.csv");

    Deck<Building> buildings;
    for(int i = 0; i < data.getNrow(); ++i){
        // construct the building then push it n times;
        SA::Building b(
            int(data.elemAsDouble(i, "wood")),
            int(data.elemAsDouble(i, "brick")),
            int(data.elemAsDouble(i, "stone")),
            int(data.elemAsDouble(i, "gold")),
            int(data.elemAsDouble(i, "exactlynressources")),
            int(data.elemAsDouble(i, "exactlynkinds")),
            int(data.elemAsDouble(i, "any7")));
        
        // add the building as many times as there are replications of that card in the deck;
        int qty = int(data.elemAsDouble(i, "qty"));
        for(int q = 0; q < qty; ++q)
            buildings.push(b);        
    }

    // shuffle the deck;
    buildings.shuffle();

    // draw two decks of 7 cards from that;
    for(int i = 0; i < 7; ++i){
        buildings1.push(buildings.draw());
        buildings2.push(buildings.draw());
    }
    
    return;
}


static SA::Symbol getSymbol(std::string s){
    if(s == "pottery")   return SA::Symbol::POTTERY;
    if(s == "writing")   return SA::Symbol::WRITING;
    if(s == "time")      return SA::Symbol::TIME;
    if(s == "transport") return SA::Symbol::TRANSPORT;
    if(s == "healing")   return SA::Symbol::HEALING;
    if(s == "weaving")   return SA::Symbol::WEAVING;
    if(s == "art")       return SA::Symbol::ART;
    if(s == "music")     return SA::Symbol::MUSIC;

    if(s == "hut")       return SA::Symbol::HUT;
    if(s == "tool")      return SA::Symbol::TOOL;
    if(s == "farmer")    return SA::Symbol::FARMER;
    if(s == "shaman")    return SA::Symbol::SHAMAN;  
}

static SA::Header getHeader(std::string s){
    if(s == "roll")          return SA::Header::ROLL;
    if(s == "food")          return SA::Header::FOOD;
    if(s == "stone")         return SA::Header::STONE;
    if(s == "gold")          return SA::Header::GOLD;
    if(s == "brick")         return SA::Header::BRICK;
    if(s == "roll_gold")     return SA::Header::ROLL_GOLD;
    if(s == "roll_wood")     return SA::Header::ROLL_WOOD;
    if(s == "roll_stone")    return SA::Header::ROLL_STONE;
    if(s == "points")        return SA::Header::POINTS;
    if(s == "plus_one_tool") return SA::Header::PLUS_ONE_TOOL;
    if(s == "plus_one_agg")  return SA::Header::PLUS_ONE_AGG;
    if(s == "card")          return SA::Header::CARD;
    if(s == "one_use_tool")  return SA::Header::ONE_USE_TOOL;
    if(s == "ressource")     return SA::Header::RESSOURCE;    
}


void SA::StoneAge::populateCivCards(){
    CSV data("data/civilization.csv");
    for(int i = 0; i < data.getNrow(); ++i){
        SA::CivilizationCard c(
            getHeader(data(i,"header")),
            int(data.elemAsDouble(i,"header_qty")),
            getSymbol(data(i,"symbol")),
            int(data.elemAsDouble(i,"symbol_qty")));
        civCards.push(c);
    }
    civCards.shuffle();
    for(int i = 0; i < 0; ++i){
        civCardsOnDisplay.push_back(civCards.draw());
    }
    return;
}

void SA::StoneAge::populateActions(){
    // we could put that in a CSV but that's simpler
    actions.push_back(Action(SA::ActionName::WOOD, 1, 7));
    actions.push_back(Action(SA::ActionName::BRICK, 1, 7));
    actions.push_back(Action(SA::ActionName::STONE, 1, 7));
    actions.push_back(Action(SA::ActionName::GOLD, 1, 7));
    actions.push_back(Action(SA::ActionName::CARD1, 1, 1));
    actions.push_back(Action(SA::ActionName::CARD2, 1, 1));
    actions.push_back(Action(SA::ActionName::CARD3, 1, 1));
    actions.push_back(Action(SA::ActionName::CARD4, 1, 1));
    actions.push_back(Action(SA::ActionName::BLDG1, 1, 1));
    actions.push_back(Action(SA::ActionName::BLDG2, 1, 1));
    actions.push_back(Action(SA::ActionName::FARM, 1, 1));
    actions.push_back(Action(SA::ActionName::MATE, 2, 2));
    actions.push_back(Action(SA::ActionName::TOOL, 1, 1));

    return;
}


void SA::StoneAge::playIt(){
    // infinite loop we check if an ending condition has been met
    // at the end of each round;

    //while(! gameOver()){
    
        initRound();

        /*
        while( ! roundIsFinished()){
            playOneAction();
        }
        finishRound();
        */
    //}
}


void SA::StoneAge::initRound(){
    // empty the board and mark all actions as available;
    for(auto& a: actions){
        a.avail = true;
        a.nmen = 0;
        a.p = SA::PlayerID::NONE;
    }
    
    // no need to draw buildings because we just look at the top of the deck

    // need to advance the civilization cards if some were taken
    while(civCardsOnDisplay.size() < nCivCardsOnDisplay){
        // we have already verified that there are enough cards to fill it up
        civCardsOnDisplay.push_back(civCards.draw());
    }

}

void SA::StoneAge::playOneAction(){
    // current player

    auto& thisPlayer = players.at(whoseTurn);

    // determine what action to take ... 

    // in order to generate more interesting samples we'll put the last humans on food
    // if we don't have enough food 
    // however we'll put these humans on food at the end
    // we save 1 human per 2 food needed, rounded up;
    
    int foodNeeded = std::max(thisPlayer.nPeople - thisPlayer.foods, 0);
    // + .75 because I don't want to round just below the unit
    int humansSavedForFood = int(foodNeeded / 2.0 + 0.75); 

    // according to my logic, since we always save these humans, we will stop spending
    // humans BEFORE we have fewer than the number that I want to save
    // so this is guaranteed to be positive
    int humansAvailable = thisPlayer.nPeopleLeft - humansSavedForFood;

    // get list of ids to chose from (1) pick one action at random (2) pick how many humans to asign (3)
    auto actionsToChooseFrom = getAvailableActions(humansAvailable);
    int indexChosen = SA::randInt(0, actionsToChooseFrom.size());
    auto& chosenAlt = actions.at(actionsToChooseFrom.at(indexChosen));

    // choose how many humans to asign. for now we chose uniform random;
    int nToAsign = SA::randInt(chosenAlt.min, chosenAlt.max + 1);
    chosenAlt.avail = false;
    chosenAlt.nmen = nToAsign;
    chosenAlt.p = whoseTurn;

    // if no action was taken, and there were humans saved to get food, we place them now
    if(humansAvailable == 0 && humansSavedForFood > 0){
        cout << "player " << whoseTurn << " places " << humansSavedForFood << " on food\n";
        foodActions.push_back(FoodAction(humansSavedForFood, whoseTurn));
        thisPlayer.nPeopleLeft -= humansSavedForFood;
        
    }

    // if the other player still has humans to play, he'll start the next round
    if(players.at(switchPlayer(whoseTurn)).nPeopleLeft > 0){
        whoseTurn = switchPlayer(whoseTurn);
    }

}


void SA::StoneAge::finishRound(){
    whoStartsRound = switchPlayer(whoStartsRound);
}



bool SA::StoneAge::gameOver(){
    // need sufficiently many civilization cards;
    int have = civCards.getSize();
    int need = nCivCardsOnDisplay - civCardsOnDisplay.size();
    bool NotEnoughCivCardsToFillBoard = have < need;
    // all building stacks need to have cards;
    bool OneBuildingStackEmpty = buildings1.getSize() == 0 || buildings2.getSize() == 0;

    return NotEnoughCivCardsToFillBoard || OneBuildingStackEmpty;
}


bool SA::StoneAge::roundIsFinished(){
    // if at least one player has humans left the round is not finished
    for(auto& p: players){
        if(p.second.nPeopleLeft > 0){
            return false;
        }
    }
    return true;
}


SA::PlayerID SA::StoneAge::switchPlayer(SA::PlayerID p){
    // 1 - 0 = 1
    // 1 - 1 = 0
    if(p == SA::PlayerID::ONE)
        return SA::PlayerID::TWO;
    if(p == SA::PlayerID::TWO)
        return SA::PlayerID::ONE;
    
    return SA::PlayerID::NONE;
}


std::vector<int> SA::StoneAge::getAvailableActions(int nHumans){
    std::vector<int> avail;

    // see if 2 of tool, agg, and mate are already chosen, in that case the other ones
    // are not available even if they have no humans on them;


    for(int i = 0; i < actions.size(); ++i){
        bool canTake = (int(toolChosen) + int(aggChosen) + int(mateChosen)) >= 2;
        const Action& a = actions.at(i);
        
        // if the action is _not_ FARM, MATE or TOOL 
        // canTake is true because there is no constraint on it;
        if(a.name != SA::ActionName::FARM &&
                a.name != SA::ActionName::MATE &&
                a.name != SA::ActionName::TOOL){
            canTake = true;
        }

        if(a.avail && canTake && nHumans > a.min){
            avail.push_back(i);
        }
    }
    return avail;
}
