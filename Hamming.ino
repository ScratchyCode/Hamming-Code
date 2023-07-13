// Coded by Pietro Squilla
/*
Uso comandi:
    Codifica:    C1011
    Decodifica:  D0110011
*/
#include <Arduino.h>

// dimensioni caratteristiche delle matrici
#define DIM_C 4
#define DIM_D 7
#define DIM_P 3

// matrici C (codifica), D (decodifica), P (parità)
int C[DIM_D][DIM_C] = {
  {1,1,0,1},
  {1,0,1,1},
  {1,0,0,0},
  {0,1,1,1},
  {0,1,0,0},
  {0,0,1,0},
  {0,0,0,1}
};
int D[DIM_C][DIM_D] = {
  {0,0,1,0,0,0,0},
  {0,0,0,0,1,0,0},
  {0,0,0,0,0,1,0},
  {0,0,0,0,0,0,1}
};
int P[DIM_P][DIM_D] = {
  {1,0,1,0,1,0,1},
  {0,1,1,0,0,1,1},
  {0,0,0,1,1,1,1}
};

// moltiplicazione matrice per vettore ad hoc per la codifica
void moltiplica(int a[DIM_D][DIM_C], int b[DIM_C], int res[DIM_D]){
  for(int i = 0; i < DIM_D; i++){
    res[i] = 0;
    for(int j = 0; j < DIM_C; j++){
      res[i] += a[i][j] * b[j];
    }
    res[i] %= 2; // risultati binari
  }
}

// moltiplicazione matrice per vettore ad hoc per la decodifica
void moltiplicaDec(int a[DIM_C][DIM_D], int b[DIM_D], int res[DIM_C]){
  for(int i = 0; i < DIM_C; i++){
    res[i] = 0;
    for(int j = 0; j < DIM_D; j++){
      res[i] += a[i][j] * b[j];
    }
    res[i] %= 2;
  }
}

// moltiplicazione matrice per vettore ad hoc per il controllo degli errori
void moltiplicaParita(int a[DIM_P][DIM_D], int b[DIM_D], int res[DIM_P]){
  for(int i = 0; i < DIM_P; i++){
    res[i] = 0;
    for(int j = 0; j < DIM_D; j++){
      res[i] += a[i][j] * b[j];
    }
    res[i] %= 2;
  }
}

void setup(){
  Serial.begin(9600);
  
  // aspetto la comunicazione seriale
  while(!Serial){
    ;
  }
  
  Serial.println("Inserisci un comando (C o D) seguito dalla stringa binaria: ");
}

void loop() {
  if(Serial.available()){
    String str = Serial.readString();
    delay(200);
    
    // eliminazione dei caratteri non validi
    for(int i = 0; i < str.length(); i++){
      if(str[i] != '0' && str[i] != '1' && str[i] != 'C' && str[i] != 'D'){
        str.remove(i, 1);
        i--; // regola l'indice dopo la rimozione
      }
    }
    
    char modalita = str[0]; 
    str = str.substring(1); 
    
    int blocchi = modalita == 'C' ? (str.length() + DIM_C - 1) / DIM_C : (str.length() + DIM_D - 1) / DIM_D;
    for(int b = 0; b < blocchi; b++){
      int bin[DIM_D] = {0};
      
      int bit = min((modalita == 'C' ? DIM_C : DIM_D), str.length() - b * (modalita == 'C' ? DIM_C : DIM_D));
      
      for(int i = 0; i < bit; i++){
        bin[i] = str[i + b * (modalita == 'C' ? DIM_C : DIM_D)] - '0';
      }
      
      if(modalita == 'C'){ // se codifico
        int ris[DIM_D];
        moltiplica(C, bin, ris);
        
        Serial.print("Codifica Hamming: ");
        for(int i = 0; i < DIM_D; i++){
          Serial.print(ris[i]);
        }
        
        Serial.println();
      }else if(modalita == 'D'){ // se decodifico
        int sindrome[DIM_P];
        moltiplicaParita(P, bin, sindrome);
        
        bool errore = false;
        for(int i = 0; i < DIM_P; i++){
          if(sindrome[i] != 0){
            errore = true;
          }
        }
        
        if(errore){
          int posErrore = 0;
          for(int i = 0; i < DIM_P; i++){
            if(sindrome[i] == 1){
              posErrore += (1 << i);
            }
          }
          
          bin[posErrore - 1] = 1 - bin[posErrore - 1];
          
          Serial.print("Errore trovato e corretto alla posizione: ");
          Serial.println(posErrore);
        }
        
        int risDec[DIM_C];
        moltiplicaDec(D, bin, risDec);
        
        Serial.print("Decodifica Hamming: ");
        for(int i = 0; i < DIM_C; i++){
          Serial.print(risDec[i]);
        }
        
        Serial.println();
      }
      
      delay(100); 
    }
  }
}
