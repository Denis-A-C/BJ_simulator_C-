#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
using namespace std;
class LcgGenerator{
private:
    uint16_t mA;
    uint16_t mC;
    uint16_t mCurrentX;
    uint16_t mM;
public:
    LcgGenerator(uint16_t a, uint16_t b, uint16_t c, uint16_t d){
        mA = a;
        mC = b;
        mCurrentX = c;
        mM = d;
    }
    uint16_t next(){
        mCurrentX = (mA * mCurrentX + mC) % mM;
        return mCurrentX;
    }
};
class Card{
private:
    string mSuite;
    uint16_t mValue;
public:
    Card(){
        mSuite = "";
        mValue = 0;
    }
    Card(const string &sir, uint16_t x){
        mSuite = sir;
        mValue = x;
    }
    bool operator == (const Card & card)const{
        if(card.mSuite == mSuite && card.mValue == mValue){
            return true;
        }else return false;
    }
    string toString ()const{
        string s;
        s = to_string(mValue) + ',' + mSuite;
        return s;
    }
    bool isFake()const{
        if(mSuite != "rosu" && mSuite != "negru" && mSuite != "trefla" && mSuite != "romb" || mValue < 2 || mValue > 14){
            return true;
        }else return false;
    }
    uint16_t getBlackjackValue()const{
        if(mValue >= 2 && mValue <= 10){
            return mValue;
        }else return 10;
    }
    uint16_t getValue()const{
        return mValue;
    }
    string getColor()const{
        return mSuite;
    }
    void print(){
        cout << mSuite << " " << mValue << endl;
    }
};
class CardPack{
private:
    vector <Card> mCards;
    uint16_t mCurrentIndex;
public:
    CardPack(bool empty = true){
        mCurrentIndex = 0;
        if(empty)
        {

        }
        else{
            mCurrentIndex = 0;
            for(int i = 0; i < 4; i++){
                for(int j = 2; j < 15; j++){
                    if(i == 0){
                        if(j != 11)
                            mCards.push_back(Card("rosu", j));
                        else continue;
                    }
                    if(i == 1){
                        if(j != 11)
                            mCards.push_back(Card("negru", j));
                        else continue;
                    }
                    if(i == 2){
                        if(j != 11)
                            mCards.push_back(Card("trefla", j));
                        else continue;
                    }
                    if(i == 3){
                        if(j != 11)
                            mCards.push_back(Card("romb", j));
                        else continue;
                    }
                }
            }
        }
    }
    bool goodForBackjack()const{
        int ok = 1;
        for(const auto & mCard : mCards){
            if(mCard.isFake() || mCard.getValue() == 11)
                ok = 0;
        }
        if(ok == 0){
            return false;
        }else return true;
    }
    bool ok()const{
        int ok = 1;
        for(const auto & mCard : mCards)
            if(mCard.isFake()){
                ok = 0;
            }
        if(ok == 0){
            return false;
        }else return true;
    }
    void addCard(const Card & card){
        mCards.push_back(card);
    }
    uint16_t getIndex() const{
        return mCurrentIndex;
    }
    void shuffle(uint16_t a1, uint16_t c1, uint16_t x01, uint16_t a2, uint16_t c2, uint16_t x02){
        mCurrentIndex = 0;
        //LcgGenerator a(a1, c1, x01, mCards.size());
        //LcgGenerator b(a2, c2, x02, mCards.size());
        int x1, x2;
        x1 = (a1 * x01 + c1) % mCards.size();
        x2 = (a2 * x02 + c2) % mCards.size();
        Card x;
        for(int i = 0; i < 50; i++){
            x = mCards[x1];
            mCards[x1] = mCards[x2];
            mCards[x2] = x;
            x1 = (a1 * x1 + c1) % mCards.size();
            x2 = (a2 * x2 + c2) % mCards.size();

        }
    }
    Card nextCard(){
        mCurrentIndex++;
        return mCards[mCurrentIndex - 1];
    }
    void printPack()const{
        for(const auto & mCard : mCards){
            cout << mCard.toString();
        }
    }
    int getSize(){
        return mCards.size();
    }
    Card getCard(uint16_t i){
        return mCards[i];
    }
    void removeCard(const Card & card){
        for(int i = 0; i < mCards.size(); i++)
            if(mCards[i] == card)
                mCards.erase(mCards.begin() + i);
    }
    void reset(){
        mCurrentIndex = 0;
    }
};
class Player{
private:
    string mName;
    uint16_t mMoney;
    uint16_t mPoints;
public:
    Player(string name = "Dealer", uint16_t money = 0){
        mName = name;
        mMoney = money;
        mPoints = 0;
    }
    string toString()const{
        string s;
        s = mName + ':' + to_string(mMoney);
        return s;
    }
    void pick(const Card & card){
        mPoints = card.getValue();
    }
    void play(CardPack & cardpack){
        for(int i = 0; i < cardpack.getSize(); i++){
            if(mPoints < 17){
                mPoints = mPoints + cardpack.nextCard().getBlackjackValue();
            }else break;
        }
    }
    void newRound(){
        mPoints = 0;
    }
    bool lostRound()const{
        if(mPoints > 21)
            return true;
    }
    uint16_t getPoints()const{
        return mPoints;
    }
    void operator --(){
        mMoney = mMoney - 10;
    }
    void operator ++(){
        mMoney = mMoney + 10;
    }
    bool lostGame()const{
        if(mMoney < 10){
            return true;
        }else return false;
    }
    void print(){
        cout << mName << ": " << mMoney << endl;
    }
    void printPoints(){
        cout << mName << " " << mPoints << endl;
    }
    void addPoints(const Card & card){
        mPoints = mPoints + card.getBlackjackValue();
    }
    uint16_t getMoney() const{
        return mMoney;
    }
};
int main(){
    CardPack pachet;
    string command, culoare, nume;
    char caracter = ',';
    int value, ok = 1, n, bani, j;
    int a1, c1, x01, a2, c2, x02;
    vector <Player*> v;
    Player dealer;
    cin >> command;
    if(command == "check_cards"){
        while(cin >> value >> caracter >> culoare){
            Card x(culoare, value);
            pachet.addCard(x);
        }
        if(pachet.ok() == 1 && pachet.goodForBackjack() == 1){
            cout << "Pregatit pentru Blackjack"<< endl;
        }else if(pachet.ok() == 1 && pachet.goodForBackjack() == 0){
            cout << "Pachet OK"<< endl;
        }else cout << "Pachet masluit" << endl;
    }else if (command == "shuffle_cards"){
        cin >> a1 >> c1 >> x01 >> a2 >> c2 >> x02;
        while(cin >> value >> caracter >> culoare){
            Card x(culoare, value);
            pachet.addCard(x);
        }
        pachet.shuffle(a1, c1, x01, a2, c2, x02);
        for(int i = 0; i < pachet.getSize(); i++){
            cout << pachet.getCard(i).getValue() <<"," << pachet.getCard(i).getColor() << endl;
        }
    }else if (command == "play_game"){
        cin >> n;
        for(int i = 0; i < n; i++){
            cin >> nume >> bani;
            v.push_back(new Player(nume, bani));
        }
        CardPack pachet1(false);
        while(cin >> a1 >> c1 >> x01 >> a2 >> c2 >> x02){
            for(auto & i : v){
                i->newRound();
            }
            dealer.newRound();
            pachet1.shuffle(a1, c1, x01, a2, c2, x02);
            pachet1.reset();
            for(auto & i : v){
                i->addPoints(pachet1.nextCard());
                i->addPoints(pachet1.nextCard());
            }
            dealer.addPoints(pachet1.nextCard());
            dealer.addPoints(pachet1.nextCard());
            for(auto & i : v)
            {
                i->play(pachet1);
            }
            dealer.play(pachet1);
            for(auto & i : v){
                if((dealer.getPoints() >= 21 && i->getPoints() > 21)){
                    --*i;
                }else if(dealer.getPoints() > 21 && i->getPoints() <= 21){
                    ++*i;
                }else if(dealer.getPoints() <= 21 && i->getPoints() > 21){
                    --*i;
                }else if(dealer.getPoints() <= 21 && i->getPoints() <= 21){
                    if(dealer.getPoints() > i->getPoints())
                        --*i;
                    if(dealer.getPoints() < i->getPoints())
                        ++*i;
                }
            }
            for(int i = 0; i < v.size(); i++){
                if(v[i]->lostGame()){
                    v.erase(v.begin() + i);
                    i--;
                }
            }
        }
        for(auto & i : v){
            if(!i->lostGame())
                i->print();
        }
    }
    return 0;
}
