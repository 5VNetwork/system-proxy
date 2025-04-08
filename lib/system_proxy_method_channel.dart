import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'system_proxy_platform_interface.dart';

/// An implementation of [SystemProxyPlatform] that uses method channels.
class MethodChannelSystemProxy extends SystemProxyPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('system_proxy');

  @override
  Future<String?> getPlatformVersion() async {
    final version = await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }
}
