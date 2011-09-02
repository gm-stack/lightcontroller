//
//  UDPController.m
//  lightcontroller
//
//  Created by gm on 20/08/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "UDPController.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>


@implementation UDPController
@synthesize prefsWindow;
@synthesize IPAddrField;

- (id)init
{
    self = [super init];
    if (self) {
        s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        si.sin_family = AF_INET;
        si.sin_port = htons(54127);
        [self setColor:[NSColor colorWithCalibratedRed:1.0 green:1.0 blue:1.0 alpha:1.0]];
    }
    
    return self;
}

- (IBAction)colorChanged:(id)sender {
    NSColor *currentColor = [sender color];
    [self setColor:currentColor];
}

- (void)setColor:(NSColor*)thecolor {
    NSColor *convertedColor = [thecolor colorUsingColorSpaceName:NSCalibratedRGBColorSpace];
    float red = [convertedColor redComponent];
    float green = [convertedColor greenComponent];
    float blue = [convertedColor blueComponent];
    char color[] = {0,0,0};
    color[0] = (red * 255.0);
    color[1] = (green * 255.0);
    color[2] = (blue * 255.0);
    
    NSString *host = [IPAddrField stringValue];
    if (host == nil) return;
    
    inet_aton([host UTF8String], &si.sin_addr);
    
    sendto(s, color, 3, 0, &si, sizeof(si));
}

- (IBAction) showPrefsWindow:(id)sender {
    [prefsWindow makeKeyAndOrderFront:self];
    NSUserDefaults *def = [NSUserDefaults standardUserDefaults];
    NSString *ipaddr = [def objectForKey:@"ipaddress"];
    if (ipaddr == nil) return;
    [IPAddrField setStringValue:ipaddr];
    [def synchronize];
}

- (IBAction) savePrefs:(id)sender {
    NSUserDefaults *def = [NSUserDefaults standardUserDefaults];
    [def setObject:[IPAddrField stringValue] forKey:@"ipaddress"];
    [def synchronize];
}

@end
