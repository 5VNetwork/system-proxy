//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <system_proxy/system_proxy_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) system_proxy_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "SystemProxyPlugin");
  system_proxy_plugin_register_with_registrar(system_proxy_registrar);
}
