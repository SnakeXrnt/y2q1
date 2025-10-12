#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include "utils.h"

int main() {
    // Process many cases until EOF
    while (true) {
        std::vector<std::pair<char,char>> edges;
        char s, t;

        // Try to read one case: [edges] s t
        if (!(std::cin >> edges)) break;      // no more input
        if (!(std::cin >> s >> t)) break;

        // Discard anything else on the line, e.g. "===> 4"
        std::string junk;
        std::getline(std::cin, junk);

        // Build undirected adjacency list
        std::unordered_map<char, std::vector<char>> adj;
        for (auto [u, v] : edges) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // Trivial cases
        if (s == t) { std::cout << 0 << "\n"; continue; }
        if (!adj.count(s) || !adj.count(t)) { std::cout << -1 << "\n"; continue; }

        // BFS
        std::unordered_map<char,int> dist;
        std::queue<char> q;
        dist[s] = 0; q.push(s);

        int answer = -1;
        while (!q.empty()) {
            char u = q.front(); q.pop();
            for (char v : adj[u]) {
                if (!dist.count(v)) {
                    dist[v] = dist[u] + 1;
                    if (v == t) { answer = dist[v]; goto done; }
                    q.push(v);
                }
            }
        }
    done:
        std::cout << answer << "\n";
    }

    return 0;
}
