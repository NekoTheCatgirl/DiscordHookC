/*
 * DiscordHookC
 * Author: Neko The Catgirl (thea@nekothecatgirl.com)
 * Copyright (c) 2026 Neko The Catgirl
 * Description: A C library for Discord Webhooks
 * License: CC Attribution no derivatives (CC BY-ND 4.0)
 */

#include <dishook.h>
#include <dishook_private.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <curl/curl.h>

struct WebhookData {
    char* content;
    char* username;
    char* avatar_url;
    bool tts;
    size_t current_embed;
    EmbedData* embeds[10];
};

struct EmbedData {
    char* title;
    char* description;
    char* url;
    char timestamp[32];
    int32_t color;
    EmbedFooterData* footer;
    EmbedMultimediaData* image;
    EmbedMultimediaData* thumbnail;
    EmbedMultimediaData* video;
    EmbedProviderData* provider;
    EmbedAuthorData* author;
    size_t current_field;
    EmbedFieldData* fields[25];
};


static bool is_footer_empty(const EmbedFooterData *footer) {
    return footer == nullptr ? true : footer->text == nullptr && footer->icon_url == nullptr ? true : false;
}

static json_object *serialize_footer(const EmbedFooterData *footer) {
    if (is_footer_empty(footer)) return nullptr;

    json_object *root = json_object_new_object();

    if (footer->text != nullptr && strlen(footer->text) > 0) {
        json_object_object_add(root, "text", json_object_new_string(footer->text));
    }
    if (footer->icon_url != nullptr && strlen(footer->icon_url) > 0) {
        json_object_object_add(root, "icon_url", json_object_new_string(footer->icon_url));
    }

    return root;
}

static void set_footer_data(EmbedFooterData *footer, const char *text, const char *icon_url) {
    if (footer == nullptr) return;

    if (footer->text != nullptr) free(footer->text);
    if (footer->icon_url != nullptr) free(footer->icon_url);

    if (text != nullptr) {
        footer->text = strdup(text);
    } else {
        footer->text = nullptr;
    }

    if (icon_url != nullptr) {
        footer->icon_url = strdup(icon_url);
    } else {
        footer->icon_url = nullptr;
    }
}

static void footer_destroy(EmbedFooterData *footer) {
    if (footer == nullptr) return;

    free(footer->text);
    free(footer->icon_url);

    free(footer);
}

static bool is_multimedia_empty(const EmbedMultimediaData *multimedia) {
    return multimedia == nullptr ? true : multimedia->url == nullptr ? true : false;
}

static json_object* serialize_multimedia(const EmbedMultimediaData *multimedia) {
    if (is_multimedia_empty(multimedia)) return nullptr;

    json_object *root = json_object_new_object();

    json_object_object_add(root, "url", json_object_new_string(multimedia->url));
    json_object_object_add(root, "height", json_object_new_int(multimedia->height));
    json_object_object_add(root, "width", json_object_new_int(multimedia->width));

    return root;
}

static void set_multimedia_data(EmbedMultimediaData *multimedia, const char *url, const int height, const int width) {
    if (multimedia == nullptr || url == nullptr) return;

    if (multimedia->url != nullptr) free(multimedia->url);

    multimedia->url = strdup(url);
    multimedia->height = height;
    multimedia->width = width;
}

static void multimedia_destroy(EmbedMultimediaData *multimedia) {
    if (multimedia == nullptr) return;

    free(multimedia->url);

    free(multimedia);
}

static bool is_provider_empty(const EmbedProviderData *provider) {
    return provider == nullptr ? true : provider->name == nullptr && provider->url == nullptr ? true : false;
}

static json_object* serialize_provider(const EmbedProviderData *provider) {
    if (is_provider_empty(provider)) return nullptr;

    json_object *root = json_object_new_object();

    json_object_object_add(root, "url", json_object_new_string(provider->url));
    json_object_object_add(root, "name", json_object_new_string(provider->name));

    return root;
}

static void set_provider_data(EmbedProviderData *provider, const char *name, const char *url) {
    if (provider == nullptr || name == nullptr || url == nullptr) return;

    if (provider->name != nullptr) free(provider->name);
    if (provider->url != nullptr) free(provider->url);

    provider->name = strdup(name);
    provider->url = strdup(url);
}

static void provider_destroy(EmbedProviderData *provider) {
    if (provider == nullptr) return;

    free(provider->name);
    free(provider->url);

    free(provider);
}

static bool is_author_empty(const EmbedAuthorData *author) {
    return author == nullptr ? true : author->name == nullptr && author->url == nullptr && author->icon_url == nullptr ? true : false;
}

