# Qt + STM32F411 USART1 제어 프로젝트

Qt 데스크톱 앱에서 USART1을 통해 STM32F411의 외부 LED 2개와 DC 모터 PWM을 제어한다.

## 디렉터리

- `firmware/`: STM32F411 레지스터 기반 펌웨어와 ARM GCC Makefile
- `qt-control/`: Qt Widgets/SerialPort 기반 제어 앱
- `Makefile`: 펌웨어와 Qt 앱 통합 빌드 진입점

## 핀 연결

| 기능 | STM32F411 핀 | 외부 연결 |
|---|---|---|
| USART1 TX | PA9 | USB-UART RX |
| USART1 RX | PA10 | USB-UART TX |
| LED 1 | PB0 | PB0 → 전류 제한 저항 → LED → GND |
| LED 2 | PB1 | PB1 → 전류 제한 저항 → LED → GND |
| Motor IN1 | PA0 / TIM5_CH1 | 모터 드라이버 IN1 |
| Motor IN2 | PA1 / TIM5_CH2 | 모터 드라이버 IN2 |

USART1은 `115200, 8 data bits, no parity, 1 stop bit, no flow control`로 동작한다. USB-UART와 STM32는 GND를 공통으로 연결하고 3.3 V TTL 레벨을 사용한다. Nucleo 보드의 기본 ST-LINK 가상 COM 포트는 보통 USART2에 연결되므로 USART1에는 별도 USB-UART 연결이 필요하다.

LED는 active-high이다. 저항 없이 LED를 연결하지 않는다. 모터는 GPIO에 직접 연결하지 말고 반드시 H-bridge 모터 드라이버와 별도 모터 전원을 사용하며, 모터 전원 GND도 STM32 GND와 공통으로 연결한다.

기존 실습 배선을 유지해 정회전(CW)은 PA1에 PWM을 출력하고, 역회전(CCW)은 PA0에 PWM을 출력한다. PWM 주파수는 20 kHz이고 듀티 범위는 0~100%이다. 방향 전환 시 두 출력을 먼저 0으로 갱신한 뒤 반대 채널을 구동한다.

## 빌드와 다운로드

저장소 루트에서 다음 명령으로 펌웨어와 Qt 앱을 함께 빌드한다.

```powershell
make
```

기본 도구 경로는 현재 실습 PC 환경에 맞춰져 있다.

- ARM GCC: `C:/arm-gnu-toolchain-15.2.rel1-mingw-w64-i686-arm-none-eabi`
- Qt/MSYS2: `C:/msys64/ucrt64/bin`
- STM32CubeProgrammer: `C:/Program Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin`

경로가 다르면 Make 변수로 덮어쓸 수 있다.

```powershell
make firmware TOOL_DIR=C:/toolchains/arm-gnu
make qt QT_BIN=C:/msys64/ucrt64/bin
```

생성 파일은 다음 위치에 있다.

- 펌웨어: `firmware/build/qt_stm32_control.bin`
- Qt 앱: `qt-control/build/release/qt_stm32_control.exe`

ST-LINK가 연결된 상태에서 펌웨어를 바로 기록하려면 다음 명령을 사용한다.

```powershell
make flash
```

Qt Creator에서는 `qt-control/qt_stm32_control.pro`를 열고 Qt SerialPort 모듈이 포함된 Qt 6 Kit로 빌드한다.

## USART 명령 프로토콜

모든 명령은 ASCII 한 줄이며 끝에 `\n`을 붙인다. 펌웨어 응답은 `\r\n`으로 끝난다. 영문 명령은 대소문자를 구분하지 않는다.

| 명령 | 동작 | 정상 응답 예시 |
|---|---|---|
| `PING` | 연결 확인 | `OK PONG` |
| `LED 1 ON` | LED 1 켜기 | `OK LED 1 ON` |
| `LED 1 OFF` | LED 1 끄기 | `OK LED 1 OFF` |
| `LED 2 ON` | LED 2 켜기 | `OK LED 2 ON` |
| `LED 2 OFF` | LED 2 끄기 | `OK LED 2 OFF` |
| `MOTOR CW 75` | 정회전, 듀티 75% | `OK MOTOR CW 75` |
| `MOTOR CCW 40` | 역회전, 듀티 40% | `OK MOTOR CCW 40` |
| `MOTOR STOP` | 모터 정지 | `OK MOTOR STOP` |

Qt 앱에서 COM 포트를 선택해 연결하면 자동으로 `PING`을 보내며, LED 버튼과 모터 듀티/방향 버튼이 위 명령을 전송한다.

