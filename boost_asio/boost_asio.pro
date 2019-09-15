TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += C:/local/boost_1_70_0

LIBS += -lws2_32
LIBS += -lmswsock

SOURCES += \
    ../main.cpp

HEADERS += \
    ../network/tcp_server.hpp \
    ../network/tcp_session.hpp \
    ../session/echo_session.hpp \
    ../session/chat_session.hpp

