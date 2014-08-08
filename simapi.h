/*
 *  Copyright 2014 Aaron Seigo <aseigo@kde.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QObject>
#pragma once

class SimApi : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString packagePath READ packagePath WRITE setPackagePath NOTIFY packagePathChanged)
    Q_PROPERTY(bool hasAccelerometer READ hasAccelerometer NOTIFY hasAccelerometerChanged)
    Q_PROPERTY(bool hasGPS READ hasGPS NOTIFY hasGPSChanged)
    Q_PROPERTY(bool hasGyro READ hasGyro NOTIFY hasGyroChanged)
    Q_PROPERTY(bool hasMagnetometer READ hasMagnetometer NOTIFY hasMagnetometerChanged)
    Q_PROPERTY(bool hasVolumeUpKey READ hasVolumeUpKey NOTIFY hasVolumeUpKeyChanged)
    Q_PROPERTY(bool hasVolumeDownKey  READ hasVolumeDownKey NOTIFY hasVolumeDownKeyChanged)
    Q_PROPERTY(bool hasVolumeMuteKey  READ hasVolumeMuteKey NOTIFY hasVolumeMuteKeyChanged)
    Q_PROPERTY(bool hasPowerKey READ hasPowerKey NOTIFY hasPowerKeyChanged)
    Q_PROPERTY(bool hasHomeKey READ hasHomeKey NOTIFY hasHomeKeyChanged)
    Q_PROPERTY(bool hasBackKey READ hasBackKey NOTIFY hasBackKeyChanged)
    Q_PROPERTY(bool hasMenuKey READ hasMenuKey NOTIFY hasMenuKeyChanged)

public:
    enum Sensor {
        Accelerometer = 1,
        GPS = 2,
        Gyro = 4,
        Magnetometer = 8
    };
    Q_ENUMS(Sensor);
    Q_DECLARE_FLAGS(Sensors, Sensor);

    enum HardwareKey {
        VolumeUpKey = 1,
        VolumeDownKey = 2,
        VolumeMuteKey = 4,
        PowerKey = 8,
        HomeKey = 16,
        BackKey = 32,
        MenuKey = 64
    };
    Q_ENUMS(HardwareKey);
    Q_DECLARE_FLAGS(HardwareKeys, HardwareKey);

    SimApi(const QString &currentPath, QObject *parent);

    QString packagePath() const;
    void setPackagePath(const QString &path);

    void setSensors(const Sensors &sensors);
    bool hasAccelerometer() const;
    bool hasGPS() const;
    bool hasGyro() const;
    bool hasMagnetometer() const;

    void setHardwareKeys(const HardwareKeys &keys);
    bool hasVolumeUpKey() const;
    bool hasVolumeDownKey() const;
    bool hasVolumeMuteKey() const;
    bool hasPowerKey() const;
    bool hasHomeKey() const;
    bool hasBackKey() const;
    bool hasMenuKey() const;

Q_SIGNALS:
    void packagePathChanged(const QString &path);
    bool hasAccelerometerChanged() const;
    bool hasGPSChanged() const;
    bool hasGyroChanged() const;
    bool hasMagnetometerChanged() const;
    bool hasVolumeUpKeyChanged() const;
    bool hasVolumeDownKeyChanged() const;
    bool hasVolumeMuteKeyChanged() const;
    bool hasPowerKeyChanged() const;
    bool hasHomeKeyChanged() const;
    bool hasBackKeyChanged() const;
    bool hasMenuKeyChanged() const;

private:
    QString m_packagePath;
    Sensors m_sensors;
    HardwareKeys m_hardwareKeys;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SimApi::HardwareKeys);

