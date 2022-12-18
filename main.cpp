#include <bits/stdc++.h>
#include <fstream>

using namespace std;

set<pair<string, bool>>
search_in_itemset(vector<string> temp, vector<string> nontemp, map<string, string> itemSet, int min_sup) {
    //           item   isTemp
    vector<pair<string, bool>> new_item_set;
    map<string, string>::iterator it;

    for (int i = 0; i < temp.size(); i++) {
        string s = temp[i];
        int freq = 0;

        for (it = itemSet.begin(); it != itemSet.end(); it++) {
            int left_b = 0, right_b = 0;
            bool danger = false;
            if (it->second.find('(')) {
                left_b = it->second.find('(');
                right_b = it->second.find(')');
                int start = left_b + 1, end = right_b;

                while (start != end) {
                    if (s.find(it->second[start]) < s.length())
                        danger = true;

                    else {
                        danger = false;
                        break;
                    }
                    start++;
                }
                if (danger)
                    continue;
            }

            bool add = false;
            for (int c = 0; c < s.size(); c++) {
                if (c == s.size() - 1)
                    continue;

                if ((it->second.find(s[c]) < it->second.length() && it->second.find(s[c + 1]) < it->second.length() &&
                     it->second.find(s[c]) < it->second.find(s[c + 1])))
                    add = true;
                else {
                    add = false;
                    break;
                }
            }
            if (add)
                freq++;

            if (freq >= min_sup)
                new_item_set.push_back(make_pair(s, true));
        }
    }

    for (int i = 0; i < nontemp.size(); i++) {
        string s = nontemp[i];
        int freq = 0;

        for (it = itemSet.begin(); it != itemSet.end(); it++) {
            bool add = false;
            if (s.find('(') == 0) {
                bool good = false;
                int right_b = it->second.find(')');
                int start = 0, end = right_b;

                while (start != end) {
                    if (s.find(it->second[start]) < s.length())
                        good = true;

                    else {
                        good = false;
                        break;
                    }
                    start++;
                }
                if (good) {
                    if (it->second.find(s[s.find(')') + 1]) > it->second.find(s[s.find(')')])) {
                        for (int c = right_b + 1; c < s.size(); c++) {
                            if (c == s.size() - 1)
                                continue;

                            if (it->second.find(s[c]) < it->second.find(s[c + 1]))
                                add = true;

                            else {
                                add = false;
                                break;
                            }
                        }
                    } else
                        add = false;

                    if (add)
                        freq++;
                }
            } else {
                int letter = it->second.find(s[0]);

                bool good = false;
                int right_b = it->second.find(')'), left_b = it->second.find('(');
                int start = left_b, end = right_b;

                while (start != end) {
                    if (s.find(it->second[start]) < s.length())
                        good = true;

                    else {
                        good = false;
                        break;
                    }
                    start++;
                }
                if (good) {

                    if (it->second.find(s[s.find('(')]) > letter) {
                        for (int c = 0; c < left_b; c++) {

                            if (it->second.find(s[c]) < it->second.find(s[c + 1]))
                                add = true;

                            else {
                                add = false;
                                break;
                            }
                        }
                    } else
                        add = false;

                    if (add)
                        freq++;
                }
            }

            if (freq >= min_sup)
                new_item_set.push_back(make_pair(s, false));
        }
    }

    set<pair<string, bool>> rtrn;
    for (int i = 0; i < new_item_set.size(); i++)
        rtrn.insert(new_item_set[i]);

    set<pair<string, bool>>::iterator st = rtrn.begin();
    for (int i = 0; i < rtrn.size(); i++) {
        if (!st->second) {
            string s = st->first;
//            s = '(' + s + ')';
            bool b = st->second;
            rtrn.erase(st);
            rtrn.insert(make_pair(s, b));
        }
        st++;
    }

    cout << "----------------------Frequent 2-item sequences are----------------------" << endl;
    for (st = rtrn.begin(); st != rtrn.end(); st++) {
        if (st->second)
            cout << st->first << " --> temporal" << endl;
        else
            cout << st->first << " --> non-temporal" << endl;
    }
    cout << "--------------------------------------------" << endl;
    return rtrn;
}

