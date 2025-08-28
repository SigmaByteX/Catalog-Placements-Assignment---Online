#include <bits/stdc++.h>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

// Convert number string in given base -> decimal integer
long long decodeValue(const string &val, int base) {
    long long num = 0;
    for (char c : val) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else digit = toupper(c) - 'A' + 10; // handles bases > 10
        num = num * base + digit;
    }
    return num;
}

// Lagrange interpolation at x=0 to find constant term (secret c)
long long lagrangeInterpolation(const vector<pair<int,long long>> &points, int k) {
    long long c = 0;
    for (int i = 0; i < k; i++) {
        long double term = points[i].second; // yi
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (0 - points[j].first);
                term /= (points[i].first - points[j].first);
            }
        }
        c += term;
    }
    return llround(c); // nearest integer
}

int main() {
    // Step 1: Read JSON file
    ifstream inFile("input.json");
    if (!inFile) {
        cerr << "Error: input.json not found!" << endl;
        return 1;
    }

    json j;
    inFile >> j;

    int n = j["keys"]["n"];
    int k = j["keys"]["k"];

    vector<pair<int,long long>> points;

    // Step 2: Decode y values
    for (auto &el : j.items()) {
        if (el.key() == "keys") continue;
        int x = stoi(el.key());
        int base = stoi(el.value()["base"].get<string>());
        string valStr = el.value()["value"];
        long long y = decodeValue(valStr, base);
        points.push_back({x, y});
    }

    // Take first k points only
    sort(points.begin(), points.end());
    if (points.size() > k) points.resize(k);

    // Step 3: Find secret c
    long long secret = lagrangeInterpolation(points, k);

    cout << "Constant term (c) = " << secret << endl;
    return 0;
}
