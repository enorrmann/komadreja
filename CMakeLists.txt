cmake_minimum_required(VERSION 3.16)
project(Komadreja VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt modules
find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Qml Quick Multimedia)

qt_add_executable(appKomadreja
    src/main.cpp
    src/audio/AudioEngine.cpp
    src/models/PadModel.cpp
)

qt_add_resources(appKomadreja "resources"
    PREFIX "/"
    FILES "assets/samples/kick.wav"
)

qt_add_qml_module(appKomadreja
    URI Komadreja
    VERSION 1.0
    QML_FILES
        qml/main.qml
        qml/components/Pad.qml
        qml/screens/SampleScreen.qml
)

target_link_libraries(appKomadreja
    PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick Qt6::Multimedia
)

if(UNIX AND NOT APPLE AND NOT ANDROID)
    target_link_libraries(appKomadreja PRIVATE pthread dl m)
endif()

# For Android deployment, these properties are useful
set_target_properties(appKomadreja PROPERTIES
    QT_ANDROID_PACKAGE_NAME "com.emilio.komadreja"
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
