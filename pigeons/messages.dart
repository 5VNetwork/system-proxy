import 'package:pigeon/pigeon.dart';

@ConfigurePigeon(PigeonOptions(
    dartOut: 'lib/messages.g.dart',
    dartOptions: DartOptions(),
    cppOptions: CppOptions(namespace: 'system_proxy'),
    cppHeaderOut: 'windows/messages.g.h',
    cppSourceOut: 'windows/messages.g.cpp'))
@HostApi()
abstract class SystemProxyHostApi {
  void setSystemProxy(SystemProxySettings settings);
  void removeSystemProxy();
}

class SystemProxySettings {
  SystemProxySettings({
    this.httpProxy,
    this.socksProxy,
    this.bypass,
  });
  String? httpProxy;
  String? socksProxy;
  String? bypass;
}
