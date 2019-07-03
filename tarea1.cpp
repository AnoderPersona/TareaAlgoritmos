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


/*
 * Tachar el patron una vez ya visto
 */

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
    nodo *nodoActual;
    nodo *nodoAux;
    
    for (int palabra = 0; palabra < N; palabra++){

        nodoActual = raiz;
        
        map<char, nodo*>::iterator relacionActual ;
        
        for (int letra = 0; letra < (data[palabra].suff).length(); letra++){
            
            if (data[palabra].suff[letra] == '\n'){
            }
            
            else if ( nodoActual -> relaciones.find(data[palabra].suff[letra]) == nodoActual -> relaciones.end()){

                nodoAux = new nodo;
                nodoAux -> costo = data[palabra].index;
                nodoActual -> relaciones.insert({(data[palabra].suff[letra]),nodoAux});
                nodoActual = nodoAux;
                
                
            }else{
                
                relacionActual = nodoActual -> relaciones.find(data[palabra].suff[letra]);

                nodoActual = relacionActual->second;
                
            }
        }   
    }
    
    return raiz;
}

//Busca dentro del Suffix Trie
int findWord(string patron, nodo *raiz){
    
    nodo *nodoActual;
    nodoActual = raiz;


    for (int index = 0; index < patron.size(); index++){

        map<char, nodo*>::iterator relacionActual ;

        if (patron[index] == '\n'){
        }
        
        else{
         //Busca en el map<char, *nodo> relaciones el char actual del patrón, y una vez que lo encuentra, el puntero apunta a su hijo
         
            relacionActual = nodoActual -> relaciones.find(patron[index]);
            
            nodoActual = relacionActual->second;

        }
    }
    return nodoActual->costo;
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
    
    Suffix sufijoActual;
    
    for (int start = 0; start < text.size(); start++){
        
        sufijoActual.suff = text.substr(start,text.size() - 1);
        sufijoActual.index = start;
        suffixes.push_back(sufijoActual);
    
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
    
    for (int start = 0; start < texto.size()-lon; start++){
        
        posPatron = texto.substr(start,lon);
        
        if (posPatron == patron){
            
            return numPatron;
        }
        
        numPatron += 1;
    }
    
    return -1;
}

void locate(string T, int N, string P,int m[], vector<Suffix> suffixes, int (&times)[3], nodo *tope){
    
    int placeCutOut, places[2], trieIndex, randomB, randomE;
    long cOTTime, SABATime, STTime = 0;
        
    
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
    

    cout << "|----------------------------------------|" << endl;
    
    cout << "   Por suffix trie: " << endl ;
    
    start = chrono::high_resolution_clock::now();

    trieIndex = findWord(P, tope);
    
    if(trieIndex != -1)
        
        cout << "   "<< trieIndex << " " << trieIndex + P.size() << endl;
    else
        
        cout << trieIndex << endl;
    
        stop = chrono::high_resolution_clock::now();
    
    duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    
    cout << "Demoró " << duration.count() << " microsegundos en encontrar e imprimir el indice" <<endl;
    
    times[2] += duration.count();
    
    cout << "|----------------------------------------|" << endl;
    
}

void experiment(string text,int N,string patron,int arr[], vector<Suffix> suffixes){
    
    int randomB,randomE, nIter;
    int times[3] = {0,0,0};
    nodo *arbol, *tope;
    
    nIter = 3000;

    cout << "Creando árbol..." << endl;
    
    arbol = ST(text, suffixes, N);
    
    cout << "Árbol creado" << endl;
    
    srand(time(NULL));
    
    for (int i = 0; i < nIter; i++){
                
        tope = arbol;
        
        randomB = rand() % (text.size()-10);
        randomE = (rand() % 30) +1;
        
        cout << "RandomB: " << randomB << " RandomE: " << randomE << endl;
        
        patron = text.substr(randomB,randomE);
        
        cout << "Iteracion " << i << endl << "patron: "<< patron << endl; 
        
        locate(text, N, patron, arr, suffixes,times, tope);
    
    }
    
    cout << "Tiempo promedio en " << nIter << " búsquedas:" << endl << "cutOutText = " << times[0]/1500 << endl << "SA = " << times[1]/1500 << endl << "ST = " << times[2]/1500<< endl << "microsegundos" << endl;
}

int main(){

    string text, patron;
    char option;
    stringstream textstream;
    vector<Suffix> suffixes;
    int sizeSuff, placeCutOut, places[2], trieIndex, m;

    nodo *arbol;
    int times[3] = {0,0,0};
    
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
    do{
        
        cout << endl << "Desea [a] experimentar con 3000 iteraciones, o desea [b] ingresar su patron? [a/b]: ";
        
        cin >> option;
        
        cout << " ________________________________________" << endl;
        
        if (option == 'b'){
                
            cout << " Ingrese patron a buscar: " << endl << "  ";
            
            cin >> patron;
            
            cout << " ----------------------------------------" << endl;
            
            cout << "   Su patron se encuentra en: " << endl;
            
            
            cout << "Creando árbol... " << endl;
            arbol = ST(text, suffixes, N);
            cout << "árbol creado" << endl;
            
            locate(text, N,patron,arr, suffixes, times, arbol);
        }
        
        else if(option == 'a'){
            
            auto start = chrono::high_resolution_clock::now();

            experiment(text,N,patron,arr, suffixes);
            
            auto stop = chrono::high_resolution_clock::now();
            
            auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);
            
            cout << "En total demoró " << duration.count() << " microsegundos" <<endl;
        }
    
    }while (option != 'a' and option != 'b');
/*    
    cout << " Ingrese patron a buscar: " << endl << "  ";
    
    cin >> patron;
    
    cout << " ----------------------------------------" << endl;
    
    cout << "   Su patron se encuentra en: " << endl;*/
    
//     auto start = chrono::high_resolution_clock::now();
// 
//     experiment(text,N,patron,arr, suffixes);
//     //locate(text, N,patron,arr, suffixes);
//     
//     auto stop = chrono::high_resolution_clock::now();
//     
//     auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);
//     
//     cout << "En total demoró " << duration.count() << " microsegundos" <<endl;
    
    return 0;
}
