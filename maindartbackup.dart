import 'dart:async';
import 'package:flutter/material.dart';
import 'package:freerasp/freerasp.dart';
import 'package:root_jailbreak_sniffer/rjsniffer.dart';

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
    'Privileged Access': false,
    'Simulator': false,
    'ADB Enabled': false,
  };

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
    bool amICompromised = await Rjsniffer.amICompromised() ?? false;
    if (amICompromised) {
      setState(() {
        _threats['Privileged Access'] = true;
      });
    }

    bool amIEmulator = await Rjsniffer.amIEmulator() ?? false;
    if (amIEmulator) {
      setState(() {
        _threats['Simulator'] = true;
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
      onPrivilegedAccess: () => setState(() => _threats['Privileged Access'] = true),
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
                            ScaffoldMessenger.of(context).showSnackBar(
                              const SnackBar(
                                content: Text('victory'),
                                duration: Duration(seconds: 2),
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
                      'Flag button appears in $_countdown seconds...',
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
