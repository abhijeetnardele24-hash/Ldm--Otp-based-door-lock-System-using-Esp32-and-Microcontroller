#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// ==================== WIFI CONFIGURATION ====================
const char* WIFI_SSID = "hmm";
const char* WIFI_PASSWORD = "12345678";

// ==================== TELEGRAM CONFIGURATION ====================
#define BOT_TOKEN "8221363925:AAH3nmbIyoPoeEmJdG6mJvFvMcUtY9QD4Ss"
#define CHAT_ID "5203680634"

// ==================== SERIAL COMMUNICATION ====================
#define RXD2 16  // ESP32 RX pin (connect to Arduino TX)
#define TXD2 17  // ESP32 TX pin (connect to Arduino RX)

// ==================== GLOBAL VARIABLES ====================
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

String generatedOTP = "";
unsigned long otpTimestamp = 0;
const unsigned long OTP_VALIDITY = 60000; // 60 seconds validity
const unsigned long BOT_CHECK_DELAY = 1000; // Check messages every 1 second
unsigned long lastBotCheck = 0;

bool otpActive = false;

// ==================== SETUP ====================
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  Serial.println("\n\n=== ESP32 Telegram OTP System ===");
  
  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✓ WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  // Configure secure client for Telegram
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  
  // Test Telegram connection
  Serial.println("Testing Telegram connection...");
  if (bot.sendMessage(CHAT_ID, "🔐 Smart Lock System Online!\nSend /unlock to generate OTP", "")) {
    Serial.println("✓ Telegram Bot Connected!");
  } else {
    Serial.println("✗ Telegram connection failed!");
  }
  
  // Notify Arduino that ESP32 is ready
  Serial2.println("ESP32_READY");
  
  Serial.println("\n=== System Ready ===");
  Serial.println("Waiting for commands...\n");
}

// ==================== MAIN LOOP ====================
void loop() {
  // Check for new Telegram messages
  if (millis() - lastBotCheck > BOT_CHECK_DELAY) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    
    lastBotCheck = millis();
  }
  
  // Check OTP timeout
  if (otpActive && (millis() - otpTimestamp > OTP_VALIDITY)) {
    Serial.println("⏱ OTP Expired!");
    bot.sendMessage(CHAT_ID, "⏱ OTP Expired! Send /unlock to generate new OTP", "");
    Serial2.println("TIMEOUT");
    otpActive = false;
    generatedOTP = "";
  }
  
  // Check for messages from Arduino (optional feedback)
  if (Serial2.available()) {
    String arduinoMsg = Serial2.readStringUntil('\n');
    arduinoMsg.trim();
    Serial.println("Arduino: " + arduinoMsg);
  }
}

// ==================== HANDLE TELEGRAM MESSAGES ====================
void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    
    Serial.println("\n--- New Message ---");
    Serial.println("From: " + from_name);
    Serial.println("Chat ID: " + chat_id);
    Serial.println("Message: " + text);
    
    // Security: Only respond to authorized chat ID
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "⛔ Unauthorized access!", "");
      Serial.println("✗ Unauthorized user blocked!");
      continue;
    }
    
    // Handle commands
    if (text == "/start") {
      String welcome = "🔐 *Smart Lock System*\n\n";
      welcome += "Available commands:\n";
      welcome += "/unlock - Generate OTP to unlock door\n";
      welcome += "/status - Check system status\n\n";
      welcome += "Powered by ESP32 + Arduino";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
    else if (text == "/unlock") {
      generateAndSendOTP(chat_id);
    }
    else if (text == "/status") {
      String status = "📊 *System Status*\n\n";
      status += "WiFi: Connected ✓\n";
      status += "Signal: " + String(WiFi.RSSI()) + " dBm\n";
      status += "OTP Active: " + String(otpActive ? "Yes" : "No") + "\n";
      if (otpActive) {
        int remainingTime = (OTP_VALIDITY - (millis() - otpTimestamp)) / 1000;
        status += "OTP Expires in: " + String(remainingTime) + "s";
      }
      bot.sendMessage(chat_id, status, "Markdown");
    }
    else if (text.length() == 4 && text.toInt() > 0) {
      // User entered a 4-digit code
      validateOTP(chat_id, text);
    }
    else {
      bot.sendMessage(chat_id, "❓ Unknown command\nSend /unlock to generate OTP", "");
    }
  }
}

// ==================== GENERATE OTP ====================
void generateAndSendOTP(String chat_id) {
  // Generate random 4-digit OTP
  randomSeed(millis() + analogRead(0));
  generatedOTP = String(random(1000, 9999));
  otpTimestamp = millis();
  otpActive = true;
  
  Serial.println("🔑 OTP Generated: " + generatedOTP);
  
  // Send OTP to user
  String message = "🔑 *Your OTP:* `" + generatedOTP + "`\n\n";
  message += "⏱ Valid for 60 seconds\n";
  message += "📱 Type the OTP to unlock door";
  bot.sendMessage(chat_id, message, "Markdown");
  
  // Notify Arduino
  Serial2.println("OTP_GENERATED");
  Serial.println("✓ OTP sent to Telegram and Arduino");
}

// ==================== VALIDATE OTP ====================
void validateOTP(String chat_id, String enteredOTP) {
  if (!otpActive) {
    bot.sendMessage(chat_id, "❌ No active OTP!\nSend /unlock first", "");
    return;
  }
  
  // Check if OTP expired
  if (millis() - otpTimestamp > OTP_VALIDITY) {
    bot.sendMessage(chat_id, "⏱ OTP Expired!\nSend /unlock to generate new OTP", "");
    Serial2.println("TIMEOUT");
    otpActive = false;
    generatedOTP = "";
    return;
  }
  
  // Validate OTP
  if (enteredOTP == generatedOTP) {
    Serial.println("✓ OTP Correct - Unlocking door!");
    bot.sendMessage(chat_id, "✅ *Correct OTP!*\n🔓 Door Unlocked", "Markdown");
    Serial2.println("UNLOCK_OK");
    
    // Invalidate OTP after use
    otpActive = false;
    generatedOTP = "";
  } else {
    Serial.println("✗ Wrong OTP entered");
    bot.sendMessage(chat_id, "❌ *Wrong OTP!*\nTry again", "Markdown");
    Serial2.println("UNLOCK_DENIED");
  }
}
