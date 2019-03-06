// récupère la config
void config_setup() {
  SPIFFS.begin();
  File configuration = SPIFFS.open("/config.txt", "r");
  StaticJsonBuffer<200> jsonBuffer;
  char char_json[200];

  String json = configuration.readStringUntil('\n');
  json.toCharArray(char_json, 200);
  JsonObject& root = jsonBuffer.parseObject(char_json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  const char* ip_char = root["ip"];
  const char* name_char = root["name"];
  const char* access_point_char = root["mode"];
  const char* ssid_char = root["ssid"];
  const char* mdp_char = root["mdp"];
  const char* canal_char = root["canal"];
  const char* invert_motors_char = root["invert_motors"];

  ip = String(ip_char);
  name = String(name_char);
  access_point = String(access_point_char);
  ssid = String(ssid_char);
  mdp = String(mdp_char);
  canal = String(canal_char);
  invert_motors = String(invert_motors_char);

}

// Met à jour la configuration du robot
String config_update (AsyncWebServerRequest * request) {
      name = request->arg("name");
      invert_motors = request->arg("invert_motors");
      access_point = request->arg("mode");
      ssid = request->arg("ssid");
      mdp = request->arg("mdp");
      canal = request->arg("canal");
      ip = request->arg("ip");
      String json;
      json += "{";
      json += "\"name\":\"" + name + "\"";
      json += ",\"invert_motors\":\"" + invert_motors + "\"";
      json += ",\"mode\":\"" + access_point + "\"";
      json += ",\"ssid\":\"" + ssid + "\"";
      json += ",\"mdp\":\"" + mdp + "\"";
      json += ",\"canal\":\"" + canal + "\"";
      json += ",\"ip\":\"" + ip + "\"";
      json += "}";
      File configuration = SPIFFS.open("/config.txt", "w");
      configuration.println(json);
      configuration.close();
      return json;
}
