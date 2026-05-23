#include <SoftwareSerial.h>

#define TRIG_PIN 9
#define ECHO_PIN 8

SoftwareSerial BT(2, 3); // D2=RX, D3=TX

// مقادیر کالیبراسیون
float EMPTY_CM = -1;  // فاصله در حالت خالی
float FULL_CM  = -1;  // فاصله در حالت پر

// ✅ آستانه هشدار (درصد) - قابل تغییر با فرمان Txx
int LEVEL_THRESHOLD = 20;  // پیش‌فرض 20%

// برای دریافت فرمان‌های چندکاراکتری مثل T30
String cmd = "";

// ---- ابزارهای کمکی ----
float clampf(float x, float a, float b) {
  if (x < a) return a;
  if (x > b) return b;
  return x;
}

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;

  return duration * 0.0343f / 2.0f;
}

void printHelp() {
  BT.println("Commands:");
  BT.println("  E      -> set EMPTY (tank empty)");
  BT.println("  F      -> set FULL  (tank full)");
  BT.println("  Txx    -> set LOW threshold percent (e.g. T30)");
  BT.println("  S      -> show calibration + threshold");
  BT.println("  ?      -> help");
}

void showCalibration() {
  BT.print("EMPTY_CM = ");
  if (EMPTY_CM < 0) BT.println("NOT SET");
  else { BT.print(EMPTY_CM, 1); BT.println(" cm"); }

  BT.print("FULL_CM  = ");
  if (FULL_CM < 0) BT.println("NOT SET");
  else { BT.print(FULL_CM, 1); BT.println(" cm"); }

  if (EMPTY_CM >= 0 && FULL_CM >= 0) {
    BT.print("Range (EMPTY-FULL) = ");
    BT.print(EMPTY_CM - FULL_CM, 1);
    BT.println(" cm");
  }

  BT.print("LOW LEVEL THRESHOLD = ");
  BT.print(LEVEL_THRESHOLD);
  BT.println("%");
}

void handleCommand(const String &c) {
  if (c.length() == 0) return;

  // تک‌حرفی‌ها
  if (c == "E" || c == "e") {
    float d = readDistanceCm();
    if (d < 0) BT.println("ERR: No echo. Try again.");
    else {
      EMPTY_CM = d;
      BT.print("OK: EMPTY set to ");
      BT.print(EMPTY_CM, 1);
      BT.println(" cm");
    }
    return;
  }

  if (c == "F" || c == "f") {
    float d = readDistanceCm();
    if (d < 0) BT.println("ERR: No echo. Try again.");
    else {
      FULL_CM = d;
      BT.print("OK: FULL set to ");
      BT.print(FULL_CM, 1);
      BT.println(" cm");
    }
    return;
  }

  if (c == "S" || c == "s") {
    showCalibration();
    return;
  }

  if (c == "?") {
    printHelp();
    return;
  }

  // فرمان آستانه: Txx
  if (c.length() >= 2 && (c[0] == 'T' || c[0] == 't')) {
    String numPart = c.substring(1);     // بعد از T
    numPart.trim();
    int val = numPart.toInt();           // اگر عدد نباشه میشه 0

    // اعتبارسنجی: 0..100
    if (val < 0 || val > 100) {
      BT.println("ERR: Threshold must be 0..100 (e.g. T30)");
      return;
    }

    LEVEL_THRESHOLD = val;
    BT.print("OK: LOW threshold set to ");
    BT.print(LEVEL_THRESHOLD);
    BT.println("%");
    return;
  }

  BT.print("Unknown cmd: ");
  BT.println(c);
  BT.println("Send ? for help");
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  BT.begin(9600);
  BT.println("Ultrasonic Level Monitor");
  printHelp();
  showCalibration();
}

void loop() {
  // ---- دریافت فرمان از موبایل (تا انتهای خط) ----
  while (BT.available()) {
    char ch = BT.read();

    // پایان خط: فرمان کامل شد
    if (ch == '\n' || ch == '\r') {
      if (cmd.length() > 0) {
        handleCommand(cmd);
        cmd = "";
      }
    } else {
      // جلوگیری از طولانی شدن بی‌دلیل
      if (cmd.length() < 20) cmd += ch;
    }
  }

  // ---- ارسال وضعیت هر 500ms ----
  static unsigned long lastSend = 0;
  if (millis() - lastSend >= 500) {
    lastSend = millis();

    float d = readDistanceCm();
    if (d < 0) {
      BT.println("Distance: NO ECHO");
      return;
    }

    if (EMPTY_CM < 0 || FULL_CM < 0 || (EMPTY_CM - FULL_CM) < 1.0f) {
      BT.print("Distance: ");
      BT.print(d, 1);
      BT.println(" cm | Calib: NOT READY (send E then F)");
      return;
    }

    float range = (EMPTY_CM - FULL_CM);

    float height = EMPTY_CM - d;
    height = clampf(height, 0, range);

    float pct = (height / range) * 100.0f;
    pct = clampf(pct, 0, 100);

    BT.print("Distance: ");
    BT.print(d, 1);
    BT.print(" cm | Height: ");
    BT.print(height, 1);
    BT.print(" cm | Level: ");
    BT.print(pct, 0);
    BT.print("% | Th:");
    BT.print(LEVEL_THRESHOLD);
    BT.print("%");

    if ((int)pct < LEVEL_THRESHOLD) {
      BT.print("  ⚠️ WARNING: LOW LEVEL!");
    }

    BT.println();
  }
}
