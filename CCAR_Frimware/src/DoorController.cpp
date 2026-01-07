#include "DoorController.h"

// 생성자 구현
DoorController::DoorController(int servoPin, int ledPin) {
    this->servoPin = servoPin;
    this->ledPin = ledPin;
    this->isOpen = false; // 기본값은 잠김
    this->currentState = IDLE;
    this->currentPos = 0;
    this->targetPos = 0;
    this->lastUpdateTime = 0;
}

void DoorController::begin() {
    // ESP32Servo 라이브러리를 이용해 핀 연결
    // ESP32는 타이머 할당이 필요하므로 setPeriodHertz 권장 (선택사항이나 안정성 위해)
    doorServo.setPeriodHertz(50); 
    doorServo.attach(servoPin, SERVO_MIN_PULSE_WIDTH, SERVO_MAX_PULSE_WIDTH); 

    // LED 핀 설정 추가
    pinMode(ledPin, OUTPUT);
    
    // 초기 상태: 잠금 (0도)
    currentPos = 0;
    targetPos = 0;
    doorServo.write(currentPos);
    digitalWrite(ledPin, LOW);
    isOpen = false;
    currentState = IDLE;
}

void DoorController::unlock() {
    // [핵심 수정] 이미 열려있으면(true) 아무것도 하지 않고 함수 종료!
    if (this->isOpen) {
        return; 
    }
    // 비차단 방식으로 상태 설정
    currentState = UNLOCKING;
    targetPos = 90;
    lastUpdateTime = millis();
}

void DoorController::lock() {
    // [핵심 수정] 이미 잠겨있으면(false) 아무것도 하지 않고 함수 종료!
    if (!this->isOpen) {
        return;
    }
    // 비차단 방식으로 상태 설정
    currentState = LOCKING;
    targetPos = 0;
    lastUpdateTime = millis();
}

bool DoorController::getStatus() {
    return this->isOpen;
}

void DoorController::update() {
    // IDLE 상태에서는 아무것도 하지 않음
    if (currentState == IDLE) {
        return;
    }
    
    // 시간 간격 체크
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime < UPDATE_INTERVAL) {
        return;
    }
    
    lastUpdateTime = currentTime;
    
    // 현재 상태에 따라 서보 위치 업데이트
    if (currentState == UNLOCKING) {
        if (currentPos < targetPos) {
            currentPos += STEP_SIZE;
            if (currentPos > targetPos) {
                currentPos = targetPos;
            }
            doorServo.write(currentPos);
        }
        
        // 목표 위치에 도달하면 상태 전환
        if (currentPos >= targetPos) {
            isOpen = true;
            digitalWrite(ledPin, HIGH); // 문 열림 -> LED 켜기
            currentState = IDLE;
        }
    }
    else if (currentState == LOCKING) {
        if (currentPos > targetPos) {
            currentPos -= STEP_SIZE;
            if (currentPos < targetPos) {
                currentPos = targetPos;
            }
            doorServo.write(currentPos);
        }
        
        // 목표 위치에 도달하면 상태 전환
        if (currentPos <= targetPos) {
            isOpen = false;
            digitalWrite(ledPin, LOW); // 문 잠김 -> LED 끄기
            currentState = IDLE;
        }
    }
}

bool DoorController::isMoving() {
    return currentState != IDLE;
}