map<char, int> item_seq(map<string, string> itemSet, int min_sup) {
    //  item  frequency
    map<char, int> seq;
    map<string, string>::iterator it = itemSet.begin(), it_in;
    int size = itemSet.size();

    for (int i = 0; i < size; i++) {
        string s;
        s = it->second;

        if (i == 0)
            for (int j = 0; j < s.size(); j++) {
                if (s[j] == '(' || s[j] == ')') //ignore ( )
                    continue;

                for (it_in = itemSet.begin(); it_in != itemSet.end(); it_in++)
                    if (it_in->second.find(s[j]) < it_in->second.size())
                        seq[s[j]]++;
            }

        for (int j = 0; j < s.size(); j++) {
            if (s[j] == '(' || s[j] == ')') // ignore ( )
                continue;

            if (seq.find(s[j]) != seq.end()) // found
                continue;

            for (it_in = itemSet.begin(); it_in != itemSet.end(); it_in++)
                if (it_in->second.find(s[j]) < it_in->second.size())
                    seq[s[j]]++;

        }
        it++;
    }

    map<char, int>::iterator i;

    // purning infrequent items
    /////////////////////////////////
    char erased[100];
    int x = 0;

    for (i = seq.begin(); i != seq.end(); i++)
        if (i->second < min_sup) {
            erased[x] = i->first;
            x++;
        }

    for (int i = 0; i < x; i++)
        seq.erase(erased[i]);
    /////////////////////////////////

    cout << "----------------------candidates of 1- item sequence----------------------" << endl;
    for (i = seq.begin(); i != seq.end(); i++)
        cout << i->first << " " << i->second << endl;
    cout << "--------------------------------------------" << endl;
    return seq;
}

vector<string> temp_seq(map<char, int> first_can) {
    vector<string> temp;
    map<char, int>::iterator i, j;

    for (i = first_can.begin(); i != first_can.end(); i++) {
        for (j = first_can.begin(); j != first_can.end(); j++) {
            string s(1, i->first);
            s += j->first;
            temp.push_back(s);
        }
    }
    cout << "----------------------All possible Temporal joins----------------------" << endl;
    for (int x = 0; x < temp.size(); x++)
        cout << temp[x] << " ";
    cout << endl;
    cout << "--------------------------------------------" << endl;

    return temp;
}

vector<string> nontemp_seq(map<char, int> first_can) {
    vector<string> nontemp;
    map<char, int>::iterator i, j;

    for (i = first_can.begin(); i != first_can.end(); i++) {
        for (j = i; j != first_can.end(); j++) {
            if (i->first == j->first)
                continue;

            string s(1, i->first);
            s += j->first;
            nontemp.push_back(s);
        }
    }
    cout << "----------------------All possible non-Temporal joins----------------------" << endl;
    for (int x = 0; x < nontemp.size(); x++)
        cout << nontemp[x] << " ";
    cout << endl;
    cout << "--------------------------------------------" << endl;

    return nontemp;
}

set<pair<string, bool>>
item_seq2(vector<string> temp, vector<string> nontemp, map<string, string> itemSet, int min_sup) {
    //           item   isTemp
    vector<pair<string, bool>> new_item_set;
    map<string, string>::iterator it;

    for (int i = 0; i < temp.size(); i++) {
        string s = temp[i];
        int freq = 0;

        for (it = itemSet.begin(); it != itemSet.end(); it++) {
            int left_b = 0;
            if (it->second.find('('))
                left_b = it->second.find('(');

            if (it->second.find(s[0]) == left_b + 1 && it->second.find(s[1]) == left_b + 2)
                continue;

            if (it->second.find(s[0]) < it->second.length() && it->second.find(s[1]) < it->second.length() &&
                it->second.find(s[0]) < it->second.find(s[1]))
                freq++;

            if (freq >= min_sup)
                new_item_set.push_back(make_pair(s, true));
        }
    }

    for (int i = 0; i < nontemp.size(); i++) {
        string s = nontemp[i];
        int freq = 0;

        for (it = itemSet.begin(); it != itemSet.end(); it++) {
            int left_b = 0;
            if (it->second.find('('))
                left_b = it->second.find('(');

            if (it->second.find(s) < it->second.length() && it->second.find(s) > left_b &&
                it->second.find(s) < left_b + 2)
                freq++;

            if (freq >= min_sup)
                new_item_set.push_back(make_pair(s, false));
        }
    }

    set<pair<string, bool>> rtrn;
    for (int i = 0; i < new_item_set.size(); i++)
        rtrn.insert(new_item_set[i]);

    set<pair<string, bool>>::iterator st = rtrn.begin();
    for (int i = 0; i < rtrn.size(); i++) {
        if (!st->second) {
            string s = st->first;
            s = '(' + s + ')';
            bool b = st->second;
            rtrn.erase(st);
            rtrn.insert(make_pair(s, b));
        }
        st++;
    }

    cout << "----------------------Frequent 2-item sequences are----------------------" << endl;
    for (st = rtrn.begin(); st != rtrn.end(); st++) {
        if (st->second)
            cout << st->first << " --> temporal" << endl;
        else
            cout << st->first << " --> non-temporal" << endl;
    }
    cout << "--------------------------------------------" << endl;
    return rtrn;
}

