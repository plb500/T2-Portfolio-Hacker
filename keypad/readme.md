# Raspberry Pi powered Cyberdyne keycard/PIN Pad replica
<p align="center">
  <img src="/docs/graphics/images/cyberdyne-keycard-reader.png" width="75%">
</p>

## Materials required
- Raspberry Pi (a 3B will work) with SD card and Raspberry Pi Lite installed (I used the legacy version)
- Waveshare 4.3" DSI LCD Display ([link](https://www.amazon.ca/Raspberry-Touchscreen-Capacitive-Raspbian-Retropie/dp/B09J3Y1RRT/ref=sr_1_1?crid=3M9HNCP7ZC69F&keywords=waveshare+4.3+dsi&qid=1693980339&sprefix=waveshare+4+3+dsi%2Caps%2C143&sr=8-1&ufe=app_do%3Aamzn1.fos.b06bdbbe-20fd-4ebc-88cf-fa04f1ca0da8))
- 16x1 I2C LCD Display ([link](https://www.buydisplay.com/lcd-display-16x1-datasheet-in-pdf-hd44780-controller-i2c-arduino))
- Solenoid lock ([link](https://www.amazon.ca/Atoplee-Drawer-Electric-Assembly-Solenoid/dp/B0125VGLT0/ref=sr_1_1_sspa?crid=T2QAWZQG1RXB&keywords=solenoid+lock&qid=1693980512&sprefix=solenoid+lock%2Caps%2C159&sr=8-1-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1))
- 3V Relay module ([link](https://www.amazon.ca/dp/B07XGZSYJV?psc=1&ref=ppx_yo2ov_dt_b_product_details))
- Pololu 5V, 5A step-down regulator ([link](https://www.pololu.com/product/2851))
- 1 x 3mm Red LED, 1 x 3mm Green LED
- Adafruit perma-proto Pi hat ([link](https://www.adafruit.com/product/2310))
- 2 x Spring loaded pin ([link](https://www.digikey.ca/en/products/detail/mill-max-manufacturing-corp/7949-0-15-20-09-14-11-0/13531603))
- Wires, connectors etc
- A case to put everything in (I milled mine out of MDF and aluminium sheet)
  <img src="/docs/graphics/images/keypad-primed.jpeg" width="50%">

## Pi setup
### Basic board/OS setup
- Ensure everything is updated and upgraded post-install
  ```
  sudo apt update
  sudo apt upgrade
  ```
- Install pre-requisites
  ```
  sudo apt install git python3-pip python3-venv python3-pyqt5 python3-pyqt5.qtserialport xserver-xorg xinit
  ```
- Rotate LCD
  Add the following lines to the end of `/boot/config.txt`
  ```
  # For 0 degree rotation:
  # (no additional entries - default setting)
  
  # For 90 degree rotation (top to right):
  display_lcd_rotate=1
  dtoverlay=rpi-ft5406,touchscreen-swapped-x-y=1,touchscreen-inverted-x=1
  
  # For 180 degree rotation (upside down)
  #display_lcd_rotate=2
  #dtoverlay=rpi-ft5406,touchscreen-inverted-x=1,touchscreen-inverted-y=1
  
  # For 270 degree rotation (top to left)
  #display_lcd_rotate=3
  #dtoverlay=rpi-ft5406,touchscreen-swapped-x-y=1,touchscreen-inverted-y=1
  ```
  For the build, strictly speaking, only the 90 degree rotation is required, but I like having these here for reference
  
### Bluetooth setup (if using bluetooth -> Portfolio/HC-05 PIN transmission)
(NB: This method works, but sucks and also feels like cheating. It seems to make the Pi really slow and unresponsive over Wifi. Much preferable to use the serial ine transmission method below)
- Bluetooth pairing (if using the HC-05 interface Arduino project)
  - Install bluetooth pre-requisites on Pi:
    ```
    sudo apt install pi-bluetooth bluetooth bluez
    ```
  - Ensure HC-05 is powered. When ready to pair it should be blinking red rapidly
  - Edit `/etc/systemd/system/dbus-org.bluez.service` and add/change the following lines (for the first line just add the `-C` parameter to the existing line)
    ```
    ExecStart=/usr/lib/bluetooth/bluetoothd -C
    ExecStartPost=/usr/bin/sdptool add SP
    ```
  - Reload systemd
    ```
    sudo systemctl daemon-reload
    ```
  - Enable bluetooth service
    ```
    sudo systemctl enable --now bluetooth
    ```
  - Edit `/etc/modules-load.d/modules.conf` and add the following line to load rfcomm module automatically
    ```
    rfcomm
    ```
  - Reboot Pi
  - Once rebooted, open bluetooth control console
    ```
    sudo bluetoothctl 
    ```
  - Pair with HC-05 module
    ```
    [bluetooth]# agent on
    [bluetooth]# scan on
      Discovery started
      [NEW] Device 98:D3:31:50:4A:C1 98-D3-31-50-4A-C1
      [CHG] Device 98:D3:31:50:4A:C1 LegacyPairing: no
      [CHG] Device 98:D3:31:50:4A:C1 Name: ARDUINOBT
      [CHG] Device 98:D3:31:50:4A:C1 Alias: ARDUINOBT
    
    [bluetooth]# scan off
      [CHG] Controller B8:27:EB:80:2D:06 Discovering: no
      Discovery stopped
    
    [bluetooth]# pair 98:D3:31:50:4A:C1
      Attempting to pair with 98:D3:31:50:4A:C1
      [CHG] Device 98:D3:31:50:4A:C1 Connected: yes
      Request PIN code
      [agent] Enter PIN code: 1379
      [CHG] Device 98:D3:31:50:4A:C1 UUIDs: 00001101-0000-1000-8000-00805f9b34fb
      [CHG] Device 98:D3:31:50:4A:C1 ServicesResolved: yes
      [CHG] Device 98:D3:31:50:4A:C1 Paired: yes
      Pairing successful
      [CHG] Device 98:D3:31:50:4A:C1 ServicesResolved: no
      [CHG] Device 98:D3:31:50:4A:C1 Connected: no
    
    [bluetooth]# trust 98:D3:31:50:4A:C1
      [CHG] Device 98:D3:31:50:4A:C1 Trusted: yes
      Changing 98:D3:31:50:4A:C1 trust succeeded
    
    [bluetooth]# exit
    ```
  - Create a serial device bound to the HC-05 module
    ```
    sudo rfcomm bind rfcomm0 <device's MAC>
    ```
  - To make the Bluetooth adapter automatically bound on boot, also add the above rfcomm bind command to the end of `/etc/rc.local`
  - Reboot Pi
 
### Serial setup (if using GPIO serial -> Portfolio PIN transmission)
- Run raspi-config
  ```
  sudo raspi-config
  ```
- Select `Interface Options` and then `Serial Port`
- Pick "No" when asked if you want a login shell over the serial port
- Pick "Yes" when asked if you want the serial interface enabled
- Reboot Pi
