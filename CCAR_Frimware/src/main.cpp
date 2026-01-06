#include <Arduino.h>
#include "DoorController.h"

// 서보 모터를 연결할 핀 번호 (GPIO 13 추천)
#define SERVO_PIN 13

// DoorController 객체 생성 (인스턴스화)
DoorController myCarDoor(SERVO_PIN);

void setup() {
    Serial.begin(115200);
    
    Serial.println(">>> [System] Initializing Door Controller...");
    myCarDoor.begin();
    
    Serial.println(">>> [System] Ready. Enter '1' to UNLOCK, '0' to LOCK.");
}

void loop() {
    // 시리얼 모니터에서 입력이 들어오면 실행
    if (Serial.available()) {
        char cmd = Serial.read(); // 한 글자 읽기
        
        if (cmd == '1') {
            Serial.println("Command: UNLOCK -> Door Opening...");
            myCarDoor.unlock();
        } 
        else if (cmd == '0') {
            Serial.println("Command: LOCK -> Door Closing...");
            myCarDoor.lock();
        }
        
        // 현재 상태 출력 (피드백 확인)
        if (myCarDoor.getStatus()) {
            Serial.println("Current Status: [ OPEN ]");
        } else {
            Serial.println("Current Status: [ LOCKED ]");
        }
    }
}