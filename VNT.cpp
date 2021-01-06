//
//  main.cpp
//  YinYu_Lo_p5
//
//  Created by yin yu lo on 12/5/20.
//  Copyright © 2020 yin yu lo. All rights reserved.
//

#include <iostream>
#include <cstdlib>
using namespace std;

template <class T> //forward declaration
class SA;

template <class T> //forward declaration
ostream& operator<<(ostream& os, SA<T> s);

template <class T>
class SM;

template <class T>
ostream& operator<<(ostream& os, SM<T> s);

template <class T>
class SA{
private:
    int low, high;
    T* p;
public:
    // default constructor, allows for writing things like SA a;
    SA(){low = 0; high = -1;p = nullptr;}

    // 2 parameter constructor lets us write SA x(10,20)
    SA(int l, int h){
        if((h - l + 1) <= 0){
            cout << "constructor error in bounds definition" << endl;
            exit(1);
        }
        low = l;
        high = h;
        p = new T[h-l+1];
    }

    // single parameter constructor lets us
    // create a SA almost like a "standard" one by writing SA x(10)
    // and getting an array x indexed from 0 to 9
    SA(int i){
        low = 0;
        high = i - 1;
        p = new T[i];
    }
    
    // copy constructor for pass by value and initialization
    SA(const SA & s){
        int size = s.high - s.low + 1;
        p = new T[size];
        for(int i = 0; i < size; i++)
            p[i] = s.p[i];
        low = s.low;
        high = s.high;
    }
    
    // destructor
    ~SA(){
        delete [] p;
    }
    
    //overloaded [] lets us write SA x(10,20); x[15]= 100;
    T& operator[](int i){
        if(i < low || i > high){
            cout << "index " << i << " out of range" << endl;
            exit(1);
        }
        return p[i-low];
    }
 
    // overloaded assignment lets us assign one SA to another
    SA & operator=(const SA & s){
        if(this == &s) return *this;
        delete [] p;
        int size = s.high - s.low + 1;
        p = new T[size];
        for(int i = 0; i < size; i++)
            p[i] = s.p[i];
        low = s.low;
        high = s.high;
        return *this;
    }

    // overloads << so we can directly print SAs
    friend ostream& operator<< <T>(ostream& os, SA<T> s);
};

template <class T>
ostream& operator<<(ostream& os, SA<T> s){
    int size = s.high - s.low + 1;
    for(int i = 0; i < size; i++)
        os << s.p[i] << " ";
    os << endl;
    return os;
};

template <class T>
class SM{
private:
    int row_low;
    int row_high;
    int col_low;
    int col_high;
    SA <SA <T> > matrix;
public:
    // default constructor, allows for writing things like Matrix a;
    SM(){
        row_low = 0;
        row_high = -1;
        col_low = 0;
        col_high = -1;
        //matrix = nullptr;
    }
    
    // 4 parameter constructor lets us write SA x(10,20,30,40)
    SM(int r_low, int r_high, int c_low, int c_high){
        if((r_high - r_low + 1) <= 0 || (c_high - c_low + 1) <= 0){
            cout << "constructor error in bounds definition" << endl;
            exit(1);
        }
        row_low = r_low;
        row_high = r_high;
        col_low = c_low;
        col_high = c_high;
        matrix = SA <SA <T> >(r_low, r_high);
        for(int i = r_low; i <= r_high; i++){
            matrix[i] = SA<T>(c_low, c_high);
        }
    }
    
    // 2 parameter constructor lets us
    // create a Matrix by writing Matrix x(5,10)
    // and getting an 2D array x's row indexed from 0 to 4
    // and x's column indexed from 0 to 9
    SM(int row, int col){
        if(row <= 0 || col <= 0){
            cout << "constructor error in bounds definition" << endl;
            exit(1);
        }
        row_low = 0;
        row_high = row - 1;
        col_low = 0;
        col_high = col - 1;
        matrix = SA <SA <T> >(row);
        for(int i = 0; i < row; i++){
            matrix[i] = SA<T>(col);
        }
    }
    
    // single parameter constructor lets us
    // create a  same size Matrix by writing Matrix x(10)
    // and getting an 2D array x's row and column indexed from 0 to 9
    SM(int n){
        if(n <= 0){
            cout << "constructor error in bounds definition" << endl;
            exit(1);
        }
        row_low = 0;
        row_high = n - 1;
        col_low = 0;
        col_high = n - 1;
        matrix = SA <SA <T> >(n);
        for(int i = 0; i < n; i++){
            matrix[i] = SA<T>(n);
        }
    }
    
    //overloaded operator()
    T& operator()(int i, int j){
        if(i < row_low || i > row_high){
            cout << "Row " << i << " out of range" << endl;
            exit(1);
        }
        if(j < col_low || j > col_high){
            cout << "Col " << j << " out of range" << endl;
            exit(1);
        }
        return matrix[i][j];
    }
    
