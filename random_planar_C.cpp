#include <iostream>
#include <set>
#include <vector>
using namespace std;

typedef int node;

struct node_linked_list {
    node v;
    node_linked_list *next;
};

struct edge {
    node v1;
    node v2;
};

struct graf {
    vector<node> V;
    vector<edge> E;
};

static int id = 1;
node buatSimpul()
{
    return id++;
}

int length(node_linked_list *A)
{
    int lengthA = 0;
    node_linked_list *Ai = A;
    do {
        lengthA++;
        Ai = Ai->next;
    } while (Ai != A);
    return lengthA;
}

node_linked_list *getRandomNode(node_linked_list *A)
{
    node_linked_list *Ai; // Untuk iterasi

    // Dapatkan simpul acak
    int r = ((rand() << 8) + rand()) % length(A);
    Ai = A;
    while (r--) {
        Ai = Ai->next;
    }

    return Ai;
}

void tambahSimpul(vector<node> &V, vector<edge> &E, node_linked_list **A)
{
    node p = buatSimpul();
    V.push_back(p);

    node_linked_list *Ai = getRandomNode(*A);

    // Tambah simpul baru dan tambahkan dua sisi baru
    node_linked_list *link_p = (node_linked_list *)malloc(sizeof(node_linked_list));
    link_p->v = p;
    link_p->next = Ai->next;
    Ai->next = link_p;
    E.push_back({Ai->v, Ai->next->v});
    if (V.size() > 2) E.push_back({link_p->v, link_p->next->v});

    *A = Ai;
}

void tambahSisi(vector<edge> &E, node_linked_list **A, set<pair<node, node>> &SE)
{
    node_linked_list *Ai = getRandomNode(*A);

    // Tambah sisi baru dengan menyambungkan A_i dengan A_i+2
    if (SE.count({Ai->v, Ai->next->next->v}) || SE.count({Ai->next->next->v, Ai->v})) return;
    
    E.push_back({Ai->v, Ai->next->next->v});
    SE.insert({Ai->v, Ai->next->next->v});
    Ai->next = Ai->next->next;

    *A = Ai;
}

set<pair<node, node>> getSpanningTree(vector<edge> E, int n) {
    set<pair<node, node>> T;
    static vector<node> adjList[100001];
    for (int i = 1; i <= n; i++) {
        adjList[i].clear();
    }
    
    for (edge e : E) {
        adjList[e.v1].push_back(e.v2);
        adjList[e.v2].push_back(e.v1);
    }
    bool visited[n+1];
    node nextNode[n+1];
    for (int i = 1; i <= n; i++) {
        visited[i] = false;
        nextNode[i] = 1;
    }
    visited[1] = true;
    for (int i = 1; i <= n; i++) {
        if (visited[i]) continue;
        node u = i;
        while (!visited[u]) {
            nextNode[u] = adjList[u][((rand() << 8) + rand()) % adjList[u].size()];
            u = nextNode[u];
        }
        u = i;
        while (!visited[u]) {
            visited[u] = true;
            T.insert({u, nextNode[u]});
            u = nextNode[u];
        }
    }
    return T;
}

static double panjangA = 0;
graf buatGrafPlanar(int n)
{
    id = 1;
    node v1 = buatSimpul();
    node v2 = buatSimpul();
    vector<node> V = {v1, v2};
    vector<edge> E = {{v1, v2}};
    set<pair<node, node>> SE = {{v1, v2}};

    node_linked_list *A = (node_linked_list *)malloc(sizeof(node_linked_list));
    node_linked_list *A2 = (node_linked_list *)malloc(sizeof(node_linked_list));

    *A = {v1, A2};
    *A2 = {v2, A};

    while (V.size() < n || E.size() < 3 * n - 6) {
        if (V.size() < n && (rand() % 2 || length(A) <= 2)) {
            tambahSimpul(V, E, &A);
        }
        else {
            tambahSisi(E, &A, SE);
        }
    }

    set<pair<node, node>> T = getSpanningTree(E, n);

    vector<edge> R;

    for (edge e : E) {
        if (T.count({e.v1, e.v2}) || T.count({e.v2, e.v1}) || rand() % 2) {
            R.push_back(e);
        }
    }

    return {V, R};
}

graf buatGrafPlanar(int n, int m)
{
    id = 1;
    node v1 = buatSimpul();
    node v2 = buatSimpul();
    vector<node> V = {v1, v2};
    vector<edge> E = {{v1, v2}};
    set<pair<node, node>> SE = {{v1, v2}};

    node_linked_list *A = (node_linked_list *)malloc(sizeof(node_linked_list));
    node_linked_list *A2 = (node_linked_list *)malloc(sizeof(node_linked_list));

    *A = {v1, A2};
    *A2 = {v2, A};

    while (V.size() < n || E.size() < 3 * n - 6) {
        cout << V.size() << " : " << E.size() << endl;
        if (V.size() < n && (rand() % 2 || length(A) <= 2)) {
            tambahSimpul(V, E, &A);
        }
        else {
            tambahSisi(E, &A, SE);
        }
    }

    set<pair<node, node>> T = getSpanningTree(E, n);
    vector<edge> R;

    m -= n - 1; 
    for (edge e : E) {
        if (T.count({e.v1, e.v2}) || T.count({e.v2, e.v1})) {
            R.push_back(e);
        }
        else if (rand() % 2 && m > 0) {
            R.push_back(e);
            m--;
        }
    }

    return {V, R};
}

int main()
{
    srand(time(NULL));
    int simpul = 10;
    int banyakCoba = 10;
    for (int i = 1; i <= banyakCoba; i++) {
        cout << "GRAF KE-" << i << endl;
        clock_t t1, t2;
        t1 = clock();
        graf G = buatGrafPlanar(simpul);
        t2 = clock();
        cout << "EXECUTION TIME: " << t2 - t1 << "ms" << endl;

        cout << "NODES: " << endl;
        for (auto v : G.V) {
            cout << v << " ";
        }
        cout << endl;

        cout << "EDGES: " << endl;
        for (auto e : G.E) {
            cout << "" << e.v1 << " " << e.v2 << endl;
        }
        cout << endl;
    }

    return 0;
}