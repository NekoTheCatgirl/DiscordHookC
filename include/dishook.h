/**
 * @file dishook.h
 * @brief Main header file for the DiscordHookC library.
 *
 * DiscordHookC
 * Author: Neko The Catgirl (thea@nekothecatgirl.com)
 * Copyright (c) 2026 Neko The Catgirl
 * Description: A C library for Discord Webhooks
 * License: CC Attribution no derivatives (CC BY-ND 4.0)
 */

#ifndef DISCORDHOOKC_DISHOOK_H
#define DISCORDHOOKC_DISHOOK_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Opaque structure representing Discord webhook data.
 * 
 * Use the helper methods to populate private data.
 */
typedef struct WebhookData WebhookData;

/**
 * @brief Opaque structure representing Discord embed data.
 * 
 * Use the helper methods to populate private data.
 */
typedef struct EmbedData EmbedData;

/**
 * @brief Create a new webhook message.
 * 
 * Important: call webhook_destroy when done with it.
 * @see webhook_destroy()
 * @return A pointer to a new WebhookData instance, or NULL on failure.
 */
WebhookData *webhook_create();

/**
 * @brief Set the content (message body) of the webhook.
 * @param data Pointer to WebhookData.
 * @param content The text content of the message.
 */
void set_webhook_content(WebhookData *data, const char *content);

/**
 * @brief Set the username of the webhook.
 * @param data Pointer to WebhookData.
 * @param username The override username for the webhook.
 */
void set_webhook_username(WebhookData *data, const char *username);

/**
 * @brief Set the avatar URL of the webhook.
 * @param data Pointer to WebhookData.
 * @param avatar_url The override avatar URL for the webhook.
 */
void set_webhook_avatar_url(WebhookData *data, const char *avatar_url);

/**
 * @brief Set whether the webhook message uses Text-To-Speech (TTS).
 * @param data Pointer to WebhookData.
 * @param tts True to enable TTS, false otherwise.
 */
void set_webhook_tts(WebhookData *data, bool tts);

/**
 * @brief Try to add an embed to the webhook.
 * @param data Pointer to WebhookData.
 * @param embed Pointer to EmbedData to add.
 * @return True if successfully added, false otherwise (e.g., limit reached).
 */
bool try_add_webhook_embed(WebhookData *data, EmbedData *embed);

/**
 * @brief Try to replace an existing embed in the webhook.
 * @param data Pointer to WebhookData.
 * @param embed Pointer to the new EmbedData.
 * @param index The index of the embed to replace.
 * @return True if successfully replaced, false otherwise.
 */
bool try_replace_webhook_embed(WebhookData *data, EmbedData *embed, size_t index);

/**
 * @brief Destroy a WebhookData instance and free its memory.
 * 
 * This will also destroy all attached embeds.
 * @param data Pointer to WebhookData to destroy.
 */
void webhook_destroy(WebhookData *data);

/**
 * @brief Create a new embed.
 * 
 * The embed data is destroyed automatically when the webhook is destroyed,
 * but if it's not added to a webhook, call `embed_destroy`.
 * @see embed_destroy()
 * @return A pointer to a new EmbedData instance, or NULL on failure.
 */
EmbedData *embed_create();

/**
 * @brief Set the title of an embed.
 * @param data Pointer to EmbedData.
 * @param title The title string.
 */
void set_embed_title(EmbedData *data, const char *title);

/**
 * @brief Set the description of an embed.
 * @param data Pointer to EmbedData.
 * @param description The description string.
 */
void set_embed_description(EmbedData *data, const char *description);

/**
 * @brief Set the URL of an embed.
 * @param data Pointer to EmbedData.
 * @param url The URL string.
 */
void set_embed_url(EmbedData *data, const char *url);

/**
 * @brief Set the timestamp of an embed.
 * @param data Pointer to EmbedData.
 * @param timestamp ISO8601 timestamp string (max 32 chars).
 */
void set_embed_timestamp(EmbedData *data, char timestamp[32]);

/**
 * @brief Set the color of an embed.
 * @param data Pointer to EmbedData.
 * @param color The color value (e.g., 0xRRGGBB).
 */