    // overloaded [] and cascaded 1d array in 1d array
    // lets us write Matrix x(10,20); x[5][15]= 100;
    SA<T>& operator[](int i){
        if(i < row_low || i > row_high){
            cout << "index " << i << " out of range" << endl;
            exit(1);
        }
        return matrix[i];
    }
    
    // overloaded multiplication lets us multiple one Matrix to another
    // The main condition of matrix multiplication is the number of cols
    // of the 1st matrix must equal to the number of rows of the 2nd one
    SM<T> operator*(SM & s){
        if((col_high - col_low + 1) != (s.row_high - s.row_low + 1)){
            cout << "Matrix multiplication must meet the certain condition, the inner dimensions not match" << endl;
            exit(1);
        }
        int result_row = row_high - row_low + 1;
        int result_col = s.col_high - s.col_low + 1;
        int inner = col_high - col_low + 1;
        
        SM <int> result(result_row, result_col);
        for(int r = 0; r < result_row; r++){
            for(int c = 0; c < result_col; c++){
                result[r][c] = 0;
            }
        }
        
        for(int r = 0; r < result_row; r++){
            for(int c = 0; c < result_col; c++){
                for(int i = 0; i < inner; i++){
                    result[r][c] += ((*this)[r + row_low][i + col_low]) * (s[i + s.row_low][c + s.col_low]);
                }
            }
        }
        return result;
        
    }
    
    // overloads << so we can directly print Matrices
    friend ostream& operator<< <T>(ostream& os, SM<T> s);
    
};

template <class T>
ostream& operator<<(ostream& os, SM<T> s){
    for(int i = s.row_low; i <= s.row_high; i++){
        for(int j = s.col_low; j <= s.col_high; j++){
            os << s.matrix[i][j] << " ";
        }
        os << endl;
    }
    return os;
}

class VNT{
private:
    int m, n;
    SM<int> matrix;
public:
    VNT(int row, int col){
        m = row;
        n = col;
        SM<int> sm(row, col);
        for(int i = 0; i < row; i++){
            for(int j = 0; j < col; j++){
                sm[i][j] = INT_MAX;
            }
        }
        matrix = sm;
    }
    bool isEmpty(){
        return matrix[0][0] == INT_MAX;
    }
    
    bool isFull(){
        return matrix[m - 1][n - 1] != INT_MAX;
    }
    
    void add(int value){
        if(isFull())
            throw (100);
        int cur_row = m - 1;
        int cur_col = n - 1;
        matrix[cur_row][cur_col] = value;
        while(!(cur_row == 0 && cur_col == 0)){
            //compare left and up
            int left_value = INT_MAX;
            int up_value = INT_MAX;
            if(cur_row > 0){
                up_value = matrix[cur_row - 1][cur_col];
            }
            if(cur_col > 0){
                left_value = matrix[cur_row][cur_col - 1];
            }
            if(left_value > up_value){
                //swap to left
                if(left_value > value && cur_col > 0){
                    matrix[cur_row][cur_col] = left_value;
                    matrix[cur_row][cur_col - 1] = value;
                    cur_col--;
                    continue;
                }
                else if(up_value > value && cur_row > 0){
                    //swap to up
                    matrix[cur_row][cur_col] = up_value;
                    matrix[cur_row - 1][cur_col] = value;
                    cur_row--;
                    continue;
                }
            }
            else{
                if (up_value > value&& cur_row > 0) {
                    // swap to up
                    matrix[cur_row][cur_col] = up_value;
                    matrix[cur_row - 1][cur_col] = value;
                    cur_row--;
                    continue;
                }
                else if (left_value > value&& cur_col > 0) {
                    // swap to left
                    matrix[cur_row][cur_col] = left_value;
                    matrix[cur_row][cur_col - 1] = value;
                    cur_col--;
                    continue;
                }
            }
            break;
        }
    }
            
    int getMin(){
        if (isEmpty())
            throw (200); // VNT is empty
        int min = matrix[0][0];
        int cur_row = 0;
        int cur_col = 0;
        // No more element in VNT
        if (matrix[0][1] == INT_MAX && matrix[1][0] == INT_MAX) {
            matrix[0][0] = INT_MAX;
            return min;
        }
        while (!(cur_row == (m - 1) && cur_col == (n - 1))) {
            // compare right and down elements
            int right_value = INT_MAX;
            int down_value = INT_MAX;
            if (cur_row < m - 1) {
                down_value = matrix[cur_row + 1][cur_col];
            }
            if (cur_col < n - 1) {
                right_value = matrix[cur_row][cur_col + 1];
            }
            if (right_value == INT_MAX && down_value == INT_MAX) {
                matrix[cur_row][cur_col] = INT_MAX;
                break;
            }
            if (right_value > down_value) {
                if (cur_row < m - 1) {
                    // swap to down
                    matrix[cur_row][cur_col] = down_value;
                    matrix[cur_row + 1][cur_col] = INT_MAX;
                    cur_row++;
                    continue;
                }
                else if (cur_col < n - 1) {
                    // swap to right
                    matrix[cur_row][cur_col] = right_value;
                    matrix[cur_row][cur_col + 1] = INT_MAX;
                    cur_col++;
                    continue;
                }
            }
            else {
                if (cur_col < n - 1) {
                    // swap to right
                    matrix[cur_row][cur_col] = right_value;
                    matrix[cur_row][cur_col + 1] = INT_MAX;
                    cur_col++;
                    continue;
                }
                else if (cur_row < m - 1) {
                    // swap to down
                    matrix[cur_row][cur_col] = down_value;
                    matrix[cur_row + 1][cur_col] = INT_MAX;
                    cur_row++;
                    continue;
                }
            }
        }
            return min;
    }
            
    
            
