import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'system_proxy_method_channel.dart';

abstract class SystemProxyPlatform extends PlatformInterface {
  /// Constructs a SystemProxyPlatform.
  SystemProxyPlatform() : super(token: _token);

  static final Object _token = Object();

  static SystemProxyPlatform _instance = MethodChannelSystemProxy();

  /// The default instance of [SystemProxyPlatform] to use.
  ///
  /// Defaults to [MethodChannelSystemProxy].
  static SystemProxyPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [SystemProxyPlatform] when
  /// they register themselves.
  static set instance(SystemProxyPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }
}