void set_embed_color(EmbedData *data, int32_t color);

/**
 * @brief Set the footer of an embed.
 * @param data Pointer to EmbedData.
 * @param text The footer text.
 * @param icon_url The footer icon URL.
 */
void set_embed_footer(EmbedData *data, const char *text, const char *icon_url);

/**
 * @brief Set the image of an embed.
 * @param data Pointer to EmbedData.
 * @param url The image URL.
 * @param height Height of the image.
 * @param width Width of the image.
 */
void set_embed_image(EmbedData *data, const char *url, int height, int width);

/**
 * @brief Set the thumbnail of an embed.
 * @param data Pointer to EmbedData.
 * @param url The thumbnail URL.
 * @param height Height of the thumbnail.
 * @param width Width of the thumbnail.
 */
void set_embed_thumbnail(EmbedData *data, const char *url, int height, int width);

/**
 * @brief Set the video of an embed.
 * @param data Pointer to EmbedData.
 * @param url The video URL.
 * @param height Height of the video.
 * @param width Width of the video.
 */
void set_embed_video(EmbedData *data, const char *url, int height, int width);

/**
 * @brief Set the provider of an embed.
 * @param data Pointer to EmbedData.
 * @param name The provider name.
 * @param url The provider URL.
 */
void set_embed_provider(EmbedData *data, const char *name, const char *url);

/**
 * @brief Set the author of an embed.
 * @param data Pointer to EmbedData.
 * @param name Author name.
 * @param url Author URL.
 * @param icon_url Author icon URL.
 */
void set_embed_author(EmbedData *data, const char *name, const char *url, const char *icon_url);

/**
 * @brief Try to add a field to an embed.
 * @param data Pointer to EmbedData.
 * @param name Field name.
 * @param value Field value.
 * @param inline_field Whether the field should be inline.
 * @return True if successfully added, false otherwise.
 */
bool try_add_embed_field(EmbedData *data, const char *name, const char *value, bool inline_field);

/**
 * @brief Try to replace a field in an embed.
 * @param data Pointer to EmbedData.
 * @param name New field name.
 * @param value New field value.
 * @param inline_field Whether the new field should be inline.
 * @param index Index of the field to replace.
 * @return True if successfully replaced, false otherwise.
 */
bool try_replace_embed_field(const EmbedData *data, const char *name, const char *value, bool inline_field, size_t index);

/**
 * @brief Destroy an EmbedData instance and free its memory.
 * @param data Pointer to EmbedData to destroy.
 */
void embed_destroy(EmbedData *data);

/**
 * @brief Sends a webhook message.
 * @param webhook_url The base webhook URL.
 * @param webhook The webhook data to send.
 */
void send_webhook(const char *webhook_url, const WebhookData *webhook);

/**
 * @brief Edits an existing webhook message.
 * @param webhook_url The base webhook URL.
 * @param message_id The ID of the message to edit.
 * @param webhook The webhook data to use for editing.
 */
void edit_webhook(const char *webhook_url, uint64_t message_id, const WebhookData *webhook);

/**
 * @brief Modifies the name of the webhook.
 * @param webhook_url The base webhook URL.
 * @param new_name The new name for the webhook.
 */
void modify_webhook_name(const char *webhook_url, const char *new_name);

/**
 * @brief Image types supported for webhook avatar modification.
 */
typedef enum ImageType {
    Jpeg, /**< JPEG image type */
    Png,  /**< PNG image type */
    Gif,  /**< GIF image type */
} ImageType;

/**
 * @brief Modifies the image of the webhook.
 * @param webhook_url The base webhook URL.
 * @param type The type of image to set.
 * @param b64_image The base64 encoded image data.
 */
void modify_webhook_image(const char *webhook_url, ImageType type, const char *b64_image);

/**
 * @brief Permanently deletes the webhook URL on Discord.
 * @param webhook_url The webhook URL to delete.
 */
void delete_webhook(const char *webhook_url);

#endif // DISCORDHOOKC_DISHOOK_H
