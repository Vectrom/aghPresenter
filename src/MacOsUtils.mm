#include "MacOsUtils.h"

#import <Foundation/Foundation.h>

bool isMacOSDarkMode() {
    NSString *interfaceStyle = [[NSUserDefaults standardUserDefaults] stringForKey:@"AppleInterfaceStyle"];

    return [interfaceStyle isEqualToString:@"Dark"];
}