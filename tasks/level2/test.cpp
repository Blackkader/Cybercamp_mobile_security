#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdint>
#include <openssl/aes.h>

// Layer 1: XOR encryption with fixed key
std::string xor_encrypt(const std::string &input, const std::string &key) {
    std::string output = input;
    for (size_t i = 0; i < input.size(); ++i) {
        output[i] = input[i] ^ key[i % key.size()];
    }
    return output;
}

// PKCS7 Padding
std::string pkcs7_pad(const std::string& data, size_t block_size) {
    size_t padding_length = block_size - (data.size() % block_size);
    std::string padded = data;
    for (size_t i = 0; i < padding_length; i++) {
        padded.push_back(static_cast<char>(padding_length));
    }
    return padded;
}

// Layer 2: AES-128 ECB encryption with PKCS7 padding
std::string aes_encrypt(const std::string& plaintext, const std::string& key) {
    // Prepare 16-byte key (AES-128)
    unsigned char aes_key[16];
    memset(aes_key, 'X', 16);  // Fill with 'X' first
    memcpy(aes_key, key.c_str(), std::min(key.size(), size_t(16)));
    
    std::cout << "AES Key: ";
    for (int i = 0; i < 16; i++) {
        std::cout << aes_key[i];
    }
    std::cout << std::endl;
    
    // Apply PKCS7 padding
    std::string padded = pkcs7_pad(plaintext, 16);
    
    std::cout << "After PKCS7 padding (size=" << padded.size() << "): ";
    for (size_t i = 0; i < padded.size(); i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << (int)(unsigned char)padded[i] << " ";
    }
    std::cout << std::dec << std::endl;
    
    // Encrypt using AES-128-ECB
    std::vector<unsigned char> output(padded.size());
    AES_KEY enc_key;
    AES_set_encrypt_key(aes_key, 128, &enc_key);
    
    for (size_t i = 0; i < padded.size(); i += 16) {
        AES_encrypt(reinterpret_cast<const unsigned char*>(padded.data() + i), 
                    output.data() + i, 
                    &enc_key);
    }
    
    return std::string(reinterpret_cast<char*>(output.data()), output.size());
}

// Combined 2-layer encryption
std::vector<uint8_t> encrypt_flag_bytes(const std::string& input) {
    const std::string xor_key = "SECRETTT123";
    const std::string aes_key = "SECRETWORKSHOP";
    
    std::cout << "Input: " << input << std::endl;
    std::cout << "Input length: " << input.size() << std::endl;
    
    // Layer 1: XOR encryption
    std::string layer1 = xor_encrypt(input, xor_key);
    
    std::cout << "After XOR: ";
    for (size_t i = 0; i < layer1.size(); i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << (int)(unsigned char)layer1[i] << " ";
    }
    std::cout << std::dec << std::endl;
    
    // Layer 2: AES encryption
    std::string layer2 = aes_encrypt(layer1, aes_key);
    
    return std::vector<uint8_t>(layer2.begin(), layer2.end());
}

int main() {
    std::string flag = "Cybercamp{n4tivE_Lib5_aRE_scArYy}";
    
    std::vector<uint8_t> encrypted = encrypt_flag_bytes(flag);
    
    std::cout << "\nFinal encrypted (unsigned): [";
    for (size_t i = 0; i < encrypted.size(); i++) {
        std::cout << (int)encrypted[i];
        if (i < encrypted.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    
    std::cout << "\nFinal encrypted (signed): [";
    for (size_t i = 0; i < encrypted.size(); i++) {
        int8_t signed_val = static_cast<int8_t>(encrypted[i]);
        std::cout << (int)signed_val;
        if (i < encrypted.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    
    return 0;
}

// Compile with: g++ -o test test.cpp -lssl -lcrypto
// Run: ./test