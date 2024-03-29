# thanks for the function definitions to github community
from typing import List
from math import log2, ceil
from random import randrange


def __hamming_common(src: List[List[int]], s_num: int, encode=True) -> None:
    """
    Here's the real magic =)
    """
    s_range = range(s_num)

    for i in src:
        sindrome = 0
        for s in s_range:
            sind = 0
            for p in range(2 ** s, len(i) + 1, 2 ** (s + 1)):
                for j in range(2 ** s):
                    if (p + j) > len(i):
                        break
                    sind ^= i[p + j - 1]

            if encode:
                i[2 ** s - 1] = sind
            else:
                sindrome += (2 ** s * sind)

        if (not encode) and sindrome:
            i[sindrome - 1] = int(not i[sindrome - 1])


def hamming_encode(msg: str, mode: int=8) -> str:
    """
    Encoding the message with Hamming code.
    :param msg: Message string to encode
    :param mode: number of significant bits
    :return: 
    """

    result = ""

    msg_b = msg.encode("utf-8")
    s_num = ceil(log2(log2(mode + 1) + mode + 1))   # number of control bits
    bit_seq = []
    for byte in msg_b:  # get bytes to binary values; every bits store to sublist
        bit_seq += list(map(int, f"{byte:08b}"))

    res_len = ceil((len(msg_b) * 8) / mode)     # length of result (bytes)
    bit_seq += [0] * (res_len * mode - len(bit_seq))    # filling zeros

    to_hamming = []

    for i in range(res_len):    # insert control bits into specified positions
        code = bit_seq[i * mode:i * mode + mode]
        for j in range(s_num):
            code.insert(2 ** j - 1, 0)
        to_hamming.append(code)

    __hamming_common(to_hamming, s_num, True)   # process

    for i in to_hamming:
        result += "".join(map(str, i))

    return result


def hamming_decode(msg: str, mode: int=8) -> str:
    """
    Decoding the message with Hamming code.
    :param msg: Message string to decode
    :param mode: number of significant bits
    :return: 
    """

    result = ""

    s_num = ceil(log2(log2(mode + 1) + mode + 1))   # number of control bits
    res_len = len(msg) // (mode + s_num)    # length of result (bytes)
    code_len = mode + s_num     # length of one code sequence

    to_hamming = []

    for i in range(res_len):    # convert binary-like string to int-list
        code = list(map(int, msg[i * code_len:i * code_len + code_len]))
        to_hamming.append(code)

    __hamming_common(to_hamming, s_num, False)  # process

    for i in to_hamming:    # delete control bits
        for j in range(s_num):
            i.pop(2 ** j - 1 - j)
        result += "".join(map(str, i))

    msg_l = []

    for i in range(len(result) // 8):   # convert from binary-sring value to integer
        val = "".join(result[i * 8:i * 8 + 8])
        msg_l.append(int(val, 2))

    result = bytes(msg_l).decode("utf-8")   # finally decode to a regular string

    return result


def noizer(msg: str, mode: int) -> str:
    """
    Generates an error in each element of a Hamming encoded message
    """
    seq = list(map(int, msg))
    s_num = ceil(log2(log2(mode + 1) + mode + 1))
    code_len = mode + s_num
    cnt = len(msg) // code_len
    result = ""

    for i in range(cnt):
        to_noize = seq[i * code_len:i * code_len + code_len]
        noize = randrange(code_len)
        to_noize[noize] = int(not to_noize[noize])
        result += "".join(map(str, to_noize))

    return result



if __name__ == "__main__":
    MODE = 8    # count of signifed bits
    
    ### Menù ###
    print("************************")
    print("*                      *")
    print("*   Codifica Hamming   *")
    print("*                      *")
    print("************************")
    print("1) Codifica\n2) Decodifica\n3) Esci")
    menu = int(input("Inserisci l'opzione scelta: "))
    
    if menu == 1:
        msg = input("Frase da codificare: ")
        enc_msg = hamming_encode(msg,MODE) # Encode your message to Hamming code
        print("enc:\n" + enc_msg)
        
        noize_msg = noizer(enc_msg,MODE)   # Noizer for encoded message
        print("\nnoized:\n" + noize_msg)
        
        dec_msg = hamming_decode(noize_msg,MODE)   # Hamming decode
        print("\ndec:\n" + dec_msg)
    
    if menu == 2:
        noize_msg = input("Codice da decodificare: ")
        
        dec_msg = hamming_decode(noize_msg,MODE)   # Hamming decode
        print("\ndec:\n" + dec_msg)
        
    if menu == 3:
        exit()
    
    elif menu > 3 or menu < 1:
        print("\nScelta non valida.\nUscita...\n")
    
    input("\nPremi INVIO per continuare...")
    
    
