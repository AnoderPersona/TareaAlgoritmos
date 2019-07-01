#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <string.h>
#include <cmath>
#include <sstream>
#include <chrono>
#include <time.h>

using namespace std;

//Estructura que contiene el indice de un sufijo y el sufijo en sí
struct Suffix{
    
    string suff;
    int index;
    
};

//Estructura para el Suffix trie
struct nodo{
    
    //Informacion del nodo
    int costo;

    //vector con todas las relaciones del elemento
    map<char,nodo*> relaciones;
    
};

//Crea el Suffix Trie
nodo* ST(string texto, vector <Suffix> data, int N){
    
    nodo *raiz = new nodo;
    nodo *p;
    nodo *q;
    
    for (int palabra = 0; palabra < N; palabra++){

        p = raiz;
        
        map<char, nodo*>::iterator it ;
        
        for (int letra = 0; letra < (data[palabra].suff).length(); letra++){

            if (data[palabra].suff[letra] == '\n'){}
            
            else if ( p -> relaciones.find(data[palabra].suff[letra]) == p -> relaciones.end()){

                q = new nodo;
                q -> costo = data[palabra].index;
                p -> relaciones.insert({(data[palabra].suff[letra]),q});
                p = q;
                
            }else{

                it = p -> relaciones.find(data[palabra].suff[letra]);

                p = it->second;
                
            }
        }   
    }
    
    return raiz;
}

//Busca dentro del Suffix Trie
int findWord(string patron, nodo *raiz){
    
//     map<char, nodo*>::iterator it ;
    
    nodo *p;
    p = raiz;

    for (int i = 0; i < patron.size(); i++){

        map<char, nodo*>::iterator it ;

        //Busca en el map<char, *nodo> relaciones el char actual del patrón, y una vez que lo encuentra, el puntero apunta a su hijo
        
        it = p -> relaciones.find(patron[i]);
        
        p = it->second;

        if(it != p->relaciones.end()){
            
            cout << "Borró el iterador" << endl;
            
            p->relaciones.erase (it);
        }
            
    }
    return p->costo;
}

//Permite ordenar el vector alfabeticamente por su sufijo
bool compare(const Suffix &a, const Suffix &b){
    int i;
    
    i = 0;
    
    while (a.suff[i] == b.suff[i]){
        
        i++;
        
    }
    return a.suff[i] < b.suff[i];
    
}

//Crea vector de sufijos para luego pasar a un arreglo
int createVector(string text,vector<Suffix> &suffixes){
    
    Suffix s;
    
    for (int i = 0; i < text.size(); i++){
        
        s.suff = text.substr(i,text.size() - 1);
        s.index = i;
        suffixes.push_back(s);
    
    }
        
    sort(suffixes.begin(), suffixes.end(), compare);
    
    return suffixes.size();
}

void SABinaryArray(string patron, string texto, int arr[], int (&res)[2],int n){
    int b, t, s, e, m;
    int beginSubArray;
    int endSubArray;
    string aux;
    
    m = patron.size();
    b = 0;
    t = n-1;
    while(b < t){
        
        s = (b+t)/2;
        beginSubArray = arr[s];
        endSubArray = (arr[s] + m - 1);
               
        aux = texto.substr(beginSubArray,m); 
        
       if(patron.compare(aux) > 0)
            b = s + 1;
        else
            t = s;
    }
    
    e = b - 1;
    t = n;
    while (e < t){
        
        s = ceil((e+t)/2.0);

        beginSubArray = arr[s];
        endSubArray = (arr[s] + m - 1);
       
        aux = texto.substr(beginSubArray,endSubArray);
        
        if(patron.compare(aux)==0){
           
            e = s;
            
        }else{
            
            t = s - 1;
        }
    }    
    
    res[0] = b;
    res[1] = e;
}

//Encuentra el patron por fuerza bruta
int cutOutText(string patron, string texto){
    
    int lon;
    int numPatron;
    string posPatron;
    
    lon = patron.size();
    numPatron = 0;
    
    for (int i = 0; i < texto.size()-lon; i++){
        
        posPatron = texto.substr(i,lon);
        
        if (posPatron == patron){
            
            return numPatron;
        }
        
        numPatron += 1;
    }
    
    return -1;
}

