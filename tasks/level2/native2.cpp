#include <jni.h>
#include <string>
#include <vector>
#include <cstring>
#include <cstdint>
#include <algorithm>
#include <cmath>
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
    
    // Apply PKCS7 padding
    std::string padded = pkcs7_pad(plaintext, 16);
    
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
    
    // Layer 1: XOR encryption
    std::string layer1 = xor_encrypt(input, xor_key);
    
    // Layer 2: AES encryption
    std::string layer2 = aes_encrypt(layer1, aes_key);
    
    return std::vector<uint8_t>(layer2.begin(), layer2.end());
}

extern "C" JNIEXPORT jbyteArray JNICALL
Java_io_securinets_level2_heavynativeflag_magic(JNIEnv* env, jobject, jstring input) {
    const char* nativeString = env->GetStringUTFChars(input, nullptr);
    std::string str(nativeString);
    env->ReleaseStringUTFChars(input, nativeString);

    std::vector<uint8_t> encrypted_bytes = encrypt_flag_bytes(str);

    jbyteArray output = env->NewByteArray(encrypted_bytes.size());
    env->SetByteArrayRegion(output, 0, encrypted_bytes.size(),
                            reinterpret_cast<jbyte*>(encrypted_bytes.data()));
    return output;
}

// ================= DECOY FUNCTIONS =================

extern "C" JNIEXPORT jint JNICALL
Java_io_securinets_level2_heavynativeflag_processMatrix(JNIEnv*, jobject, jint a, jint b) {
    return a + b;
}

extern "C" JNIEXPORT jstring JNICALL
Java_io_securinets_level2_heavynativeflag_transformData(JNIEnv* env, jobject, jstring str) {
    const char *nativeString = env->GetStringUTFChars(str, 0);
    std::string result = "Processed: " + std::string(nativeString);
    env->ReleaseStringUTFChars(str, nativeString);
    return env->NewStringUTF(result.c_str());
}

extern "C" JNIEXPORT jdouble JNICALL
Java_io_securinets_level2_heavynativeflag_calculateWave(JNIEnv*, jobject, jdouble x) {
    return std::sin(x) * std::cos(x);
}

extern "C" JNIEXPORT jintArray JNICALL
Java_io_securinets_level2_heavynativeflag_doubleValues(JNIEnv* env, jobject, jintArray arr) {
    jsize length = env->GetArrayLength(arr);
    jint *elements = env->GetIntArrayElements(arr, 0);
    
    for (jsize i = 0; i < length; i++) {
        elements[i] = elements[i] * 2;
    }
    
    env->ReleaseIntArrayElements(arr, elements, 0);
    return arr;
}

