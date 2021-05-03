#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

struct state {
    vector<vector<string>> stacksVector;//stores the individial blocks
    vector<string> arrOrig;//stores the entire column
    vector<state> successorStates;

    state(vector<string> arr) {//constructor
        arrOrig = arr;
        stacksVector.resize(arr.size());
        for(unsigned int i = 0; i < arr.size(); i++) {
            stacksVector[i].resize(arr[i].length());
            for(unsigned int j = 0; j < arr[i].length(); j++) {
                stacksVector[i][j] = arr[i][j];
            }
        }
    }
    vector<state> successors() {
        vector<string> arrcpy;
        arrcpy.resize(arrOrig.size());
        string topelement = "";
        for(unsigned int i = 0; i < stacksVector.size(); i++) {
            arrcpy = arrOrig;
            for(int a = 0; a < stacksVector.size(); a++) {//remove \r from strings
                //cout << arrcpy[a].at(arrcpy[a].size() - 1) << endl;
                if(arrcpy[a].empty()) {
                    continue;
                }
                else if(arrcpy[a].at(arrcpy[a].size() - 1) == '\r') {
                    arrcpy[a].erase(arrcpy[a].size() - 1);
                }
            }
            if(arrcpy[i][arrcpy[i].length() - 1] == NULL) {
                continue;
            }
            else {
                topelement = arrcpy[i][arrcpy[i].length() - 1];
                arrcpy[i].erase(arrcpy[i].end() - 1);
                for(unsigned int j = i; j < stacksVector.size() - 1; j++) {//generate successors for columns past the current position
                    arrcpy[j + 1] = arrcpy[j + 1].append(topelement);
                    state newstate(arrcpy);
                    successorStates.push_back(newstate);
                    arrcpy[j + 1].pop_back();
                }
                for(unsigned int k = i; k > 0; k--) {//generate successors for columns before the current position
                    arrcpy[k - 1] = arrcpy[k - 1].append(topelement);
                    state newstate(arrcpy);
                    successorStates.push_back(newstate);
                    arrcpy[k - 1].pop_back();
                }
            }
        }
        return successorStates;
    } 
    bool match(state* matchstate) {return false;}
    string hash(){return NULL;}
    void print(){
        for(int i = 0; i < stacksVector.size(); i++) {
            cout << arrOrig[i] << endl;
        }
    }
};




struct node {
    node* parent = nullptr;
    state* currentstate = nullptr;
    vector<state> successorStatesNode;
    vector<string> goalstate;
    int pathcost;
    node(vector<string> arr, node* parent, vector<string> goalstate, int pathcost) {//constructor
        currentstate = new state(arr);
        this->parent = parent;
        this->goalstate = goalstate;
        this->pathcost = pathcost;
    }
    ~node() {
        //delete currentstate;
    }
    bool goal_test() {
        if(goalstate == currentstate->arrOrig) {
            return true;
        }
        else {
            return false;
        }
    }
    void successorsNode() {
        successorStatesNode = currentstate->successors();
        for(int i = 0; i < successorStatesNode.size(); i++) {
            node successornode(successorStatesNode[i].arrOrig, parent, goalstate, pathcost);
        }
    }
    void print_path() {
        cout << "0: " << currentstate->arrOrig[0] << endl;
        cout << "1: " << currentstate->arrOrig[1] << endl;
        cout << "2: " << currentstate->arrOrig[2] << endl;


        // while(true) {
        //     for(int i = 0; i < currentstate->arrOrig.size(); i++) {
        //         cout << i << ": " << currentstate->arrOrig[i] << endl;
        //     }
        //     currentstate = this->parent->currentstate;
        // }
    }
    int calculate_heuristic() {//heuristic function
        int counter = 0;
        for(int i = 0; i < currentstate->stacksVector.size(); i++) {
            for(int j = 0; j < currentstate->stacksVector[i].size(); j++) {
                if(currentstate->stacksVector[i][j] == string(1, goalstate[i][j])) {
                    
                }
            }
        }
    }
};




