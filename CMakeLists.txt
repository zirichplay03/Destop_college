cmake_minimum_required(VERSION 3.10)

project(QtApp)

set(CMAKE_CXX_STANDARD 17)
# Включаем автоматическую обработку moc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Путь к MySQL библиотекам и заголовкам
set(MYSQL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/include")
set(MYSQL_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/share")
set(MYSQL_DLL "${CMAKE_SOURCE_DIR}/lib/share/libmysql.dll")
set(MYSQL_SO "${CMAKE_SOURCE_DIR}/lib/share/libmysqlclient.so")  # Для Linux

# Указываем пути к заголовкам MySQL
include_directories(${MYSQL_INCLUDE_DIR})

# Находим компоненты Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(PROJECT_SOURCES
        src/LoginWindow.cpp
        src/Student.cpp
        src/Teacher.cpp
        src/main.cpp
        src/picture.qrc
        src/testing/teacher_check.cpp
        src/testing/test_student.cpp
)

# Добавляем исполняемый файл
add_executable(QtApp ${PROJECT_SOURCES})

# Линкуем библиотеки Qt и MySQL (все в одной форме)
if(WIN32)
    # Для Windows используем .lib или .dll файлы
    set(MYSQL_LIB "${CMAKE_SOURCE_DIR}/lib/share/libmysql.lib")
    target_link_libraries(QtApp PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets ${MYSQL_LIB})

    # Копируем DLL в папку сборки для Windows
    add_custom_command(TARGET QtApp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MYSQL_DLL}" $<TARGET_FILE_DIR:QtApp>
    )

elseif(UNIX)
    # Для Linux используем .so файлы
    set(MYSQL_LIB "${CMAKE_SOURCE_DIR}/lib/share/libmysqlclient.so")
    target_link_libraries(QtApp PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets ${MYSQL_LIB})

    # Копируем .so в папку сборки для Linux
    add_custom_command(TARGET QtApp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MYSQL_SO}" $<TARGET_FILE_DIR:QtApp>
    )
endif()
