
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <fstream>
#include <chrono>

using namespace std;

ifstream cin("2.txt");
ofstream cout("rez.txt");

struct CARD{
    int number;
    int color;
    /*
    1 - black
    2 - purple
    3 - grey
    4 - green
    */
};

struct BLOCK{
    vector<CARD> card;
    bool lead = true;
};

struct MOVE{
    CARD cardMoved;
    BLOCK blockMovedFrom;
    BLOCK blockMovedTo;
};

void PRINT(vector<CARD> v){
    cout<<"\n";
    for(auto i : v){
        cout<<i.number<<i.color<<" ";
    }
}


void PRINT(vector<BLOCK> v){
    cout<<"\n";
    for(auto i : v){
        cout<<"\n    ";
        for(auto j : i.card){
            cout<<j.number<<j.color<<" ";
        }
    }
}

void PRINT(vector<MOVE> v){
    cout<<"\n";
    for(auto m : v){
        cout<<"\nMOVE--------\n";
        cout<<"Card moved: "<<m.cardMoved.number<<m.cardMoved.color<<"\n";
        cout<<"From block: ";
        for(auto i : m.blockMovedFrom.card){
            cout<<i.number<<i.color<<" ";
        }
        cout<<"\nTo block: ";
        for(auto i : m.blockMovedTo.card){
            cout<<i.number<<i.color<<" ";
        }
    }
    cout<<"\n";
}

void input(vector<CARD> & myCards, vector<BLOCK> & table){
    cout<<"Input your cards <number color>\n0-joker,1-black,2-purple,3-grey,4-green\nTo end type -1 -1\n";
    while(true){
        CARD temp;
        cin>>temp.number>>temp.color;
        if(temp.number == -1 || temp.color == -1){
            break;
        }
        myCards.push_back(temp);
    }
    cout<<"Input card blocks on the table <number color>\n0-joker,1-black,2-purple,3-grey,4-green\nTo end type -1 -1\n";
    while(true){
        BLOCK btemp;
        while(true){
            CARD ctemp;
            cin>>ctemp.number>>ctemp.color;
            if(ctemp.color == -1 || ctemp.number == -1){
                break;
            }
            btemp.card.push_back(ctemp);
        }
        table.push_back(btemp);
        cout<<"To end type 0, to start a new block type 1\n";
        bool toggle;
        cin>>toggle;
        if(!toggle){
            break;
        }
        cout<<"Enter cards on on the next block\n0-joker,1-black,2-purple,3-grey,4-green\nTo end type -1 -1\n";
    }
}

bool checkIfColor(vector<CARD> cards){
    bool color[5] = {0};
    int number = -1;
    if(cards.size() < 3 || cards.size() > 4){
        return false;
    }
    for(auto i : cards){
        if(number == -1){
            number = i.number;
        }
        if(number != i.number){
            return false;
        }
        if(!color[i.color]){
            color[i.color] = 1;
        }else{
            return false;
        }
    }
    return true;
}

bool compareCards(const CARD& a, const CARD& b) {
    if(a.number == b.number){
        return a.color < b.color;
    }
    return a.number < b.number;
}

bool checkIfRow(vector<CARD> cards){
    int color = -1;
    int number = -1;
    sort(cards.begin(), cards.end(), compareCards);
    if(cards.size() < 3){
        return false;
    }
    for(auto i : cards){
        if(color == -1){
            color = i.color;
        }
        if(number == -1){
            number = i.number;
        }
        if(i.color != color){
            return false;
        }
        if(number == i.number){
            number++;
        }else{
            return false;
        }
    }
    return true;
}

bool correct(vector<BLOCK> table){
    for(auto t : table){
        if(!checkIfColor(t.card) && !checkIfRow(t.card)){
            return false;
        }
    }
    return true;
}

uint64_t stateHashGenerate(const vector<CARD>& myCards, const vector<BLOCK>& table) {
    uint64_t h = 0;
    for (const auto& c : myCards) {
        h = h * 31 + c.number * 5 + c.color;
    }
    for (const auto& b : table) {
        for (const auto& c : b.card) {
            h = h * 31 + c.number * 5 + c.color;
        }
        h *= 7;
    }
    return h;
}

