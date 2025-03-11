# Navilos RTOS 제작 공부 레포지토리

<img src="https://ebook.insightbook.co.kr/media/cover/74.jpg?v=2" alt="Sample Image" height="300">

사용한 교재: **임베디드 OS 개발 프로젝트 ARM 기반 펌웨어/RTOS의 원리와 구조**, 이만우 지음, 인사이트


## 짧은 디렉토리 구조 설명
- boot:
  - 부팅 시 가장 첫 진입점
  - Exception vector table과 main.c
- build:
  - Makefile로 빌드된 각 소스파일의 object files
  - 링커에 의해 생성된 map file
  - 에뮬레이터에 의해 실행될 커널 실행파일 (bin, axf)
- hal:
  - 타겟 상관없이 사용할 수 있는 HAL 공용 인터페이스
  - Interrupt(GIC), Timer, UART 관련 HAL API들이 구현
  - rvpb (Realview PB-a8)
    - 특정 타겟 (Realview PB-a8)에 맞게끔 HAL API의 상세한 구현부가 포함
- includes
  - 각 소스파일에서 공통적으로 include하는 표준 API들 중 일부가 구현
- kernel
  - 커널 동작 관련 핵심 코드들
  - 태스크, 스케줄러, 동기화 등이 구현
- lib
  - 특정 타겟 specific한 include 파일들
- result_pictures
  - QEMU 실행 결과 이미지들
  - 각 테스트에 대한 결과들

## To build and execute

> 개발환경: Windows 11 WSL2 (Ubuntu 24.04) + Vscode

### 1. Install build-essential packages
  ```bash
  sudo apt install -y build-essential crossbuild-essential-armel
  sudo apt install -y libncurses-dev
  # (I don't know are those necessary but)
  sudo apt install -y lib32z1 lib32ncurses-dev, 
  ```
### 2. Install ARM cross toolchain (두 가지 방법 중 하나를 선택하세요, 방법 2 추천)
**1. Download toolchain from ARM official website ( [link](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) )**
 ```bash
 # Download toolchain
 wget https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz

 # Extract file and make symbolic link for comfortable use
 sudo tar xzfJ arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz -C /usr/share/
 sudo ln -S /usr/share/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi/bin/* /usr/bin/
 ```
- 최신버전의 ARM 툴체인을 받은 뒤 압축해제 한 뒤 심볼릭 링크를 생성하는 명령어입니다.
- 'arm-none-eabi-gdb'가 포함돼있다는 장점이 있습니다만, gcc 컴파일 시 cc1 관련 오류가 발생합니다.
- 아마 툴체인에 포함돼있는 'arm-none-eabi-gcc'를 빌드할 때 cc1이 필요한 특정 옵션이 꺼진체로 빌드된 것 같습니다.

**2. Download and install package from apt**
- `sudo apt install gcc-arm-none-eabi`로 설치합니다.
- 'arm-none-eabi-gdb'가 포함돼있지 않습니다.
- `sudo apt install gdb-multiarch`로 gdb를 설치한 뒤
- `sudo ln -s /usr/bin/gdb-multiarch /usr/bin/arm-none-eabi-gdb`로 심볼릭 링크를 만들면 진행하기 편합니다.

### 3. Install QEMU
   - `sudo apt install qemu-system-arm`
   - realview-pb-a8 모델을 사용합니다.
   - 실행 예시) `qemu-system-arm -machine realview-pb-a8 -kernel (커널파일) -nographic`

### 4. Build project : `make all`


## 프로젝트 완료 후 습득한 지식
- 개발 환경 설정  (QEMU, Linux, Makefile, arm-none-eabi- cross toolchain)
- 컴파일러에 대한 이해  (AS, LD, GCC)
- 링커에 대한 이해  (.ld, linker script)
- 부트로더에 대한 이해  (Entry.S, Exception vector table, Reset handler)
- 레지스터의 사용에 대한 이해 
- 인터럽트 처리
- 메모리 관리
- 주변장치 제어
- 멀티코어 간 메시지 처리  (Event, Message)
- 멀티코어 동기화  (Semaphore, Mutex)
