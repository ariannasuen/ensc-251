#include <iostream>
#include <string>
#include "card.hpp"
#include "deck.hpp"
#include "common.hpp"

// Helper function to print a single card
void printCard(const Card& c) {
    std::cout << rankToString(c.getRank()) << " of " << suitToString(c.getSuit());
}

// Helper function to print the entire deck 
// Note: This empties the deck by dealing all cards, so createDeck() must be called afterwards to reset.
void printEntireDeck(Deck& d) {
    int count = 1;
    while (!d.isEmpty()) {
        std::cout << count << ": ";
        printCard(d.dealCard());
        std::cout << "\n";
        count++;
    }
}

// Helper function to simulate drawing multiple cards (handles the 0 parameter test)
void drawMultipleCards(Deck& d, int numToDraw) {
    if (numToDraw == 0) {
        std::cout << "Attempted to draw 0 cards.\n";
        return;
    }
    for (int i = 0; i < numToDraw; ++i) {
        if (!d.isEmpty()) {
            d.dealCard();
        }
    }
}

int main() {
    std::cout << "========== CARD CLASS TESTS ==========\n";
    
    // 1. Try default constructor
    Card defaultCard;
    std::cout << "Testing Default Constructor (Should be Two of No Suit):\n";
    printCard(defaultCard);
    std::cout << " | Integer Value: " << defaultCard.rankValue() << "\n\n";

    // 2. Try making a custom card & test getters
    Card customCard(Rank::Ace, Suit::Spades);
    std::cout << "Testing Custom Constructor & Getters (Should be Ace of Spades):\n";
    std::cout << "Rank from getter: " << rankToString(customCard.getRank()) << "\n";
    std::cout << "Suit from getter: " << suitToString(customCard.getSuit()) << "\n";
    std::cout << "Integer Value: " << customCard.rankValue() << "\n\n";


    std::cout << "========== DECK SHUFFLE TESTS ==========\n";
    Deck testDeck;

    // 3. Print deck before shuffling
    std::cout << "--- Deck BEFORE Shuffling ---\n";
    printEntireDeck(testDeck);
    
    // 4. Print deck after shuffling
    testDeck.createDeck(); // Reset the deck back to 52 cards
    testDeck.shuffle();
    std::cout << "\n--- Deck AFTER Shuffling ---\n";
    printEntireDeck(testDeck);


    std::cout << "\n========== DECK DRAW TESTS ==========\n";
    
    // 5. Check number of cards changes when drawing one card
    testDeck.createDeck(); // Reset to full deck
    std::cout << "Initial Deck Size: " << testDeck.size() << "\n";
    
    testDeck.dealCard();
    std::cout << "Deck Size after drawing 1 card: " << testDeck.size() << "\n";

    // 6. Check number of cards changes when drawing multiple cards
    drawMultipleCards(testDeck, 5);
    std::cout << "Deck Size after drawing 5 more cards: " << testDeck.size() << "\n";

    // 7. Check that zero is the output when the deck is empty
    std::cout << "\nEmptying the rest of the deck...\n";
    while (!testDeck.isEmpty()) {
        testDeck.dealCard();
    }
    std::cout << "Deck size when completely empty: " << testDeck.size() << "\n";

    // 8. Check that the deck is full if you put zero as a parameter
    std::cout << "\n--- Testing Zero Parameter Draw ---\n";
    testDeck.createDeck(); // Reset to full deck
    std::cout << "Deck size before 0-card draw: " << testDeck.size() << "\n";
    drawMultipleCards(testDeck, 0);
    std::cout << "Deck size after 0-card draw: " << testDeck.size() << "\n";
    
    if (testDeck.size() == 52) {
        std::cout << "Success: The deck is still full!\n";
    }

    return 0;
}