# 🔌 COMPLETE WIRING GUIDE
## Smart Lock System - ESP32 + Arduino + Telegram OTP

**Project by:** Abhijeet Nardele  
**Date:** November 2025  
**Status:** ✅ WORKING!  

---

## 📋 COMPONENTS LIST

| Component | Quantity | Description |
|-----------|----------|-------------|
| ESP32 (38-pin) | 1 | WiFi + Telegram handler |
| Arduino Uno | 1 | Hardware controller |
| 16x2 LCD I2C | 1 | Display module |
| 5V Relay Module | 1 | Solenoid switch |
| 12V Solenoid Lock | 1 | Door lock mechanism |
| 12V Power Adapter | 1 | Power supply |
| Female DC Jack | 1 | Power connector |
| Breadboard | 1 | Power distribution |
| Jumper Wires | Multiple | Connections |
| USB Cables | 2 | ESP32 + Arduino power |

---

## 🎯 STEP 1: BREADBOARD POWER SETUP

### **12V Adapter to Breadboard:**

Connect the 12V power adapter to breadboard power rails for distribution.

| From | To | Wire Color |
|------|-----|-----------|
| 12V Adapter (+) | Breadboard (+) rail (RED line) | Red |
| 12V Adapter (-) | Breadboard (-) rail (BLUE/BLACK line) | Black |

**Visual:**
```
   12V Power Adapter
   ┌──────────┐
   │  Female  │
   │   Jack   │
   └─┬─────┬──┘
     │     │
    (+)   (-)
     │     │
     ▼     ▼
┌─────────────────────────┐
│      Breadboard         │
│  [++++++++] [────────]  │
│   Red rail   Blue rail  │
│                         │
└─────────────────────────┘
```

---

## 🎯 STEP 2: ARDUINO POWER FROM BREADBOARD

Power Arduino from the breadboard power rails.

| From | To | Wire Color |
|------|-----|-----------|
| Breadboard (+) rail | Arduino VIN pin | Red |
| Breadboard (-) rail | Arduino GND pin | Black |

**Arduino Power Section (Left side, top):**
```
IOREF  ●
RESET  ●
3.3V   ●
5V     ●
GND    ●
GND    ●
VIN    ● ◄── Red wire from Breadboard (+)
```

**Arduino GND can be any of the 3 GND pins on the board.**

---

## 🎯 STEP 3: LCD I2C TO ARDUINO

Connect the 16x2 LCD I2C module to Arduino and breadboard.

### **LCD Connections:**

| LCD Pin | To | Wire Color |
|---------|-----|-----------|
| GND | Breadboard (-) rail | Black |
| VCC | Arduino 5V pin | Red |
| SDA | Arduino A4 pin | Blue |
| SCL | Arduino A5 pin | Yellow |

**Arduino Locations:**
```
LEFT SIDE (Power):
5V     ● ◄── LCD VCC (Red wire)

RIGHT SIDE (Analog):
A4     ● ◄── LCD SDA (Blue wire)
A5     ● ◄── LCD SCL (Yellow wire)
```

**LCD Module:**
```
LCD I2C
┌───────────┐
│ [Screen]  │
└─┬─┬─┬─┬───┘
  │ │ │ │
 GND│ │SCL──► Yellow ──► Arduino A5
  │ │SDA───► Blue ────► Arduino A4
  │VCC────► Red ─────► Arduino 5V
  │
  └─ Black ──► Breadboard (-) rail
```

---

## 🎯 STEP 4: RELAY MODULE TO ARDUINO

Connect relay module for control side (3 pins: VCC, GND, IN).

### **Relay Control Side:**

| Relay Pin | To | Wire Color |
|-----------|-----|-----------|
| VCC | Arduino 5V pin | Red |
| GND | Breadboard (-) rail | Black |
| IN | Arduino Pin 7 | Orange |

