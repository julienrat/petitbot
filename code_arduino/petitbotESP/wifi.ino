
// Crée la connection au réseau WIFI
void wifi_setup() {
  //WiFi.hostname(name.c_str());
  if (access_point.equals("access_point")) {
    wifi_createAccessPoint();
  } else {
    wifi_connect();
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Crée un point d'accès wifi
void wifi_createAccessPoint () {
  Serial.println("Configuration Point d'acces");
  Serial.print("nom du point d'acces : ");
  // Serial.println(name);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("DNSServer CaptivePortal example");
}

// Connecte le robot à un réseau wifi existant
void wifi_connect () {
  Serial.print("Connexion au reseau : ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  
  int timeout = 0;
  WiFi.begin(ssid.c_str(), mdp.c_str());
  
  Serial.println("connection en cours...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    timeout++;
    Serial.print(".");

    // Si la connection au réseau échoue, on passe en point d'accès
    if (timeout > 50) {
      Serial.println("TimeOUT");
      access_point = "access_point";
      wifi_createAccessPoint();
      return;
    }
  }
}

// Retourne un scan des réseau wifi au format JSON dans une string
String wifi_getJSONScan() {
    String jsonResult = "[";
    int n = WiFi.scanComplete();
    if (n == -2) {
      WiFi.scanNetworks(true);
    } else if (n) {
      for (int i = 0; i < n; ++i) {
        if (i) jsonResult += ",";
        jsonResult += "{";
        jsonResult += "\"rssi\":" + String(WiFi.RSSI(i));
        jsonResult += ",\"ssid\":\"" + WiFi.SSID(i) + "\"";
        jsonResult += ",\"bssid\":\"" + WiFi.BSSIDstr(i) + "\"";
        jsonResult += ",\"channel\":" + String(WiFi.channel(i));
        jsonResult += ",\"secure\":" + String(WiFi.encryptionType(i));
        // TODO : fix this
       // jsonResult += ",\"hidden\":" + String(WiFi.isHidden(i) ? "true" : "false");
        jsonResult += "}";
      }
      WiFi.scanDelete();
      if (WiFi.scanComplete() == -2) {
        WiFi.scanNetworks(true);
      }
    }
    jsonResult += "]";
    return jsonResult;
}
