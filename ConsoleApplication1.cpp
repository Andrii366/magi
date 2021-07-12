#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <time.h>
#include <algorithm>    // std::copy

constexpr auto word_length = 6;
constexpr auto N = 8;
constexpr auto D = 3;

using namespace std;

template <typename T>
void print_arrays(T** arr, int len) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < len; j++) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void main_diagonal_1(int** arr) {
    for (int i = 0; i < N; i++)
        arr[i][i] = -1;
}

template <typename T>
void all_zeros(T** arr) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            arr[i][j] = 0;
        }
    }
}

template <typename T>
void memory_sharing(T** ptrarray, int len) {
    for (int i = 0; i < N; i++) {
        ptrarray[i] = new T[len] ;
    }
}

template <typename T>
void memory_deleting(T** ptrarray) {
    for (int i = 0; i < N; i++) {
        delete[]ptrarray[i];
    }
}

void compute_dist(bool** ptr_input, int** vertices) {
    int Hamming_dist; 
    all_zeros(vertices);
    for (int i = 0; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            Hamming_dist = 0;
            for (int k = 0; k < word_length; k++) {
                if ((ptr_input[i][k] ^ ptr_input[j][k]) == 1)
                    Hamming_dist++;
            }
            if (Hamming_dist >= D) {
                vertices[i][j] = j + 1;
                vertices[j][i] = i + 1;
            }
            else {
                vertices[i][j] = 0;
                vertices[j][i] = 0;
            }
        }
    }
    memory_deleting(ptr_input);
}

void read_file(bool** ptr_input) {
    ifstream file("data.txt");
    int i = 0, n_str = 0;
    char c_buff;
    while (file.get(c_buff)) {
        if (c_buff == '0')
            ptr_input[n_str][i] = 0;
        else if (c_buff == '1')
            ptr_input[n_str][i] = 1;
        else continue;
        if (++i == word_length) {
            i = 0;
            ++n_str;
        }
    }
    file.close();
}

void compute_cliq(int** vertices) {
    int** cliques_array = new int* [N];

    vector<int> vec_main;
    vector<int> vec_buff;

    memory_sharing(cliques_array, N);
    all_zeros(cliques_array);
    //print_arrays(vertices, N);
    int i, j, k;
    bool f = true;

    for (i = 0; i < N; i++) {
        vec_buff.push_back(i);
        for (j = 0; j < N; j++) {
            if (vertices[i][j] > 0) {         
                for (k = 0; k < (signed)vec_buff.size(); k++) {
                    if (vertices[vec_buff[k]][vertices[i][j] - 1]  == 0) { //vec_buff[k] =  numb of string  vertices[i][j] = numb of tomb
                        f = false;
                        break;
                    } 
                }
                if (f == true) {
                    vec_buff.push_back(vertices[i][j] - 1);
                }
            }
        }
        f = true;
        if (vec_buff.size() > vec_main.size()) {
            vec_main.resize(vec_buff.size());
            copy(vec_buff.begin(), vec_buff.end(), vec_main.begin());
            if (vec_main.size() == N)
                break;
        }
        vec_buff.resize(0);
    }
    cout << endl;
    for (i = 0; i < (signed)vec_main.size(); i++) {
        cout << vec_main[i] + 1 << " ";
    }
    memory_deleting(vertices);
    memory_deleting(cliques_array);
}

void write_bits() {
    srand(time(NULL));
    ofstream fout;
    fout.open("data.txt");
    for (int i = 0; i < N; i++) {
        bitset<word_length> b1(rand()%64);//i        2^word_length
        fout << b1 << endl;
        cout << b1 << endl;
    }
    fout.close();
}

void start() {
    int** ptr_vertices = new int* [N];
    bool** ptr_input = new bool* [N];
    memory_sharing(ptr_vertices, N);
    memory_sharing(ptr_input, word_length);

    
    read_file(ptr_input);
    compute_dist(ptr_input, ptr_vertices);
    compute_cliq(ptr_vertices);
}

int main() {
    //write_bits();
    start();
    return 0;
}
