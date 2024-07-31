# SPDX-FileCopyrightText: Year Author <email@company.com>
#
# SPDX-License-Identifier: BSD-2-Clause

kcmutils_add_qml_kcm(kcm_imaupdate)

target_sources(kcm_imaupdate PRIVATE
    #main.cpp
    imaupdatesettings.cpp
    imaupdatesettings.h
    # Add the QML resource file here
    qml.qrc
)

target_link_libraries(kcm_imaupdate PRIVATE
    Qt::Quick
    KF6::CoreAddons
    KF6::KCMUtils
    KF6::I18n
)

# Ensure QML files are found
qt_add_resources(kcm_imaupdate "qml.qrc" PREFIX "/kcm/kcm_imaupdate")
