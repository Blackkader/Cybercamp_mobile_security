import hashlib

key = b"i hope you blooded this !"
hash_bytes = hashlib.sha256(key).digest()  # bytes, not string

with open("flag.png", "rb") as f:
    data = bytearray(f.read())

# XOR each byte with the hash
for i in range(len(data)):
    data[i] ^= hash_bytes[i % len(hash_bytes)]

with open("woho.png", "wb") as f:
    f.write(data)
