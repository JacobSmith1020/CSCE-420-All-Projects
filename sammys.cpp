#include <iostream>
#include <vector>
#include <string>
#include "parser.hpp"
#include "NatDed.hpp"

using namespace std;

int main() {
    //test functions
    cout << "--Test functions for each rule of inference--------------------------" << endl << endl;

    modus_ponens_test();
    implication_elimination_test();
    and_elimination_test();
    and_introduction_test();
    or_introduction_test();
    double_negation_elimination_test();
    resolution_test();
    commutativity_test();

    cout << "--End of test functions----------------------------------------------" << endl << endl;


    //Proof that the correct label for box 2 is white (by natural deduction)
    vector<Expr*> kb = load_kb("sammys.kb");

    //step 1: modus_ponens((implies L3B (not C3B)) L3B) derives (not C3B)
    kb.push_back(modus_ponens(kb[10], kb[5]));//25
    //step 2: implication_elimination((implies L3B (not C3B))) derives (or (not L3B) (not C3B))
    kb.push_back(implication_elimination(kb[10]));//26
    //step 3: inside of (implies O3Y (or C3Y C3B)), resolution((or C3Y C3B) (or (not L3B) (not C3B))) derives (implies O3Y (or C3Y (not L3B)))
    string step3_1 = kb[11]->toString().substr(13, 12);
    Expr* step3_2 = commutativity(parse(step3_1));
    Expr* step3_3 = commutativity(kb[26]);
    Expr* step3_4 = resolution(step3_3, step3_2);
    Expr* step3_5 = commutativity(step3_4);
    string step3_6 = kb[11]->toString().substr(0, 13) + step3_5->toString() + ")";
    kb.push_back(parse(step3_6));//27
    //step 4: inside of (implies O3Y (or C3Y (not L3B))), implication_elimination((or C3Y (not L3B))) derives (implies O3Y (implies L3B C3Y))
    string step4_1 = kb[27]->toString().substr(13, 18);
    Expr* step4_2 = commutativity(parse(step4_1));
    Expr* step4_3 = implication_elimination(step4_2);
    string step4_4 = step4_3->toString().substr(9, 15);
    Expr* step4_5 = double_negation_elimination(parse(step4_4));
    string step4_6 = kb[27]->toString().substr(0, 13) + step4_3->toString().substr(0, 9) + step4_5->toString() + " " + step4_3->toString().substr(25, 5) + ")";
    kb.push_back(parse(step4_6));//28
    //step 5: inside of (implies O3Y (implies L3B C3Y)), modus_ponens((implies L3B C3Y) L3B) derives (implies O3Y C3Y)
    string step5_1 = kb[28]->toString().substr(13, 17);
    Expr* step5_2 = modus_ponens(parse(step5_1), kb[5]);
    string step5_3 = kb[28]->toString().substr(0, 13) + step5_2->toString() + ")";
    kb.push_back(parse(step5_3));//29
    //step 6: modus_ponens((implies O3Y C3Y) O3Y) dervies C3Y -- this step proves that the correct label for box 3 is yellow
    kb.push_back(modus_ponens(kb[29], kb[4]));//30
    //step 7: inside of (implies C3Y (or (and C1W C2B) (and C1B C2W))), implication_elimination((or (and C1W C2B) (and C1B C2W))) derives 
    //        (implies C3Y (implies (not (and C1W C2B)) (and C1B C2W)))
    string step7_1 = kb[24]->toString().substr(13, 32);
    Expr* step7_2 = implication_elimination(parse(step7_1));
    string step7_3 = kb[24]->toString().substr(0, 13) + step7_2->toString() + ")";
    kb.push_back(parse(step7_3));//31
    //step 8: inside of (implies C3Y (implies (not (and C1W C2B)) (and C1B C2W))), and_elimination((and C1W C2B)) and and_elimination((and C1B C2W)) derives
    //        (implies C3Y (implies (not C1W) C2W)) as well as (implies C3Y (implies (not C1W) C1B))
    string step8_1 = kb[31]->toString().substr(42, 13);
    vector<Expr*> step8_2 = and_elimination(parse(step8_1));
    string step8_3 = kb[31]->toString().substr(27, 13);
    vector<Expr*> step8_4 = and_elimination(parse(step8_3));
    string step8_5 = kb[31]->toString().substr(0, 27) + step8_4[0]->toString() + ") " + step8_2[1]->toString() + "))";
    kb.push_back(parse(step8_5));//32
    //step 9: modus_ponens((implies L1W (not C1W)) L1W) derives (not C1W)
    kb.push_back(modus_ponens(kb[21], kb[1]));//33
    //step 10: inside of (implies C3Y (implies (not C1W) C2W)) as well as (implies C3Y (implies (not C1W) C1B)), modus_ponens((implies (not C1W) C2W) (not C1W)) and
    //        modus_ponens((implies (not C1W) C1B) (not C1W)) derives (implies C3Y C2W) and (implies C3Y C1B)
    string step10_1 = kb[32]->toString().substr(13, 23);
    Expr* step10_2 = modus_ponens(parse(step10_1), kb[33]);
    string step10_3 = kb[32]->toString().substr(0, 13) + step10_2->toString() + ")";
    kb.push_back(parse(step10_3));//34
    //step 11: modus_ponens((implies C3Y C2W)) and modus_ponens((implies C3Y C1B)) derives C2W and C1B -- the correct label for box 2 is white and box 1 is both -- end of proof
    kb.push_back(modus_ponens(kb[34], kb[30]));//35

    //print out knowledge base
    cout << "--Print out final knowledge base-------------------------------------" << endl << endl;
    for(int i = 0; i < kb.size(); i++) {
        cout << kb[i]->toString() << endl;
    }
    cout << endl << "--End of final knowledge base----------------------------------------" << endl << endl;
    return 0;
}