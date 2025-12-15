from Crypto.Cipher import DES

def rot13(text: str) -> str:
    """Apply ROT13 to a string."""
    result = []
    for c in text:
        if 'a' <= c <= 'z':
            result.append(chr((ord(c) - ord('a') + 13) % 26 + ord('a')))
        elif 'A' <= c <= 'Z':
            result.append(chr((ord(c) - ord('A') + 13) % 26 + ord('A')))
        else:
            result.append(c)
    return ''.join(result)

def des_encrypt(plaintext: bytes, key: bytes) -> bytes:
    """Encrypt bytes with DES in ECB mode with zero padding to 8-byte blocks."""
    key = key.ljust(8, b'X')[:8]  # pad/truncate to 8 bytes
    cipher = DES.new(key, DES.MODE_ECB)
    # Pad plaintext to 8-byte multiple
    padded_len = ((len(plaintext) // 8) + 1) * 8
    padded = plaintext.ljust(padded_len, b'\0')
    return cipher.encrypt(padded)

def des_decrypt(ciphertext: bytes, key: bytes) -> bytes:
    """Decrypt DES ciphertext bytes."""
    key = key.ljust(8, b'X')[:8]
    cipher = DES.new(key, DES.MODE_ECB)
    decrypted = cipher.decrypt(ciphertext)
    # Remove zero padding
    return decrypted.rstrip(b'\0')

def encrypt_flag_bytes(input_str: str) -> bytes:
    """Full multi-layer encryption (ROT13 + DES layers)."""
    # Layer 1: ROT13
    layer1 = rot13(input_str).encode('utf-8')

    # Layer 2: DES with first 8 bytes of input (padded with 'X')
    key2 = input_str[:8].ljust(8, 'X').encode('utf-8')
    layer2 = des_encrypt(layer1, key2)
    layer3=b""
    for i in range (len(layer2)):
        layer3+=bytes([layer2[i]^key2[i%len(key2)]])
    # Layer 3: DES with input[6:10] (4 bytes, padded with 'X')
    key3 = input_str[6:10].ljust(8, 'X').encode('utf-8')
    layer4 = des_encrypt(layer3, key3)
    layer5=b""
    for i in range (len(layer4)):
        layer5+=bytes([layer4[i]^key3[i%len(key3)]])
        
        
    # Layer 4: DES with input[2:6] (4 bytes, padded with 'X')
    key4 = input_str[2:6].ljust(8, 'X').encode('utf-8')
    layer6 = des_encrypt(layer5, key4)
    layer7=b""
    for i in range (len(layer6)):
        layer7+=bytes([layer6[i]^key4[i%len(key4)]])
    return layer7

def to_pybytes(data: bytes) -> str:
    return "b'" + "".join(f"\\x{b:02x}" for b in data) + "'"


def decrypt_flag_bytes(encrypted_bytes: bytes, input_str: str) -> str:
    """Reverse the multi-layer encryption."""
    # Reverse Layer 4: XOR then DES decrypt
    key4 = input_str[2:6].ljust(8, 'X').encode('utf-8')
    layer7 = encrypted_bytes
    layer6 = b""
    for i in range(len(layer7)):
        layer6 += bytes([layer7[i] ^ key4[i % len(key4)]])
    layer5 = des_decrypt(layer6, key4)

    # Reverse Layer 3: XOR then DES decrypt
    key3 = input_str[6:10].ljust(8, 'X').encode('utf-8')
    layer4 = layer5
    layer3 = b""
    for i in range(len(layer4)):
        layer3 += bytes([layer4[i] ^ key3[i % len(key3)]])
    layer2 = des_decrypt(layer3, key3)

    # Reverse Layer 2: XOR then DES decrypt
    key2 = input_str[:8].ljust(8, 'X').encode('utf-8')
    layer2_data = layer2
    layer1_bytes = b""
    for i in range(len(layer2_data)):
        layer1_bytes += bytes([layer2_data[i] ^ key2[i % len(key2)]])
    layer1_bytes = des_decrypt(layer1_bytes, key2)

    # Layer 1: ROT13
    layer1_str = layer1_bytes.decode('utf-8', errors='ignore')
    return rot13(layer1_str)

# ------------------ Demo ------------------
if __name__ == "__main__":
    flag = "Cybercamp{n4tivE_Lib5_aRE_scArYy}"
    encrypted = encrypt_flag_bytes(flag)
    encrypted2= [int(b) for b in encrypted]
    #print(to_pybytes(encrypted))    

    print(f"Encrypted bytes: {encrypted2}")
    with open("output.dat", "wb") as f:
        f.write(encrypted)
    
    decrypted = decrypt_flag_bytes(encrypted, flag)
    print(f"Decrypted: {decrypted}")
