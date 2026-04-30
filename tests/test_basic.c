/*
 * DiscordHookC
 * Author: Neko The Catgirl (thea@nekothecatgirl.com)
 * Copyright (c) 2026 Neko The Catgirl
 * Description: A C library for Discord Webhooks
 * License: CC Attribution no derivatives (CC BY-ND 4.0)
 */

#include <dishook.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void test_webhook_creation() {
    WebhookData *data = webhook_create();
    assert(data != NULL);
    webhook_destroy(data);
    printf("test_webhook_creation passed\n");
}

void test_embed_creation() {
    EmbedData *embed = embed_create();
    assert(embed != NULL);
    embed_destroy(embed);
    printf("test_embed_creation passed\n");
}

void test_add_embed() {
    WebhookData *data = webhook_create();
    EmbedData *embed = embed_create();
    bool success = try_add_webhook_embed(data, embed);
    assert(success == true);
    webhook_destroy(data);
    printf("test_add_embed passed\n");
}

int main() {
    test_webhook_creation();
    test_embed_creation();
    test_add_embed();
    printf("All tests passed!\n");
    return 0;
}
