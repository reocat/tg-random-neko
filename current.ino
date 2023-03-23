/*******************************************************************
    A telegram bot for your ESP32 that demonstrates sending an image
    from URL.

    Parts:
    ESP32 D1 Mini stlye Dev board* - http://s.click.aliexpress.com/e/C6ds4my
    (or any ESP32 board)

      = Affilate

    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/

    Example originally written by Vadim Sinitski 

    Library written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Wifi network station credentials
#define WIFI_SSID "TP-Link_E3F4"
#define WIFI_PASSWORD "91513687"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "5736860380:AAFZVi5bSWzlv6C5vbv1aVh1K1swEPrxjZs"

const unsigned long BOT_MTBS = 1000;  // mean time between scan messages

unsigned long bot_lasttime;  // last time messages' scan has been done
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void handleNewMessages(int numNewMessages) {
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/neko") {
      HTTPClient http;
      http.begin("https://cataas.com/cat?json=true");
      int httpResponseCode = http.GET();

      if (httpResponseCode == 200) {
        String payload = http.getString();

        // Parse JSON payload
        const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(7) + JSON_OBJECT_SIZE(8) + JSON_OBJECT_SIZE(9) + 340;
        DynamicJsonDocument doc(capacity);
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.f_str());
        } else {
          const char* gatoUrl = doc["url"];
          bot.sendPhoto(chat_id, "https://cataas.com/" + String(gatoUrl));
        }
      } else {
        Serial.print("HTTP GET failed, error code: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    }

    if (text == "/nekogirl") {
      HTTPClient http;
      http.begin("https://nekos.life/api/v2/img/neko");
      int httpResponseCode = http.GET();

      if (httpResponseCode == 200) {
        String payload = http.getString();

        // Parse JSON payload
        const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(7) + JSON_OBJECT_SIZE(8) + JSON_OBJECT_SIZE(9) + 340;
        DynamicJsonDocument doc(capacity);
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.f_str());
        } else {
          const char* nekoUrl = doc["url"];
          bot.sendPhoto(chat_id, nekoUrl);
        }
      } else {
        Serial.print("HTTP GET failed, error code: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    }
    if (text == "/kitsunegirl") {
      HTTPClient http;
      http.begin("https://nekos.life/api/v2/img/fox_girl");
      int httpResponseCode = http.GET();

      if (httpResponseCode == 200) {
        String payload = http.getString();

        // Parse JSON payload
        const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(7) + JSON_OBJECT_SIZE(8) + JSON_OBJECT_SIZE(9) + 340;
        DynamicJsonDocument doc(capacity);
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.f_str());
        } else {
          const char* kitsuneUrl = doc["url"];
          bot.sendPhoto(chat_id, kitsuneUrl);
        }
      } else {
        Serial.print("HTTP GET failed, error code: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org");  // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600) {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void loop() {
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}
