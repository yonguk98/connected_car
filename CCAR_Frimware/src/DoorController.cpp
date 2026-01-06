#include "DoorController.h"

// 생성자 구현
DoorController::DoorController(int pin) {
    this->servoPin = pin;
    this->isOpen = false; // 기본값은 잠김
}

void DoorController::begin() {
    // ESP32Servo 라이브러리를 이용해 핀 연결
    // ESP32는 타이머 할당이 필요하므로 setPeriodHertz 권장 (선택사항이나 안정성 위해)
    doorServo.setPeriodHertz(50); 
    doorServo.attach(servoPin, 500, 2400); 

    // 초기 상태: 잠금 (0도)
    lock();
}

void DoorController::unlock() {
    // 한 번에 90도로 가지 말고, 천천히 이동 (Soft Start)
    for (int pos = 0; pos <= 90; pos += 2) { 
        doorServo.write(pos);
        delay(10); // 속도 조절 (숫자가 클수록 느려짐)
    }
    this->isOpen = true;
}

void DoorController::lock() {
    // 닫을 때도 천천히
    for (int pos = 90; pos >= 0; pos -= 2) { 
        doorServo.write(pos);
        delay(10);
    }
    this->isOpen = false;
}

bool DoorController::getStatus() {
    return this->isOpen;
}