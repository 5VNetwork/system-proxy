#ifndef FLUTTER_PLUGIN_SYSTEM_PROXY_PLUGIN_H_
#define FLUTTER_PLUGIN_SYSTEM_PROXY_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

#include "messages.g.h"

namespace system_proxy
{

    class SystemProxyPlugin : public flutter::Plugin, public SystemProxyHostApi
    {
    public:
        static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

        SystemProxyPlugin();

        virtual ~SystemProxyPlugin();

        // Disallow copy and assign.
        SystemProxyPlugin(const SystemProxyPlugin &) = delete;
        SystemProxyPlugin &operator=(const SystemProxyPlugin &) = delete;

        // Called when a method is called on this plugin's channel from Dart.
  /*      void HandleMethodCall(
            const flutter::MethodCall<flutter::EncodableValue> &method_call,
            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);*/

        std::optional<FlutterError> SetSystemProxy(const SystemProxySettings &settings) override;
        std::optional<FlutterError> RemoveSystemProxy() override;
    };

} // namespace system_proxy

#endif // FLUTTER_PLUGIN_SYSTEM_PROXY_PLUGIN_H_
