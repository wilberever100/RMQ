// StaticTree.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;



struct NodeCartesian {

    NodeCartesian(int key) {
        left_ = nullptr;
        right_ = nullptr;
        key_ = key;
    }

    NodeCartesian(int key, NodeCartesian*  left, NodeCartesian*  right) {
        left_ = left;
        right_ = right;
        key_ = key;
    }

    NodeCartesian* left_;
    NodeCartesian* right_;

    int index_euler_;
    int key_;   //altura    
    int index_;

};
void print_vector(vector<int> a) {
    for (int i = 0; i < a.size(); i++) {
        cout << a[i] << " ";
    }
}

struct BigGroup {

    BigGroup(vector<int> arr, vector<int> arr_values) {
        //i+1 is the range of the min
        //constructing table
        for (int i = 1; i < arr_values.size();i=(i+1)*2-1) {
            for (int j = 0; j <= arr_values.size() - i - 1; j++) {
                table.push_back(get_min(j, j + i,arr_values,arr));
            }
        }

        print_vector(table);

    }


    int get_min(int i, int j,vector<int> arr_values, vector<int> arr) {
        int temp = 0, index, min = INT_MAX;

        for (int k = i; k <= j; k++) {
            temp = arr_values[k];
            if (min > temp) {
                min = temp;
                index = k;
            }
        }
        return arr[index];
    }

    vector<int> table;  //save the min in ranges --- give the euler index
};
struct Group {
    Group(vector<int> arr) {
        pattern = arr;
        construct_table();
    }
    void construct_table() {
        //i+1 is the range of the min
        for (int i = 1; i < pattern.size();i++) {
            for (int j = 0; j <= pattern.size() - i - 1; j++) {
                min.push_back(get_min(j, j + i));
            }
        }
        //print_vector(min);
    }
    int get_min(int i, int j) {
        int temp=0,index,min=INT_MAX;

        for (int k = i; k <= j; k++) {
            temp+= pattern[k];
            if (min > temp) {
                min = temp;
                index = k;
            }
        }
        return index;
    }
    int min_range(int i, int j) {
        int k = j - i+1;    //quantity of elements
        //int index = pow(2, k - 1) - 1;
        int index = 0;
        for (int x = pattern.size() - 1,tmp=2; x > 0; x--,tmp++) {
            
            if (!(tmp >= k)) {
                index += x;
            }
            else {
                break;
            }
        }
        index += i;
        return min[index];
    }



    vector<int> min;
    //vector < vector <int> > min(6, std::vector<int>(6,0));
    vector<int> pattern;
};



class CartesianTree {
public:
    CartesianTree(vector<int> arr_ptr) {
        //arr_ptr_ = arr_ptr;
        //vector<int>& arr = *arr_ptr_;
        arr = arr_ptr;
        transformed_.resize(arr.size(),-1);
        index_.resize(arr.size(),-1);
        create_cartesian();

        
    }
    int look_min(int i, int j) {
        //vector<int>& arr = *arr_ptr_;
        if (i > j) {
            return -1;
        }

        int mini = INT_MAX;
        int index;
        for (int k = i; k <= j; k++) {
            if (arr[k] < mini) {
                mini = arr[k];
                index = k;
            }
        }


        return index;
    }

    void create_cartesian() {
        //vector<int>& arr = *arr_ptr_;
        int ind = look_min(0, arr.size() - 1);

        //header_ = new NodeCartesian(arr[ind]);
        header_ = new NodeCartesian(0);
        header_->index_ = ind;

        func(header_, 0, ind, arr.size() - 1);
        //print_dfs();

        //EulerTour
        //in_order(header_);
        
        euler_tree(header_);
        
        //Colocando a cada nodo el índice de la primera visita en el array euler
        
        vector<bool> nodes_visited(arr.size(),0);
        for (int i = 0; i < euler_nodes_.size(); i++) {
            if (nodes_visited[euler_nodes_[i]->index_] == 0) {
                nodes_visited[euler_nodes_[i]->index_] = 1;
                euler_nodes_[i]->index_euler_ = i;
            }
            
        }
        //print_vector(euler_);
        
        cant_grupo = log2(euler_.size() + 1) / 2;
        //Split in big groups 
        vector<int> big;
        vector<int> big_values;
        cant_grupo = 3;
        int mini = INT_MAX;
        int ind_min = -1;
        for (int i = 0,j; i < euler_.size(); i++) {

            if (i % cant_grupo == 0 && i>0) {
                big.push_back(ind_min);
                big_values.push_back(mini);
                mini = INT_MAX;
                ind_min = -1;
            }
            if (euler_[i] < mini) {
                mini = euler_[i];
                ind_min = i;

            }
        }
        //Verify if the last element(that couldn't be compared) is the min of his group
        /*if (euler_[euler_.size() - 1] < euler_[big[big.size() - 1]]) {
            big[big.size() - 1] = euler_[euler_.size() - 1];
        }*/
        //Add the last group
        big.push_back(ind_min);
        big_values.push_back(mini);
        

        print_vector(big);

        cout << endl;
        BigGroup big_group(big, big_values);

        //Split in groups and put  0 +1 -1
        partir_grupos();

        //create all paterns  0 +1 -1
        create_all_patterns();


    }

