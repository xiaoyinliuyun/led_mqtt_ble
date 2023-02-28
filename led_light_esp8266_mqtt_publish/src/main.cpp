#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "HUAWEI-802-A";
const char* password = "y781881228";

const char* mqttServer = "test.ranye-iot.net";

int delayTime = 1000;  // 1m

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void connectMQTTServer() {
  String clientId = "esp8266-" + WiFi.macAddress();

  if (mqttClient.connect(clientId.c_str())) {
    Serial.println("Connect Success. ");
    Serial.print("Server Address: ");
    Serial.println(mqttServer);
    Serial.print("ClientId: ");
    Serial.println(clientId);
  } else {
    Serial.println("Connect Failure.");
    Serial.print("Client State: ");
    Serial.println(mqttClient.state());
    delay(3000);
  }
}

void connectWifi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected!");
  Serial.println();
}

void sendCode(bool r, bool g, bool b, int i) {
  // 根据rgb和 i，组成下列指令
  byte high = 0x01;
  if (r) {
    high = high << 2;
  } else if (g) {
    high = high << 1;
  }
  // high = high << 8;
  byte low = (byte)i;

  // 待发布的信息h4l255
  // Serial.print('H');
  // Serial.print(high);
  // Serial.print('L');
  // Serial.print(low);
  // Serial.print('\0');

  // 0100(0x0400 ~ 0x04FF), 0010(0x0200 ~0x02FF), 0001(0x0100 ~ 0x01FF)
  // 确定发布的主题：
  String topic = "sos";
  char publishTopic[topic.length() + 1];
  strcpy(publishTopic, topic.c_str());

  char publishMessage[] = { 'H', high, 'L', i, '\0' };

  if (mqttClient.connected()) {
    // 保持客户端心跳
    mqttClient.loop();

    if (mqttClient.publish(publishTopic, publishMessage)) {
      Serial.print("Topic: ");
      Serial.println(publishTopic);
      Serial.print("Message: ");
      Serial.println(publishMessage);
    } else {
      Serial.println("Publish Failed");
    }

  } else {
    connectMQTTServer();
  }
}

/**
 * 数值增大，亮度减弱
 */
void increase(bool r, bool g, bool b) {
  for (int i = 0; i < 256; i += 8) {
    sendCode(r, g, b, i);
    delay(delayTime);
  }
}

/**
 * 数值减小，亮度增强
 */
void decrease(bool r, bool g, bool b) {
  for (int i = 255; i >= 0; i -= 8) {
    sendCode(r, g, b, i);
    delay(delayTime);
  }
}

void setup() {
  // put your setup code here, to run once:
  // put your setup code here, to run once:
  Serial.begin(9600);

  // 设置无线终端模式
  WiFi.mode(WIFI_STA);
  // 连接WiFi
  connectWifi();

  // 设置要连接的服务器及端口
  mqttClient.setServer(mqttServer, 1883);
  // 连接mqtt服务器
  connectMQTTServer();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // 业务操作
  decrease(true, false, false);
  increase(true, false, false);
  decrease(false, true, false);
  increase(false, true, false);
  decrease(false, false, true);
  increase(false, false, true);
}