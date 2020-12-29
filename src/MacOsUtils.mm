#include "MacOsUtils.h"

bool isMacOSDarkMode() {
    NSString *interfaceStyle = [[NSUserDefaults standardUserDefaults] stringForKey:@"AppleInterfaceStyle"];

    return [interfaceStyle isEqualToString:@"Dark"];
}