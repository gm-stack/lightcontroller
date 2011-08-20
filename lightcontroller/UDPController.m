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

- (id)init
{
    self = [super init];
    if (self) {
        [self setColor:[NSColor colorWithCalibratedRed:1.0 green:1.0 blue:1.0 alpha:1.0]];
    }
    
    return self;
}

- (IBAction)colorChanged:(id)sender {
    NSColor *currentColor = [sender color];
    [self setColor:currentColor];
}

- (void)setColor:(NSColor*)thecolor {
    float red = [thecolor redComponent];
    float green = [thecolor greenComponent];
    float blue = [thecolor blueComponent];
    char color[] = {0,0,0};
    color[0] = (red * 255.0);
    color[1] = (green * 255.0);
    color[2] = (blue * 255.0);
    
    struct sockaddr_in si;
    int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    si.sin_family = AF_INET;
    si.sin_port = htons(54127);
    inet_aton("172.22.0.12", &si.sin_addr);
    sendto(s, color, 3, 0, &si, sizeof(si));
}

@end