set<pair<string, bool>> generator(set<pair<string, bool>> can, int min_sup, map<string, string> itemSet) {
    //           seq    isTemp      -1st    -last
    multimap<pair<string, bool>, pair<string, string>> new_cans;
    set<pair<string, bool>>::iterator st;

    for (st = can.begin(); st != can.end(); st++) {
        string minus_first, minus_last;
        if (!st->second) {
            if (st->first.size() == 4) {
                minus_first = st->first[2];
                minus_last = st->first[1];

                new_cans.insert(make_pair(make_pair(st->first, st->second), make_pair(minus_first, minus_last)));
                new_cans.insert(make_pair(make_pair(st->first, st->second), make_pair(minus_last, minus_first)));
            }
        } else {
            minus_first = st->first.substr(1);
            minus_last = st->first.substr(0, st->first.size() - 1);
            new_cans.insert(make_pair(make_pair(st->first, st->second), make_pair(minus_first, minus_last)));
        }
    }
    multimap<pair<string, bool>, pair<string, string>>::iterator it;
    cout << "----------------------New Candidates----------------------" << endl << "seq -1st -last" << endl;
    for (it = new_cans.begin(); it != new_cans.end(); it++)
        cout << it->first.first << " " << it->second.first << " " << it->second.second << endl;
    cout << "--------------------------------------------" << endl;

    bool isAllGood = false;
    multimap<pair<string, bool>, pair<string, string>>::iterator i, j;
    //  seq    isTemp
    map<string, bool> new_seq;
    for (i = new_cans.begin(); i != new_cans.end(); i++) {
        for (j = new_cans.begin(); j != new_cans.end(); j++) {
            if (i->first.first == j->first.first)
                continue;

            string s;
            //last item in first == first item in second ==> join
            if (i->second.first == j->second.second) {
                    isAllGood = true;
                if (!j->first.second) //non-temporal
                    s = i->second.first + j->first.first;
                else
                    s = i->first.first + j->second.first;

                if (i->second.first.size() > 1)
                    s = i->second.second + j->second.first;

                if (!j->first.second || !i->first.second)
                    new_seq[s] = false;
                else
                    new_seq[s] = true;
//                cout << i->first.first << " + " << j->first.first << " = " << s << endl;
            }
        }
    }
    if(!isAllGood)
    {
        set<pair<string, bool>> x;
        return x;
    }
//    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    map<string, bool>::iterator it2;
    vector<string> temp, nontemp;
    for (it2 = new_seq.begin(); it2 != new_seq.end(); it2++) {
        cout << it2->first << " " << it2->second << endl;

        if (it2->second)
            temp.push_back(it2->first);
        else
            nontemp.push_back(it2->first);
    }

    return search_in_itemset(temp, nontemp, itemSet, min_sup);
}

int main() {
    ifstream gspFile;
    gspFile.open("C:\\Users\\Mehrael Ashraf\\CLionProjects\\gsp\\items.csv");

    // Cust_ID   items
    map<string, string> itemSet;
    int i = 0;
    while (gspFile.good()) {
        string line, line2;
        getline(gspFile, line, ','); //transaction date ==> ignore it
        getline(gspFile, line, ','); //Customer_ID
        getline(gspFile, line2); // Item

        if (line2.length() > 1)
            line2 = "(" + line2 + ")";

        if (itemSet[line].empty())
            itemSet[line] = line2;
        else
            itemSet[line] = itemSet[line] + line2;
    }
// now we have our itemset
    cout << "ITEMSET" << endl;
    map<string, string>::iterator it;
    for (it = itemSet.begin(); it != itemSet.end(); it++)
        cout << it->second << endl;

    int min_sup;
    cout << "Min Support =";
    cin >> min_sup;

    map<char, int> first_can = item_seq(itemSet, min_sup);

    vector<string> temporal = temp_seq(first_can);

    vector<string> nontemporal = nontemp_seq(first_can);

    set<pair<string, bool>> second_can = item_seq2(temporal, nontemporal, itemSet, min_sup);

    while(true)
    {
        set<pair<string, bool>> can = generator(second_can, min_sup, itemSet);

        if(can.size() > 0)
            second_can = can;
        else
            break;

    }

    return 0;
}
