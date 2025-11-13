# Smart Lock System - Telegram OTP Based

## 📋 Project Overview
A smart door lock system using **ESP32** for Telegram OTP generation and **Arduino** for hardware control (LCD, Relay, Solenoid Lock).

### Features
- 🔐 OTP-based authentication via Telegram
- 📱 No physical keypad needed - all control via smartphone
- 🔒 12V Solenoid lock control via relay
- 📺 16x2 LCD I2C display for status
- ⏱ Auto-lock after 5 seconds
- 🔒 60-second OTP validity
- 🛡 Authorized user access only

---

## 🔧 Hardware Components

| Component | Quantity | Purpose |
|-----------|----------|---------|
| ESP32 | 1 | WiFi & Telegram OTP handler |
| Arduino Uno/Nano | 1 | Hardware controller |
| 16x2 LCD I2C | 1 | Status display |
| Relay Module (5V) | 1 | Switch 12V solenoid |
| 12V Solenoid Lock | 1 | Door lock mechanism |
| 12V Power Adapter | 1 | Power supply |
| Female DC Jack | 1 | Power connection |
| Jumper Wires | Multiple | Connections |
| Breadboard (optional) | 1 | Prototyping |

---

## 📐 Wiring Diagram

### ESP32 Connections
```
ESP32 GPIO17 (TX) ────────► Arduino Pin 10 (RX)
ESP32 GPIO16 (RX) ────────► Arduino Pin 11 (TX)
ESP32 GND ────────────────► Arduino GND
ESP32 Power: USB Cable
```

### Arduino Connections

#### LCD I2C (16x2)
```
LCD VCC ──────► Arduino 5V
LCD GND ──────► Arduino GND
LCD SDA ──────► Arduino A4
LCD SCL ──────► Arduino A5
```

#### Relay Module
```
Relay VCC ────► Arduino 5V
Relay GND ────► Arduino GND
Relay IN ─────► Arduino Pin 7
```

#### Solenoid Lock (via Relay)
```
12V Adapter (+) ──────► Relay COM
Relay NO ─────────────► Solenoid Lock (+)
Solenoid Lock (-) ────► 12V Adapter GND (-)
```

#### Power Supply
```
12V Adapter ──► Female Jack ──► Arduino VIN + GND
                             OR Arduino USB Power (separate)
```

---

## 📊 Complete Wiring Schematic

```
┌─────────────┐                    ┌─────────────────┐
│   ESP32     │                    │   Arduino Uno   │
│             │                    │                 │
│  GPIO17(TX) ├───────────────────►│ Pin 10 (RX)     │
│  GPIO16(RX) │◄───────────────────┤ Pin 11 (TX)     │
│     GND     ├────────────────────┤ GND             │
│             │                    │                 │
│  USB Power  │                    │ Pin 7 ──────────┼─► Relay IN
└─────────────┘                    │ A4 (SDA) ───────┼─► LCD SDA
                                   │ A5 (SCL) ───────┼─► LCD SCL
                                   │ 5V ─────────────┼─► LCD VCC
                                   │ GND ────────────┼─► LCD GND
                                   │ 5V ─────────────┼─► Relay VCC
                                   │ GND ────────────┼─► Relay GND
                                   │                 │
                                   │ VIN + GND ◄─────┼─ 12V Jack
                                   └─────────────────┘

┌──────────────┐        ┌─────────────┐
│ Relay Module │        │ 12V Adapter │
│              │        │             │
│ COM ◄────────┼────────┤ (+)         │
│ NO ──────────┼────┐   │ (-)  ───────┼──┐
│              │    │   └─────────────┘  │
└──────────────┘    │                    │
                    │   ┌──────────────┐ │
                    └──►│ Solenoid (+) │ │
                        │ Solenoid (-) │◄┘
                        └──────────────┘
```

---

## 💻 Software Setup

### 1. Arduino IDE Setup

