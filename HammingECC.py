# Coded by ScratchyCode
# Installare la libreria con: pip install hamming-codec
from hamming_codec import encode, decode

def byte_to_int(byte):
    result = 0
    for b in byte:
        result = result * 256 + int(b)
    return result

def int_to_byte(value,length):
    result = []
    for i in range(0,length):
        result.append(value >> (i * 8) & 0xff)
    result.reverse()
    return result


############
### MAIN ###
############

print("************************")
print("*                      *")
print("*   Codifica Hamming   *")
print("*                      *")
print("************************")
print("1) Codifica file\n2) Decodifica file\n3) Esci")
menu = int(input("Inserisci l'opzione scelta: "))

if menu == 1:
    # lettura byte da file
    filename = input("Nome file: ")
    output = open("enc.bin","a+b")
    
    print("Codifica in corso...")
    
    with open(filename,'rb') as pf:
        while True:
            byte = pf.read(1)
            if not byte:
                break
            
            byte = str(bin(byte_to_int(byte)))
            byte = byte[2:]
            
            # padding di zeri non significativi per definire tutti i bit
            if(len(byte) < 8):
                tmp = byte
                zero = '0'
                for i in range(8 - len(byte)):
                    tmp = zero + tmp
                byte = tmp
            
            # split dei byte in due parti
            byte1 = byte[0:4]
            byte2 = byte[4:8]
            
            # codifica
            enc1 = encode(int(byte1,2),4)
            enc2 = encode(int(byte2,2),4)
            
            # padding per scrivere su file
            # sui 4 bit la codifica produce sempre 7 bit -> ne manca uno per scrivere un byte
            padding = '0' # padding significativo
            enc1 = enc1 + padding
            enc2 = enc2 + padding
            
            # scrivo il byte codificato come due byte distinti
            output.write(bytes(int_to_byte(int(enc1,2),1)))
            output.write(bytes(int_to_byte(int(enc2,2),1)))
    
    output.close()


if menu == 2:
    # lettura byte da file
    filename = input("Nome file: ")
    output = open("decoded","a+b")
    
    print("Decodifica in corso...")
    
    with open(filename,'rb') as pf:
        while True:
            byte1 = pf.read(1)
            if not byte1:
                break
            
            byte2 = pf.read(1)
            if not byte2:
                break
            
            byte1 = str(bin(byte_to_int(byte1)))
            byte1 = byte1[2:]
            byte2 = str(bin(byte_to_int(byte2)))
            byte2 = byte2[2:]
            
            # padding di zeri non significativi per definire il byte
            if(len(byte1) < 8):
                tmp1 = byte1
                zero = '0'
                for i in range(8 - len(byte1)):
                    tmp1 = zero + tmp1
                byte1 = tmp1
                
            if(len(byte2) < 8):
                tmp2 = byte2
                zero = '0'
                for i in range(8 - len(byte2)):
                    tmp2 = zero + tmp2
                byte2 = tmp2
            
            # rimuovo il padding per ottenere il codice di hamming
            byte1 = byte1[0:7]
            byte2 = byte2[0:7]
            
            # decodifica
            dec1 = decode(int(byte1,2),7)
            dec2 = decode(int(byte2,2),7)
            
            # ricombino il byte originale
            dec = dec1 + dec2
            
            output.write(bytes(int_to_byte(int(dec,2),1)))

    pf.close()
    output.close()


if menu == 3:
    exit()


elif menu > 3 or menu < 1:
    print("\nScelta non valida.\nUscita...\n")
    exit()


input("\nPremi INVIO per continuare...")
