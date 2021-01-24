#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

class CardDeck
{
private:
  vector <int> deck;
  enum {deck_size = 52, common_denominator = 100,
        num_of_suits = 4, num_of_faces = 13};
public:
  void initializeDeck();
  void shuffleDeck(); //consider making public
  int topCard();
  CardDeck();
};

CardDeck :: CardDeck(){
  cout << "doing it again!\n\n";
  initializeDeck();
  shuffleDeck();
}

void CardDeck :: initializeDeck(){
  for (int suit = 0; suit < num_of_suits; suit++){
    for (int face_value = 0; face_value < num_of_faces; face_value++){
        deck.push_back((suit*common_denominator) + face_value);
    }
  }
}

void CardDeck :: shuffleDeck(){
  int j;
  srand((unsigned) time(NULL));
  //shuffling deck; exchange index i with j
  for(int i = 0; i < deck_size; i++){
    j = rand() % deck_size;
    swap(deck[i],deck[j]);
  }
}

int CardDeck :: topCard(){
  int top_card = deck.back();
  deck.pop_back();
  return top_card;
}

class Dealer
{
private:
    CardDeck dck;
    void getResults();
protected:
    string id;
    int card_total;
    vector <int> hand;
public:
    Dealer();
    bool is_over;
    bool isBust();
    int removeTopCard();
    void setHand(int card);
    int getCardTotal();
    int getFirstCard();
    string getID();
};

class Player : public Dealer
{
private:
    int offset;
public:
    Player();
    bool getMove(Dealer dlr);
};

Dealer :: Dealer(){
  id = "House";
  card_total = 0;
  is_over = FALSE;
}

bool Dealer :: isBust(){
  return card_total >= 17;
}

int Dealer :: getCardTotal(){
  return card_total;
}

int Dealer :: removeTopCard(){
  return dck.topCard();
}

void Dealer :: setHand(int card){
  int value [13]   = {2,3,4,5,6,7,8,9,10,10,10,10,11};
  string suit [4]  = {"clubs", "diamonds", "hearts", "spades"};
  string face [13] = {"two","three","four","five","six","seven",
                      "eight","nine","ten","jack","king","queen",
                      "ace"};

  hand.push_back(card % 100);
  card_total = value[card % 100] + card_total;
  cout << id << " is dealt a { ";
  cout << face[card % 100] << " , ";
  cout << suit[card / 100] << " }\n";
  getResults();
}

int Dealer :: getFirstCard(){
  return hand.at(0);
}

void Dealer :: getResults(){
  if (card_total == 21){
    cout << id << " has blackjack" << endl;
    is_over = TRUE;
  } else if (card_total > 21){
    cout << id << " has gone bust" << endl;
    is_over = TRUE;
  } else {
    cout << id << "has a card total of " << card_total << endl;
    is_over = FALSE;
  }
}

string Dealer :: getID(){
  return id;
}

Player :: Player(){
  static int num_of_players = 0;
  card_total = 0;
  is_over = FALSE;
  num_of_players++;
  id = to_string(num_of_players);
}

bool Player :: getMove(Dealer dlr){
  bool auto_move[7][10] = {
                            {1,1,1,1,1,1,1,1,1,1},
                            {1,1,0,0,0,1,1,1,1,1},
                            {0,0,0,0,0,1,1,1,1,1},
                            {0,0,0,0,0,1,1,1,1,1},
                            {0,0,0,0,0,1,1,1,1,1},
                            {0,0,0,0,0,1,1,1,1,1},
                            {0,0,0,0,0,0,0,0,0,0}
                          };
  if (getCardTotal() < 11){
    offset = 0;
  } else if (getCardTotal() > 17){
    offset = 6;
  } else {
    offset = getCardTotal() - 11;
  }
  return auto_move[offset][dlr.getFirstCard() - 2];
}

class BlackJack
{
  private:
    vector <Player> players;
    enum {deal_limit = 2, game_delay = 1000};
    void playerTurn();
    void randomGameDelay();
    void dealerTurn();
    void results();
    Dealer dlr;
  public:
    BlackJack(int players);
    void playGame();
};

BlackJack :: BlackJack(int players){
  for(int i = 0; i < players; i++){
    Player new_plr;
    this->players.push_back(new_plr);
  }
}

void BlackJack :: playGame(){
  for(int i = 0; i < deal_limit; i++){
    for(Player &p : players){
      p.setHand(dlr.removeTopCard());
      Sleep(game_delay);
      cout << "\n";
    }
    dlr.setHand(dlr.removeTopCard());
    cout << "\n";
    Sleep(game_delay);
  }
  randomGameDelay();
  playerTurn();
  dealerTurn();
  results();
}

void BlackJack :: playerTurn(){
  for(Player &p : players){
    do{
      switch(p.getMove(dlr))
      {
        case 1: p.setHand(dlr.removeTopCard()); break;
        case 0: break;
      }
      randomGameDelay();
      cout << "\n";
    } while(p.getMove(dlr));
  }
}

void BlackJack :: dealerTurn(){
  while(dlr.getCardTotal() <= 17){
    dlr.setHand(dlr.removeTopCard());
    randomGameDelay();
    cout << "\n";
  }
}

void BlackJack :: results(){
  for(Player &p : players){
    if(!p.is_over && !dlr.is_over){
      if (p.getCardTotal() == dlr.getCardTotal()){
        cout << "push" << endl;
      } else if (p.getCardTotal() > dlr.getCardTotal()){
        cout << p.getID() << " WINS" << endl;
      } else {
        cout << dlr.getID() << " WINS" << endl;
      }
    }
  }
}

void BlackJack :: randomGameDelay(){
  int delays[5] = {500, 800, 1000, 1300, 1500};
  srand((unsigned) time(NULL));
  Sleep(delays[rand() % 5]);
}

int main(){
  bool play;
  BlackJack game(3);

  do{
    game.playGame();
    cout << "play again? Y[1] or N[0]\t";
    cin >> play;
  } while(play);

}
