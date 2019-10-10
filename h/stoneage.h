#ifndef STONEAGE_HPP
#define STONEAGE_HPP

//#include "game.h"
#include <vector>
#include <deque>
#include <map>
#include "deck.hpp"


namespace SA {

typedef std::pair<int, int> TokenPair;

// the symbols on civilization cards;
enum class Symbol {
    // regular symbols;
    POTTERY,
    WRITING,
    TIME,
    TRANSPORT,
    HEALING,
    WEAVING,
    ART,
    MUSIC,

    // bonus symbols for the end;
    HUT,
    TOOL,
    FARMER,
    SHAMAN
};

enum class Header {
    ROLL, // roll 1 die per person then get split the ressources
    FOOD,
    STONE,
    GOLD,
    BRICK, 
    ROLL_GOLD,
    ROLL_WOOD,
    ROLL_STONE,
    POINTS,
    PLUS_ONE_TOOL,
    PLUS_ONE_AGG,
    CARD,
    ONE_USE_TOOL,
    RESSOURCE
};

enum class PlayerID{
    ONE,
    TWO,
    NONE
};

enum class ActionName{
    WOOD,
    BRICK,
    STONE,
    GOLD,
    CARD1,
    CARD2,
    CARD3,
    CARD4,
    BLDG1,
    BLDG2,
    FARM,
    MATE,
    TOOL
};

struct CivilizationCard {
    CivilizationCard(Header h, int headerQty, Symbol s, int symbolQty):
        h(h),
        headerQty(headerQty),
        s(s),
        symbolQty(symbolQty){}

    Header h;
    int headerQty;
    Symbol s;
    int symbolQty;
};

struct Building{
    Building(int wood, int brick, int stone, int gold, int nressources, int nkinds, int any):
        wood(wood),
        brick(brick),
        stone(stone),
        gold(gold),
        nressources(nressources),
        nkinds(nkinds),
        any(any) {}

    int wood;
    int brick;
    int stone;
    int gold;
    int nressources;
    int nkinds;
    int any;
};

struct Player {
    Player(): 
        nPoints(0),
        nAggs(0),
        nTools(0),
        nPeople(0),
        woods(0),
        bricks(0),
        stones(0),
        golds(0),
        foods(0) {}

    int nPoints;
    int nAggs;
    int nTools;
    int nPeople;
    int nPeopleLeft;

    Deck<CivilizationCard> cardsWithUsableBonus;
    Deck<CivilizationCard> cardsForPoints;
    Deck<Building> buildings;

    int woods;
    int bricks;
    int stones;
    int golds;
    int foods;
};

struct FoodAction {
    int nmen;
    PlayerID p;
    FoodAction(int nmen, PlayerID p): nmen(nmen), p(p){}
};

struct Action {
    bool avail;
    int nmen;
    PlayerID p;

    const ActionName name;
    const int min;
    const int max;

    Action(ActionName name, int min, int max): 
        avail(false), 
        nmen(0), 
        p(PlayerID::NONE), 
        name(name), 
        min(min), 
        max(max) 
        {}
};


class StoneAge{
    public:
        StoneAge();

    private:
        void populateBuildings();
        void populateCivCards();
        void populateActions();

        void playIt();
        void initRound();
        void playOneAction();
        void finishRound();

        bool gameOver();
        bool roundIsFinished();

        SA::PlayerID switchPlayer(SA::PlayerID p);
        std::vector<int> getAvailableActions(int nHumans);
        
        Deck<CivilizationCard> civCards;
        std::deque<CivilizationCard> civCardsOnDisplay;

        Deck<Building> buildings1;
        Deck<Building> buildings2;
        std::vector<Action> actions;
        std::vector<FoodAction> foodActions;
        
        //std::vector<Player> players;
        std::map<SA::PlayerID, Player> players;

        int wood;
        int brick;
        int stone;
        int gold;
        int food;

        const int nCivCardsOnDisplay = 4;
        //int whoseTurn;
        //int whoStartsRound;
        PlayerID whoseTurn;
        PlayerID whoStartsRound;
        bool toolChosen;
        bool aggChosen;
        bool mateChosen;

};

} // namespace SA
#endif