#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cmath>
using namespace std;

const int SIZE = 200000;
int target[SIZE];
int temp[SIZE];

void target_initialize() {
    for (int i = 0; i < SIZE; i++) {
        target[i] = i + 1;
    }
}

void target_reverse_initialize(){
    for(int i=0;i<SIZE;i++){
        target[i]=SIZE-i;
    }
}

void randomize(int seed) {
    for (int i = 0; i < seed; i++) {
        random_shuffle(begin(target), end(target));
    }
}

bool correct() {
    for (int i = 0; i < SIZE - 1; i++) {
        if (target[i] > target[i + 1]) return false;
    }
    return true;
}

void swap(int arr[], int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void insertion_sort() {
    for (int i = 1; i < SIZE; i++) {
        for (int j = i - 1; j >= 0; j--) {
            if (target[j + 1] >= target[j]) {
                break;
            }
            swap(target, j, j + 1);
        }
    }
}

void merge(int l, int m, int r) {
    for (int i = l; i <= r; i++) {
        temp[i] = target[i];
    }
    int i = l;
    int j = m + 1;
    int k = l;
    while (k <= r) {
        if (i > m || j > r) break;
        if (temp[i] < temp[j]) target[k] = temp[i++];
        else target[k] = temp[j++];
        k++;
    }
    while (i < m + 1) {  // if i has not reached to the final point
        target[k++] = temp[i++];
    }
    while (j < m + 1) {  // if j has not reached to the final point
        target[k++] = temp[j++];
    }
}

int partition(int l, int r) {
    if (l == r) return l;
    int m = (l + r) / 2;
    partition(l, m);
    partition(m + 1, r);
    merge(l, m, r);
    return m;
}

void merge_sort() {
    partition(0, SIZE - 1);
}

void separate_pivot_last(int l, int r) {
    if (l >= r) return;
    int pivot = target[r];
    int i = l - 1;
    int j = l;
    for (j = l; j < r; j++) {
        if (target[j] > pivot) continue;
        i++;
        swap(target, i, j);
    }
    i++;
    swap(target, i, r);
    separate_pivot_last(l, i - 1);
    separate_pivot_last(i + 1, r);
}

void quick_sort_last() {
    separate_pivot_last(0, SIZE - 1);
}

void insertion_sort_for_med(int l, int r) {
    for (int i=l+1; i<=r; i+=1) {
        for (int j=i-1; j>=l; j--) {
            if (target[j+1] >= target[j]) {
                break;
            }
            swap(target, j, j+1);
        }
    }
}

int medOfMed(int temp_arr_size){
    if(temp_arr_size==1) return temp[0];
    int i = 0;
    int j = 0;
    int new_arr_size = 0;
    while(i<temp_arr_size){
        if(i+4 < temp_arr_size){
            insertion_sort_for_med(i, i+4);
            temp[j] = temp[i+2];
        }
        else{
            insertion_sort_for_med(i, temp_arr_size-1);
            temp[j] = temp[(i+temp_arr_size-1)/2];
        }
        j++;
        i+=5;
        new_arr_size++;
    }
    if(new_arr_size == 1) return temp[0];
    else return medOfMed(new_arr_size);
}

void separate_pivot_med(int l, int r) {
    if (l >= r) return;
    int temp_j = 0;
    for(int i=l;i<=r;i++){
        temp[temp_j]=target[i];
        temp_j++;
    }
    int size = r-l+1;
    int pivot = medOfMed(size);
    int j = l;
    int i = l;
    for (; i<r; i++) {
        if (target[i] < pivot) {
            swap(target, i, j);
            j++;
        }
        else if (target[i] == pivot) {
            swap(target, i, r);
            i--;
        }
    }
    swap(target, j, r);
    separate_pivot_med(l, j - 1);
    separate_pivot_med(j + 1, r);
}

void quick_sort_med() {
    separate_pivot_med(0, SIZE - 1);
}

int max_among_three(int i, int j, int k) {
    if (target[i] >= target[j] && target[i] >= target[k]) return i;
    else if (target[j] >= target[i] && target[j] >= target[k]) return j;
    else if (target[k] >= target[i] && target[k] >= target[j]) return k;
    else return -1;
}

// Heapify when the root node is the only node with potential problem
void max_heapify_adaptive(int root, int heap_size) {
    int max = root;
    while (2 * root + 1 < heap_size) {  // condition to have child node
        if (2 * root + 2 < heap_size) {  // have r-child
            max = max_among_three(root, 2 * root + 1, 2 * root + 2);
            if (max == root) return;
            else if (max == 2 * root + 1) {
                swap(target, root, 2 * root + 1);
                root = 2 * root + 1;
            }
            else if (max == 2 * root + 2) {
                swap(target, root, 2 * root + 2);
                root = 2 * root + 2;
            }
            else {
                cout << "ERROR CASE" << endl;
                return;
            }
        }
        else {  // do not have r-child
            if (target[root] >= target[2 * root + 1]) max = root;
            else if (target[2 * root + 1] >= target[root]) max = 2 * root + 1;
            else {
                cout << "ERROR CASE" << endl;
                return;
            }
            if (max == root) break;
            else if (max == 2 * root + 1) {
                swap(target, root, 2 * root + 1);
                root = 2 * root + 1;
            }
            else {
                cout << "ERROR CASE" << endl;
                return;
            }
        }
    }
}

void max_heapify() {
    for (int i = SIZE / 2 - 1; i >= 0; i--) {
        max_heapify_adaptive(i, SIZE);
    }
}

void heap_sort() {
    max_heapify();
    for (int i = 0; i < SIZE - 1; i++) {
        swap(target, 0, SIZE - 1 - i);
        max_heapify_adaptive(0, SIZE - i - 1);
    }
}

void stooge(int l, int r) {
    if (l >= r) return;
    else if (r == l + 1) {
        if (target[l] > target[r]) swap(target, l, r);
        return;
    }
    int size = r - l + 1;
    int new_size = (2 * size + 2) / 3;
    if (target[l] > target[r]) swap(target, l, r);
    stooge(l, l + new_size - 1);
    stooge(r - new_size + 1, r);
    stooge(l, l + new_size - 1);
}

void stooge_sort() {
    stooge(0, SIZE - 1);
}

void test_print() { // For debugging
    cout << target[0] << " " << target[1] << " " << target[2] << " " << target[3] << " " << target[4] << endl;
}

void arr_swap(int arr[], int l, int r) {
    int temp = arr[l];
    arr[l] = arr[r];
    arr[r] = temp;
}

int main(int argc, const char * argv[]) {
    int seed = 1;
    string s = "";
    time_t start, end, duration;
    cout << "Number of Data : " << SIZE << endl;
    for (seed = 5; seed <= 5; seed++) {
        cout << "=============================================================" << endl;
        cout << "*******************  Test with seed #" << seed << "!!  *******************" << endl;
        cout << "=============================================================" << endl;
        /*
        for (int j = 2; j <= 4; j++) {
            target_initialize();
            randomize(seed);
            switch (j) {
                case 1:
                    s = "Insertion sort";
                    start = time(0);
                    insertion_sort();
                    end = time(0);
                    if (correct()) cout << "Sorting time of " << s << " is \t\t\t\t\t";
                    break;
                case 2:
                    s = "Merge sort";
                    start = time(0);
                    merge_sort();
                    end = time(0);
                    if (correct()) cout << "Sorting time of " << s << " is \t\t\t\t\t\t";
                    break;
                case 3:
                    s = "Quick sort with random pivot";
                    start = time(0);
                    quick_sort_last();
                    end = time(0);
                    if (correct()) cout << "Sorting time of " << s << " is \t";
                    break;
                case 4:
                    s = "Quick sort with median pivot";
                    start = time(0);
                    quick_sort_med();
                    end = time(0);
                    if (correct()) cout << "Sorting time of " << s << " is \t";
                    break;
                case 5:
                    s = "Heap sort";
                    start = time(0);
                    heap_sort();
                    end = time(0);
                    if (correct()) cout << "Sorting time of " << s << " is \t\t\t\t\t\t";
                    break;
                case 6:
                    s = "Stooge sort a.k.a shim sort";
                    start = time(0);
                    //stooge_sort();
                    end = time(0);
                    if (correct()) cout << "Sorting time of " << s << " is \t\t";
                    break;
                    
                default:
                    start = time(0);
                    end = time(0);
                    break;
            }
            duration = end - start;
            cout << duration << " seconds" << endl;
        }
         */
        target_initialize();
        start=time(0);
        quick_sort_last();
        end=time(0);
        duration=end-start;
        if(correct()) cout<<"Sorting time with worst case is \t\t";
        cout << duration << "seconds" << endl;
        cout << "=============================================================" << endl;
        cout << "***************  Test with seed #" << seed << " has ended  ***************" << endl;
        cout << "=============================================================" << endl << endl << endl;
    }
    
    return 0;
}
