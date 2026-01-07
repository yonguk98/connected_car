#include <Arduino.h>
#include "DoorController.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// 서보 모터를 연결할 핀 번호 (GPIO 13 추천)
#define SERVO_PIN 13
#define LED_PIN 2  // 상태 표시용 LED 핀 번호 (GPIO 2 추천)

// DoorController 객체 생성 (인스턴스화)
DoorController myCarDoor(SERVO_PIN, LED_PIN);

void setup() {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // 브라운아웃 감지 비활성화

    Serial.begin(115200);
    
    Serial.println("\n\n!!! SYSTEM REBOOTED !!!\n\n");

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