extern "C" JNIEXPORT jlong JNICALL
Java_io_securinets_level2_heavynativeflag_factorial(JNIEnv*, jobject, jint n) {
    if (n <= 1) return 1;
    return n * Java_io_securinets_level2_heavynativeflag_factorial(nullptr, nullptr, n - 1);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_io_securinets_level2_heavynativeflag_validate(JNIEnv*, jobject, jstring str) {
    return JNI_TRUE;
}

extern "C" JNIEXPORT jint JNICALL
Java_io_securinets_level2_heavynativeflag_compute(JNIEnv*, jobject, jint a, jint b, jint c) {
    return a * b + c;
}

extern "C" JNIEXPORT jstring JNICALL
Java_io_securinets_level2_heavynativeflag_mergeStrings(JNIEnv* env, jobject, jstring a, jstring b) {
    const char *strA = env->GetStringUTFChars(a, 0);
    const char *strB = env->GetStringUTFChars(b, 0);
    std::string result = std::string(strA) + "|" + std::string(strB);
    env->ReleaseStringUTFChars(a, strA);
    env->ReleaseStringUTFChars(b, strB);
    return env->NewStringUTF(result.c_str());
}

extern "C" JNIEXPORT jdouble JNICALL
Java_io_securinets_level2_heavynativeflag_exponential(JNIEnv*, jobject, jdouble a, jdouble b) {
    return std::pow(a, b);
}

extern "C" JNIEXPORT jint JNICALL
Java_io_securinets_level2_heavynativeflag_getAnswer(JNIEnv*, jobject, jintArray arr) {
    return 42;
}

extern "C" JNIEXPORT jstring JNICALL
Java_io_securinets_level2_heavynativeflag_formatNumber(JNIEnv* env, jobject, jint num) {
    std::string result = "Number: " + std::to_string(num);
    return env->NewStringUTF(result.c_str());
}

extern "C" JNIEXPORT jint JNICALL
Java_io_securinets_level2_heavynativeflag_remainder(JNIEnv*, jobject, jint a, jint b) {
    return a % b;
}

extern "C" JNIEXPORT jdouble JNICALL
Java_io_securinets_level2_heavynativeflag_logarithm(JNIEnv*, jobject, jdouble x) {
    return std::log(x);
}

extern "C" JNIEXPORT jint JNICALL
Java_io_securinets_level2_heavynativeflag_fibonacci(JNIEnv*, jobject, jint n) {
    if (n <= 1) return n;
    return Java_io_securinets_level2_heavynativeflag_fibonacci(nullptr, nullptr, n - 1) + 
           Java_io_securinets_level2_heavynativeflag_fibonacci(nullptr, nullptr, n - 2);
}

extern "C" JNIEXPORT jstring JNICALL
Java_io_securinets_level2_heavynativeflag_reverseString(JNIEnv* env, jobject, jstring str) {
    const char *nativeString = env->GetStringUTFChars(str, 0);
    std::string result = std::string(nativeString);
    std::reverse(result.begin(), result.end());
    env->ReleaseStringUTFChars(str, nativeString);
    return env->NewStringUTF(result.c_str());
}

extern "C" JNIEXPORT jint JNICALL
Java_io_securinets_level2_heavynativeflag_evaluate(JNIEnv*, jobject, jint a, jint b, jint c, jint d) {
    return (a + b) * (c - d);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_io_securinets_level2_heavynativeflag_isPrime(JNIEnv*, jobject, jint num) {
    if (num < 2) return JNI_FALSE;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return JNI_FALSE;
    }
    return JNI_TRUE;
}

extern "C" JNIEXPORT jint JNICALL
Java_io_securinets_level2_heavynativeflag_checkSum(JNIEnv*, jobject, jintArray arr) {
    return 0;
}

extern "C" JNIEXPORT jstring JNICALL
Java_io_securinets_level2_heavynativeflag_repeat(JNIEnv* env, jobject, jstring str, jint times) {
    const char *nativeString = env->GetStringUTFChars(str, 0);
    std::string result;
    for (int i = 0; i < times; i++) {
        result += nativeString;
    }
    env->ReleaseStringUTFChars(str, nativeString);
    return env->NewStringUTF(result.c_str());
}

extern "C" JNIEXPORT jdouble JNICALL
Java_io_securinets_level2_heavynativeflag_distance(JNIEnv*, jobject, jdouble x, jdouble y) {
    return std::sqrt(x * x + y * y);
}

extern "C" JNIEXPORT jint JNICALL
Java_io_securinets_level2_heavynativeflag_bitwiseAnd(JNIEnv*, jobject, jint a, jint b) {
    return a & b;
}

extern "C" JNIEXPORT jstring JNICALL
Java_io_securinets_level2_heavynativeflag_wrapString(JNIEnv* env, jobject, jstring str) {
    const char *nativeString = env->GetStringUTFChars(str, 0);
    std::string result = "ENC_" + std::string(nativeString) + "_DEC";
    env->ReleaseStringUTFChars(str, nativeString);
    return env->NewStringUTF(result.c_str());
}

extern "C" JNIEXPORT jint JNICALL
Java_io_securinets_level2_heavynativeflag_bitwiseOr(JNIEnv*, jobject, jint a, jint b) {
    return a | b;
}

extern "C" JNIEXPORT jdouble JNICALL
Java_io_securinets_level2_heavynativeflag_euler(JNIEnv*, jobject, jdouble x) {
    return std::exp(x);
}

extern "C" JNIEXPORT jint JNICALL
Java_io_securinets_level2_heavynativeflag_sumSeries(JNIEnv*, jobject, jint n) {
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += i;
    }
    return sum;
}

extern "C" JNIEXPORT jstring JNICALL
Java_io_securinets_level2_heavynativeflag_concatenate(JNIEnv* env, jobject, jstring a, jstring b, jstring c) {
    const char *strA = env->GetStringUTFChars(a, 0);
    const char *strB = env->GetStringUTFChars(b, 0);
    const char *strC = env->GetStringUTFChars(c, 0);
    std::string result = std::string(strA) + "-" + std::string(strB) + "-" + std::string(strC);
    env->ReleaseStringUTFChars(a, strA);
    env->ReleaseStringUTFChars(b, strB);
    env->ReleaseStringUTFChars(c, strC);
    return env->NewStringUTF(result.c_str());
}

extern "C" JNIEXPORT jint JNICALL
Java_io_securinets_level2_heavynativeflag_bitwiseXor(JNIEnv*, jobject, jint a, jint b) {
    return a ^ b;
}

extern "C" JNIEXPORT jdouble JNICALL
Java_io_securinets_level2_heavynativeflag_linearCombination(JNIEnv*, jobject, jdouble a, jdouble b, jdouble c) {
    return a * b + c;
}

extern "C" JNIEXPORT jint JNICALL
Java_io_securinets_level2_heavynativeflag_square(JNIEnv*, jobject, jint n) {
    return n * n;
}

extern "C" JNIEXPORT jstring JNICALL
Java_io_securinets_level2_heavynativeflag_encodeBytes(JNIEnv* env, jobject, jstring str) {
    const char *nativeString = env->GetStringUTFChars(str, 0);
    std::string result;
    for (char c : std::string(nativeString)) {
        result += std::to_string((int)c) + ".";
    }
    env->ReleaseStringUTFChars(str, nativeString);
    return env->NewStringUTF(result.c_str());
}