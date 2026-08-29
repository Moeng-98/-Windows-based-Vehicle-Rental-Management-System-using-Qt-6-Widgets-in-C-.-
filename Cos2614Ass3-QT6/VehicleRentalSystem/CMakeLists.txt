cmake_minimum_required(VERSION 3.19)
project(VehicleRentalSystem LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(VehicleRentalSystem
    main.cpp
    mainwindow.cpp

    manager.cpp
    vehicle.cpp
    car.cpp
    motorcycle.cpp

    mainwindow.h
    manager.h
    vehicle.h
    car.h
    motorcycle.h

    utils.h
    README.md
)

target_link_libraries(VehicleRentalSystem
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS VehicleRentalSystem
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)