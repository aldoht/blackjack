#include <iostream>
#include <random>
#include <list>
#include <map>
const int BLACKJACK = 21;

class Card {
	private:
		std::string suit;
		std::string value;
		const std::string suits[4] = {"♤", "♧", "♡", "♢"};
		const std::string values[14] = {"A", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
	public:
		Card() {
			std::random_device rd; // Creates random_device object based on a implementation-defined token by the compiler and platform
			std::mt19937 generator(rd()); // Returns a random number when 'generator' is called using random_device 'rd' constructor as seed
			std::uniform_int_distribution<int> dist(1, 1000); // Uses the probabilistic mass function to ensure ints in [1, 1000] have an equal likelihood of being produced
			this->value = values[dist(generator) % 14]; // Reduces the random int in [1, 1000] to be in [0, 13]
			this->suit = suits[dist(generator) % 4]; // Reduces the random int in [1, 1000] to be in [0, 3]
		}
		std::string getValue() {
			return this->value;
		}
		std::string getSuit() {
			return this->suit;
		}
		bool isEqual(Card otherCard) {
			return (suit == otherCard.suit && value == otherCard.value);
		}
		int getNumericalValue() {
			std::map<std::string, int> specialValuesMap;
			specialValuesMap["A"] = 11;
			specialValuesMap["J"] = 10;
			specialValuesMap["Q"] = 10;
			specialValuesMap["K"] = 10;
			if (specialValuesMap.find(value) != specialValuesMap.end()) // Checks if the card value is found in the special values map
				return specialValuesMap[value];
			return std::stoi(value, nullptr, 10);
		}
};

class Hand {
	private:
		std::list<Card> hand;
		Card firstCard;
		Card secondCard;
		bool didDoubleDown = false;
	public:
		Hand() : firstCard(), secondCard() {
			this->hand.push_back(firstCard);
			this->hand.push_back(secondCard);
		}
		void showHand() {
			for (auto card : this->hand) {
				std::cout << card.getValue() << card.getSuit();
				if (!card.isEqual(this->hand.back()))
					std::cout << ", ";
				else {
					if (this->didDoubleDown)
						std::cout << " ☆" << std::endl;
					else
						std::cout << std::endl;
				}
			}
		}
		bool hasAce() {
			for (auto card : this->hand)
				if (card.getValue() == "A")
					return true;
			return false;
		}
		int count() {
			int sum = 0;
			for (auto card : this->hand)
				sum += card.getNumericalValue();
			if (this->hasAce() && sum > BLACKJACK) // Handles "soft" and "hard" hand ambiguity cases
				return sum-10;
			return sum;
		}
		void addCard() {
			this->hand.push_back(Card());
		}
		std::list<Card>& getHand() { // Returns a reference to the hand
			return this->hand;
		}
		bool hasPair() {
			return (this->firstCard.getValue() == this->secondCard.getValue());
		}
		bool canSplit() {
			return (this->hasPair() && this->hand.size() == 2);
		}
		void setDoubleDownStatus(bool status) {
			this->didDoubleDown = status;
		}
		bool getDoubleDownStatus() {
			return this->didDoubleDown;
		}
};

class Dealer {
	protected:
		std::list<Hand> userHands;
	public:
		Dealer() {
			this->userHands.push_back(Hand());
		}
		bool canHit() {
			return this->userHands.front().count() < 17;
		}
		void hit() {
			if (!this->canHit()) // Base case will be reached when dealer gets more than 16 in its hand
				return;
			this->userHands.front().addCard();
			this->hit();
		}
		void showHands() {
			for (auto hand : this->userHands)
				hand.showHand();
		}
};

class Player : public Dealer {
	public:
		bool canHit(Hand& actualHand) {
			return (actualHand.count() < BLACKJACK && !actualHand.getDoubleDownStatus());
		}
		std::list<Hand>& getHands() { // Returns a reference to all hands
			return this->userHands;
		}
		void hit(Hand& actualHand) { // Modifies the reference passed so it doesn't create a copy of it
			if (!this->canHit(actualHand)) // Remove later
				return;
			actualHand.addCard();
		}
		void addHand(Hand hand) {
			this->userHands.push_back(hand);
		}
		Hand split(Hand& actualHand) { // Requires a reference to the actual hand so it can modify it
			Hand newHand;
			actualHand.getHand().push_back(newHand.getHand().front());
			newHand.getHand().pop_front();
			newHand.getHand().push_front(actualHand.getHand().front());
			actualHand.getHand().pop_front();
			return newHand;
		}
		void doubleDown(Hand& actualHand) {
			actualHand.addCard();
			actualHand.setDoubleDownStatus(true);
		}
};


// Functions displayGame agregarLibreria meter a una clase Views?
int gameMenu() {
	int numPlayers;
	do {
		std::cout << "Select the number of players (max. is 8): ";
		std::cin >> numPlayers;
		if (std::cin.fail() || (numPlayers > 8 || numPlayers < 1)) {
			std::cout << "That is not a valid input!" << std::endl;
			std::cin.clear();
			std::cin.ignore();
		}
	} while (std::cin.fail() || (numPlayers > 8 || numPlayers < 1));
	return numPlayers;
}
int mainMenu() {
	int option;
	std::cout << "Welcome to terminal blackjack!" << std::endl;
	std::cout << "Choose one of the following options:" << std::endl;
	std::cout << "\t 1. Play" << std::endl;
	std::cout << "\t 2. How to play" << std::endl;
	std::cout << "\t 3. Quit" << std::endl;
	do {
		std::cout << "\t Your option is: ";
		std::cin >> option;
		if (std::cin.fail() || (option > 3 || option < 1)) {
			std::cout << "\t That is not a valid option!" << std::endl;
			std::cin.clear();
			std::cin.ignore();
		}
	} while (std::cin.fail() || (option > 3 || option < 1));
	return option;
}
int main() {
	switch(mainMenu()) {
		case 1:
		{
			int players = gameMenu();
			// Next Step
			break;
		}
		case 2:
			break;
		case 3:
			break;
	}
	return 0;
}