void locate(string T, int N, string P,int m[], vector<Suffix> suffixes, int (&times)[3], nodo *arbol){
    
    int placeCutOut, places[2], trieIndex, randomB, randomE;
    long cOTTime, SABATime, STTime = 0;

    
//     nodo *arbol;
//     
//     arbol = ST(T, suffixes, N);
    /* 
    for (int i = 0; i < 1500; i++){
        
        srand(time(NULL));
        
        randomB = rand() % (T.size()-10);
        randomE = randomB + (rand() % 10) +1;
        
        P = T.substr(randomB,randomE);
        
        cout << "Iteracion " << i << endl << "patron: "<< P << endl; 
        
        /*--------- Resultados por cutOutText -------------------*/
        auto start = chrono::high_resolution_clock::now();
        
        cout << "|----------------------------------------|" << endl;
        
        cout << "   Por fuerza bruta: " << endl << "   ";
        placeCutOut = cutOutText(P, T);
        
        if (placeCutOut != -1)
            cout << placeCutOut << " " << placeCutOut + P.size()<< endl;
        else
            cout << -1 << endl;
        
        auto stop = chrono::high_resolution_clock::now();
        
        auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);
        
        times[0] += duration.count();
        
        cout << "Demoró " << duration.count() << " microsegundos" <<endl;
        
        /*--------- Resultados por suffixArray ------------------*/
        start = chrono::high_resolution_clock::now();

        SABinaryArray(P, T, m, places, N);
        
        cout << "|----------------------------------------|" << endl;
        
        cout << "   Por suffix array: " << endl  << "   ";
        
        cout << m[places[0]] << " " <<m[places[0]] + P.size() << endl;

        stop = chrono::high_resolution_clock::now();
        
        duration = chrono::duration_cast<chrono::microseconds>(stop-start);
        
        cout << "Demoró " << duration.count() << " microsegundos" <<endl;
        
        times[1] += duration.count();
        
        /*--------- Resultados por suffixTrie -------------------*/
        
        //start = chrono::high_resolution_clock::now();

        cout << "|----------------------------------------|" << endl;
        
        cout << "   Por suffix trie: " << endl ;

        //arbol = ST(T, suffixes, N);
        
        //stop = chrono::high_resolution_clock::now();
        
        //duration = chrono::duration_cast<chrono::microseconds>(stop-start);
        
        //cout << "Demoró " << duration.count() << " microsegundos en crear el arbol" <<endl;
        
        start = chrono::high_resolution_clock::now();

        trieIndex = findWord(P, arbol);
        if(trieIndex != -1)
            cout << "   "<< trieIndex << " " << trieIndex + P.size() << endl;
        else
            cout << trieIndex << endl;
        
            stop = chrono::high_resolution_clock::now();
        
        duration = chrono::duration_cast<chrono::microseconds>(stop-start);
        
        cout << "Demoró " << duration.count() << " microsegundos en encontrar e imprimir el indice" <<endl;
        
        times[2] += duration.count();
        
        cout << "|----------------------------------------|" << endl;
    //}
    
}

void experiment(string text,int N,string patron,int arr[], vector<Suffix> suffixes){
    
    int randomB,randomE, nIter;
    int times[3] = {0,0,0};
    nodo *arbol;
    
    nIter = 3000;

    cout << "Creando árbol..." << endl;
    
    arbol = ST(text, suffixes, N);
    
    cout << "Árbol creado" << endl;
    
    srand(time(NULL));
    
    for (int i = 0; i < nIter; i++){
                
        randomB = rand() % (text.size()-10);
        randomE = (rand() % 30) +1;
        
        cout << "RandomB: " << randomB << " RandomE: " << randomE << endl;
        
        patron = text.substr(randomB,randomE);
        
        cout << "Iteracion " << i << endl << "patron: "<< patron << endl; 
        
        locate(text, N, patron, arr, suffixes,times, arbol);
    
    }
    
    cout << "Tiempo promedio en " << nIter << " búsquedas:" << endl << "cutOutText = " << times[0]/1500 << endl << "SA = " << times[1]/1500 << endl << "ST = " << times[2]/1500<< endl;
}

int main(){

    string text, patron;
    stringstream textstream;
    vector<Suffix> suffixes;
    int sizeSuff, placeCutOut, places[2], trieIndex, m;

    nodo *arbol;
    
    ifstream txt;
    txt.open ("test.txt");

    
    //guardar texto en una variable
    textstream << txt.rdbuf();
    text = textstream.str();
    
    txt.close();
    
    //reemplaza '!' en el texto por espacios, para poder agregar un '!' al final para que sea el caracter especial
    replace(text.begin(), text.end(), '!',' ');
    text += '!';
    
    cout << "Su texto es: " << endl << endl << text << endl;
    
    sizeSuff = createVector(text, suffixes);
    
    const int N = sizeSuff;
    
    int arr[N];

    
    //arreglo con los sufijos ordenados
    for (int i = 0; i < N; i++){
        
        arr[i] = suffixes[i].index;
    }
    
    
    cout << " ________________________________________" << endl;
    
    cout << " Ingrese patron a buscar: " << endl << "  ";
    
    cin >> patron;
    
    cout << " ----------------------------------------" << endl;
    
    cout << "   Su patron se encuentra en: " << endl;
    
    auto start = chrono::high_resolution_clock::now();

    experiment(text,N,patron,arr, suffixes);
    //locate(text, N,patron,arr, suffixes);
    
    auto stop = chrono::high_resolution_clock::now();
    
    auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    
    cout << "En total demoró " << duration.count() << " microsegundos" <<endl;
    
    return 0;
}
