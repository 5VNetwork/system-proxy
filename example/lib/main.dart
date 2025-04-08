import 'package:flutter/material.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:system_proxy/messages.g.dart';
import 'package:system_proxy/system_proxy.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  final _systemProxyPlugin = SystemProxy();
  final _httpProxyController = TextEditingController();
  final _socksProxyController = TextEditingController();
  final _bypassListController = TextEditingController();
  String _errorText = '';
  @override
  void initState() {
    super.initState();
    _httpProxyController.text = 'http://127.0.0.1:7890';
    _socksProxyController.text = 'socks5://127.0.0.1:7890';
    _bypassListController.text = 'localhost;127.0.0.1';
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: Column(
            children: [
              TextField(
                controller: _httpProxyController,
                decoration: const InputDecoration(labelText: 'HTTP Proxy'),
              ),
              TextField(
                controller: _socksProxyController,
                decoration: const InputDecoration(labelText: 'SOCKS Proxy'),
              ),
              TextField(
                controller: _bypassListController,
                decoration: const InputDecoration(labelText: 'Bypass List'),
              ),
              ElevatedButton(
                onPressed: () async {
                  try {
                    await _systemProxyPlugin.setSystemProxy(SystemProxySettings(
                      httpProxy: _httpProxyController.text.isEmpty
                          ? null
                          : _httpProxyController.text,
                      socksProxy: _socksProxyController.text.isEmpty
                          ? null
                          : _socksProxyController.text,
                      bypass: _bypassListController.text.isEmpty
                          ? null
                          : _bypassListController.text,
                    ));
                  } catch (e) {
                    print(e);
                    setState(() {
                      _errorText = e.toString();
                    });
                  }
                },
                child: const Text('Set System Proxy'),
              ),
              ElevatedButton(
                onPressed: () async {
                  try {
                    await _systemProxyPlugin.removeSystemProxy();
                  } catch (e) {
                    print(e);
                    setState(() {
                      _errorText = e.toString();
                    });
                  }
                },
                child: const Text('Remove System Proxy'),
              ),
              const SizedBox(height: 20),
              Text(_errorText),
            ],
          ),
        ),
      ),
    );
  }
}