    int RMQ(int i, int j) {


    }

    vector<int> get_patterns(int n)
    {
        // array to store binary number 
        vector<int> binaryNum(cant_grupo-1);
        
        // counter for binary array 
        int i = 0;
        while (n > 0) {

            // storing remainder in binary array 
            binaryNum[i] = ((n % 2) == 1) ? 1 : -1;
            n = n / 2;
            i++;
        }

        vector<int> to_return(cant_grupo);
        // getting binary array in reverse order 
        for (int j = i - 1,k=0; j >= 0; j--,k++)
            to_return[i] = binaryNum[j];
        return to_return;
    }

    void create_all_patterns() {
        vector<int> temp;
        
        for (int i=0; i < pow(2, cant_grupo - 1)-1; i++) {
            temp = get_patterns(i);
            temp.insert(temp.begin(), 0);
            mini_groups.push_back(new Group(temp));
        }
    }
    int get_index(vector<int> temp) {
        int ind = 0;
        for (int i = 0; i <temp.size() ; i--) {
            ind += temp[i] * pow(2, temp.size()-i-1);
        }
        return ind;
    }

    void partir_grupos() {
        
        cout<<endl << cant_grupo << endl;
        
        cant_grupo = 3;
        vector<int> temp = euler_;
        vector<int> group_temp(cant_grupo);
        for (int i = 0; i < euler_.size(); i++) {
            if (i % cant_grupo == 0) {
                euler_[i] = 0;
            }
            else {
                euler_[i] = temp[i] - temp[i - 1];
            }
            group_temp[i % cant_grupo];
            if (i == euler_.size() - 1 && euler_.size()%cant_grupo!=0) {
                
                vector<int> tmmmp;
                for (int l = 0; l < group_temp.size() % cant_grupo; l++) {
                    tmmmp.push_back(group_temp[l]);
                }
                group_temp.resize(tmmmp.size());
            }
            ind_mini_groups.push_back(get_index(group_temp));
        }
        //print_vector(euler_);

    }

   

    
    void func(NodeCartesian* node, int i, int k, int j) {
        //vector<int>& arr = *arr_ptr_;   
        int ind_left = look_min(i, k - 1);
        int ind_right= look_min(k + 1, j);
        if (ind_left != -1) {
            //node->left_ = new NodeCartesian(arr[ind_left]);
            node->left_ = new NodeCartesian(node->key_+1);
            node->left_->index_ = ind_left;
            transformed_[ind_left] = node->key_ + 1;
            func(node->left_, i, ind_left,k-1);
        }
            

        if (ind_right != -1) {
            //node->right_ = new NodeCartesian(arr[ind_right]);
            node->right_=new NodeCartesian(node->key_ + 1);
            node->right_->index_ = ind_right;
            transformed_[ind_right] = node->key_ + 1;
            func(node->right_, k+1, ind_right, j);
        }
        
        

    }
    int in_order(NodeCartesian* node,int count=0) {
        if (node == NULL)
            return count;

        /* first recur on left child */
        count=in_order(node->left_,count);

        /* then print the data of node */
        //cout << node->data << " ";
        transformed_[count] = node->key_;
        count++;

        /* now recur on right child */
        count=in_order(node->right_,count);
        return count;
    }
    void euler_tree(NodeCartesian* root,int count=0)
    {
        // store current node's data 
        euler_.push_back(root->key_);
        euler_nodes_.push_back(root);

        // If left node exists 
        if (root->left_)
        {
            // traverse left subtree 
            euler_tree(root->left_);

            // store parent node's data  
            euler_.push_back(root->key_);
            euler_nodes_.push_back(root);
        }

        // If right node exists 
        if (root->right_)
        {
            // traverse right subtree 
            euler_tree(root->right_);

            // store parent node's data 
            euler_.push_back(root->key_);
            euler_nodes_.push_back(root);
        }
    }




    void print_dfs() {
        
        stack<NodeCartesian*> s;
        s.push(header_);
        
        while (!s.empty()) {
            NodeCartesian* run = s.top();
            cout << run->index_euler_ << endl;
            s.pop();
            if (run->right_) {
                s.push(run->right_);
            }
            if (run->left_) {
                s.push(run->left_);
            }
        }

        
    }
    

    vector<int> transformed_;    //+1/-1
    vector<int> euler_;
    vector<NodeCartesian*> euler_nodes_;
    int cant_grupo;
    vector<Group*> mini_groups;
    vector<int> ind_mini_groups;


    vector<int> index_;
    vector<int>* arr_ptr_;
    vector<int> arr;
    NodeCartesian* header_;
};

int main()
{
    vector<int> arrx = { 8,7,2,8,6,9,4,5 };

    CartesianTree arbol(arrx);
    /*vector<int> a = { 0,1,-1,-1,1 };
    Group tmp(a);*/

}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