bool compareBlocks(const BLOCK & a, const BLOCK & b){
    size_t n = min(a.card.size(), b.card.size());
    for (size_t i = 0; i < n; ++i) {
        if (a.card[i].number != b.card[i].number){
            return a.card[i].number < b.card[i].number;
        }
        if (a.card[i].color != b.card[i].color){
            return a.card[i].color < b.card[i].color;
        }
    }
    return a.card.size() < b.card.size();
}

void sortCards(vector<CARD> & myCards, vector<BLOCK> & table){
    sort(myCards.begin(), myCards.end(), compareCards);
    for(auto & i : table){
        sort(i.card.begin(), i.card.end(), compareCards);
    }
    sort(table.begin(), table.end(), compareBlocks);
}

bool possibilityExists(CARD pathCard, vector<CARD> myCards, vector<BLOCK> table){
    int colorCount[5] = {0};
    int numberCount[14] = {0};
    for(auto i : myCards){
        if(pathCard.number == i.number){
            colorCount[i.color] = 1;
        }
        if(pathCard.color == i.color){
            numberCount[i.number] = 1;
        }
    }
    for(auto i : table){
        for(auto j : i.card){
            if(pathCard.number == j.number){
                colorCount[j.color] = 1;
            }
            if(pathCard.color == j.color){
                numberCount[j.number] = 1;
            }
        }
    }
    int count = 0;
    for(int i = 1; i < 5; i++){
        if(colorCount[i] == 1){
            count++;
        }
        if(count >= 3){
            return true;
        }
    }
    count = 0;
    for(int i = pathCard.number; i<14;i++){
        if(numberCount[i] == 1){
            count++;
        }else{
            break;
        }
    }
    for(int i = pathCard.number-1; i>0;i--){
        if(numberCount[i] == 1){
            count++;
        }else{
            break;
        }
    }
    if(count >= 3){
        return true;
    }
    return false;
}

void checkLeads(CARD pathCard, vector<BLOCK> & table){
    for(auto & i : table){
        i.lead = false;
        if(!checkIfColor(i.card) && !checkIfRow(i.card)){
            i.lead = true;
            continue;
        }
        for(auto j : i.card){
            if(pathCard.color == j.color || pathCard.number == j.number){
                i.lead = true;
                break;
            }
        }
    }
}

