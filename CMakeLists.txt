project(plasma-phone-sim)

# minimal requirements
cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
set (QT_MIN_VERSION "5.2.0")
set(KF5_MIN_VERSION "5.0.0")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


find_package(ECM 0.0.9 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(ECMOptionalAddSubdirectory)
include(ECMInstallIcons)
include(ECMSetupVersion)
include(ECMMarkNonGuiExecutable)
include(ECMGenerateHeaders)
include(GenerateExportHeader)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED Core Gui Widgets Qml Quick)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS Plasma Service)
find_package(KF5 REQUIRED COMPONENTS PlasmaQuick)


feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

include(CheckIncludeFiles)

set(plasma-phone-sim_SRCS
    main.cpp
)

#qt5_wrap_ui(plasma-phone-sim_SRCS .ui)

add_executable(plasma-phone-sim ${plasma-phone-sim_SRCS})
target_link_libraries(plasma-phone-sim
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  Qt5::Quick
  KF5::I18n
  KF5::PlasmaQuick
)

install(TARGETS plasma-phone-sim ${INSTALL_TARGETS_DEFAULT_ARGS})