#### Install Arduino IDE
- Download from: https://www.arduino.cc/en/software
- Install for Windows

#### Install ESP32 Board Support
1. Open Arduino IDE
2. Go to: **File → Preferences**
3. Add to "Additional Board Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to: **Tools → Board → Boards Manager**
5. Search "ESP32" and install **ESP32 by Espressif**

#### Install Required Libraries

**For ESP32:**
1. Go to: **Sketch → Include Library → Manage Libraries**
2. Search and install:
   - `UniversalTelegramBot` by Brian Lough

**For Arduino:**
1. Search and install:
   - `LiquidCrystal I2C` by Frank de Brabander

---

### 2. Telegram Bot Setup

#### Create Telegram Bot
1. Open Telegram app
2. Search for **@BotFather**
3. Send `/newbot`
4. Choose bot name (e.g., "My Smart Lock")
5. Choose username (e.g., "mysmartlock_bot")
6. Copy the **Bot Token** (already in your code)

#### Get Your Chat ID
1. Search for **@userinfobot** on Telegram
2. Start the bot
3. It will show your **Chat ID** (already in your code)

**Your Credentials (Already Configured):**
- WiFi SSID: `use ur personal wifi name`
- WiFi Password: `12345678`
- Bot Token: `ur bot token`
- Chat ID: `u will get when u create the chat id`

---

## 🚀 Upload Instructions

### Upload to ESP32
1. Connect ESP32 to computer via USB
2. Open: `ESP32_Telegram_OTP.ino`
3. Select: **Tools → Board → ESP32 Dev Module**
4. Select: **Tools → Port** → (Your ESP32 COM port)
5. Click: **Upload** button
6. Wait for "Done uploading"
7. Open **Serial Monitor** (115200 baud) to verify

### Upload to Arduino
1. Connect Arduino to computer via USB
2. Open: `Arduino_Lock_Controller.ino`
3. Select: **Tools → Board → Arduino Uno** (or your board)
4. Select: **Tools → Port** → (Your Arduino COM port)
5. Click: **Upload** button
6. Wait for "Done uploading"
7. Open **Serial Monitor** (9600 baud) to verify

---

## 🧪 Testing Steps

### 1. Initial Testing
1. Power both ESP32 and Arduino
2. Check LCD displays: "Smart Lock Ready - Locked"
3. Check ESP32 Serial Monitor: "WiFi Connected"
4. Check Telegram: Should receive "Smart Lock System Online!"

### 2. OTP Testing
1. Send `/unlock` to your Telegram bot
2. Bot sends 4-digit OTP (e.g., `1234`)
3. LCD shows: "OTP Sent! Check Telegram"
4. Type the OTP (e.g., `1234`) in Telegram chat
5. Bot replies: "✅ Correct OTP! 🔓 Door Unlocked"
6. LCD shows: "Access Granted! Door Unlocked"
7. Relay activates (click sound)
8. After 5 seconds, door auto-locks

### 3. Wrong OTP Testing
1. Send `/unlock`
2. Type wrong OTP (e.g., `0000`)
3. Bot replies: "❌ Wrong OTP!"
4. LCD shows: "Wrong OTP! Access Denied"
5. LCD backlight flashes 3 times

### 4. OTP Timeout Testing
1. Send `/unlock`
2. Wait 60+ seconds
3. Type the OTP
4. Bot replies: "⏱ OTP Expired!"
5. LCD shows: "OTP Expired! Try Again"

---

## 📱 Telegram Commands

| Command | Description |
|---------|-------------|
| `/start` | Show welcome message & commands |
| `/unlock` | Generate OTP to unlock door |
| `/status` | Check system status & WiFi |
| `1234` | Enter 4-digit OTP to unlock |

---

## 🔧 Troubleshooting

