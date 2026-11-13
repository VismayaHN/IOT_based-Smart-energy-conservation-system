
#include <WiFi.h>
#include <WebServer.h>

// ---------- WiFi (CHANGE THESE) ----------
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

// ---------- Pins (from circuit diagram) ----------
const int IR_PIN    = 2;    // D2  - IR sensor OUT
const int LDR_PIN   = 12;   // D12 - LDR module DO
const int RELAY_PIN = 5;    // D5  - Relay IN (fan)
const int LED_PIN   = 4;    // D4  - LED / bulb indicator

// ---------- Sensor / relay polarity ----------
// Most IR modules output LOW when a person/object is detected.
const bool IR_ACTIVE_LOW    = true;
// Most LDR modules output HIGH when it is DARK (adjust with the pot).
const bool LDR_DARK_IS_HIGH = true;
// Most 1-channel relay boards turn ON with a LOW signal.
const bool RELAY_ACTIVE_LOW = true;

// ---------- Timing ----------
const unsigned long HOLD_TIME = 10000;  // ms to keep ON after last detection

WebServer server(80);

bool occupied = false;
bool dark = false;
bool loadsOn = false;
unsigned long lastSeen = 0;
unsigned long onSince = 0;
unsigned long totalOnMs = 0;   // total time loads were ON

void setLoads(bool on) {
  digitalWrite(RELAY_PIN, (on ^ RELAY_ACTIVE_LOW) ? HIGH : LOW);
  digitalWrite(LED_PIN, on ? HIGH : LOW);
  if (on && !loadsOn)  onSince = millis();
  if (!on && loadsOn)  totalOnMs += millis() - onSince;
  loadsOn = on;
}

void readSensors() {
  int ir  = digitalRead(IR_PIN);
  int ldr = digitalRead(LDR_PIN);
  occupied = IR_ACTIVE_LOW ? (ir == LOW) : (ir == HIGH);
  dark     = LDR_DARK_IS_HIGH ? (ldr == HIGH) : (ldr == LOW);
}

void controlLogic() {
  readSensors();
  if (occupied) lastSeen = millis();

  bool recentlyOccupied = (millis() - lastSeen) < HOLD_TIME && lastSeen != 0;

  if (recentlyOccupied && dark) {
    if (!loadsOn) setLoads(true);
  } else {
    if (loadsOn) setLoads(false);
  }
}

String statusJson() {
  unsigned long onMs = totalOnMs + (loadsOn ? millis() - onSince : 0);
  String s = "{";
  s += "\"occupied\":" + String(occupied ? "true" : "false") + ",";
  s += "\"dark\":"     + String(dark ? "true" : "false") + ",";
  s += "\"loads\":"    + String(loadsOn ? "true" : "false") + ",";
  s += "\"on_seconds\":" + String(onMs / 1000) + ",";
  s += "\"uptime_seconds\":" + String(millis() / 1000);
  s += "}";
  return s;
}

const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head>
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Smart Energy Conservation</title>
<style>
 body{font-family:sans-serif;background:#0f172a;color:#e2e8f0;text-align:center;margin:0;padding:16px}
 h2{margin:8px 0 16px}
 .card{background:#1e293b;border-radius:12px;padding:16px;margin:10px auto;max-width:360px}
 .v{font-size:1.6rem;font-weight:bold}
 .on{color:#22c55e}.off{color:#ef4444}
</style></head><body>
<h2>IoT Smart Energy Conservation</h2>
<div class="card">Room<div id="occ" class="v">--</div></div>
<div class="card">Brightness<div id="dark" class="v">--</div></div>
<div class="card">Light + Fan<div id="loads" class="v">--</div></div>
<div class="card">Total ON time<div id="on" class="v">--</div></div>
<script>
function set(id,t,c){let e=document.getElementById(id);e.textContent=t;e.className='v '+c}
async function u(){
 try{
  let d=await (await fetch('/status')).json();
  set('occ',d.occupied?'Occupied':'Vacant',d.occupied?'on':'off');
  set('dark',d.dark?'Dark':'Bright',d.dark?'off':'on');
  set('loads',d.loads?'ON':'OFF',d.loads?'on':'off');
  document.getElementById('on').textContent=d.on_seconds+' s';
 }catch(e){}
}
setInterval(u,1000);u();
</script></body></html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(IR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  setLoads(false);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected! Open this on your phone: http://" + WiFi.localIP().toString());
  } else {
    Serial.println("\nWiFi failed - system still works offline.");
  }

  server.on("/", []() { server.send_P(200, "text/html", PAGE); });
  server.on("/status", []() { server.send(200, "application/json", statusJson()); });
  server.begin();
}

void loop() {
  controlLogic();
  server.handleClient();

  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 1000) {
    lastPrint = millis();
    Serial.printf("Occupied:%d Dark:%d Loads:%d\n", occupied, dark, loadsOn);
  }
}
