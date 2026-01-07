#ifndef DOOR_CONTROLLER_H
#define DOOR_CONTROLLER_H

#include <ESP32Servo.h>

class DoorController {
private:
    Servo doorServo;
    int servoPin;
    int ledPin;
    bool isOpen; // 현재 상태 저장 (True: 열림, False: 잠김)
    
    // Servo pulse width constants (in microseconds)
    static constexpr int SERVO_MIN_PULSE_WIDTH = 500;   // Minimum pulse width for 0 degrees
    static constexpr int SERVO_MAX_PULSE_WIDTH = 2400;  // Maximum pulse width for 180 degrees

public:
    // 생성자: 핀 번호를 받아서 초기화
    DoorController(int servoPin, int ledPin);

    // 하드웨어 초기화 (setup에서 호출)
    void begin();

    // 문 열기 (90도)
    void unlock();

    // 문 잠그기 (0도)
    void lock();

    // 현재 상태 반환 (Getter)
    bool getStatus();
};

#endif