void searchMoves(vector<CARD> myCards, vector<BLOCK> table, vector<MOVE> & bestMoves){
    const int maxDepth = 5;
    
    int maxPoints = 0;
    vector<MOVE> movesToBest;
    set<uint64_t> visitedStates;

    struct QUE{
        vector<MOVE> previousMoves;
        vector<CARD> myCards;
        vector<BLOCK> table;
        int points;
        int depth;
        CARD pathForCard;
    };

    queue<QUE> q;
    q.push({{}, myCards, table, 0, 0, {-1, -1}});
    while(!q.empty()){
        QUE temp = q.front();
        q.pop();
        if(temp.pathForCard.color != -1 && !possibilityExists(temp.pathForCard, temp.myCards, temp.table)){
            continue;
        }

        checkLeads(temp.pathForCard, temp.table);

        if(temp.points > maxPoints && correct(temp.table)){
            maxPoints = temp.points;
            movesToBest = temp.previousMoves;
        }
        if(temp.depth > maxDepth){continue;}

        sortCards(temp.myCards, temp.table);
        uint64_t hash = stateHashGenerate(temp.myCards, temp.table);
        if(visitedStates.count(hash)){
            continue;
        }
        visitedStates.insert(hash);

        //Pushes myCard on a new table block
        for(size_t idx = 0; idx < temp.myCards.size(); idx++){
            QUE pushTemp = temp;
            pushTemp.depth++;
            pushTemp.myCards.erase(pushTemp.myCards.begin() + idx);
            pushTemp.points++;
            MOVE moveTemp;
            moveTemp.cardMoved = temp.myCards[idx];
            moveTemp.blockMovedFrom.card = temp.myCards;
            moveTemp.blockMovedTo.card.push_back({-1, -1});
            pushTemp.previousMoves.push_back(moveTemp);
            BLOCK blockTemp;
            blockTemp.card.push_back(temp.myCards[idx]);
            pushTemp.table.push_back(blockTemp);
            if(pushTemp.pathForCard.color == -1){
                pushTemp.pathForCard = temp.myCards[idx];
            }
            q.push(pushTemp);
        }

        //Pushes myCard on all old table blocks
        for(size_t idxMyCards = 0; idxMyCards < temp.myCards.size(); idxMyCards++){
            for(size_t idxTable = 0; idxTable < temp.table.size(); idxTable++){
                if(!table[idxTable].lead){
                    continue;
                }
                QUE pushTemp = temp;
                pushTemp.depth++;
                pushTemp.points++;
                pushTemp.myCards.erase(pushTemp.myCards.begin() + idxMyCards);
                pushTemp.table[idxTable].card.push_back(temp.myCards[idxMyCards]);
                MOVE moveTemp;
                moveTemp.cardMoved = temp.myCards[idxMyCards];
                moveTemp.blockMovedFrom.card = temp.myCards;
                moveTemp.blockMovedTo.card = temp.table[idxTable].card;
                pushTemp.previousMoves.push_back(moveTemp);
                if(pushTemp.pathForCard.color == -1){
                    pushTemp.pathForCard = temp.myCards[idxMyCards];
                }
                if(checkIfColor(pushTemp.table[idxTable].card) || checkIfRow(pushTemp.table[idxTable].card) || (!checkIfColor(temp.table[idxTable].card) && !checkIfRow(temp.table[idxTable].card))){
                    q.push(pushTemp); 
                }
            }
        }

        if(temp.pathForCard.color == -1){
            continue;
        }

        //Pushes all table cards to all other blocks
        for(size_t idxTableFrom = 0; idxTableFrom < temp.table.size(); idxTableFrom++){
            for(size_t idxTableTo = 0; idxTableTo < temp.table.size(); idxTableTo++){
                for(size_t idxCard = 0; idxCard < temp.table[idxTableFrom].card.size(); idxCard++){
                    if(idxTableFrom == idxTableTo){
                        continue;
                    }
                    if(!table[idxTableFrom].lead && !table[idxTableTo].lead){
                        continue;
                    }
                    QUE pushTemp = temp;
                    pushTemp.depth++;
                    pushTemp.table[idxTableTo].card.push_back(temp.table[idxTableFrom].card[idxCard]);
                    pushTemp.table[idxTableFrom].card.erase(pushTemp.table[idxTableFrom].card.begin() + idxCard);
                    if(pushTemp.table[idxTableFrom].card.empty()){
                        pushTemp.table.erase(pushTemp.table.begin() + idxTableFrom);
                    }
                    MOVE moveTemp;
                    moveTemp.cardMoved = temp.table[idxTableFrom].card[idxCard];
                    moveTemp.blockMovedFrom = temp.table[idxTableFrom];
                    moveTemp.blockMovedTo = temp.table[idxTableTo];
                    pushTemp.previousMoves.push_back(moveTemp);
                    if(checkIfColor(pushTemp.table[idxTableTo].card) || checkIfRow(pushTemp.table[idxTableTo].card) || (!checkIfColor(temp.table[idxTableTo].card) && !checkIfRow(temp.table[idxTableTo].card))){
                        q.push(pushTemp);
                    }
                }
            }
        }
    }
    bestMoves = movesToBest;
}

int main(){
    auto start = chrono::high_resolution_clock::now();
    vector<BLOCK> table;
    vector<CARD> myCards;
    vector<MOVE> bestMoves;
    input(myCards, table);
    searchMoves(myCards, table, bestMoves);
    PRINT(bestMoves);
    PRINT(myCards);
    PRINT(table);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "\nExecution time: " << duration.count() << " seconds\n";
    return 0;
}