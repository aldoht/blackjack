#include <iostream>
#include <cstdlib>
#include <ctime>
#include <list>
#include <map>
using namespace std;
const int BLACKJACK = 21;
class Card {
	public:
		string suit;
		string value;
		Card(){
			static const bool initialized = []() {
				srand(time(nullptr));
				return true;
			}();
			this->suit = suits[rand() % 4];
			this->value = values[rand() % 13];
		}
		Card& operator=(const Card& other) {
			if (this != &other) {
				suit = other.suit;
				value = other.value;
			}
			return *this;
		}
	private:
		const string suits[4] = {"\u2664", "\u2667", "\u2661", "\u2662"};
		const string values[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
};
class Person {
	private:
		list<Card> hand;
		Card firstCard;
		Card secondCard;
	public:
		Person() : firstCard(), secondCard() {
			hand.push_front(firstCard);
			hand.push_front(secondCard);
		}
		Person& operator=(const Person& other) {
			if (this != &other) {
				firstCard = other.firstCard;
				secondCard = other.secondCard;
				hand = other.hand;
			}
			return *this;
		}
		void showHand() {
			for (list<Card>::iterator it = hand.begin(); it != hand.end(); ++it) {
				const auto& card = *it;
				cout << card.value << card.suit;
				if (it != prev(this->hand.end())) {
					cout << ", ";
				} else {
					cout << endl;
				}
			}
		}
		int count() {
			int sum = 0;
			bool hasAce = false;
			map<string, int> specialValuesMap;
			specialValuesMap["A"] = 11;
			specialValuesMap["J"] = 10;
			specialValuesMap["Q"] = 10;
			specialValuesMap["K"] = 10;
			for (list<Card>::iterator it = hand.begin(); it != hand.end(); ++it) {
				const auto& card = *it; // Just for legibility
				if (!card.value.compare("A"))
					hasAce = true;
				if (!card.value.compare("A") || !card.value.compare("J") || !card.value.compare("Q") || !card.value.compare("K"))
					sum += specialValuesMap[card.value];
				else
					sum += stoi(card.value, nullptr, 10);
			}
			if (hasAce && sum > BLACKJACK)
				return sum-10;
			return sum;
		}
		void hit() {
			Card newCard = Card();
			hand.push_back(newCard);
		}
		bool checkPairs() {
			return (!firstCard.value.compare(secondCard.value));
		}
		int numberOfCards() {
			return hand.size();
		}
};
class Player: public Person {
	public:
		Player(){}
		bool canHit() {
			if (!didDoubleDown && this->count() < BLACKJACK)
				return true;
			return false;
		}
		bool canDoubleDown() {
			if (count() == 2)
				return true;
			return false;
		}
		bool canSplit = checkPairs();
		bool didDoubleDown = false;
		Player& operator=(const Player& other) {
			if (this != &other) {
				static_cast<Person&>(*this) = static_cast<const Person&>(other);
			}
			return *this;
		}
};

// Functions mainMenu displayGame
int gameMenu() {
	int numPlayers;
	do {
		cout << "Select the number of players (max. is 8): ";
		cin >> numPlayers;
		if (cin.fail() || (numPlayers > 8 || numPlayers < 1)) {
			cout << "That is not a valid input!" << endl;
			cin.clear();
			cin.ignore();
		}
	} while (cin.fail() || (numPlayers > 8 || numPlayers < 1));
	return numPlayers;
}
int mainMenu() {
	int option;
	cout << "Welcome to terminal blackjack!" << endl;
	cout << "Choose one of the following options:" << endl;
	cout << "\t 1. Play" << endl;
	cout << "\t 2. How to play" << endl;
	cout << "\t 3. Quit" << endl;
	do {
		cout << "\t Your option is: ";
		cin >> option;
		if (cin.fail() || (option > 3 || option < 1)) {
			cout << "\t That is not a valid option!" << endl;
			cin.clear();
			cin.ignore();
		}
	} while (cin.fail() || (option > 3 || option < 1));
	return option;
}
int main() {
	switch(mainMenu()) {
		case 1:
		{
			int players = gameMenu();
			Player allPlayers[players];

			// Next Step
			break;
		}
		case 2:
			break;
		case 3:
			break;
	}

	return 1;
}
