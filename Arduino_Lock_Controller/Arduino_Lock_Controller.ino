#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// ==================== PIN CONFIGURATION ====================
#define RELAY_PIN 7           // Relay module control pin
#define ESP_RX_PIN 10         // Arduino RX from ESP32 TX
#define ESP_TX_PIN 11         // Arduino TX to ESP32 RX

// ==================== LCD CONFIGURATION ====================
// Common I2C addresses: 0x27 or 0x3F
// If LCD doesn't work, try changing address
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ==================== SERIAL COMMUNICATION ====================
SoftwareSerial esp32Serial(ESP_RX_PIN, ESP_TX_PIN);

// ==================== TIMING CONFIGURATION ====================
const unsigned long UNLOCK_DURATION = 5000;  // 5 seconds unlock time
const unsigned long DISPLAY_DELAY = 2000;    // 2 seconds for messages

// ==================== GLOBAL VARIABLES ====================
bool doorLocked = true;

// ==================== SETUP ====================
void setup() {
  // Initialize hardware serial for debugging
  Serial.begin(9600);
  Serial.println("\n=== Arduino Door Lock Controller ===");
  
  // Initialize ESP32 serial communication
  esp32Serial.begin(9600);
  Serial.println("ESP32 Serial initialized");
  
  // Initialize relay pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Start with lock engaged (relay off)
  doorLocked = true;
  Serial.println("Relay initialized - Door LOCKED");
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Display startup message
  lcd.setCursor(0, 0);
  lcd.print("Smart Lock v1.0");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  
  // Show ready status
  showReady();
  Serial.println("=== System Ready ===\n");
}

// ==================== MAIN LOOP ====================
void loop() {
  // Check for commands from ESP32
  if (esp32Serial.available()) {
    String command = esp32Serial.readStringUntil('\n');
    command.trim();
    
    if (command.length() > 0) {
      Serial.println("Received: " + command);
      handleCommand(command);
    }
  }
  
  // Add any additional monitoring here if needed
}

// ==================== HANDLE ESP32 COMMANDS ====================
void handleCommand(String command) {
  if (command == "ESP32_READY") {
    // ESP32 is ready
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connected");
    lcd.setCursor(0, 1);
    lcd.print("System Online");
    delay(DISPLAY_DELAY);
    showReady();
    Serial.println("ESP32 connected and ready");
  }
  else if (command == "OTP_GENERATED") {
    // OTP was sent to user
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("OTP Sent!");
    lcd.setCursor(0, 1);
    lcd.print("Check Telegram");
    Serial.println("OTP generated - waiting for user input");
  }
  else if (command == "UNLOCK_OK") {
    // Correct OTP - unlock door
    unlockDoor();
  }
  else if (command == "UNLOCK_DENIED") {
    // Wrong OTP entered
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wrong OTP!");
    lcd.setCursor(0, 1);
    lcd.print("Access Denied");
    
    // Beep pattern (if buzzer connected to relay temporarily)
    flashDisplay(3);
    
    delay(DISPLAY_DELAY);
    showReady();
    Serial.println("Access denied - wrong OTP");
  }
  else if (command == "TIMEOUT") {
    // OTP expired
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("OTP Expired!");
    lcd.setCursor(0, 1);
    lcd.print("Try Again");
    delay(DISPLAY_DELAY);
    showReady();
    Serial.println("OTP expired");
  }
  else {
    // Unknown command
    Serial.println("Unknown command: " + command);
  }
}

// ==================== UNLOCK DOOR FUNCTION ====================
void unlockDoor() {
  Serial.println(">>> UNLOCKING DOOR <<<");
  
  // Display unlock message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access Granted!");
  lcd.setCursor(0, 1);
  lcd.print("Door Unlocked");
  
  // Activate relay to unlock (solenoid energized)
  digitalWrite(RELAY_PIN, HIGH);
  doorLocked = false;
  Serial.println("Relay ON - Solenoid unlocked");
  
  // Send confirmation to ESP32
  esp32Serial.println("DOOR_UNLOCKED");
  
  // Keep door unlocked for specified duration
  unsigned long unlockStart = millis();
  int countdown = UNLOCK_DURATION / 1000;
  
  while (millis() - unlockStart < UNLOCK_DURATION) {
    // Update countdown on LCD
    if ((millis() - unlockStart) % 1000 < 50) { // Update every second
      lcd.setCursor(0, 1);
      lcd.print("Locking in ");
      lcd.print(countdown);
      lcd.print("s   ");
      countdown--;
      delay(50); // Debounce
    }
  }
  
  // Lock door again
  lockDoor();
}

// ==================== LOCK DOOR FUNCTION ====================
void lockDoor() {
  Serial.println(">>> LOCKING DOOR <<<");
  
  // Deactivate relay to lock (solenoid de-energized)
  digitalWrite(RELAY_PIN, LOW);
  doorLocked = true;
  Serial.println("Relay OFF - Solenoid locked");
  
  // Display lock message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Door Locked");
  lcd.setCursor(0, 1);
  lcd.print("Secured");
  
  // Send confirmation to ESP32
  esp32Serial.println("DOOR_LOCKED");
  
  delay(DISPLAY_DELAY);
  showReady();
}

// ==================== SHOW READY STATUS ====================
void showReady() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Lock");
  lcd.setCursor(0, 1);
  lcd.print("Ready - Locked");
  Serial.println("Status: Ready and Locked");
}

// ==================== FLASH DISPLAY (ERROR INDICATOR) ====================
void flashDisplay(int times) {
  for (int i = 0; i < times; i++) {
    lcd.noBacklight();
    delay(200);
    lcd.backlight();
    delay(200);
  }
}

// ==================== STATUS CHECK (OPTIONAL) ====================
void printStatus() {
  Serial.println("--- System Status ---");
  Serial.print("Door Locked: ");
  Serial.println(doorLocked ? "YES" : "NO");
  Serial.print("Relay State: ");
  Serial.println(digitalRead(RELAY_PIN) ? "HIGH (Unlocked)" : "LOW (Locked)");
  Serial.println("--------------------");
}
