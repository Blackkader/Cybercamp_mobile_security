from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad

def xor_encrypt(data: bytes, key: bytes) -> bytes:
    """XOR encrypt data with a repeating key."""
    result = bytearray()
    for i in range(len(data)):
        result.append(data[i] ^ key[i % len(key)])
    return bytes(result)

def xor_decrypt(data: bytes, key: bytes) -> bytes:
    """XOR decrypt (same as encrypt for XOR)."""
    return xor_encrypt(data, key)

def aes_encrypt(plaintext: bytes, key: bytes) -> bytes:
    """Encrypt bytes with AES-128 in ECB mode with PKCS7 padding."""
    # Prepare 16-byte key (AES-128)
    key = key.ljust(16, b'X')[:16]
    cipher = AES.new(key, AES.MODE_ECB)
    # Apply PKCS7 padding
    padded = pad(plaintext, AES.block_size)
    return cipher.encrypt(padded)

def aes_decrypt(ciphertext: bytes, key: bytes) -> bytes:
    """Decrypt AES-128 ciphertext bytes."""
    key = key.ljust(16, b'X')[:16]
    cipher = AES.new(key, AES.MODE_ECB)
    decrypted = cipher.decrypt(ciphertext)
    # Remove PKCS7 padding
    return unpad(decrypted, AES.block_size)

def encrypt_flag_bytes(input_str: str) -> bytes:
    """2-layer encryption: XOR + AES."""
    xor_key = b"SECRETTT123"
    aes_key = b"SECRETWORKSHOP"
    
    # Layer 1: XOR encryption
    layer1 = xor_encrypt(input_str.encode('utf-8'), xor_key)
    
    # Layer 2: AES encryption
    layer2 = aes_encrypt(layer1, aes_key)
    
    return layer2

def decrypt_flag_bytes(encrypted_bytes: bytes, input_str: str = None) -> str:
    """Reverse the 2-layer encryption."""
    xor_key = b"SECRETTT123"
    aes_key = b"SECRETWORKSHOP"
    
    # Reverse Layer 2: AES decrypt
    layer1 = aes_decrypt(encrypted_bytes, aes_key)
    
    # Reverse Layer 1: XOR decrypt
    plaintext = xor_decrypt(layer1, xor_key)
    
    return plaintext.decode('utf-8', errors='ignore')

def to_pybytes(data: bytes) -> str:
    return "b'" + "".join(f"\\x{b:02x}" for b in data) + "'"

# ------------------ Demo ------------------
if __name__ == "__main__":
    flag = "Cybercamp{n4tivE_Lib5_aRE_scArYy}"
    encrypted = encrypt_flag_bytes(flag)
    encrypted2 = [int(b) for b in encrypted]
    
    # Show both unsigned and signed representations
    encrypted_signed = [b if b < 128 else b - 256 for b in encrypted2]
    
    print(f"Encrypted bytes (unsigned): {encrypted2}")
    print(f"Encrypted bytes (signed):   {encrypted_signed}")
    print(f"Encrypted hex: {to_pybytes(encrypted)}")
    
    with open("output.dat", "wb") as f:
        f.write(encrypted)
    
    # Decrypt without needing the original input
    decrypted = decrypt_flag_bytes(encrypted)
    print(f"Decrypted: {decrypted}")
    
    # Verify
    assert decrypted == flag, "Decryption failed!"
    print("✓ Encryption/Decryption verified!")