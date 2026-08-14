QT_BIN  ?= C:/msys64/ucrt64/bin
QMAKE   ?= $(QT_BIN)/qmake.exe
QT_MAKE ?= $(QT_BIN)/mingw32-make.exe

export PATH := $(QT_BIN);$(PATH)

.PHONY: all firmware qt flash run clean

all: firmware qt

firmware:
	$(MAKE) -C firmware

qt: qt-control/build/Makefile
	"$(QT_MAKE)" -C qt-control/build

qt-control/build/Makefile: qt-control/qt_stm32_control.pro \
                           qt-control/main.cpp qt-control/mainwindow.cpp \
                           qt-control/mainwindow.h qt-control/serialcontroller.cpp \
                           qt-control/serialcontroller.h
	@powershell.exe -NoProfile -Command "New-Item -ItemType Directory -Force -Path 'qt-control/build' | Out-Null"
	cd qt-control/build && "$(QMAKE)" ../qt_stm32_control.pro

flash:
	$(MAKE) -C firmware flash

run: all
	$(MAKE) flash
	powershell.exe -NoProfile -Command "Start-Process -FilePath '$(CURDIR)/qt-control/build/release/qt_stm32_control.exe'"

clean:
	$(MAKE) -C firmware clean
	$(RM) -r qt-control/build