**Arduino Locations:**
```
LEFT SIDE (Power):
5V     ● ◄── Relay VCC (Red wire)

LEFT SIDE (Digital):
Pin 7  ● ◄── Relay IN (Orange wire)
```

**Relay Module:**
```
Relay Control Side
┌──────────┐
│ VCC      │──► Red ────► Arduino 5V
│ GND      │──► Black ──► Breadboard (-) rail
│ IN       │──► Orange ─► Arduino Pin 7
└──────────┘
```

---

## 🎯 STEP 5: RELAY SWITCH TO SOLENOID

Connect relay switch side (3 terminals: COM, NO, NC) to solenoid lock.

### **Relay Switch Side:**

| Connection | Wire Color | Description |
|------------|-----------|-------------|
| Breadboard (+) rail → Relay COM | Red (thick) | Power input |
| Relay NO → Solenoid (+) | Red | Power to solenoid |
| Solenoid (-) → Breadboard (-) rail | Black | Ground return |

**⚠️ IMPORTANT:** Use **NO (Normally Open)** terminal, not NC!

**Circuit Flow:**
```
Breadboard (+) rail
      │
      ▼
  Relay COM terminal
      │
  Relay NO terminal
      │
      ▼
Solenoid Lock (+)
      │
Solenoid Lock (-)
      │
      ▼
Breadboard (-) rail
```

**Relay Terminals:**
```
┌──────────────┐
│   Relay      │
│  COM NO NC   │
└───●──●──●────┘
    │  │  │
    │  │  └── (Empty - not used)
    │  │
    │  └─ Red wire ──► Solenoid (+)
    │
    └─ Red wire ──► Breadboard (+) rail
```

---

## 🎯 STEP 6: ESP32 TO ARDUINO (COMMUNICATION)

Connect ESP32 to Arduino for serial communication (3 wires only).

### **ESP32 Communication Wires:**

| ESP32 Pin | To | Wire Color | Purpose |
|-----------|-----|-----------|---------|
| GPIO17 | Arduino Pin 10 | Green | TX - Send data |
| GPIO16 | Arduino Pin 11 | White | RX - Receive data |
| GND | Breadboard (-) rail | Black | Common ground |

**ESP32 Pin Location (38-pin board):**

USB at bottom, count from USB on RIGHT side:
- **8th pin from USB = GPIO16**
- **9th pin from USB = GPIO17**

```
ESP32 (Right side)
┌────────┐
│        │
│        │● GPIO23
│        │● GPIO22
│        │● GPIO1
│        │● GPIO3
│        │● GPIO21
│        │● GND
│        │● GPIO19
│        │● GPIO18
│  ESP32 │● GPIO5
│        │● GPIO17 ◄── Green wire to Arduino Pin 10
│        │● GPIO16 ◄── White wire to Arduino Pin 11
│        │● GPIO4
│        │● GPIO0
└────────┘
   USB
```

**Arduino Digital Pins:**
```
LEFT SIDE (Digital):
Pin 10 ● ◄── Green wire from ESP32 GPIO17
Pin 11 ● ◄── White wire from ESP32 GPIO16
```

**Wiring:**
```
ESP32                          Arduino Uno
┌─────────────┐                ┌─────────────┐
│  GPIO17     ├──Green────────►│ Pin 10      │
│  GPIO16     │◄──White────────┤ Pin 11      │
│  GND        ├──Black───┐     │             │
└─────────────┘          │     └─────────────┘
                         │
                         ▼
                  Breadboard (-) rail
```

---

## 🎯 STEP 7: POWER CONNECTIONS

Final power connections for all devices.

### **Power Summary:**

| Device | Power Source | Connection |
|--------|-------------|------------|
| Arduino | 12V Adapter | Breadboard (+) → Arduino VIN |
| ESP32 | USB Cable | Laptop/USB Adapter |
| LCD | Arduino 5V | Arduino 5V pin |
| Relay | Arduino 5V | Arduino 5V pin |
| Solenoid | 12V via Relay | Breadboard (+) → Relay → Solenoid |

