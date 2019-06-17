#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

struct nodo{
    
    //Informacion del nodo
    int costo;
    
    //vector con todas las relaciones del elemento
    map<char,*hijo> relaciones;
    
};

void split(string texto, char delimiter, vector<string> *vec){
    
    string palabra;
    
    palabra = ""
    
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

void ST(string patron, string texto){
    
    
    
    nodo *raiz = new nodo;
    nodo *p;
    nodo *q;
    int cont;
    
    cont = 0;
    vector<string> textVector;
    
    split(texto, ' ', textVector);
    
    for (int palabra = 0; palabra < textVector.size(); palabra++){
        
        p = raiz;
        
        for (int letra = 0; letra < textVector[palabra].length(); letra++){
            
            cont += 1;
            
            if (p -> relaciones.count(textVector[palabra][letra] == 0){
                
                q = new nodo;
                q -> costo = cont;
                p -> relaciones.insert(pair<char, *hijo> (textVector[palabra][letra],*q));
                
            }else{
                
                p = p -> relaciones.find([textVector[palabra][letra]]);
                
            }
        
    }
    
    
}