node BFS(node initialstate) {
    if(initialstate.goal_test()) {
        initialstate.print_path();
        return initialstate;
    }
    int frontpos = 0;
    node currentnode = initialstate;
    cout << currentnode.currentstate->arrOrig[1];
    vector<node> frontier;//fifo queue
    frontier.push_back(initialstate);
    vector<node> reached = {initialstate};
    while(!frontier.empty()) {
        currentnode = frontier.at(frontpos);
        frontpos += 1;
        currentnode.successorsNode();
        cout << currentnode.successorStatesNode.at(0).arrOrig[0] << endl;
        cout << currentnode.successorStatesNode.at(0).arrOrig[1] << endl << endl;
        for(int i = 0; i < currentnode.successorStatesNode.size(); i++) {
            node childnode(currentnode.successorStatesNode[i].arrOrig, &currentnode, currentnode.goalstate, currentnode.pathcost + 1);
            // cout << childnode.currentstate->arrOrig[0] << endl;
            // cout << childnode.currentstate->arrOrig[1] << endl;
            if(childnode.goal_test()) {//child node is the goal state; end here
                childnode.print_path();
                return childnode;
            }
            else {//child node is not the goal node; push to queue and continue
                reached.push_back(childnode);
                frontier.push_back(childnode);
            }
        }
    }
    return initialstate;
}


node Astar(node initialstate) {
    if(initialstate.goal_test()) {
        initialstate.print_path();
        return initialstate;
    }
    int frontpos = 0;
    node currentnode = initialstate;
    cout << currentnode.currentstate->arrOrig[1];
    vector<node> frontier;//fifo queue
    frontier.push_back(initialstate);
    vector<node> reached = {initialstate};
    while(!frontier.empty()) {
        currentnode = frontier.at(frontpos);
        frontpos += 1;
        currentnode.successorsNode();
        cout << currentnode.successorStatesNode.at(0).arrOrig[0] << endl;
        cout << currentnode.successorStatesNode.at(0).arrOrig[1] << endl << endl;
        for(int i = 0; i < currentnode.successorStatesNode.size(); i++) {
            node childnode(currentnode.successorStatesNode[i].arrOrig, &currentnode, currentnode.goalstate, currentnode.pathcost + 1);
            // cout << childnode.currentstate->arrOrig[0] << endl;
            // cout << childnode.currentstate->arrOrig[1] << endl;
            if(childnode.goal_test()) {//child node is the goal state; end here
                childnode.print_path();
                return childnode;
            }
            else {//child node is not the goal node; push to queue and continue
                reached.push_back(childnode);
                frontier.push_back(childnode);
            }
        }
    }
    return initialstate;
}




int main(int argc, char **argv) {
    string testfile = "";
    string text = "";
    int stacks = 0;
    int blocks = 0;
    int moves = 0;
    if(argc == 2) {
        testfile = argv[1];
    }
    else {
        cout << "Incorrect number of arguments" << endl;
        exit(-1);
    }

    ifstream filestream(testfile);
    getline(filestream, text, ' ');//get stacks
    stacks = stoi(text);
    getline(filestream, text, ' ');//get blocks
    blocks = stoi(text);
    getline(filestream, text);//get moves
    moves = stoi(text);
    getline(filestream, text);//pass over '>' line

    vector<string> stacksArrayStart;
    stacksArrayStart.resize(stacks);
    vector<string> goalArrayStart;
    goalArrayStart.resize(stacks);

    for(int i = 0; i < stacks; i++) {
        getline(filestream, stacksArrayStart[i]);//get initial state
    }
    getline(filestream, text);//pass over '>' line
    for(int i = 0; i < stacks; i++) {
        getline(filestream, goalArrayStart[i]);//get goal state
    }

    //testing section
    //state teststate(stacksArrayStart);
    //vector<state> testsuccessors = teststate.successors();
    //teststate.print();
    //print out successors
    // cout << testsuccessors[3].arrOrig[0] << endl;
    // cout << testsuccessors[3].arrOrig[1] << endl;
    // cout << testsuccessors[3].arrOrig[2] << endl;
    for(int a = 0; a < goalArrayStart.size(); a++) {//remove \r from goal array
        goalArrayStart[a].erase(goalArrayStart[a].size() - 1);
    }
    node initial = node(stacksArrayStart, nullptr, goalArrayStart, 0);
    //BFS(initial);
    Astar(initial);
    cin.get();
}