**ESP32 Power:**
```
ESP32
┌─────────────┐
│   [Micro    │
│    USB] ◄───┼──── USB Cable ───► Laptop USB Port
└─────────────┘
```

---

## 📊 COMPLETE CONNECTION TABLE

### **Breadboard (+) Positive Rail Connections:**

| # | Device | Connection |
|---|--------|------------|
| 1 | 12V Adapter (+) | Input power |
| 2 | Arduino VIN | Powers Arduino |
| 3 | Relay COM | Switch input for solenoid |

### **Breadboard (-) Negative Rail Connections:**

| # | Device | Connection |
|---|--------|------------|
| 1 | 12V Adapter (-) | Input ground |
| 2 | Arduino GND | Arduino ground |
| 3 | LCD GND | LCD ground |
| 4 | Relay GND | Relay ground |
| 5 | ESP32 GND | ESP32 ground |
| 6 | Solenoid (-) | Solenoid ground |

### **Arduino Direct Pin Connections:**

| Arduino Pin | Device | Wire Color | Purpose |
|-------------|--------|-----------|---------|
| VIN | Breadboard (+) | Red | 12V power input |
| GND | Breadboard (-) | Black | Ground |
| 5V | LCD VCC | Red | LCD power |
| 5V | Relay VCC | Red | Relay power |
| A4 | LCD SDA | Blue | LCD data |
| A5 | LCD SCL | Yellow | LCD clock |
| Pin 7 | Relay IN | Orange | Relay control |
| Pin 10 | ESP32 GPIO17 | Green | Receive from ESP32 |
| Pin 11 | ESP32 GPIO16 | White | Send to ESP32 |

### **ESP32 Connections:**

| ESP32 Pin | Device | Wire Color | Purpose |
|-----------|--------|-----------|---------|
| GPIO17 | Arduino Pin 10 | Green | TX - send data |
| GPIO16 | Arduino Pin 11 | White | RX - receive data |
| GND | Breadboard (-) | Black | Common ground |
| USB Port | Laptop | USB Cable | Power |

---

## 🎨 COMPLETE SYSTEM DIAGRAM

```
┌─────────────────────────────────────────────────────────┐
│                   POWER DISTRIBUTION                    │
│                                                         │
│              [12V Power Adapter]                        │
│                      │                                  │
│               ┌──────┴──────┐                          │
│               │             │                          │
│              (+)           (-)                         │
│               │             │                          │
│               ▼             ▼                          │
│   ┌────────────────────────────────┐                  │
│   │       BREADBOARD               │                  │
│   │  [++++] Rail    [────] Rail    │                  │
│   │   │ │ │           │ │ │ │ │    │                  │
│   └───┼─┼─┼───────────┼─┼─┼─┼─┼────┘                  │
│       │ │ │           │ │ │ │ │                       │
└───────┼─┼─┼───────────┼─┼─┼─┼─┼───────────────────────┘
        │ │ │           │ │ │ │ │
        │ │ │           │ │ │ │ └──► ESP32 GND
        │ │ │           │ │ │ └────► Solenoid (-)
        │ │ │           │ │ └──────► Relay GND
        │ │ │           │ └────────► LCD GND
        │ │ │           └──────────► Arduino GND
        │ │ │
        │ │ └──────► Relay COM
        │ └────────► Arduino VIN
        │
   ┌────┴─────────┐     ┌──────────┐
   │   Arduino    │     │  ESP32   │
   │              │     │          │
   │ Pin7─────────┼───►Relay       │
   │ Pin10────────┼────┤GPIO17     │
   │ Pin11────────┼────┤GPIO16     │
   │ A4───────────┼─LCD├──────     │
   │ A5───────────┼─SDA│  USB ◄────┼─── Laptop
   │ 5V───────────┼─SCL└──────────┘
   └──────────────┘ VCC
         │
      [Relay]──► Solenoid Lock
```

---

## ✅ PRE-FLIGHT CHECKLIST

