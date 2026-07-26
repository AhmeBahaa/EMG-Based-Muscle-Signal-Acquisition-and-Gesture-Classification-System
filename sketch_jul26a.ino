// Modified by: Ahmed Bahaa El-din bahaaahmed202@gmail.com 
/* 
   EMG Monitor (ESP32-C3)
   - EMG on GPIO4
   - LED on GPIO2
   - Web graph FIXED version
*/
#include <WiFi.h>
#include <WebServer.h>
const char* ssid = "-";
const char* password = "-";
const int emgPin = 4;
const int ledPin = 2;
float alpha = 0.59;
int rawValue = 0;
int filteredValue = 0;
float fVal = 0;
int thresholdValue = 12;
bool active = false;
unsigned long activeTime = 0;
const unsigned long ledTime = 5000;
WebServer server(80);
// -------------------------------------------------------
// SIMPLE HTML (NO PROGMEM → NO CRASH)
// -------------------------------------------------------
String htmlPage = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>EMG Monitor</title>
<style>
body { font-family: Arial; padding: 20px; background: #f5f5f5; }
.card { background:white; padding:15px; border-radius:10px; box-shadow:0 0 8px #888; }
canvas { width:100%; background:black; margin-top:10px; }
</style>
</head>
<body>
<h2>ESP32-C3 EMG Monitor</h2>
<div class="card">
Raw: <span id="raw">--</span><br>
Filtered: <span id="filtered">--</span><br>
LED: <span id="led">--</span>
</div>
<canvas id="graph" width="400" height="150"></canvas>
<script>
let ctx = document.getElementById("graph").getContext("2d");
let points = new Array(200).fill(0);

function draw() {
  ctx.fillStyle = "black";
  ctx.fillRect(0,0,400,150);
  ctx.strokeStyle = "lime";
  ctx.beginPath();
  for(let i=0;i<points.length;i++){
      let x = i * (400/200);
      let y = 150 - (points[i] / 500.0) * 150; 
      if(i==0) ctx.moveTo(x,y);
      else ctx.lineTo(x,y);
  }
  ctx.stroke();
}
function update(){
  fetch("/data")
  .then(r => r.json())
  .then(d => {
    document.getElementById("raw").innerText = d.raw;
    document.getElementById("filtered").innerText = d.filtered;
    document.getElementById("led").innerText = d.led ? "ON":"OFF";
    points.push(d.filtered);
    if(points.length > 200) points.shift();
    draw();
  });
}
setInterval(update, 100);
</script>
</body>
</html>
)=====";
// -------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting...");
  while(WiFi.status() != WL_CONNECTED){
    delay(300);
    Serial.print(".");
  }
  Serial.print("\nIP: ");
  Serial.println(WiFi.localIP());
  server.on("/", [](){
    server.send(200, "text/html", htmlPage);
  });
  server.on("/data", [](){
    String j = "{";
    j += "\"raw\":" + String(rawValue) + ",";
    j += "\"filtered\":" + String(filteredValue) + ",";
    j += "\"led\":" + String(digitalRead(ledPin));
    j += "}";
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", j);
  });
  server.begin();
  Serial.println("Server started");
}
void loop() {
  server.handleClient();
  rawValue = analogRead(emgPin);
  fVal = alpha * rawValue + (1 - alpha) * fVal;
  filteredValue = (int)fVal;

  if(!active && filteredValue > thresholdValue){
    active = true;
    activeTime = millis();
    digitalWrite(ledPin, HIGH);
  }
  if(active && millis() - activeTime >= ledTime){
    active = false;
    digitalWrite(ledPin, LOW);
  }
  Serial.print(rawValue); Serial.print(",");
  Serial.print(filteredValue); Serial.print(",");
  Serial.println(active ? 1 : 0);
  delay(20);
}
