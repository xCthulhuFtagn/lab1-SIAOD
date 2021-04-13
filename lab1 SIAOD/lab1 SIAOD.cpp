#define USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <string>

using namespace std;

void gen(const vector<vector<double>>& graph, unsigned start, unsigned end, double len, double cur_len = 0, vector <unsigned> ans = {}){
    ans.push_back(start);
    if (start == end) {
        cout << "Path: ";
        for (unsigned i = 0; i < ans.size(); ++i){
            cout << ans[i] + 1 << ' ';
        }
        cout << endl;
        return;
    }
    for (unsigned i = 0; i < graph.size(); ++i){
        if (graph[start][i] > 0 && cur_len + graph[start][i] <= len){
            gen(graph, i, end, len, cur_len + graph[start][i], ans);
        }
    }
}

void BellmanFord(const vector<vector<double>>& graph, unsigned start, unsigned end){
    unsigned n = graph.size();
    queue <unsigned> x({ start });
    vector <double> dp(n);
    for (unsigned i = 0; i < n; ++i){
        dp[i] = INFINITY;
    }
    dp[start] = 0;
    while (!x.empty()){
        unsigned w = x.front();
        for (unsigned i = 0; i < n; ++i){
            if (dp[i] > dp[w] + graph[w][i]) {
                x.push(i);
                dp[i] = dp[w] + graph[w][i];
            }
        }
        x.pop();
    }
    if (!isinf(dp[end])){
        cout << "Length of the shortest way: " << dp[end] << endl;
        gen(graph, start, end, dp[end]);
    }
    else
        cout << "There isn't any path" << endl;
}

void Prima(const vector<vector<double>>& graph) {
    unsigned n = graph.size();
    vector<bool> used(n);
    vector<double> min_e(n, INFINITY);
    vector <unsigned> sel_e(n, -1);
    min_e[0] = 0;
    double ans = 0;
    for (unsigned i = 0; i < n; ++i) {
        unsigned v;
        for (v = 0; v < n && used[v]; ++v);
        for (unsigned j = v + 1; j < n; ++j) {
            if (!used[j] && min_e[j] < min_e[v]) {
                v = j;
            }
        }
        if (isinf(min_e[v])) {
            cout << "No MSTree!" << endl;
            exit(0);
        }
        ans += min_e[v];
        used[v] = true;
        for (unsigned to = 0; to < n; ++to){
            if (!used[to] && graph[v][to] < min_e[to]) {
                min_e[to] = graph[v][to];
                sel_e[to] = v;
            }
        }
    }
    cout << "Weight of MSTree: " << ans << endl;
    cout << "Edges of MSTree:" << endl;
    for (unsigned i = 1; i < sel_e.size(); ++i) {
        cout << '(' << sel_e[i] + 1 << ", " << i + 1 << ')' << endl;
    }
}

struct edge{
    unsigned i, j;
    double weight;
};

int main()
{
    setlocale(LC_ALL, "RUS");
    unsigned size = 0, start, end;
    ifstream fin("graph.txt");
    vector <edge> tmp_vec;
    while (true){
        unsigned tmp_j, tmp_k;
        double tmp_w;
        fin >> tmp_j >> tmp_k >> tmp_w;
        if (!fin) break;
        size = max(max(tmp_j, tmp_k), size);
        tmp_vec.push_back({tmp_j - 1, tmp_k - 1, tmp_w});
    }
    fin.close();
    vector<vector<double>> matrix(size, vector<double>(size));
    for (auto& i : matrix)
        for (double& x : i)
            x = INFINITY;
    for (unsigned i = 0; i < size; ++i)
        matrix[i][i] = 0;
    for (unsigned i = 0; i < tmp_vec.size(); ++i){
        matrix[tmp_vec[i].i][tmp_vec[i].j] = tmp_vec[i].weight;
    }
    unsigned choice;
    try {
        cout << "Select algorithm for graph with " << size << " vertices:" << endl;
        cout << "1) Bellman-Ford" << endl;
        cout << "2) Prima" << endl;
        cin >> choice;
        if (!cin || choice == 0 || choice > 2) throw invalid_argument("Wrong selection format!");
    }
    catch (exception& bad) {
        cout << bad.what() << endl;
        return -2;
    }
    if (choice == 2) goto PRIMA;
    try {
        cout << "Enter start and end: ";
        cin >> start >> end;
        --start; --end;
        if (!cin || start >= size || end >= size) throw invalid_argument("Wrong start/end format!");
    }
    catch (exception& bad) {
        cout << bad.what() << endl;
        return -2;
    }
    BellmanFord(matrix, start, end);
    return 0;
    PRIMA:
    for (unsigned i = 0; i < tmp_vec.size(); ++i){
        matrix[tmp_vec[i].j][tmp_vec[i].i] = tmp_vec[i].weight;
    }
    Prima(matrix);
    return 0;
}
