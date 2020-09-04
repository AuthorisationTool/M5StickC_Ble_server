#include <M5StickC.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_BID_UUID "704fcaaa-a20d-4d29-b984-e099a6828549"
#define CHARECTERISTIC_BID_UUID "c3fa560c-1f67-4ae3-97ec-ed73831270d8"

#define SERVICE_ACC_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARECTERISTIC_ACCX_UUID "002c6092-3f0d-4370-a45d-e3f5bccd6938"
#define CHARECTERISTIC_ACCY_UUID "dfae7e37-f5ac-4380-b456-b15471004470"
#define CHARECTERISTIC_ACCZ_UUID "c257b861-20ac-47fd-9a0b-7182164f8a8b"

#define SERVICE_GYRO_UUID         "6038dc34-80c6-4f03-8f95-735c87714fce"
#define CHARECTERISTIC_GYROX_UUID "8bec6f2b-6cf5-48e2-b9c4-5146049ae800"
#define CHARECTERISTIC_GYROY_UUID "14834712-21e8-4d1d-95e5-f3988aa71844"
#define CHARECTERISTIC_GYROZ_UUID "3faecf53-f369-4171-ba2c-0d3368f262e3"

#define SERVICE_LOC_UUID          "1a2e8798-0300-4c63-b7b0-9c2b62fa36d6"
#define CHARECTERISTIC_LATITUDE   "b9221126-97c6-440f-b790-167c9d2ebb1d"
#define CHARECTERISTIC_LONGITUDE  "b6ad73d3-b9bf-4316-8b4a-15fe9966f650"

#define SERVICE_ATT_UUID          "72272a3c-52b3-4650-8c09-e92c4e96fa69"
#define CHARECTERISTIC_PITCH_UUID "b63cec8c-35ee-40c2-8efc-237b7ac5b515"
#define CHARECTERISTIC_ROLL_UUID "cc535fe7-2692-4003-8642-1a58fa1a07f9"
#define CHARECTERISTIC_YAW_UUID "3752f9b5-ff47-4737-ac92-5a56db2a1aa3"


float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristicGyroX = NULL;
BLECharacteristic* pCharacteristicGyroY = NULL;
BLECharacteristic* pCharacteristicGyroZ = NULL;

BLECharacteristic* pCharacteristicAccX = NULL;
BLECharacteristic* pCharacteristicAccY = NULL;
BLECharacteristic* pCharacteristicAccZ = NULL;

BLECharacteristic* pCharacteristicPitch = NULL;
BLECharacteristic* pCharacteristicRoll = NULL;
BLECharacteristic* pCharacteristicYaw = NULL;


BLECharacteristic* pCharacteristicID = NULL;
bool deviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      M5.Lcd.setCursor(0,70);
      M5.Lcd.println("connect");
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      M5.Lcd.setCursor(0,70);
      M5.Lcd.println("disconnect");
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    M5.Lcd.setCursor(0,70);
    M5.Lcd.println("read");
  }
  
  void onWrite(BLECharacteristic *pCharacteristic) {
    M5.Lcd.println("write");
    std::string value = pCharacteristic->getValue();
    M5.Lcd.println(value.c_str());
  }
};

