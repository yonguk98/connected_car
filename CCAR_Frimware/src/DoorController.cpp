#include "DoorController.h"

// 생성자 구현
DoorController::DoorController(int servoPin, int ledPin) {
    this->servoPin = servoPin;
    this->ledPin = ledPin;
    this->isOpen = false; // 기본값은 잠김
}

void DoorController::begin() {
    // ESP32Servo 라이브러리를 이용해 핀 연결
    // ESP32는 타이머 할당이 필요하므로 setPeriodHertz 권장 (선택사항이나 안정성 위해)
    doorServo.setPeriodHertz(50); 
    doorServo.attach(servoPin, SERVO_MIN_PULSE_WIDTH, SERVO_MAX_PULSE_WIDTH); 

    // LED 핀 설정 추가
    pinMode(ledPin, OUTPUT);
    
    // 초기 상태: 잠금 (0도)
    lock();
}

void DoorController::unlock() {
    // [핵심 수정] 이미 열려있으면(true) 아무것도 하지 않고 함수 종료!
    if (this->isOpen) {
        return; 
    }
    // 한 번에 90도로 가지 말고, 천천히 이동 (Soft Start)
    for (int pos = 0; pos <= 90; pos += 2) { 
        doorServo.write(pos);
        delay(10); // 속도 조절 (숫자가 클수록 느려짐)
    }
    this->isOpen = true;
    digitalWrite(ledPin, HIGH); // 문 열림 -> LED 켜기
}

void DoorController::lock() {
    // [핵심 수정] 이미 잠겨있으면(false) 아무것도 하지 않고 함수 종료!
    if (!this->isOpen) {
        return;
    }
    // 닫을 때도 천천히
    for (int pos = 90; pos >= 0; pos -= 2) { 
        doorServo.write(pos);
        delay(10);
    }
    this->isOpen = false;
    digitalWrite(ledPin, LOW); // 문 잠김 -> LED 끄기
}

bool DoorController::getStatus() {
    return this->isOpen;
}