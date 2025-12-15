from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
import hashlib
import base64
def derive_aes_key(key_str: str) -> bytes:
    """ Derive a 16‑byte AES key (AES‑128) from a SHA‑256 hash of a string """
    sha = hashlib.sha256(key_str.encode("utf‑8")).digest()
    return sha[:16]  # AES‑128 key

def derive_iv(iv_str: str) -> bytes:
    """ Derive a 16‑byte IV from a SHA‑256 hash of a string (deterministic) """
    sha = hashlib.sha256(iv_str.encode("utf‑8")).digest()
    return sha[:16]  # 16‑byte IV for AES‑CBC

def aes_cbc_encrypt(data: bytes, key_bytes: bytes, iv_bytes: bytes) -> bytes:
    cipher = AES.new(key_bytes, AES.MODE_CBC, iv=iv_bytes)
    return cipher.encrypt(pad(data, AES.block_size))

def aes_cbc_decrypt(enc_data: bytes, key_bytes: bytes, iv_bytes: bytes) -> bytes:
    cipher = AES.new(key_bytes, AES.MODE_CBC, iv=iv_bytes)
    return unpad(cipher.decrypt(enc_data), AES.block_size)


def encrypt_custom(input_str: str, key: str, iv_source: str) -> bytes:
    # Convert plaintext to list of byte values
    bytes_data = [ord(c) for c in input_str]

    # Step A — ROT47
    stepA = []
    for b in bytes_data:
        if 33 <= b <= 126:
            stepA.append(33 + ((b - 33 + 47) % 94))
        else:
            stepA.append(b)

    # Step B — left bit rotation
    def left_rotate(val):
        return ((val << 3) & 0xFF) | (val >> 5)
    stepB = [left_rotate(b) for b in stepA]

    # Step C — XOR with repeating key
    key_bytes_seq = key.encode("utf‑8")
    stepC = [b ^ key_bytes_seq[i % len(key_bytes_seq)] for i, b in enumerate(stepB)]

    # Step D — swap bytes in pairs
    for i in range(0, len(stepC) - 1, 2):
        stepC[i], stepC[i + 1] = stepC[i + 1], stepC[i]

    # AES‑CBC encryption with deterministic key and IV
    aes_key = derive_aes_key(key)
    iv_bytes = derive_iv(iv_source)
    encrypted = aes_cbc_encrypt(bytes(stepC), aes_key, iv_bytes)

    return encrypted


def decrypt_custom(data_bytes: bytes, key: str, iv_source: str) -> str:
    # Derive key and IV
    aes_key = derive_aes_key(key)
    iv_bytes = derive_iv(iv_source)

    # AES‑CBC decrypt
    decrypted_bytes = aes_cbc_decrypt(data_bytes, aes_key, iv_bytes)

    # Reverse Step D — swap bytes back
    arr = list(decrypted_bytes)
    for i in range(0, len(arr) - 1, 2):
        arr[i], arr[i + 1] = arr[i + 1], arr[i]

    # Reverse Step C — XOR with repeating key
    key_bytes_seq = key.encode("utf‑8")
    stepB = [b ^ key_bytes_seq[i % len(key_bytes_seq)] for i, b in enumerate(arr)]

    # Reverse Step B — right bit rotation
    def right_rotate(val):
        return ((val >> 3) & 0xFF) | ((val << 5) & 0xFF)
    stepA = [right_rotate(b) for b in stepB]

    # Reverse Step A — ROT47
    original = []
    for b in stepA:
        if 33 <= b <= 126:
            original.append(33 + ((b - 33 - 47 + 94) % 94))
        else:
            original.append(b)

    return "".join(chr(x) for x in original)


# Example usage
if __name__ == "__main__":
    plaintext = "Cybercamp{y0u_n4il3d_tH1s_One!_G0AT_fR1dA}"
    key = "67b230geagiouhae89ezrhudcxgd8977OJA"
    iv_source = "aeubg896879@zeaaze"

    encrypted = encrypt_custom(plaintext, key, iv_source)
    encrypted_b64 = base64.b64encode(encrypted).decode("utf‑8")
    print("Encrypted (Base64):", encrypted_b64)
    
    decrypted = decrypt_custom(encrypted, key, iv_source)
    print("Decrypted:", decrypted)
