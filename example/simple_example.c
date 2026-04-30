/*
 * DiscordHookC
 * Author: Neko The Catgirl (thea@nekothecatgirl.com)
 * Copyright (c) 2026 Neko The Catgirl
 * Description: A C library for Discord Webhooks
 * License: CC Attribution no derivatives (CC BY-ND 4.0)
 */

#include <dishook.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
#ifdef WEBHOOK_URL
    const char* webhook_url = WEBHOOK_URL;
#else
    const char* webhook_url = getenv("WEBHOOK_URL");
#endif
    if (webhook_url == nullptr) {
        fprintf(stderr, "Error: WEBHOOK_URL environment variable is not set\n");
        return 1;
    }

    WebhookData *data = webhook_create();

    set_webhook_content(data, "Hello, World!");

    send_webhook(webhook_url, data);

    webhook_destroy(data);

    return 0;
}