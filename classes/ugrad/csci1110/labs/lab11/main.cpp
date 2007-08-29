#include <iostream>
using namespace std;
 
int GetRandom( int, int );
string GetCard( int );
void ShuffleCards( int[] );
 
int GetRandom( int low, int high ) {
   int r;
 
   int combo = high - low + 1;
   r = rand() / ((RAND_MAX + 1)/combo) % (combo + 1);
 
   return abs(r);
}
 
string GetCard( int card ) {
   string cardRank;
   string cardSuit;
   int cardS;
   int cardR;
    
   cardS = card / 13; 
   if ( cardS == 0 )
      cardSuit = "Spades";
   else if ( cardS == 1 )
      cardSuit = "Hearts";
   else if ( cardS == 2 )
      cardSuit = "Clubs";
   else if ( cardS == 3 )
      cardSuit = "Diamonds";
   else
      cardSuit = "cardSuit: error!";
 
   cardR = card % 13;
   switch (cardR) {
      case 0 : cardRank = "Ace";
                break;
      case 1 : cardRank = "2";
                break;
      case 2 : cardRank = "3";
                break;
      case 3 : cardRank = "4";
                break;
      case 4 : cardRank = "5";
                break;
      case 5 : cardRank = "6";
                break;
      case 6 : cardRank = "7";
                break;
      case 7 : cardRank = "8";
                break;
      case 8 : cardRank = "9";
                break;
      case 9: cardRank = "10";
                break;
      case 10: cardRank = "Jack";
                break;
      case 11: cardRank = "Queen";
                break;
      case 12: cardRank = "King";
                break;
      default : cardRank = "cardRank: error!";
   }
   return (cardRank + " of " + cardSuit);
}
 
void ShuffleCards( int cards[51] ) {
   int tempCard, randCard;
 
   for ( int i = 0; i < 52; i++ ) {
      randCard = GetRandom(0, 51);
 
      tempCard = cards[i];
      cards[i] = cards[randCard];
      cards[randCard] = tempCard; 
   }
}
 
int main() {
   string cardName;
   int cards[52];
   int cardCounter = 0;
 
   srand((unsigned)time( NULL ));
 
   // Initialize the deck
   for(int i = 0; i < 52; i++) { 
      cards[i] = i;
      //cardName = GetCard( cards[i] ); 
      //cout << cardName << endl;
   }
   
   // Shuffle the cards seven times to make sure they are good and shuffled
   for(int i = 0; i < 100; i++) {
      ShuffleCards( cards );
   }
 
   // Deal 'em
   for(int i = 1; i < 6; i++) {
      cout << "Hand #" << i << endl;
      for(int j = 0; j < 5; j++) {
         cardName = GetCard( cards[cardCounter] ); 
         cout << cardName << endl;
         cardCounter++;
      }
      cout << endl;
   }
 
   return 0;
}
