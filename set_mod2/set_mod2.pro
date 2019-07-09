#QT       -= core gui
#CONFIG -= qt
#QMAKE_EXTENSION_SHLIB = pyd # set extension

## dll setting
#TEMPLATE = lib
#CONFIG       += plugin

#INCLUDEPATH  += C:/Users/nitta/AppData/Local/Programs/Python/Python35/include
#LIBS += C:/Users/nitta/AppData/Local/Programs/Python/Python35/libs/python35.lib

## DESTDIR = $$_PRO_FILE_PWD_
#DESTDIR = $$clean_path($$_PRO_FILE_PWD_/../out)

## TARGET = conv_type # must same PYBIND11_MODULE name

#SOURCES += \
#			main.cpp
#win32{
#	win32-msvc{
#		QMAKE_CXXFLAGS += /utf-8
#	}
#}

## message($$TARGET)
#DEFINES += QT_TARGET=$$TARGET

include(../pybind_setting.pri)

SOURCES += main.cpp
