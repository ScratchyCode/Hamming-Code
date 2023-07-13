// Coded by Pietro Squilla
// Hamming (7,4)
#include <Arduino.h>

// dimensioni caratteristiche delle matrici
#define DIM_C 4
#define DIM_D 7
#define DIM_P 3

// matrici C (codifica), D (decodifica) e P (controllo parità)
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

// funzione generalizzata per moltiplicare una matrice per un vettore
void moltiplica(int rows, int cols, int* a, int* b, int* res){
  for(int i = 0; i < rows; i++){
    res[i] = 0;
    for(int j = 0; j < cols; j++){
      res[i] += *((a + i*cols) + j) * b[j];
    }
    res[i] %= 2;
  }
}

void setup(){
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Inserisci un comando (C o D) seguito dalla stringa binaria: ");
}

void loop(){
  if(Serial.available()){
    String str = Serial.readString();
    delay(200);
    
    for(int i = 0; i < str.length(); i++){
      if(str[i] != '0' && str[i] != '1' && str[i] != 'C' && str[i] != 'D'){
        str.remove(i, 1);
        i--;
      }
    }
    
    if(str[0] != 'C' && str[0] != 'D'){
      Serial.println("Il comando deve iniziare con 'C' o 'D'");
      return;
    }

    char modalita = str[0]; 
    str = str.substring(1); 

    int blocchi = modalita == 'C' ? (str.length() + DIM_C - 1) / DIM_C : (str.length() + DIM_D - 1) / DIM_D;

    String decodificata = "";
    String errori = "";
    
    for(int b = 0; b < blocchi; b++){
      if(b == 0 && modalita == 'C'){
        Serial.print("Codifica Hamming: ");
      }
      
      int bin[DIM_D] = {0};
      
      int bit = min((modalita == 'C' ? DIM_C : DIM_D), str.length() - b * (modalita == 'C' ? DIM_C : DIM_D));
      
      for(int i = 0; i < bit; i++){
        bin[i] = str[i + b * (modalita == 'C' ? DIM_C : DIM_D)] - '0';
      }
      
      if(modalita == 'C'){ 
        int ris[DIM_D];
        moltiplica(DIM_D, DIM_C, (int*)C, bin, ris);
        
        for(int i = 0; i < DIM_D; i++){
          Serial.print(ris[i]);
        }
      }else if(modalita == 'D'){
        int sindrome[DIM_P];
        moltiplica(DIM_P, DIM_D, (int*)P, bin, sindrome);
        
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
          
          errori += "Errore trovato e corretto alla posizione: ";
          errori += String(posErrore);
          errori += " nel blocco ";
          errori += String(b + 1);
          errori += "\n";
        }
        
        int risDec[DIM_C];
        moltiplica(DIM_C, DIM_D, (int*)D, bin, risDec);
        
        for(int i = 0; i < DIM_C; i++){
          decodificata += String(risDec[i]);
        }
      }
      
      if(b == blocchi - 1){
        if(modalita == 'D'){
          Serial.println("Decodifica Hamming: " + decodificata);
          if(errori != ""){
            Serial.println(errori);
          }
        }else{
          Serial.println();
        }
      }
    }
  }
}