Before powering on, verify all connections:

### **Breadboard Connections:**
- [ ] 12V Adapter (+) connected to breadboard (+) rail
- [ ] 12V Adapter (-) connected to breadboard (-) rail
- [ ] Breadboard (+) has 3 wires: Arduino VIN, Relay COM, (spare)
- [ ] Breadboard (-) has 6 wires: 12V (-), Arduino GND, LCD GND, Relay GND, ESP32 GND, Solenoid (-)

### **Arduino Power Connections:**
- [ ] Arduino VIN ← Red wire ← Breadboard (+)
- [ ] Arduino GND ← Black wire ← Breadboard (-)
- [ ] Arduino 5V → LCD VCC (Red)
- [ ] Arduino 5V → Relay VCC (Red)

### **Arduino Data Connections:**
- [ ] Arduino A4 ← LCD SDA (Blue)
- [ ] Arduino A5 ← LCD SCL (Yellow)
- [ ] Arduino Pin 7 ← Relay IN (Orange)
- [ ] Arduino Pin 10 ← ESP32 GPIO17 (Green)
- [ ] Arduino Pin 11 ← ESP32 GPIO16 (White)

### **Relay Connections:**
- [ ] Relay VCC → Arduino 5V
- [ ] Relay GND → Breadboard (-)
- [ ] Relay IN → Arduino Pin 7
- [ ] Relay COM ← Breadboard (+)
- [ ] Relay NO → Solenoid (+)

### **Solenoid Connections:**
- [ ] Solenoid (+) ← Relay NO
- [ ] Solenoid (-) → Breadboard (-)

### **ESP32 Connections:**
- [ ] ESP32 GPIO17 → Arduino Pin 10 (Green)
- [ ] ESP32 GPIO16 → Arduino Pin 11 (White)
- [ ] ESP32 GND → Breadboard (-) (Black)
- [ ] ESP32 USB → Laptop power

### **LCD Connections:**
- [ ] LCD VCC → Arduino 5V (Red)
- [ ] LCD GND → Breadboard (-) (Black)
- [ ] LCD SDA → Arduino A4 (Blue)
- [ ] LCD SCL → Arduino A5 (Yellow)

---

## 🧪 TESTING PROCEDURE

### **Step 1: Visual Inspection**
1. Check all wire connections are tight
2. Verify no loose wires touching each other
3. Confirm polarity: Red = (+), Black = (-)
4. Check breadboard rails have power

### **Step 2: Power On Test**
1. Connect ESP32 USB to laptop
2. ESP32 should power on (LED lights up)
3. Check Serial Monitor: Should show "WiFi Connected"

### **Step 3: Arduino Upload**
1. Keep ESP32 connected
2. Connect Arduino USB to laptop
3. Upload `Arduino_Lock_Controller.ino`
4. LCD should light up and show: "Smart Lock v1.0"

### **Step 4: Communication Test**
1. Both boards connected and running
2. Send `/unlock` to Telegram bot
3. ESP32 receives command
4. LCD shows: "OTP Sent! Check Telegram"
5. Telegram shows 4-digit OTP

### **Step 5: Unlock Test**
1. Type OTP in Telegram
2. ESP32 validates OTP
3. Arduino receives "UNLOCK_OK"
4. Relay clicks (audible)
5. LCD shows: "Access Granted!"
6. Solenoid unlocks (buzzing sound)
7. After 5 seconds, auto-locks

---

## 🔧 TROUBLESHOOTING

### **LCD Not Working:**
**Problem:** No display or gibberish
**Solution:**
- Check I2C address: Try 0x27 or 0x3F in code
- Verify SDA/SCL connections (A4/A5)
- Check 5V and GND connections
- Adjust contrast potentiometer on LCD

### **Relay Not Clicking:**
**Problem:** No sound when unlocking
**Solution:**
- Check Pin 7 connection
- Verify 5V and GND to relay
- Test with LED on Pin 7
- Check relay type (active HIGH/LOW)

