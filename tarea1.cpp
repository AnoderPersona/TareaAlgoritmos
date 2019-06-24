#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <string.h>

#include <sstream>

using namespace std;

//Estructura que contiene el indice de un sufijo y el sufijo en sí
struct Suffix{
    
    string suff;
    int index;
    
};

//Permite usar el método .split() de python
void split(string texto, char delimiter, vector<string> &vec){
    
    string palabra;
    
    palabra = "";
    
    for (int i = 0; i < texto.length(); i++){
        
        if (texto[i] != delimiter){
            
            palabra += texto[i];
            
        }else{
            
            palabra += '$';
            vec.push_back(palabra);
            palabra = "";
        }
    }
}


/*ver bien que pasa en compare
 * ver por que no imprime por fuerza bruta
 */


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
    
    int b;
    int t;
    int s;
    int e;
    //int n;
    int m;
    
    int beginSubArray;
    int endSubArray;
    
    //n = sizeof(arr)/sizeof(int) - 1;
    
    //n = arr.size();
    m = patron.size();
    
    b = 1;
    t = n-1;
    
    while(b < t){
        s = (b+t)/2;
        
        beginSubArray = arr[s];
        endSubArray = (arr[s] + m - 1);
        
        if(patron > (texto.substr (beginSubArray,endSubArray))){
            b = s + 1;
        }else{
            t = s;
        }
    }
    e = b - 1;
    t = n;

    while (e < t){
        s = (e + t)/2;
        
        beginSubArray = arr[s];
        endSubArray = (arr[s] + m - 1);
        
        if(patron == (texto.substr (beginSubArray,endSubArray))){
            e = s;
        }else{
            t = s - 1;
        }
    }    
    res[0] = b;
    res[1] = e;
}


int cutOutText(string patron, string texto){
    
    int lon;
    int numPatron;
    string posPatron;
    
    lon = patron.size();
    numPatron = 0;
    
    for (int i = 0; i < texto.size()-lon; i++){
        
        posPatron = "";
        
        for(int j = 0; j < lon; j++){
            
            posPatron += texto[i+j];
            
        }if (posPatron == patron){
            
            numPatron += 1;
        }
    }
    
    return numPatron;
}

int main(){
    
    string text;
    string patron;
    stringstream textstream;
    vector<Suffix> suffixes;
    int sizeSuff;
    int places[2];
    int placeCutOut;
    
    ifstream txt;
    txt.open ("test.txt");

    
    //guardar texto en una variable
    textstream << txt.rdbuf();
    text = textstream.str();
    
    txt.close();
    
    //reemplaza '!' en el texto por espacios, para poder agregar un '!' al final para que sea el caracter especial
    replace(text.begin(), text.end(), '!',' ');
    text += '!';
    
    sizeSuff = createVector(text, suffixes);
    
    const int N = sizeSuff;
    
    int arr[N];

    
    //arreglo con los sufijos ordenados
    for (int i = 0; i < N; i++){
        
        arr[i] = suffixes[i].index;
    }
    
    
    cout << "Ingrese patron a buscar: " << endl;
    
    cin >> patron;
    
    /*--------- Resultados por cutOutText -------------------*/
    cout << "Por manera bruta: " << endl;
    placeCutOut = cutOutText(patron, text);
    cout << placeCutOut << endl;
    
    /*--------- Resultados por suffixArray ------------------*/
    cout << "Por suffix array: " << endl;
    SABinaryArray(patron, text, arr, places, N);
    
    for (int i = 0; i < 2; i++){
        
        cout << places[i] <<" ";
        
    }
    cout << endl;
        
    
    /*--------- Resultados por suffixTrie -------------------*/

        
    return 0;
}
