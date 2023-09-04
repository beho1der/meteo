
#define CLIENT_ID     "meteo_hall" // Must be unique on the MQTT network

// wifi settings сохраняются при первой настройке. Доступ к настройкам по адресу http://192.168.4.1 
#define WIFI_SSID "captain2G"
#define WIFI_PASSWORD  "137137137"

// mqtt server settings
#define MQTT_SERVER    "192.168.32.70"
#define MQTT_PORT      1884
#define MQTT_USER      "root"
#define MQTT_PASSWORD  "password"

// mqtt client settings
// Note PubSubClient.h has a MQTT_MAX_PACKET_SIZE of 128 defined, so either raise it to 256 or use short topics
#define CONTROL_TOPIC        "home/hall/#"
#define ONLINE_TOPIC         "home/hall/meteo_hall/online"
#define CURR_TEMP            "home/hall/meteo_hall/temperature/state"
#define HUMIDITY             "home/hall/meteo_hall/humidity/state"
#define PRESSURE             "home/hall/meteo_hall/pressure/state"
#define CURR_CO2             "home/hall/meteo_hall/co2/state"
#define METEO_SETTINGS           "home/hall/sensor/meteo/set"
/*  Датчик CO2
 *   
 *   Команды:
 *   home/sensors/co2_test/set    zero       Калибровка 400ppm по текущей концентрации CO2 в воздухе
 *   home/sensors/co2_test/set    abc_off    Выключение автокалибровки
 *   home/sensors/co2_test/set    abc_on     Включение автокалибровки
 *   
 */


// sketch settings
const unsigned int READ_SENSORS_INTERVAL_MS = 60000;
const unsigned int BLINK_INTERVAL_MS = 5000;

const char mqtt_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>MQTT config page</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script>
    function submitMessage() {
      alert("Saved config to memory");
      setTimeout(function(){ document.location.reload(false); }, 500);   
    }
  </script></head><body>
  <form action="/mqtt" target="hidden-form">
    inputString (current value %inputString%): <input type="text" name="inputString">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>
  <form action="/get" target="hidden-form">
    inputInt (current value %inputInt%): <input type="number " name="inputInt">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>
  <form action="/get" target="hidden-form">
    inputFloat (current value %inputFloat%): <input type="number " name="inputFloat">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form>
  <iframe style="display:none" name="hidden-form"></iframe>
</body></html>)rawliteral";