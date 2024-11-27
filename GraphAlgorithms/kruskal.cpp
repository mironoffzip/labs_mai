#include <iostream>

using namespace std;

struct Edge {
    int u, v;   // Вершины ребра
    int weight;
};

void MakeSet(int parent[], int rank[], int V);
int FindSet(int parent[], int x);
void Union(int parent[], int rank[], int x, int y);
void Kruskal(int V, int E, Edge edges[], Edge result[]);

int main() {
    int V = 12;
    int E = 17;
    int weightSum = 0;

    Edge edges[] = {
            {1, 5, 4},
            {5, 7, 9},
            {3, 7, 12},
            {3, 10, 15},
            {10, 8, 7},
            {9, 8, 6},
            {2, 1, 14},
            {9, 1, 10},
            {2, 9, 3},
            {9, 6, 8},
            {6, 3, 2},
            {11, 3, 13},
            {4, 11, 5},
            {12, 11, 8},
            {12, 4, 11},
            {4, 2, 17},
            {12, 2, 19}
    };

    Edge* result = new Edge[V - 1]; // Динамическое выделение памяти

    Kruskal(V, E, edges, result);

    // Количество рёбер в MST равно V - 1
    int edgeCount = V - 1;
    for (int i = 0; i < edgeCount; ++i) {
        cout << result[i].u << "->" << result[i].v << "     Weight: " << result[i].weight  << endl;
        weightSum += result[i].weight;
    }
    cout << endl;
    cout << "Minimum frame weight: " << weightSum << endl;

    delete[] result; // Освобождение памяти
    return 0;
}

void MakeSet(int parent[], int rank[], int V) {
    for (int i = 0; i < V; i++) {
        parent[i] = i; // Каждая вершина является своим родителем
        rank[i] = 0;
    }
}

int FindSet(int parent[], int x) {
    if (parent[x] != x) {
        parent[x] = FindSet(parent, parent[x]);
    }
    return parent[x];
}

void Union(int parent[], int rank[], int x, int y) {
    int rootX = FindSet(parent, x);
    int rootY = FindSet(parent, y);

    if (rootX != rootY) {
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
}

void Kruskal(int V, int E, Edge edges[], Edge result[]) {
    // Массивы для Union-Find
    int parent[V];
    int rank[V];

    // Шаг 1: Инициализация множества вершин
    MakeSet(parent, rank, V);

    // Шаг 2: Сортировка рёбер по возрастанию веса - пузырьковая
    for (int i = 0; i < E - 1; ++i) {
        for (int j = 0; j < E - i - 1; ++j) {
            if (edges[j].weight > edges[j + 1].weight) {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }

    // Шаг 3: Построение остовного дерева
    int edgeCount = 0;                // Количество рёбер в результате
    int vertices[V];        // Хранилище для вершин
    int vertexCount = 0;              // Текущее количество уникальных вершин

    for (int i = 0; i < E; ++i) {
        int u = edges[i].u - 1; // Индексы с 0
        int v = edges[i].v - 1; // Индексы с 0

        // Проверка, принадлежат ли вершины разным компонентам
        if (FindSet(parent, u) != FindSet(parent, v)) {
            result[edgeCount++] = edges[i]; // Добавляем ребро к результату
            Union(parent, rank, u, v);     // Объединяем множество

            // Добавление вершин в массив (если уникальные)
            bool foundU = false, foundV = false;
            for (int j = 0; j < vertexCount; ++j) {
                if (vertices[j] == edges[i].u) foundU = true;
                if (vertices[j] == edges[i].v) foundV = true;
            }
            if (!foundU) vertices[vertexCount++] = edges[i].u;
            if (!foundV) vertices[vertexCount++] = edges[i].v;

            // Вывод текущего состояния вершин и добавленного ребра
            cout << "Edge added: " << edges[i].u << "->" << edges[i].v << "\t";
            cout << "Current vertices in MST: ";
            for (int j = 0; j < vertexCount; ++j) {
                cout << vertices[j] << " ";
            }
            cout << endl << endl;
        }

        // Если найдено достаточно рёбер для MST (V - 1), можно завершить
        if (edgeCount == V - 1) {
            break;
        }
    }
}
