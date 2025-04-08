import 'package:flutter_test/flutter_test.dart';
import 'package:system_proxy/system_proxy.dart';
import 'package:system_proxy/system_proxy_platform_interface.dart';
import 'package:system_proxy/system_proxy_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockSystemProxyPlatform
    with MockPlatformInterfaceMixin
    implements SystemProxyPlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');
}

void main() {
  // final SystemProxyPlatform initialPlatform = SystemProxyPlatform.instance;

  // test('$MethodChannelSystemProxy is the default instance', () {
  //   expect(initialPlatform, isInstanceOf<MethodChannelSystemProxy>());
  // });

  // test('getPlatformVersion', () async {
  //   SystemProxy systemProxyPlugin = SystemProxy();
  //   MockSystemProxyPlatform fakePlatform = MockSystemProxyPlatform();
  //   SystemProxyPlatform.instance = fakePlatform;

  // });
}
