# QR Code Scanner Server

### Build Docker Image

```
$ docker build -t python-qrcode .
```

### Run the container
Replace the IP with your local machine IP Address

```
$ docker run -p 192.168.0.107:8080:8080 -i -t python-qrcode
```

#Prepared Arduino IDE with the following libraries
* [esp32cam.h](https://github.com/yoursunny/esp32cam)
* [ArduinoWebsockets.h](https://github.com/gilmaimon/ArduinoWebsockets)

Update the SSID, Password, Server Address and Port and deploy the code
