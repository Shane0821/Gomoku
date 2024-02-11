#include <regex>

#include "bits/stdc++.h"

using namespace std;

int convertToBase10(string s) {
    int base10 = 0;
    int power = 1;
    for (int i = s.size() - 1; i >= 0; i--) {
        int digit = s[i] - '0';
        base10 += digit * power;
        power *= 3;
    }
    return base10;
}

string getType(const string &s) {
    if (regex_search(s, regex("11111"))) return "FIVE";

    if (regex_search(s, regex("211112"))) return "LIVE_FOUR";
    if (regex_search(s, regex("1211121"))) return "LIVE_FOUR";
    if (regex_search(s, regex("11211211"))) return "LIVE_FOUR";

    if (regex_search(s, regex("21111"))) return "SLEEP_FOUR";
    if (regex_search(s, regex("12111"))) return "SLEEP_FOUR";
    if (regex_search(s, regex("11211"))) return "SLEEP_FOUR";
    if (regex_search(s, regex("11121"))) return "SLEEP_FOUR";
    if (regex_search(s, regex("11112"))) return "SLEEP_FOUR";

    if (regex_search(s, regex("221112"))) return "LIVE_THREE";
    if (regex_search(s, regex("211122"))) return "LIVE_THREE";
    if (regex_search(s, regex("211212"))) return "LIVE_THREE";
    if (regex_search(s, regex("212112"))) return "LIVE_THREE";
    if (regex_search(s, regex("1212121"))) return "LIVE_THREE";

    if (regex_search(s, regex("11122"))) return "SLEEP_THREE";
    if (regex_search(s, regex("21112"))) return "SLEEP_THREE";
    if (regex_search(s, regex("22111"))) return "SLEEP_THREE";
    if (regex_search(s, regex("11212"))) return "SLEEP_THREE";
    if (regex_search(s, regex("21211"))) return "SLEEP_THREE";
    if (regex_search(s, regex("12112"))) return "SLEEP_THREE";
    if (regex_search(s, regex("21121"))) return "SLEEP_THREE";
    if (regex_search(s, regex("12211221"))) return "SLEEP_THREE";
    if (regex_search(s, regex("12211"))) return "SLEEP_THREE";
    if (regex_search(s, regex("11221"))) return "SLEEP_THREE";
    if (regex_search(s, regex("12121"))) return "SLEEP_THREE";

    if (regex_search(s, regex("212122"))) return "LIVE_TWO";
    if (regex_search(s, regex("221212"))) return "LIVE_TWO";
    if (regex_search(s, regex("212212"))) return "LIVE_TWO";
    if (regex_search(s, regex("221122"))) return "LIVE_TWO";

    if (regex_search(s, regex("12221"))) return "SLEEP_TWO";
    if (regex_search(s, regex("12212"))) return "SLEEP_TWO";
    if (regex_search(s, regex("21221"))) return "SLEEP_TWO";
    if (regex_search(s, regex("12122"))) return "SLEEP_TWO";
    if (regex_search(s, regex("22121"))) return "SLEEP_TWO";
    if (regex_search(s, regex("21212"))) return "SLEEP_TWO";
    if (regex_search(s, regex("11222"))) return "SLEEP_TWO";
    if (regex_search(s, regex("11222"))) return "SLEEP_TWO";
    if (regex_search(s, regex("21122"))) return "SLEEP_TWO";
    if (regex_search(s, regex("22112"))) return "SLEEP_TWO";
    if (regex_search(s, regex("22211"))) return "SLEEP_TWO";

    return "BASE";
}

vector<string> patterns;

void dfs(string s, int cont2, int cont1, int cnt1) {
    if (cont2 > 2) return;
    if (s.size() == 9 ||
        (cont2 == 2 && s.size() > 2 && s[s.size() - 1] == '2' &&
         s[s.size() - 2] == '2') ||
        (s.size() > 1 && s.back() == '0')) {
        if (s.size() == 9 && s[4] != '1') return;

        for (int i = 0, pre = 0; i < s.size(); i++) {
            if (s[i] == '1') {
                pre++;
            } else {
                break;
            }
            if (pre == cnt1 && pre <= 4) {
                return;
            }
        }

        if (s[0] == '0' && s[s.size() - 1] == '0') {
            if (cnt1 == s.size() - 2 && cnt1 <= 4) return;
            if (s.size() - 2 <= 4) return;
        }

        if (cnt1 && s.size() > 4) {
            if (s.back() == '0') s.pop_back();
            if (s[0] == '0') s.erase(s.begin());
            patterns.push_back(s);
        }
        return;
    }
    dfs(s + "0", 0, 0, cnt1);
    if (cont1 < 5) dfs(s + "1", 0, cont1 + 1, cnt1 + 1);
    if (cont2 < 2) dfs(s + "2", cont2 + 1, 0, cnt1);
}

int main() {
    freopen("1.out", "w", stdout);
    cout << setw(11) << left;
    dfs("", 0, 0, 0);

    auto cmp = [](const string &a, const string &b) {
        return convertToBase10(a) < convertToBase10(b);
    };

    sort(patterns.begin(), patterns.end(), cmp);

    auto last = unique(patterns.begin(), patterns.end());

    for (auto it = patterns.begin(); it != last; it++) {
        string s = *it;
        cout << s << " " << setw(11) << left << stoi(s) << " " << setw(11) << left
             << convertToBase10(s) << setw(11) << left << getType(s) << setw(11) << left
             << endl;
    }
    return 0;
}