### LCD Not Working
**Problem:** LCD shows nothing or gibberish
**Solution:**
1. Check I2C address in code (line 12): `0x27` or `0x3F`
2. Run I2C scanner sketch to find address
3. Check SDA/SCL connections (A4/A5)
4. Adjust contrast potentiometer on LCD backpack

### ESP32 Can't Connect to WiFi
**Problem:** "WiFi connection failed"
**Solution:**
1. Verify WiFi SSID: `hmm`
2. Verify password: `12345678`
3. Check 2.4GHz WiFi (ESP32 doesn't support 5GHz)
4. Move closer to router

### Telegram Bot Not Responding
**Problem:** Bot doesn't reply
**Solution:**
1. Verify Bot Token is correct
2. Check Chat ID matches your account
3. Start conversation with bot first (`/start`)
4. Check ESP32 serial monitor for errors
5. Verify internet connection

### Solenoid Doesn't Unlock
**Problem:** No click sound from relay
**Solution:**
1. Check relay wiring to Pin 7
2. Verify 12V adapter is connected
3. Test relay with LED first
4. Check relay type (active HIGH/LOW)
5. Measure voltage at solenoid terminals

### Serial Communication Issues
**Problem:** ESP32 and Arduino not communicating
**Solution:**
1. Verify cross connections: ESP32 TX → Arduino RX
2. Check baud rate (9600 on both)
3. Ensure common ground connection
4. Try swapping TX/RX pins
5. Check SoftwareSerial pins (10, 11)

---

## ⚙️ Configuration Options

### Change OTP Validity Time
Edit `ESP32_Telegram_OTP.ino` line 23:
```cpp
const unsigned long OTP_VALIDITY = 60000; // 60 seconds (change as needed)
```

### Change Unlock Duration
Edit `Arduino_Lock_Controller.ino` line 18:
```cpp
const unsigned long UNLOCK_DURATION = 5000; // 5 seconds (change as needed)
```

### Change LCD I2C Address
Edit `Arduino_Lock_Controller.ino` line 12:
```cpp
LiquidCrystal_I2C lcd(0x27, 16, 2); // Try 0x3F if 0x27 doesn't work
```

### Change Relay Pin
Edit `Arduino_Lock_Controller.ino` line 5:
```cpp
#define RELAY_PIN 7 // Change to any digital pin
```

---

## 🔒 Security Notes

✅ **Implemented Security:**
- Only authorized Chat ID can control lock
- OTP expires after 60 seconds
- OTP invalidated after single use
- No keypad = no physical attack surface

⚠️ **Recommendations:**
- Use strong WiFi password
- Keep Bot Token secret
- Don't share your Chat ID
- Use HTTPS for any web interface
- Consider adding backup unlock method

---

## 📈 Future Enhancements

- 📸 Add camera for visual verification
- 🔔 Add buzzer for audio feedback
- 🔋 Add battery backup (UPS)
- 📊 Add access log with timestamps
- 👥 Multiple user support with different Chat IDs
- 🏠 Integrate with Home Assistant
- 🔐 Add RFID/NFC as backup unlock method
- 📧 Email notifications on unlock
- 🌐 Web dashboard for access logs

---

## 📝 Project Structure

```
ldmfinal_project/
│
├── ESP32_Telegram_OTP/
│   └── ESP32_Telegram_OTP.ino    # ESP32 code for Telegram
│
├── Arduino_Lock_Controller/
│   └── Arduino_Lock_Controller.ino # Arduino code for hardware
│
└── README.md                      # This file
```

---

## 📞 Support

If you encounter issues:
1. Check Serial Monitor output on both devices
2. Verify all wiring connections
3. Test each component individually
4. Review troubleshooting section above

---

## 📄 License

This project is open-source for educational purposes.

---

## 👨‍💻 Author

**Project:** Smart Lock System  
**Date:** 2025  
**Components:** ESP32 + Arduino + Telegram Bot  

---

**🎉 Enjoy your Smart Lock System! 🔐**