    // binary search
    bool BSearch(int value, int rs, int re, int cs, int ce) {
        if (rs > re || cs > ce)
            return false;
        if (rs == re && cs == ce)
            return matrix[rs][cs] == value;
        int rm = (rs + re) / 2;
        int cm = (cs + ce) / 2;
        if (matrix[rm][cm] == value)
            return true;
        if (value > matrix[rm][cm]) {
            return BSearch(value, rm + 1, re, cs, cm) || BSearch(value, rs, re, cm + 1, ce);
        }
        else {
            return BSearch(value, rs, rm - 1, cs, ce) || BSearch(value, rm, re, cs, cm - 1);
        }
    }
            
    bool find(int value) {
        if (isEmpty())
            return false;
        return BSearch(value, 0, m - 1, 0, n - 1);
    }
            
    friend ostream& operator<< (ostream& os, VNT v);
};
    
ostream& operator<< (ostream& os, VNT v) {
    os << v.matrix;
    return os;
};
            
void sort(VNT &ar, int a[], int size) {
    for (int i = 0; i < size; i++) {
        ar.add(a[i]);
    }
    for (int i = 0; i < size; i++) {
        a[i] = ar.getMin();
    }
}
            
int main() {
    VNT a(5, 6);
    cout << "Using add function:" << endl;
    a.add(25);
    cout << a << endl;
    a.add(2);
    a.add(11);
    a.add(35);
    a.add(9);
    a.add(6);
    a.add(8);
    a.add(19);
    a.add(16);
    a.add(21);
    cout << "Using find function searching 23 in the VNT:" << endl;
    if (a.find(23))
        cout << "23 is in the VNT" << endl << endl;
    else
        cout << "Cannot find 23 in the VNT" << endl << endl;
    a.add(31);
    a.add(42);
    a.add(3);
    a.add(9);
    a.add(51);
    a.add(41);
    a.add(23);
    a.add(16);
    a.add(13);
    a.add(27);
    a.add(5);
    a.add(13);
    a.add(27);
    a.add(71);
    a.add(33);
    a.add(99);
    a.add(46);
    a.add(25);
    a.add(19);
    cout << a << endl;
    a.add(16);
    cout << a << endl;
    
    cout << "Using getMin function:" << endl;
    cout << "The minimum number is : " << a.getMin() << endl;
    cout << a << endl;
    cout << "Using add function:" << endl;
    a.add(78);
    cout << a << endl;
            
    cout << "Using find function again to find 23 in the VNT:" << endl;
    if (a.find(23))
        cout << "23 is in the VNT" << endl << endl;
    else
        cout << "Cannot find 23 in the VNT" << endl << endl;
    
    int firstSearched = 100;
    int secondSearched = 0;
    int thirdSearched = 31;
    cout<< firstSearched << " is in VNT is: " << a.find(firstSearched) << endl;
    cout<< secondSearched << " is in VNT is: " << a.find(secondSearched) << endl;
    cout<< thirdSearched << " is in VNT is: " << a.find(thirdSearched) << endl;
    cout << "0 means false, 1 means true." << endl << endl;
            
    cout << "Using getMin function:" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "The NO." << (i + 1) << " minimum is : " << a.getMin() << endl;
        cout << a << endl;
    }
     
    cout << "Using sort function:" << endl;
    VNT b(3, 3);
    int array[9] = {11, 9, 2, 31, 4, 15, 99, 41, 10};
    cout << "Sort the array {11, 9, 2, 31, 4, 15, 99, 41, 10}:" << endl;
    sort(b, array, 9);
    for (int i = 0; i < 9; i++) {
        cout << array[i] << " ";
    }
    cout << endl << endl;
            
    /*
     Now create a 1d int array with 50 integers in neither ascending or
     descending order as explained earlier and print out the unsorted version
     */
    int arraySize = 50;
    int* k = new int[arraySize];
    cout << "The array k initially is:" << endl;
    for(int i = 0; i < arraySize; i++){
        int addedNumber = (i*31)%50;
        k[i] = addedNumber;
        cout << addedNumber << " ";
    }
    cout<<endl<<endl;
    VNT c(7, 8);
    sort(c, k, arraySize);
    
    //now reprint the now sorted array k
    cout<<"The array k, now sorted by VNT, is:"<<endl;
    for(int i = 0; i < arraySize; i++){
        cout << k[i] << " ";
    }
    cout << endl;
    
            
    return 0;
}