static json_object* serialize_author(const EmbedAuthorData *author) {
    if (is_author_empty(author)) return nullptr;

    json_object *root = json_object_new_object();

    json_object_object_add(root, "url", json_object_new_string(author->url));
    json_object_object_add(root, "name", json_object_new_string(author->name));
    json_object_object_add(root, "icon_url", json_object_new_string(author->icon_url));

    return root;
}

static void set_author_data(EmbedAuthorData *author, const char *name, const char *url, const char *icon_url) {
    if (author == nullptr || name == nullptr || url == nullptr || icon_url == nullptr) return;

    if (author->name != nullptr) free(author->name);
    if (author->url != nullptr) free(author->url);
    if (author->icon_url != nullptr) free(author->icon_url);

    author->name = strdup(name);
    author->url = strdup(url);
    author->icon_url = strdup(icon_url);
}

static void author_destroy(EmbedAuthorData *author) {
    if (author == nullptr) return;

    free(author->name);
    free(author->url);
    free(author->icon_url);

    free(author);
}

static struct json_object* serialize_field(const EmbedFieldData *field) {
    if (field == nullptr) return nullptr;

    json_object *root = json_object_new_object();

    json_object_object_add(root, "name", json_object_new_string(field->name));
    json_object_object_add(root, "value", json_object_new_string(field->value));
    json_object_object_add(root, "inline", json_object_new_boolean(field->inl));

    return root;
}

static void set_field_data(EmbedFieldData *field, const char *name, const char *value, bool inl) {
    if (field == nullptr || name == nullptr || value == nullptr) return;

    if (field->name != nullptr) free(field->name);
    if (field->value != nullptr) free(field->value);

    field->name = strdup(name);
    field->value = strdup(value);
    field->inl = inl;
}

static void field_destroy(EmbedFieldData *field) {
    if (field == nullptr) return;

    free(field->name);
    free(field->value);

    free(field);
}

static bool any_not_null_embeds(const WebhookData* webhook) {
    for (int i = 0; i < 10; i++) {
        if (webhook->embeds[i] != nullptr) {
            return true;
        }
    }
    return false;
}

static bool any_not_null_fields(const EmbedData* embed) {
    for (int i = 0; i < 25; i++) {
        if (embed->fields[i] != nullptr) {
            return true;
        }
    }
    return false;
}

static json_object* serialize_embed(const EmbedData* embed) {
    if (embed == nullptr) return nullptr;
    json_object* root = json_object_new_object();

    if (embed->title != nullptr && strlen(embed->title) > 0) {
        json_object_object_add(root, "title", json_object_new_string(embed->title));
    }

    json_object_object_add(root, "type", json_object_new_string("rich"));

    if (embed->description != nullptr && strlen(embed->description) > 0) {
        json_object_object_add(root, "description", json_object_new_string(embed->description));
    }

    if (embed->url != nullptr && strlen(embed->url) > 0) {
        json_object_object_add(root, "url", json_object_new_string(embed->url));
    }

    json_object_object_add(root, "timestamp", json_object_new_string(embed->timestamp));

    json_object_object_add(root, "color", json_object_new_int(embed->color));

    if (embed->footer != nullptr) {
        json_object_object_add(root, "footer", serialize_footer(embed->footer));
    }

    if (embed->image != nullptr) {
        json_object_object_add(root, "image", serialize_multimedia(embed->image));
    }

    if (embed->thumbnail != nullptr) {
        json_object_object_add(root, "thumbnail", serialize_multimedia(embed->thumbnail));
    }

    if (embed->video != nullptr) {
        json_object_object_add(root, "video", serialize_multimedia(embed->video));
    }

    if (embed->provider != nullptr) {
        json_object_object_add(root, "provider", serialize_provider(embed->provider));
    }

    if (embed->author != nullptr) {
        json_object_object_add(root, "author", serialize_author(embed->author));
    }

    if (!any_not_null_fields(embed)) return root;

    json_object* fields_array = json_object_new_array();
    json_object_object_add(root, "fields", fields_array);

    for (int i = 0; i < 25; i++) {
        if (embed->fields[i] != nullptr) {
            json_object_array_add(fields_array, serialize_field(embed->fields[i]));
        }
    }

    return root;
}

static json_object* serialize_webhook(const WebhookData* webhook) {
    if (webhook == nullptr) return nullptr;

    json_object* root = json_object_new_object();

    if (webhook->content != nullptr && strlen(webhook->content) > 0) {
        json_object_object_add(root, "content", json_object_new_string(webhook->content));
    }

    if (webhook->username != nullptr && strlen(webhook->username) > 0) {
        json_object_object_add(root, "username", json_object_new_string(webhook->username));
    }

    if (webhook->avatar_url != nullptr && strlen(webhook->avatar_url) > 0) {
        json_object_object_add(root, "avatar_url", json_object_new_string(webhook->avatar_url));
    }

    json_object_object_add(root, "tts", json_object_new_boolean(webhook->tts));

    if (!any_not_null_embeds(webhook)) return root;

    json_object* embeds = json_object_new_array();
    json_object_object_add(root, "embeds", embeds);
    for (int i = 0; i < 10; i++) {
        if (webhook->embeds[i] == NULL) continue;
        json_object_array_add(embeds, serialize_embed(webhook->embeds[i]));
    }

    return root;
}

