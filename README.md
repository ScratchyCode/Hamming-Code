# FileECC (Error Correction Code)
In FileECC.PY usata la codifica Hamming(7,4) leggendo i byte di un file, dividendoli a metà per eseguire la codifica su ogni parte ed eseguendo il padding di un bit per ognuno, ottenendo cosi una resistenza elevanta agli errori attraverso la ridondanza delle informazioni; da un byte non codificato se ne ottengono due byte codificati, quindi si raddoppia la dimensione dei file, utile per lo storage di backup sensibili.

# Hamming Arduino
In Hamming.ino implementata la stessa codifica senza l'uso di librerie esterne, attraverso le proprietà lineari della codifica ed il calcolo matriciale.


Per usarlo bisogna comunicare sulla porta seriale la stringa binaria da codificare (o decodificare) seguita dal corrispettivo comando:

1) C per codificare (es: C1011)
2) D per decodificare (es: D1001101)
