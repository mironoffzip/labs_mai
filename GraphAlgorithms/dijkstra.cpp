#include <iostream>
#include <limits> // Подключение для использования INT_MAX (максимального значения целого числа)

using namespace std;

// Структура графа
struct Graph{
    int V; // Количество вершин
    int E; // Количество рёбер
    int** Adj; // Матрица смежности для представления графа
};

// Структура узла приоритетной очереди
struct Node {
    int vertex; // Вершина
    int distance; // Расстояние (приоритет)
    Node* next; // Указатель на следующий узел
};

// Структура приоритетной очереди
struct PriorityQueue{
    Node* head; // Указатель на голову очереди
};

// Создание графа с использованием матрицы смежности
Graph* adjMatrixOfGraph(){
    int i, u, v, w;
    Graph* G = new Graph;
    if(!G){
        cout << "Memory Error." << endl;
        return nullptr; // Проверка на выделение памяти
    }

    cout << "Enter the number of vertices: ";
    cin >> G->V;
    cout << "Enter the number of edges: ";
    cin >> G->E;

    // Создание и инициализация матрицы смежности
    G->Adj = new int*[G->V];
    for(u = 0; u < G->V; u++){
        G->Adj[u] = new int[G->V];
        for(v = 0; v < G->V; v++){
            G->Adj[u][v] = 0; // Инициализация всех значений нулями
        }
    }

    // Ввод рёбер графа
    cout << "Enter the edges (format: u v weight):\n";
    for(i = 0; i < G->E; i++){
        cout << "Edge " << i + 1 << ": ";
        cin >> u >> v >> w;
        u--; v--; // Приведение вершин к нумерации с нуля
        G->Adj[u][v] = w; // Задание веса ребра
    }

    return G;
}

// Вывод матрицы смежности графа
void printG(Graph* G){
    cout << "Adjacency Matrix of the Graph:\n";
    for(int u = 0; u < G->V; ++u){
        for(int v = 0; v < G->V; ++v){
            cout << G->Adj[u][v] << "\t";
        }
        cout << endl;
    }
}

// Освобождение памяти графа
void freeG(Graph* G){
    for(int i = 0; i < G->V; i++){
        delete[] G->Adj[i];
    }
    delete[] G->Adj;
    delete G;
}

// Инициализация пустой очереди
void initQueue(PriorityQueue& pq){
    pq.head = nullptr;
}

// Проверка на пустоту очереди
bool isEmptyQ(PriorityQueue& pq){
    return pq.head == nullptr;
}

// Вставка элемента в очередь с учетом приоритета
void push(PriorityQueue& pq, int vertex, int distance){
    Node* newNode = new Node;
    newNode->vertex = vertex;
    newNode->distance = distance;
    newNode->next = nullptr;

    // Вставка элемента с упорядочением по расстоянию
    if(isEmptyQ(pq) || pq.head->distance > distance){
        newNode->next = pq.head;
        pq.head = newNode;
    } else {
        Node* current = pq.head;
        while(current->next && current->next->distance <= distance){
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

// Удаление элемента с наименьшим приоритетом из очереди
void pop(PriorityQueue& pq){
    if(isEmptyQ(pq)) return;
    Node* temp = pq.head;
    pq.head = pq.head->next;
    delete temp;
}

// Получение элемента с наименьшим приоритетом
Node* top(PriorityQueue& pq){
    return pq.head;
}

// Освобождение памяти, выделенной под очередь
void freeQ(PriorityQueue& pq){
    while(!isEmptyQ(pq)){
        pop(pq);
    }
}

// Функция для отображения текущего состояния массивов расстояний и предшественников
void printIntermediateResults(int dist[], int predecessor[], int V){
    cout << "Vertex\tDistance\tPredecessor" << endl;
    for(int i = 0; i < V; i++){
        cout << i + 1 << "\t" << (dist[i] == INT_MAX ? -1 : dist[i]) << "\t\t";
        if(predecessor[i] != -1){
            cout << predecessor[i] + 1;
        } else{
            cout << "None";
        }
        cout << endl;
    }
    cout << endl;
}

// Вывод пути от исходной вершинны до данной
void printPath(int predecessor[], int vertex){
    if(predecessor[vertex] == -1){
        cout << vertex + 1;
        return;
    }
    printPath(predecessor, predecessor[vertex]);
    cout << " -> " << vertex + 1;
}

// Реализация алгоритма Дейкстры для поиска кратчайших путей
void dijkstra(Graph* G, int source){
    int V = G->V;
    int dist[V]; // Массив для хранения кратчайших расстояний
    int predecessor[V]; // Массив для хранения предшествующих вершин
    bool visited[V]; // Массив для отметки посещённых вершин

    // Инициализация массивов
    for(int i = 0; i < V; i++){
        dist[i] = INT_MAX; // Все расстояния изначально бесконечны
        predecessor[i] = -1; // Все предшественники изначально отсутствуют
        visited[i] = false; // Все вершины изначально не посещены
    }
    dist[source] = 0;

    PriorityQueue pq;
    initQueue(pq);
    push(pq, source, 0);

    while(!isEmptyQ(pq)){
        Node* minNode = top(pq); // Извлечение вершины с минимальным расстоянием
        int u = minNode->vertex;
        pop(pq);

        if(visited[u]) continue; // Пропуск уже посещенных вершин
        visited[u] = true;

        // Обновление расстояний до соседних вершин
        for(int v = 0; v < V; v++){
            if(G->Adj[u][v] != 0){
                int weight = G->Adj[u][v];
                if(!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]){
                    dist[v] = dist[u] + weight;
                    predecessor[v] = u; // Установка предшественника
                    push(pq, v, dist[v]); // Добавление вершины в очередь
                }
            }
        }

        // Печать промежуточных результатов
        printIntermediateResults(dist, predecessor, V);
    }

    // Вывод кратчайших путей и расстояний
    cout << "Vertex\tDistance from source " << source + 1 << "\tPath" << endl;
    for(int i = 0; i < V; i++){
        cout << i + 1 << "\t" << (dist[i] == INT_MAX ? -1 : dist[i]) << "\t\t";

        // Вывод пути
        if(dist[i] == INT_MAX){
            cout << "No path";
        } else{
            printPath(predecessor, i);
        }
        cout << endl;
    }

    freeQ(pq); // Освобождение памяти очереди
}

int main(){
    Graph* G = adjMatrixOfGraph(); // Создание графа
    if(G){
        printG(G); // Вывод графа
        int source;
        cout << "Enter the source vertex: ";
        cin >> source;
        source--; // Приведение к нумерации с нуля
        dijkstra(G, source); // Запуск алгоритма Дейкстры
        freeG(G); // Освобождение памяти графа
    }

    return 0;
}
