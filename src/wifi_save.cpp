#include "wifi_save.h"

WiFiServer server(80);
int client_count = 0;

int record_rst_time() {
  int rst_time = 0;
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
  printf("\n");
  printf("Opening Non-Volatile Storage (NVS) handle... ");
  nvs_handle my_handle;
  err = nvs_open("storage", NVS_READWRITE, &my_handle);
  if (err != ESP_OK) {
    printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
  } else {
    printf("Done\n");
    printf("Reading restart counter from NVS ... ");
    int32_t restart_counter = 0;
    err = nvs_get_i32(my_handle, "restart_counter", &restart_counter);
    switch (err) {
      case ESP_OK:
        printf("Done\n");
        printf("Restart counter = %d\n", restart_counter);
        rst_time = restart_counter;
        break;
      case ESP_ERR_NVS_NOT_FOUND:
        printf("The value is not initialized yet!\n");
        break;
      default:
        printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
    printf("Updating restart counter in NVS ... ");
    restart_counter++;
    err = nvs_set_i32(my_handle, "restart_counter", restart_counter);
    printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
    printf("Committing updates in NVS ... ");
    err = nvs_commit(my_handle);
    printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
    nvs_close(my_handle);
  }

  printf("\n");
  return rst_time;
}

void record_wifi(char *ssid, char *password) {
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
  printf("\n");
  printf("Opening Non-Volatile Wifi (NVS) handle... ");
  nvs_handle my_handle;
  err = nvs_open("Wifi", NVS_READWRITE, &my_handle);
  if (err != ESP_OK) {
    printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
  } else {
    printf("Done\n");

    // Write
    printf("Updating ssid in NVS ... ");
    err = nvs_set_str(my_handle, "ssid", ssid);
    printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

    printf("Updating password in NVS ... ");
    err = nvs_set_str(my_handle, "password", password);
    printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

    printf("Committing updates in NVS ... ");
    err = nvs_commit(my_handle);
    printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

    nvs_close(my_handle);
  }

  printf("\n");
}

void check_wifi(char *ssid, char *password) {
  char saved_ssid[SSID_LENGTH];
  size_t ssid_length = SSID_LENGTH;
  char saved_password[SSID_LENGTH];
  size_t password_length = SSID_LENGTH;
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
  printf("\n");
  printf("Opening Non-Volatile Wifi (NVS) handle... \n");
  nvs_handle my_handle;
  err = nvs_open("Wifi", NVS_READWRITE, &my_handle);
  if (err != ESP_OK) {
    printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
  } else {
    printf("Done\n");
    printf("Reading ssid and password from NVS ... \n");
    err = nvs_get_str(my_handle, "ssid", saved_ssid, &ssid_length);
    switch (err) {
      case ESP_OK:
        printf("Done\n");
        printf("ssid: %s\n", saved_ssid);
        printf("ssid length= %d\n", ssid_length);
        strcpy(ssid, saved_ssid);
        break;
      case ESP_ERR_NVS_NOT_FOUND:
        printf("The value is not initialized yet!\n");
        Serial.println("Please connect to \"RPM-Display Config\".");
      Serial.println("And visit 192.168.4.1 to set WIFI.");
      ap_init();
      while (wifi_config_server())
      digitalWrite(LED_BLUE, LOW);
        ;
      delay(3000);
      esp_restart();
        break;
      default:
        printf("Error (%s) reading!\n", esp_err_to_name(err));
    }

    err = nvs_get_str(my_handle, "password", saved_password, &password_length);
    switch (err) {
      case ESP_OK:
        printf("Done\n");
        strcpy(password, saved_password);
        break;
      case ESP_ERR_NVS_NOT_FOUND:
        printf("The value is not initialized yet!\n");
        break;
      default:
        printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
    nvs_close(my_handle);
  }

  printf("\n");
  return;
}

void ap_init() {
  WiFi.softAP("RPM-Display Config");
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
}

int wifi_config_server() {

  WiFiClient client = server.available();

  if (client)
  {
    Serial.println("---------------------------------------------------");
    Serial.printf("Index:%d\n", client_count);
    client_count++;
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("<!doctype html>");
            client.print("<head>");
            client.print("<title>RPM-Display - Config Mode</title>");
            client.print("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"/>");
            client.print("</head>");
            client.print("<center><h1>RPM-Display</h1><br><h2>ESP32 WIFI CONFIG</h2><br></center>");
            client.print("<center>Click <a href=\"/wifi_set\">here</a> to set WIFI.<br></center>");
            client.print("</body>");
            client.print("</html>");
            client.println();
          break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
        if (currentLine.endsWith("GET /wifi_set")) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

            client.print("<!doctype html>");
            client.print("<head>");
            client.print("<title>RPM-Display - Config Mode - WiFi</title>");
            client.print("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"/>");
            client.print("</head>");
            client.print("<center><h1>RPM-Display</h1><br><h2>ESP32 WIFI CONFIG</h2><br></center>");
            client.print("<center><form action=\"/set_over\">SSID:<br><input type=\"text\" name=\"ssid\"><br>PASSWORD:<br><input type=\"password\" name=\"password\"><br><br></center>");
            client.print("<center><input type=\"submit\" value=\"Set\"></form></center>");
            client.print("</body>");
            client.print("</html>");
            client.println();
          break;
        }

        if (currentLine.endsWith("GET /set_over")) {
          String get_request = "";
          while (1) {
            char c_get = client.read();
            Serial.write(c_get);
            if (c_get == '\n') {
              break;
            } else {
              get_request += c_get;
            }
          }

          set_wifi_from_url(get_request);

          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
            client.print("<!doctype html>");
            client.print("<head>");
            client.print("<title>RPM-Display - Config Mode - Success</title>");
            client.print("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"/>");
            client.print("</head>");
            client.print("<center><h1>RPM-Display</h1><br><h2>ESP32 WIFI CONFIG</h2><br></center>");
            client.print("<center>Set Successful<br></center>");
            client.print("</body>");
            client.print("</html>");
            client.println();
          client.stop();
          Serial.println("Client Disconnected.");

          return 0;
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
  return 1;
}

void set_wifi_from_url(String get_url) {
  int str_len = 0;
  int ssid_add = 0;
  int pwd_add = 0;
  int end_add = 0;

  String ssid = "";
  String pwd = "";

  str_len = get_url.length();
  ssid_add = get_url.indexOf('?');
  pwd_add = get_url.indexOf('&');
  end_add = get_url.indexOf(' ');

  ssid = get_url.substring(ssid_add + 6, pwd_add);
  pwd = get_url.substring(pwd_add + 10, end_add);

  Serial.println();
  Serial.println("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
  Serial.println("Get ssid and password from url:");
  Serial.println(get_url);
  Serial.println(ssid);
  Serial.println(pwd);
  Serial.println("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");

  record_wifi((char *)ssid.c_str(), (char *)pwd.c_str());
}

int wifi_set_main() {
  char ssid[SSID_LENGTH];
  char password[SSID_LENGTH];
  pinMode(WIFI_SET_PIN, INPUT_PULLUP);
  pinMode(LED_BLUE, OUTPUT);
  check_wifi(ssid, password);
  Serial.println("Check WIFI_SET_PIN");
  digitalWrite(LED_BLUE, HIGH);
  int runtime = millis();
  int starttime = runtime;
  while ((runtime - starttime) < 3000) {
    if (digitalRead(WIFI_SET_PIN) == LOW) {
      Serial.println("Please connect to \"RPM-Display Config\".");
      Serial.println("And visit 192.168.4.1 to set WIFI.");
      ap_init();
      while (wifi_config_server())
      digitalWrite(LED_BLUE, LOW);
        ;
      delay(3000);
      esp_restart();
      return 0;
    }
    Serial.print(".");
    delay(100);
    runtime = millis();
  }
  Serial.println();
  Serial.println("Connecting WIFI");
  digitalWrite(LED_BLUE, HIGH);
  WiFi.begin(ssid, password);

  int connect_count = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    connect_count++;
    if (connect_count > 10)
      return 0;
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  return 1;
}

void nvs_test() {
  char ssid[SSID_LENGTH] = "";
  char password[SSID_LENGTH] = "";
  int rst_time = 0;

  check_wifi(ssid, password);
  rst_time = record_rst_time();

  sprintf(ssid, "ssid_%d", rst_time);
  sprintf(password, "password_%d", rst_time);

  record_wifi(ssid, password);
  for (int i = 10; i >= 0; i--) {
    printf("Restarting in %d seconds...\n", i);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  printf("Restarting now.\n");
  fflush(stdout);
  esp_restart();
}