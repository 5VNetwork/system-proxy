import 'package:system_proxy/messages.g.dart';

import 'system_proxy_platform_interface.dart';

class SystemProxy {
  static final SystemProxyHostApi _hostApi = SystemProxyHostApi();

  static Future<void> setSystemProxy(SystemProxySettings settings) {
    return _hostApi.setSystemProxy(settings);
  }

  static Future<void> removeSystemProxy() {
    return _hostApi.removeSystemProxy();
  }
}