WebhookData* webhook_create() {
    WebhookData* data = calloc(1, sizeof(WebhookData));

    data->current_embed = 0;

    return data;
}

void set_webhook_content(WebhookData *data, const char *content) {
    if (data == nullptr) return;

    if (data->content != nullptr) {
        free(data->content);
    }

    data->content = strdup(content);
}

void set_webhook_username(WebhookData *data, const char *username) {
    if (data == nullptr) return;

    if (data->username != nullptr) {
        free(data->username);
    }

    data->username = strdup(username);
}

void set_webhook_avatar_url(WebhookData *data, const char *avatar_url) {
    if (data == nullptr) return;

    if (data->avatar_url != nullptr) {
        free(data->avatar_url);
    }

    data->avatar_url = strdup(avatar_url);
}

void set_webhook_tts(WebhookData *data, const bool tts) {
    if (data == nullptr) return;

    data->tts = tts;
}

bool try_add_webhook_embed(WebhookData *data, EmbedData *embed) {
    if (data == nullptr || embed == nullptr) return false;

    if (data->current_embed >= 10) return false;

    data->embeds[data->current_embed++] = embed;

    return true;
}

bool try_replace_webhook_embed(WebhookData *data, EmbedData *embed, const size_t index) {
    if (data == nullptr || embed == nullptr) return false;

    if (index >= 10 || index >= data->current_embed) return false;

    data->embeds[index] = embed;

    return true;
}

void webhook_destroy(WebhookData *data) {
    if (data == nullptr) return;

    free(data->content);
    free(data->username);
    free(data->avatar_url);

    for (size_t i = 0; i < 10; i++) {
        embed_destroy(data->embeds[i]);
    }

    free(data);
}

EmbedData* embed_create() {
    EmbedData* data = calloc(1, sizeof(EmbedData));

    data->current_field = 0;

    return data;
}

void set_embed_title(EmbedData *data, const char *title) {
    if (data == nullptr || title == nullptr) return;

    if (data->title != nullptr) {
        free(data->title);
    }

    data->title = strdup(title);
}

void set_embed_description(EmbedData *data, const char *description) {
    if (data == nullptr || description == nullptr) return;

    if (data->description != nullptr) {
        free(data->description);
    }

    data->description = strdup(description);
}

void set_embed_url(EmbedData *data, const char *url) {
    if (data == nullptr || url == nullptr) return;

    if (data->url != nullptr) {
        free(data->url);
    }

    data->url = strdup(url);
}

void set_embed_timestamp(EmbedData *data, char timestamp[32]) {
    if (data == nullptr) return;

    memcpy(data->timestamp, timestamp, sizeof(data->timestamp));
}

void set_embed_color(EmbedData *data, const int32_t color) {
    if (data == nullptr) return;

    data->color = color;
}

void set_embed_footer(EmbedData *data, const char *text, const char *icon_url) {
    if (data == nullptr || text == nullptr) return;

    if (data->footer == nullptr) {
        data->footer = malloc(sizeof(EmbedFooterData));
        if (data->footer == nullptr) return;
    }

    set_footer_data(data->footer, text, icon_url);
}

void set_embed_image(EmbedData *data, const char *url, const int height, const int width) {
    if (data == nullptr || url == nullptr) return;

    if (data->image == nullptr) {
        data->image = malloc(sizeof(EmbedMultimediaData));
        if (data->image == nullptr) return;
    }

    set_multimedia_data(data->image, url, height, width);
}

void set_embed_thumbnail(EmbedData *data, const char *url, const int height, const int width) {
    if (data == nullptr || url == nullptr) return;

    if (data->thumbnail == nullptr) {
        data->thumbnail = malloc(sizeof(EmbedMultimediaData));
        if (data->thumbnail == nullptr) return;
    }

    set_multimedia_data(data->thumbnail, url, height, width);
}

void set_embed_video(EmbedData *data, const char *url, const int height, const int width) {
    if (data == nullptr || url == nullptr) return;

    if (data->video == nullptr) {
        data->video = malloc(sizeof(EmbedMultimediaData));
        if (data->video == nullptr) return;
    }

    set_multimedia_data(data->video, url, height, width);
}

