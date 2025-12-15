import 'dart:async';
import 'dart:convert';
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:freerasp/freerasp.dart';
import 'package:root_jailbreak_sniffer/rjsniffer.dart';
import 'package:jailbreak_root_detection/jailbreak_root_detection.dart';
import 'package:crypto/crypto.dart';
import 'package:encrypt/encrypt.dart' as enc;

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'bypassme ? please ? ',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: const MyHomePage(title: 'Bypass Me'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  final Map<String, bool> _threats = {
    'App Integrity': false,
    'Debug': false,
    'Device Binding': false,
    'Hooks': false,
    'Root / Jailbreak': false,
    'Simulator': false,
    'ADB Enabled': false,
  };

  // New encrypted flag and keys based on the AES script
  static const _encryptedFlagBase64 = 'nVEqYeK4h198tdW+gqemATVfYALTOK4R5rENnz61gC2Rcu+Sol IVsuYMDlzfL4jd';
  static const _key = "67b230geagiouhae89ezrhudcxgd8977OJA";
  static const _ivSource = "aeubg896879@zeaaze";

  String decryptCustom() {
    try {


      // --- Step 1: Base64 decode ---
      List<int> dataBytes;
      try {
        dataBytes = base64.decode(_encryptedFlagBase64);

      } catch (e) {

        rethrow;
      }

      // --- Step 2: Derive key & IV ---
      final aesKey = _deriveAesKey(_key);
      final ivBytes = _deriveIv(_ivSource);


      // --- Step 3: AES‑CBC decrypt ---
      Uint8List decrypted;
      try {
        decrypted = _aesCbcDecrypt(Uint8List.fromList(dataBytes), aesKey, ivBytes);

      } catch (e) {

        rethrow;
      }

      // --- Step 4: Swap bytes back ---
      List<int> arr = List<int>.from(decrypted);
      for (int i = 0; i < arr.length - 1; i += 2) {
        final temp = arr[i];
        arr[i] = arr[i + 1];
        arr[i + 1] = temp;
      }


      // --- Step 5: XOR with repeating key ---
      final keyBytesSeq = utf8.encode(_key);
      List<int> stepB = [];
      for (int i = 0; i < arr.length; i++) {
        stepB.add(arr[i] ^ keyBytesSeq[i % keyBytesSeq.length]);
      }


      // --- Step 6: Right bit rotation ---
      int rightRotate(int val) {
        return ((val >> 3) & 0xFF) | ((val << 5) & 0xFF);
      }
      final stepA = stepB.map((b) => rightRotate(b)).toList();


      // --- Step 7: ROT47 reversal ---
      List<int> original = [];
      for (int b in stepA) {
        if (b >= 33 && b <= 126) {
          original.add(33 + ((b - 33 - 47 + 94) % 94));
        } else {
          original.add(b);
        }
      }


      final result = String.fromCharCodes(original);



      return result;
    } catch (e) {
      return "Decryption failed: $e";
    }
  }


  Uint8List _deriveAesKey(String keyStr) {
    final bytes = utf8.encode(keyStr);
    final digest = sha256.convert(bytes);
    return Uint8List.fromList(digest.bytes.sublist(0, 16));
  }

  Uint8List _deriveIv(String ivStr) {
    final bytes = utf8.encode(ivStr);
    final digest = sha256.convert(bytes);
    return Uint8List.fromList(digest.bytes.sublist(0, 16));
  }

  Uint8List _aesCbcDecrypt(Uint8List data, Uint8List keyBytes, Uint8List ivBytes) {
    final key = enc.Key(keyBytes);
    final iv = enc.IV(ivBytes);

    final encrypter = enc.Encrypter(
      enc.AES(key, mode: enc.AESMode.cbc, padding: 'PKCS7'),
    );

    // MUST wrap raw ciphertext in enc.Encrypted
    final encrypted = enc.Encrypted(data);

    try {
      final decryptedBytes = encrypter.decryptBytes(encrypted, iv: iv);
      return Uint8List.fromList(decryptedBytes);
    } catch (e) {

      throw FormatException("AES‑CBC decryption failed — wrong key/IV or bad ciphertext");
    }
  }

  bool get allGreen => !_threats.values.any((isTriggered) => isTriggered);

  int _countdown = 5;
  bool _isButtonVisible = false;
  Timer? _timer;

  @override
  void initState() {
    super.initState();
    initSecurityChecks();
    startTimer();
  }

  @override
  void dispose() {
    _timer?.cancel();
    super.dispose();
  }

  void startTimer() {
    _timer = Timer.periodic(const Duration(seconds: 1), (timer) {
      if (_countdown > 1) {
        setState(() {
          _countdown--;
        });
      } else {
        _timer?.cancel();
        setState(() {
          _isButtonVisible = true;
        });
      }
    });
  }

  Future<void> initSecurityChecks() async {
    WidgetsFlutterBinding.ensureInitialized();
    await initTalsec();
    await initSniffer();
  }

  Future<void> initSniffer() async {
    bool amIEmulator = await Rjsniffer.amIEmulator() ?? false;
    if (amIEmulator) {
      setState(() {
        _threats['Simulator'] = true;
      });
    } else {
      setState(() {
        _threats['Simulator'] = false;
      });
    }
  }

  Future<void> initTalsec() async {
    final config = TalsecConfig(
      androidConfig: AndroidConfig(
        packageName: 'io.securinets.bypassme',
        signingCertHashes: ['1OirqkdsvogUFvSE5ib95sSoeCkYthSFYyMSgTTe910='],
        supportedStores: [],
      ),
      watcherMail: 'your_email_address@example.com',
      isProd: true,
    );

    final callback = ThreatCallback(
      onAppIntegrity: () => setState(() => _threats['App Integrity'] = true),
      onDebug: () => setState(() => _threats['Debug'] = true),
      onDeviceBinding: () => setState(() => _threats['Device Binding'] = true),
      onPrivilegedAccess : () => setState(() => _threats['Root / Jailbreak'] = true),
      onSimulator: () => setState(() => _threats['Simulator'] = true),
      onHooks: () => setState(() => _threats['Hooks'] = true),
      onADBEnabled: () => setState(() => _threats['ADB Enabled'] = true),
    );

    await Talsec.instance.start(config);
    Talsec.instance.attachListener(callback);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      extendBodyBehindAppBar: true,
      appBar: AppBar(
        backgroundColor: Colors.transparent,
        elevation: 0,
        title: Text(widget.title,
            style: const TextStyle(
                color: Colors.white, fontWeight: FontWeight.bold)),
      ),
      body: Container(
        decoration: const BoxDecoration(
          image: DecorationImage(
            image: AssetImage("assets/wallpaper.png"),
            fit: BoxFit.cover,
          ),
        ),
        child: Center(
          child: Container(
            color: Colors.black.withOpacity(0.5),
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: <Widget>[
                ..._threats.entries.map((entry) {
                  return Padding(
                    padding: const EdgeInsets.symmetric(
                        vertical: 8.0, horizontal: 24.0),
                    child: Row(
                      mainAxisAlignment: MainAxisAlignment.spaceBetween,
                      children: [
                        Text(entry.key,
                            style: const TextStyle(
                                fontSize: 18,
                                color: Colors.white,
                                fontWeight: FontWeight.bold)),
                        Container(
                          width: 24,
                          height: 24,
                          decoration: BoxDecoration(
                            color: entry.value ? Colors.red : Colors.green,
                            shape: BoxShape.circle,
                          ),
                        ),
                      ],
                    ),
                  );
                }).toList(),
                const SizedBox(height: 32),
                if (_isButtonVisible)
                  ElevatedButton(
                    style: ElevatedButton.styleFrom(
                        padding: const EdgeInsets.symmetric(
                            horizontal: 48, vertical: 16)),
                    onPressed: allGreen
                        ? () {
                            final decryptedFlag = decryptCustom();
                            ScaffoldMessenger.of(context).showSnackBar(
                              SnackBar(
                                content: Text(decryptedFlag),
                                duration: const Duration(seconds: 10),
                              ),
                            );
                          }
                        : null,
                    child: const Text('FLAG!', style: TextStyle(fontSize: 20)),
                  )
                else
                  Padding(
                    padding: const EdgeInsets.all(8.0),
                    child: Text(
                      "Flag button appears in $_countdown seconds...",
                      style: const TextStyle(
                          fontSize: 20,
                          color: Colors.white,
                          fontWeight: FontWeight.bold),
                    ),
                  ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
