#include <bits/stdc++.h>
using namespace std;

const int MAXN = 34011;
const double SOLUTION = 37000;
const int NPARENT = 10;
const int NCHILD = 2;
const int MUTATION = 0;
const int MUTATION_RATE = 0;
const char* FILENAME = "output_6.txt";

struct Point {
    double x, y;

    Point() {}
    Point(double x, double y) : x(x), y(y) {}

    Point operator - (Point a) { return Point(x-a.x, y-a.y); }

    double len() { return sqrt(x*x + y*y); }
} a[MAXN];

int n;

struct Result {
    double len;
    int id[MAXN];

    void calculate() {
        len = 0;
        for(int i = 1; i <= n; ++i) {
            int u = id[i], v = (i == n) ? id[1] : id[i+1];
            len += (a[u] - a[v]).len();
        }
    }
} current, optimal, child[NPARENT * NCHILD + 11], parent[NPARENT + 11];

bool operator < (const Result &a, const Result &b) {
    return a.len < b.len;
}

void ans() {
    cout << (fixed) << setprecision(3) << optimal.len << " 0" << endl;
    for(int i = 1; i <= n; ++i)
        cout << optimal.id[i] - 1 << ' ';
    cout << endl;
}

void save() {
    fstream fout; fout.open(FILENAME, fstream :: out);
    fout << (fixed) << setprecision(3) << optimal.len << " 0\n";
    for(int i = 1; i <= n; ++i) {
        fout << optimal.id[i] - 1 << ' ';
    }
    fout << endl;
}

bool optimalUpdated;

void update(Result &optimal, const Result &current) {
    if (current.len >= optimal.len) return ;

    cerr << "\nUpdated to: " << current.len << endl;
    optimal.len = current.len;
    for(int i = 1; i <= n; ++i)
        optimal.id[i] = current.id[i];
    save();
    optimalUpdated = true;
}

void load() {
    fstream fin; fin.open(FILENAME, fstream :: in);
    int tmp;
    fin >> current.len >> tmp;
    for(int i = 1; i <= n; ++i) {
        fin >> current.id[i];
        ++current.id[i];
    }
    update(optimal, current);
}

void solve() {
    for(int i = 1; i <= n; ++i) {
        current.id[i] = i;
    }
    for(int i = 1; i <= n; ++i) {
        swap(current.id[i], current.id[rand() % i + 1]);
    }
    current.calculate();
    update(optimal, current);
}

void optimize() {
    int nParent = 1;
    for(int i = 1; i <= n; ++i)
        parent[1].id[i] = optimal.id[i];
    parent[1].calculate();

    int badTurn = 0;
    for(int turn = 0; turn < 1000111000; ++turn) {
        cerr << '.';
        ++badTurn;
        optimalUpdated = false;

        // cerr << "Start gen child\n";
        int nChild = 0;
        int mid = n / 2;
        for(int parentId = 1; parentId <= nParent; ++parentId) {
            for(int born = 1; born <= NCHILD; ++born) {
                ++nChild;
                // cerr << "Turn = " << turn << " " << parentId << " " << born << " " << nChild << endl;
                for(int i = 1; i <= n; ++i)
                    child[nChild].id[i] = parent[parentId].id[i];
                if (born > 1) {
                    int dice = rand() % 4;
                    if (dice == 0) {
                        int len = rand() % (mid / 2);
                        int from1 = rand() % (mid-len) + 1;
                        int to1 = from1 + len - 1;

                        int from2 = rand() % (n - mid - len) + mid + 1;
                        int to2 = from2 + len - 1;

                        for(int x = from1, y = to2; x <= to1; ++x, --y)
                            swap(child[nChild].id[x], child[nChild].id[y]);
                    }
                    else if (dice == 1) {
                        int len = rand() % 15;
                        int from1 = rand() % (mid-len) + 1;
                        int to1 = from1 + len;
                        reverse(child[nChild].id + from1, child[nChild].id + to1);
                    }
                    else if (dice == 2) {
                        int x = rand() % n + 1;
                        int y = rand() % n + 1;
                        swap(child[nChild].id[x], child[nChild].id[y]);
                    }
                    else {
                        for(int i = 1; i <= n; ++i) {
                            if (rand() % 100 <= MUTATION_RATE) {
                                swap(child[nChild].id[i], child[nChild].id[rand() %i + 1]);
                            }
                        }
                    }
                }
                child[nChild].calculate();
            }
        }
        // cerr << "Done gen child\n";
        sort(child+1, child+nChild+1);
        nParent = min(NPARENT, nChild);
        for(int i = 1; i <= nParent; ++i) {
            parent[i].len = child[i].len;
            for(int x = 1; x <= n; ++x)
                parent[i].id[x] = child[i].id[x];
        }
        for(int i = nParent; i > nParent - MUTATION && i > 0; --i) {
            for(int x = 1; x <= n; ++x)
                if (rand() % 100 <= MUTATION_RATE) {
                    swap(parent[i].id[x], parent[i].id[rand() % x + 1]);
                }
            parent[i].calculate();
        }
        // cerr << "Done creating parent\n";

        update(optimal, parent[1]);

        if (optimalUpdated) {
            badTurn = 0;
        }
    }
}

int main(int argc, char** argv) {
    cerr << (fixed) << setprecision(3);

    fstream fin; fin.open(argv[1], fstream :: in);
    fin >> n;
    for(int i = 1; i <= n; ++i) {
        fin >> a[i].x >> a[i].y;
    }
    for(int i = 1; i <= n; ++i) {
        optimal.id[i] = i;
    }
    optimal.calculate();
    load();
    // solve();
    optimize();
    ans();
}
