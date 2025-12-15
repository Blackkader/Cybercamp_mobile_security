#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import base64

# =========================
# ROT10 (ROT13-style, but +10)
# =========================

def rot10(s):
    out = []
    for c in s:
        o = ord(c)

        # a-z
        if 97 <= o <= 122:
            out.append(chr((o - 97 + 10) % 26 + 97))

        # A-Z
        elif 65 <= o <= 90:
            out.append(chr((o - 65 + 10) % 26 + 65))

        # 0-9
        elif 48 <= o <= 57:
            out.append(chr((o - 48 + 10) % 10 + 48))

        else:
            out.append(c)

    return ''.join(out)


def rot10_reverse(s):
    out = []
    for c in s:
        o = ord(c)

        # a-z
        if 97 <= o <= 122:
            out.append(chr((o - 97 - 10) % 26 + 97))

        # A-Z
        elif 65 <= o <= 90:
            out.append(chr((o - 65 - 10) % 26 + 65))

        # 0-9
        elif 48 <= o <= 57:
            out.append(chr((o - 48 - 10) % 10 + 48))

        else:
            out.append(c)

    return ''.join(out)

# =========================
# Bit rotation helpers
# =========================

def rotl(byte, shift=1):
    return ((byte << shift) & 0xFF) | (byte >> (8 - shift))


def rotr(byte, shift=1):
    return (byte >> shift) | ((byte << (8 - shift)) & 0xFF)

# =========================
# Encryption / Decryption
# =========================

def encrypt_password(login, password):
    key = login.encode("utf-8")
    data = password.encode("utf-8")

    # 1️⃣ XOR
    xored = bytes(data[i] ^ key[i % len(key)] for i in range(len(data)))

    # 2️⃣ Base64
    b64 = base64.b64encode(xored).decode("ascii")

    # 3️⃣ ROT10
    rot = rot10(b64)

    # 4️⃣ Left bit shift
    encrypted = [rotl(ord(c), 1) for c in rot]

    return encrypted


def decrypt_password(login, encrypted):
    key = login.encode("utf-8")

    # 4️⃣ Reverse left bit shift
    rot = ''.join(chr(rotr(b, 1)) for b in encrypted)

    # 3️⃣ Reverse ROT10
    b64 = rot10_reverse(rot)

    # 2️⃣ Base64 decode
    xored = base64.b64decode(b64)

    # 1️⃣ Reverse XOR
    original = bytes(xored[i] ^ key[i % len(key)] for i in range(len(xored)))

    return original.decode("utf-8")

# =========================
# Demo
# =========================

if __name__ == "__main__":
    login = "logedin"
    password = "i hope you blooded this !"

    encrypted = encrypt_password(login, password)
    print("Encrypted:", encrypted)

    decrypted = decrypt_password(login, encrypted)
    print("Decrypted:", decrypted)