void set_embed_provider(EmbedData *data, const char *name, const char *url) {
    if (data == nullptr || name == nullptr || url == nullptr) return;

    if (data->provider == nullptr) {
        data->provider = malloc(sizeof(EmbedProviderData));
        if (data->provider == nullptr) return;
    }

    set_provider_data(data->provider, name, url);
}

void set_embed_author(EmbedData *data, const char *name, const char *url, const char *icon_url) {
    if (data == nullptr || name == nullptr) return;

    if (data->author == nullptr) {
        data->author = malloc(sizeof(EmbedAuthorData));
        if (data->author == nullptr) return;
    }

    set_author_data(data->author, name, url, icon_url);
}

bool try_add_embed_field(EmbedData *data, const char *name, const char *value, bool inline_field) {
    if (data == nullptr || name == nullptr || value == nullptr) return false;

    if (data->current_field >= 25) return false;

    EmbedFieldData *field = malloc(sizeof(EmbedFieldData));
    if (field == nullptr) return false;

    set_field_data(field, name, value, inline_field);

    data->fields[data->current_field++] = field;

    return true;
}

bool try_replace_embed_field(const EmbedData *data, const char *name, const char *value, bool inline_field, size_t index) {
    if (data == nullptr) return false;

    if (index >= 25 || index >= data->current_field) return false;

    EmbedFieldData *field = data->fields[index];
    set_field_data(field, name, value, inline_field);
    return true;
}

void embed_destroy(EmbedData *data) {
    if (data == nullptr) return;

    free(data->title);
    free(data->description);
    free(data->url);

    footer_destroy(data->footer);

    multimedia_destroy(data->image);
    multimedia_destroy(data->thumbnail);
    multimedia_destroy(data->video);

    provider_destroy(data->provider);

    author_destroy(data->author);

    for (size_t i = 0; i < 25; i++) {
        field_destroy(data->fields[i]);
    }

    free(data);
}

void send_webhook(const char *webhook_url, const WebhookData *webhook) {
    if (webhook == nullptr || webhook_url == nullptr) return;

    json_object *json = serialize_webhook(webhook);
    const char* json_str = json_object_to_json_string(json);

    CURL *curl = curl_easy_init();
    if (curl == nullptr) {
        json_object_put(json);
        return;
    }

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, webhook_url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    json_object_put(json);
}

void edit_webhook(const char *webhook_url, uint64_t message_id, const WebhookData *webhook) {
    if (webhook == nullptr || webhook_url == nullptr) return;

    json_object *json = serialize_webhook(webhook);
    const char* json_str = json_object_to_json_string(json);

    CURL *curl = curl_easy_init();
    if (curl == nullptr) {
        json_object_put(json);
        return;
    }

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    char url[1024];
    snprintf(url, sizeof(url), "%s/messages/%" PRIu64, webhook_url, message_id);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    json_object_put(json);
}

void modify_webhook_name(const char *webhook_url, const char *new_name) {
    if (webhook_url == nullptr || new_name == nullptr) return;

    json_object* root = json_object_new_object();
    json_object_object_add(root, "name", json_object_new_string(new_name));
    const char* json_str = json_object_to_json_string(root);

    CURL* curl = curl_easy_init();
    if (curl == nullptr) {
        json_object_put(root);
        return;
    }

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, webhook_url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    json_object_put(root);
}

static const char* get_image_type_string(ImageType type) {
    switch (type) {
        case Jpeg: return "image/jpeg";
        case Png:  return "image/png";
        case Gif:  return "image/gif";
        default:   return "image/jpeg";
    }
}

void modify_webhook_image(const char *webhook_url, ImageType type, const char *b64_image) {
    if (webhook_url == nullptr || b64_image == nullptr) return;

    // Format: data:image/type;base64,DATA
    const char* type_str = get_image_type_string(type);
    size_t avatar_len = strlen("data:") + strlen(type_str) + strlen(";base64,") + strlen(b64_image) + 1;
    char* avatar = malloc(avatar_len);
    if (avatar == nullptr) return;
    snprintf(avatar, avatar_len, "data:%s;base64,%s", type_str, b64_image);

    json_object* root = json_object_new_object();
    json_object_object_add(root, "avatar", json_object_new_string(avatar));
    const char* json_str = json_object_to_json_string(root);

    CURL* curl = curl_easy_init();
    if (curl == nullptr) {
        json_object_put(root);
        free(avatar);
        return;
    }

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, webhook_url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    json_object_put(root);
    free(avatar);
}

void delete_webhook(const char *webhook_url) {
    if (webhook_url == nullptr) return;

    CURL *curl = curl_easy_init();
    if (curl == nullptr) {
        return;
    }

    curl_easy_setopt(curl, CURLOPT_URL, webhook_url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

    curl_easy_perform(curl);

    curl_easy_cleanup(curl);
}
