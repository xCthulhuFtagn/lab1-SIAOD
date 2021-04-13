#define USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <vector>
#include <queue>

using namespace std;

void gen(int n, const vector<vector<double>>& graph, unsigned w, unsigned w2, double len, double now_len = 0, vector <int> ans = {}){
    ans.push_back(w);
    if (w == w2) {
        cout << "Path: ";
        for (int i = 0; i < ans.size(); ++i){
            cout << ans[i] + 1 << ' ';
        }
        cout << '\n';
        return;
    }
    for (int i = 0; i < n; ++i){
        if (graph[w][i] > 0 && now_len + graph[w][i] <= len){
            gen(n, graph, i, w2, len, now_len + graph[w][i], ans);
        }
    }
}

void BellmanFord(const vector<vector<double>>& graph, unsigned start, unsigned end){
    unsigned n = graph.size();
    queue <unsigned> x({ start });
    vector <double> dp(n);
    for (unsigned i = 0; i < n; ++i){
        dp[i] = -1;
    }
    dp[start] = 0;
    while (!x.empty()){
        unsigned w = x.front();
        for (unsigned i = 0; i < n; ++i){
            if (graph[w][i] > 0) {
                if (dp[i] < 0 || dp[i] > dp[w] + graph[w][i]) {
                    x.push(i);
                    dp[i] = dp[w] + graph[w][i];
                }
            }
        }
        x.pop();
    }
    if (dp[end] >= 0 && !isinf(dp[end])){
        cout << "Length of the shortest way: " << dp[end] << endl;
        gen(n, graph, start, end, dp[end]);
    }
    else
        cout << "There isn't way" << endl;
}

void Prima(const vector<vector<double>>& graph) {
    unsigned n = graph.size();
    vector<bool> used (n);
    vector<double> min_e (n, INFINITY);
    vector <unsigned> sel_e (n, -1);
    vector<vector<unsigned>> tree(n);
    min_e[0] = 0;
    for (unsigned i = 0; i < n; ++i) {
        unsigned v = 0;
        for (; v < n; ++v){
            if (!used[v]) break;
        }
        for (unsigned j = v + 1; j < n; ++j) {
            if (!used[j] && (v == -1 || min_e[j] < min_e[v])) {
                v = j;
            }
        }
        if (isinf(min_e[v])) {
            cout << "No MST!" << endl;
            exit(0);
        }
        used[v] = true;
        for (unsigned to = 0; to < n; ++to){
            if (used[to]) continue;
            if (graph[v][to] < min_e[to]) {
                min_e[to] = graph[v][to];
			    sel_e[to] = v;
            }
        }
    }
    double ans = 0;
    for (unsigned i = 1; i < sel_e.size(); ++i) {
        cout << sel_e[i] + 1 << " <-> " << i + 1 << endl;
        ans += min_e[i];
    }
    cout << "Weight of tree: " << ans << endl;
}

int main()
{
    setlocale(LC_ALL, "RUS");
    unsigned size, start, end;
    cout << "Enter number of vertices in your graph: ";
    try {
        cin >> size;
        if (!cin) throw exception("Wrong format in number of vertices!");
    }
    catch (exception& bad) {
        cout << bad.what() << endl;
        return -1;
    }
    vector<vector<double>> matrix(size, vector<double>(size));
    for (auto& i : matrix)
        for (double& x : i)
            x = INFINITY;
    for (unsigned i = 0; i < size; ++i)
        matrix[i][i] = 0;
    cout << "Enter quantity of edges: ";
    try {
        cin >> size;
        if (!cin) throw exception("Wrong format in number of vertices!");
    }
    catch (exception& bad) {
        cout << bad.what() << endl;
        return -1;
    }
    cout << "Enter edges:" << endl;
    for (unsigned i = 0; i < size; ++i){
        unsigned j, k;
        double weight;
        try {
            cin >> j >> k >> weight;
            matrix[j - 1][k - 1] = weight;
            if (!cin) throw exception("Wrong format in one of vertices!");
        }
        catch (exception& bad) {
            cout << bad.what() << endl;
            return -2;
        }
    }
    size = matrix.size();
    try {
        cout << "Enter start and end: ";
        cin >> start >> end;
        --start; --end;
        if (!cin || start >= size || end >= size) throw exception("Wrong start/end format!");
    }
    catch (exception& bad) {
        cout << bad.what()<<endl;
        return -2;
    }
    BellmanFord(matrix, start, end);
    for (unsigned i = 0; i < size; ++i) {
        for(unsigned j = 0; j <= i; ++j){
            if (!isinf(matrix[i][j])) {
                matrix[j][i] = matrix[i][j];
            }
            else if (!isinf(matrix[j][i])) {
                matrix[i][j] = matrix[j][i];
            } else {
                matrix[i][j] = INFINITY;
                matrix[j][i] = INFINITY;
            }
        }
        matrix[i][i] = INFINITY;
    }
    cout << endl << "Graph for Prima:" << endl;
    for (unsigned i = 0; i < size; ++i) {
        for (unsigned j = 0; j < size; ++j) {
            cout << matrix[i][j] << '\t';
        }
        cout << endl;
    }
    cout << endl;
    Prima(matrix);
}

/*
12
0 2 3 23 45 6 2 3 0 3 2 4
0 0 3 23 45 6 2 3 0 3 2 5
0 2 0 23 45 6 2 3 0 3 2 5
0 2 3 0 45 61 2 0 3 2 43 5
0 2 3 53 0 9 2 3 0 2 44 5
0 2 3 3 8 0 2 3 0 3 62 5
0 2 3 3 4 5 0 3 0 3 2 5
0 2 3 23 5 6 2 0 0 34 2 5
0 8 3 12 45 6 2 3 0 3 4 5
0 0 3 21 45 6 2 3 0 0 4 5
0 1 3 25 45 6 2 3 0 0 4 5
7 58 3 3 4 4 3 8 99 9 9 0
*/

/*
14
1 4 3
1 5 2
2 5 2
5 10 3
4 8 2
4 9 7
9 12 1
8 11 4
10 12 2
10 13 1
6 13 5
6 7 3
3 7 2
2 6 4
*/