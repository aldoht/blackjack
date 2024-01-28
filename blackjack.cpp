#include <iostream>
#include <random>
#include <list>
#include <map>
#include <limits>
const int BLACKJACK = 21;
class Card {
	private:
		std::string suit;
		std::string value;
		const std::string suits[4] = {"♤", "♧", "♡", "♢"};
		const std::string values[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
	public:
		Card() {
			std::random_device rd; // Creates random_device object based on a implementation-defined token by the compiler and platform
			std::mt19937 generator(rd()); // Returns a random number when 'generator' is called using random_device 'rd' constructor as seed
			std::uniform_int_distribution<int> dist(1, 1000); // Uses the probabilistic mass function to ensure ints in [1, 1000] have an equal likelihood of being produced
			this->value = values[dist(generator) % 13]; // Reduces the random int in [1, 1000] to be in [0, 12]
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
		bool winStatus = false;
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
		bool getWinStatus() {
			return this->winStatus;
		}
		void setWinStatus(bool status) {
			this->winStatus = status;
		}
};

class Dealer {
	protected:
		std::list<Hand> userHands;
		bool busted = false;
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
		void firstShowHand() {
			Hand hand = this->userHands.front();
			std::cout << hand.getHand().front().getValue() << hand.getHand().front().getSuit() << ", ??" << std::endl;
		}
		void setBustedStatus(bool status) {
			this->busted = status;
		}
		bool getBustedStatus() {
			return this->busted;
		}
		Hand getHand() {
			return this->userHands.front();
		}
};

class Player : public Dealer {
	public:
		bool canHit(Hand& actualHand) {
			return (actualHand.count() < BLACKJACK && !actualHand.getDoubleDownStatus());
		}
		bool canDouble(Hand& actualHand) {
			return (actualHand.getHand().size() == 2);
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


// Functions
void clearTerminal() {
	std::cout << "\033[2J\033[1;1H";
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
void playTurn(Hand& hand, Player& player, int count) {
	bool endTurn;
	do {
		int option;
		std::cout << "What do you want to do with the following hand: ";
		hand.showHand();
		do {
			std::cout << "\t1. Hit" << std::endl <<
					"\t2. Stand" << std::endl <<
					"\t3. Double Down" << std::endl <<
					"\t4. Split" << std::endl <<
					"\tYour option is: ";
			std::cin >> option;
			if (std::cin.fail() || (option > 4 || option < 1)) {
				std::cout << "That is not a valid input!" << std::endl;
				std::cin.clear();
				std::cin.ignore();
			}
		} while (std::cin.fail() || (option > 4 || option < 1));
		switch (option) {
			case 1:
				if (!player.canHit(hand))
					break;
				player.hit(hand);
				break;
			case 2:
				std::cout << "Player " << count+1 << " has standed on this hand." << std::endl;
				endTurn = true;
				break;
			case 3:
				if (!player.canDouble(hand)) {
					std::cout << "This hand can't double down!" << std::endl;
					break;
				}
				player.doubleDown(hand);
				std::cout << "Player " << count+1 << " has double downed this hand." << std::endl;
				endTurn = true;
				break;
			case 4:
				if (!hand.canSplit()) {
					std::cout << "This hand can't split!" << std::endl;
					break;
				}
				player.getHands().push_back(player.split(hand));
				std::cout << "Player " << count+1 << " has split this hand." << std::endl;
				break;
		}
		std::cout << "Hand: ";
		hand.showHand();
		if (hand.count() > BLACKJACK) {
			std::cout << "Player " << count+1 << " has busted this hand!" << std::endl;
			endTurn = true;
		}
	} while (endTurn != true);
}
char displayGame(Dealer dealer, Player players[], int size) {
	std::cout << "Dealer's hand: ";
	dealer.firstShowHand();
	std::cout << "- - - - - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
	for (int i = 0; i < size; i++) { // Iterates through all players
		for (auto hand : players[i].getHands()) { // Iterates through all the player's hands
			std::cout << "Player " << i+1 << ": ";
			hand.showHand();
		}
	}
	std::cout << "Enter a character to continue...";
	std::cin.get();
	clearTerminal();
	dealer.hit();
	dealer.setBustedStatus(dealer.getHand().count() > BLACKJACK);
	for (int i = 0; i < size; i++) { // Iterates through all players
		std::cout << "Dealer's hand: ";
		dealer.firstShowHand();
		std::cout << "- - - - - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
		std::cout << "It's player " << i+1 << "'s turn:" << std::endl;
		for (auto& hand : players[i].getHands()) { // Iterates through all the player's hands
			bool endTurn = false;
			playTurn(hand, players[i], i);
			if (dealer.getBustedStatus())
				hand.setWinStatus(true);
			else {
				if ((hand.count() == BLACKJACK && dealer.getHand().count() != BLACKJACK) || (hand.count() > dealer.getHand().count() && hand.count() <= BLACKJACK))
					hand.setWinStatus(true);
				// Player 21 against any dealer's hand except for a blackjack will result in a player win
				// Player hand higher than dealer's hand without busting will result in a player win
				// Any other case is not taken into account because winStatus is already false and any other case left involves the player losing its hand
			}
			std::cout << "Press any key to continue... ";
			std::cin.get();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			clearTerminal();
		}
	}
	std::cout << "Dealer: ";
	dealer.getHand().showHand();
	std::cout << "- - - - - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
	for (int i = 0; i < size; i++) { // Iterates through all players
		for (auto hand : players[i].getHands()) { // Iterates through all the player's hands
			std::cout << "Player " << i+1 << ": ";
			hand.showHand();
			if (hand.getWinStatus())
				std::cout << "-> You have won this hand!" << std::endl;
			else
				std::cout << "-> You have lost this hand!" << std::endl;
		}
	}
	std::cout << "- - - - - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
	char exitChar;
	do {
		std::cout << "Do you want to keep playing? (y/n): ";
		std::cin >> exitChar;
		if (std::cin.fail() || (toupper(exitChar) != 'Y' && toupper(exitChar) != 'N')) {
			std::cout << "That is not a valid input!" << std::endl;
			std::cin.clear();
			std::cin.ignore();
		}
	} while (std::cin.fail() || (toupper(exitChar) != 'Y' && toupper(exitChar) != 'N'));
	return toupper(exitChar);
}
int main() {
	bool keepPlaying = true;
	do {
		clearTerminal();
		switch(mainMenu()) {
			case 1:
			{
				int players = gameMenu();
				Player allPlayers[players];
				do {
					for (int i = 0; i < players; i++) // Creates new players for each iteration
						allPlayers[i] = Player();
					clearTerminal();
				} while (displayGame(Dealer(), allPlayers, players) == 'Y');
				break;
			}
			case 2:
				std::cout << "- - - - - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
				std::cout << "You get dealt a hand with 2 cards." << std::endl;
				std::cout << "Dealer has a hand with only one card showing, the second is shown just after every player has played their turn." << std::endl;
				std::cout << "You have to get as close as you can to 21, if you exceed 21 or the dealer has a higher count than you, you lose." << std::endl;
				std::cout << "Aces can count as 1 or 11 depending on if your count exceeds 21 while counting it as 11 or not, faces count as 10 and every other card counts as its numerical value." << std::endl;
				std::cout << "You are given 4 options for every hand you have:" << std::endl;
				std::cout << "\t1. Hit: You get another random card added to your current hand." << std::endl;
				std::cout << "\t2. Stand: No changes will be made to your current hand." << std::endl;
				std::cout << "\t3. Double Down: Your 'bet' is doubled, and you get only one extra card." << std::endl;
				std::cout << "\t3. Split: If you have a pair as your initial cards, you can split your actual hand into two new ones using one card of the pair as first card of each new hand, then you can play your hands again independently." << std::endl;
				std::cout << "Dealer has to hit until he reaches 17 or higher." << std::endl;
				std::cout << "Drawing with the dealer will count as a player loss." << std::endl;
				std::cout << "Have fun getting those BLACKJACKS (21)!" << std::endl;
				std::cout << "P.S: You can't card count in this version but I am thinking of doing a version in the future to do so!" << std::endl;
				std::cout << "Press any key to continue... ";
				std::cin.get();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignores all input until a newline is reached
				clearTerminal();
				break;
			case 3:
				std::cout << "- - - - - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
				std::cout << "Thank you for playing!" << std::endl << "But remember... the house always wins." << std::endl;
				keepPlaying = false;
				break;
		}
	} while (keepPlaying);
	return 0;
}