### **Solenoid Not Working:**
**Problem:** Relay clicks but solenoid doesn't move
**Solution:**
- Check 12V power supply
- Verify COM and NO connections
- Test with multimeter (12V at NO when relay ON)
- Check solenoid polarity

### **ESP32-Arduino Not Communicating:**
**Problem:** OTP doesn't reach Arduino
**Solution:**
- Verify TX→RX, RX→TX cross connection
- Check GND connection (CRITICAL!)
- Test baud rate (9600)
- Swap TX/RX if needed

### **ESP32 Upload Failed:**
**Problem:** "Packet transfer stopped" error
**Solution:**
- Hold BOOT button during upload
- Change upload speed to 115200
- Try different USB cable
- Check USB drivers

---

## 📱 TELEGRAM COMMANDS

| Command | Description |
|---------|-------------|
| `/start` | Show welcome and help |
| `/unlock` | Generate OTP to unlock |
| `/status` | Check system status |
| `1234` | Enter 4-digit OTP (your actual OTP) |

---

## 💾 CODE FILES LOCATION

**ESP32 Code:**
```
C:\Users\Abhijeet Nardele\Desktop\ldmfinal_project\ESP32_Telegram_OTP\ESP32_Telegram_OTP.ino
```

**Arduino Code:**
```
C:\Users\Abhijeet Nardele\Desktop\ldmfinal_project\Arduino_Lock_Controller\Arduino_Lock_Controller.ino
```

**This Guide:**
```
C:\Users\Abhijeet Nardele\Desktop\ldmfinal_project\COMPLETE_WIRING_GUIDE.md
```

---

## 📊 QUICK REFERENCE - PIN SUMMARY

### **Arduino Uno Pin Usage:**

| Pin | Device | Function |
|-----|--------|----------|
| VIN | 12V Power | Input power |
| 5V | LCD + Relay | Power output |
| GND | Common | Ground |
| A4 | LCD SDA | I2C data |
| A5 | LCD SCL | I2C clock |
| Pin 7 | Relay IN | Control signal |
| Pin 10 | ESP32 RX | Serial receive |
| Pin 11 | ESP32 TX | Serial transmit |

### **ESP32 Pin Usage:**

| Pin | Device | Function |
|-----|--------|----------|
| GPIO17 | Arduino Pin 10 | Serial TX |
| GPIO16 | Arduino Pin 11 | Serial RX |
| GND | Common ground | Ground |
| USB | Power | 5V input |

---

## 🎯 SUCCESS INDICATORS

✅ **System is working when:**
1. LCD shows "Smart Lock Ready - Locked"
2. Telegram receives "System Online" message
3. `/unlock` generates OTP on Telegram
4. LCD updates to "OTP Sent!"
5. Entering OTP unlocks door (relay clicks)
6. LCD shows "Access Granted!"
7. Door auto-locks after 5 seconds

---

## 📝 NOTES

**Important Points:**
- All grounds MUST be connected together via breadboard (-) rail
- Use breadboard for clean power distribution
- ESP32 and Arduino run independently after upload
- Both boards need to be powered for system to work
- OTP expires after 60 seconds
- Door auto-locks after 5 seconds

**Safety:**
- Check 12V adapter polarity before connecting
- Ensure solenoid is rated for 12V
- Don't exceed relay current rating
- Keep wires organized to avoid shorts

---

## 🎉 PROJECT COMPLETED!

**Date:** November 11, 2025  
**Status:** ✅ FULLY WORKING  
**Tested:** ESP32 OTP generation + Arduino hardware control  
**Result:** SUCCESS!  

**Congratulations on completing this project! 🚀**

---

**For support or questions, refer to:**
- README.md (general info)
- Code comments (in .ino files)
- This wiring guide (complete reference)

**Project saved in:**
`C:\Users\Abhijeet Nardele\Desktop\ldmfinal_project\`

---

**END OF WIRING GUIDE**
