#ifndef DOOR_CONTROLLER_H
#define DOOR_CONTROLLER_H

#include <ESP32Servo.h>

class DoorController {
private:
    Servo doorServo;
    int servoPin;
    int ledPin;
    bool isOpen; // 현재 상태 저장 (True: 열림, False: 잠김)
    
    // 비차단 제어를 위한 상태 머신
    enum State {
        IDLE,       // 아무 동작도 하지 않음
        UNLOCKING,  // 잠금 해제 중
        LOCKING     // 잠금 중
    };
    State currentState;
    int currentPos;      // 현재 서보 위치
    int targetPos;       // 목표 서보 위치
    unsigned long lastUpdateTime; // 마지막 업데이트 시간
    const int UPDATE_INTERVAL = 10; // 업데이트 간격 (ms)
    const int STEP_SIZE = 2;        // 한 번에 이동할 각도

public:
    // 생성자: 핀 번호를 받아서 초기화
    DoorController(int servoPin, int ledPin);

    // 하드웨어 초기화 (setup에서 호출)
    void begin();

    // 문 열기 (90도) - 비차단
    void unlock();

    // 문 잠그기 (0도) - 비차단
    void lock();

    // 현재 상태 반환 (Getter)
    bool getStatus();
    
    // 상태 머신 업데이트 (loop에서 주기적으로 호출)
    void update();
    
    // 서보가 현재 움직이고 있는지 확인
    bool isMoving();
};

#endif