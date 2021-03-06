#include(../pybind_setting.pri)

QT -= core gui
CONFIG -= qt
QMAKE_EXTENSION_SHLIB = pyd # 拡張子をdllからpydに変更

# dll setting
TEMPLATE = lib
CONFIG += plugin

# pybind11のヘッダファイルとライブラリのパスを指定


EIGENDIR = C:/eigen-eigen-323c052e1731
#PYBINDDIR = C:/Users/nitta/AppData/Local/Programs/Python/Python35/include
PYBINDLIB = C:/Users/nitta/Anaconda3/libs/python37.lib

INCLUDEPATH  += $$EIGENDIR C:/Users/nitta/Anaconda3/Include C:/Users/nitta/AppData/Roaming/Python/Python37/Include
LIBS += $$PYBINDLIB

# 出力ファイルをoutフォルダに設定(これは任意)
DESTDIR = $$clean_path($$_PRO_FILE_PWD_/../out)

win32{
    win32-msvc{
        QMAKE_CXXFLAGS += /utf-8 # visual studioはUTF-8用の指定が必要
    }
}
DEFINES += QT_TARGET=$$TARGET # モジュール名をdefineしとくとcppファイルでもそのまま使える

SOURCES += main.cpp
