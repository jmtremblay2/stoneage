#ifndef DECK_HPP
#define DECK_HPP

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>

template<class T>
class Deck {
    public:
        Deck() {}
        Deck(const std::vector<T>& otherdeck) : deck(otherdeck) {};
        Deck(const Deck<T>& otherdeck): 
            deck(otherdeck.deck),
            discardPile(otherdeck.discardPile) {};


        T draw(){
            // put the top card of the deck on the top of the discard pile
            // and also return it to the user
            if(deck.size() == 0)
                throw std::invalid_argument("deck is empty");
            discardPile.push_back(deck.back());
            deck.pop_back();
            return discardPile.back();
        };

        void push(const T& card){
            // put the top card on the top of the dircard pile
            deck.push_back(card);
            return;
        };

        const T& operator[](int i){

            return deck.at(i);
        };

        int getSize() const {
            return deck.size();
        };

        void shuffle() {
            // combine the deck and the discard pile
            deck.insert(
                deck.end(),
                discardPile.begin(),
                discardPile.end());
            // shuffle the deck
            std::random_shuffle (deck.begin(), deck.end() );
            // empty discard pile
            discardPile.clear();
            return;
        };

        // for sanity checks... need to define the << operator for fancy types
        void printIt() const {
            std::cout << "deck:  ";
            for(auto i: deck)
                std::cout << i << ",  ";
            std::cout << std::endl;

            std::cout << "discard pile:  ";
            for(auto i: discardPile)
                std::cout << i << ",  ";
            std::cout << std::endl;
        }
        
    protected:
        std::vector<T> deck;
        std::vector<T> discardPile;
};

#endif