void setup() {
  
  Serial.begin(115200);
  M5.begin();
  M5.IMU.Init();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(40, 0);
  M5.Lcd.println("IMU TEST");
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.println("  X       Y       Z");
  M5.Lcd.setCursor(0, 50);
  M5.Lcd.println("  Pitch   Roll    Yaw");
 
  BLEDevice::init("m5-stack");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pServiceID = pServer->createService(SERVICE_BID_UUID);
  BLEService *pServiceGyro = pServer->createService(SERVICE_GYRO_UUID);
  BLEService *pServiceAcc = pServer->createService(SERVICE_ACC_UUID);
  BLEService *pServiceAtt = pServer->createService(SERVICE_ATT_UUID);
  
  pCharacteristicID = pServiceID->createCharacteristic(
                                         CHARECTERISTIC_BID_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                       );
  pCharacteristicID->setCallbacks(new MyCallbacks());
  pCharacteristicID->addDescriptor(new BLE2902());
  pCharacteristicID->setValue("Bluetooth_Badge_1");

  pCharacteristicGyroX = pServiceGyro->createCharacteristic(
                                         CHARECTERISTIC_GYROX_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                         );
  pCharacteristicGyroX->setCallbacks(new MyCallbacks());
  pCharacteristicGyroX->addDescriptor(new BLE2902());

   pCharacteristicGyroY = pServiceGyro->createCharacteristic(
                                         CHARECTERISTIC_GYROY_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                         );
  pCharacteristicGyroY->setCallbacks(new MyCallbacks());
  pCharacteristicGyroY->addDescriptor(new BLE2902());

   pCharacteristicGyroZ = pServiceGyro->createCharacteristic(
                                         CHARECTERISTIC_GYROZ_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                         );
  pCharacteristicGyroZ->setCallbacks(new MyCallbacks());
  pCharacteristicGyroZ->addDescriptor(new BLE2902());

  pCharacteristicAccX = pServiceAcc->createCharacteristic(
                                         CHARECTERISTIC_ACCX_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                                          );
  pCharacteristicAccX->setCallbacks(new MyCallbacks());
  pCharacteristicAccX->addDescriptor(new BLE2902());

  pCharacteristicAccY = pServiceAcc->createCharacteristic(
                                         CHARECTERISTIC_ACCY_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                                          );
  pCharacteristicAccY->setCallbacks(new MyCallbacks());
  pCharacteristicAccY->addDescriptor(new BLE2902());

  pCharacteristicAccZ = pServiceAcc->createCharacteristic(
                                         CHARECTERISTIC_ACCZ_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                                          );
  pCharacteristicAccZ->setCallbacks(new MyCallbacks());
  pCharacteristicAccZ->addDescriptor(new BLE2902());

  pCharacteristicPitch = pServiceAtt->createCharacteristic(
                                         CHARECTERISTIC_PITCH_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                                          );
  pCharacteristicPitch->setCallbacks(new MyCallbacks());
  pCharacteristicPitch->addDescriptor(new BLE2902());

  pCharacteristicRoll = pServiceAtt->createCharacteristic(
                                         CHARECTERISTIC_ROLL_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                                          );
  pCharacteristicRoll->setCallbacks(new MyCallbacks());
  pCharacteristicRoll->addDescriptor(new BLE2902());

    pCharacteristicYaw = pServiceAtt->createCharacteristic(
                                         CHARECTERISTIC_YAW_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                                          );
  pCharacteristicYaw->setCallbacks(new MyCallbacks());
  pCharacteristicYaw->addDescriptor(new BLE2902());

  pServiceAtt->start();
  pServiceID->start();
  pServiceAcc->start();
  pServiceGyro->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

                 
char* convertFloatToString(float var)
{ // begin function

  char temp[10];
  String varAsString;
    
    // perform conversion
    dtostrf(var,6,2,temp);
     
  return temp;
  
} // end function

void loop() {
   M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
  M5.IMU.getAccelData(&accX,&accY,&accZ);
  M5.IMU.getAhrsData(&pitch,&roll,&yaw);
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.printf("%6.2f  %6.2f  %6.2f      ", gyroX, gyroY, gyroZ);
  M5.Lcd.setCursor(140, 20);
  M5.Lcd.print("G");
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", accX, accY, accZ);
  M5.Lcd.setCursor(140, 30);
  M5.Lcd.print("o/s");
  M5.Lcd.setCursor(0, 60);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", pitch, roll, yaw);
  
  if (deviceConnected) {  
      M5.Lcd.setCursor(0, 70);
      M5.Lcd.printf("device connected successfuly");
      
     
      pCharacteristicGyroX->setValue(gyroX);
      pCharacteristicGyroY->setValue(gyroY);
      pCharacteristicGyroZ->setValue(gyroZ);
      
      pCharacteristicAccX->setValue(accX);
      pCharacteristicAccY->setValue(accY);
      pCharacteristicAccZ->setValue(accZ);

      pCharacteristicPitch->setValue(pitch);
      pCharacteristicRoll->setValue(roll);
      pCharacteristicYaw->setValue(yaw);
      
     
  }
  delay(200);
  M5.update();
}