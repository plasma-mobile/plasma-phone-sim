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
    Q_PROPERTY(float accelX READ accelX NOTIFY accelXChanged);
    Q_PROPERTY(float accelY READ accelY NOTIFY accelYChanged);
    Q_PROPERTY(float accelZ READ accelZ NOTIFY accelZChanged);
    Q_PROPERTY(bool hasGravitySensor READ hasGravitySensor NOTIFY hasGravitySensorChanged)
    Q_PROPERTY(float gravityX READ gravityX NOTIFY gravityXChanged);
    Q_PROPERTY(float gravityY READ gravityY NOTIFY gravityYChanged);
    Q_PROPERTY(float gravityZ READ gravityZ NOTIFY gravityZChanged);
    Q_PROPERTY(bool hasGPS READ hasGPS NOTIFY hasGPSChanged)
    Q_PROPERTY(bool hasGyro READ hasGyro NOTIFY hasGyroChanged)
    Q_PROPERTY(float gravityX READ gravityX NOTIFY gravityXChanged);
    Q_PROPERTY(float gravityY READ gravityY NOTIFY gravityYChanged);
    Q_PROPERTY(float gravityZ READ gravityZ NOTIFY gravityZChanged);
    Q_PROPERTY(bool hasMagnetometer READ hasMagnetometer NOTIFY hasMagnetometerChanged)
    Q_PROPERTY(float magneticX READ magneticX NOTIFY magneticXChanged);
    Q_PROPERTY(float magneticY READ magneticY NOTIFY magneticYChanged);
    Q_PROPERTY(float magneticZ READ magneticZ NOTIFY magneticZChanged);
    Q_PROPERTY(float magneticBiasX READ magneticBiasX NOTIFY magneticBiasXChanged);
    Q_PROPERTY(float magneticBiasY READ magneticBiasY NOTIFY magneticBiasYChanged);
    Q_PROPERTY(float magneticBiasZ READ magneticBiasZ NOTIFY magneticBiasZChanged);
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
        Magnetometer = 8,
        GravitySensor = 16,
        Light = 32,
        Pressure = 64,
        Proximity = 128,
        Orientation = 256,
        Temperature = 512,
        Humidity = 1024
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
    float accelX() const;
    float accelY() const;
    float accelZ() const;
    void setAccel(float x, float y, float z);

    bool hasGravitySensor() const;
    float gravityX() const;
    float gravityY() const;
    float gravityZ() const;
    void setGravity(float x, float y, float z);


    bool hasGPS() const;

    bool hasGyro() const;
    float gyroX() const;
    float gyroY() const;
    float gyroZ() const;
    void setGyro(float x, float y, float z);

    bool hasMagnetometer() const;
    float magneticX() const;
    float magneticY() const;
    float magneticZ() const;
    float magneticBiasX() const;
    float magneticBiasY() const;
    float magneticBiasZ() const;
    void setMagentic(float x, float xBias,
                     float y, float yBias,
                     float z, float zBias);

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
    void hasAccelerometerChanged() const;
    void accelXChanged() const;
    void accelYChanged() const;
    void accelZChanged() const;
    void hasGravitySensorChanged() const;
    void gravityXChanged() const;
    void gravityYChanged() const;
    void gravityZChanged() const;
    void hasGPSChanged() const;
    void hasGyroChanged() const;
    void gyroXChanged() const;
    void gyroYChanged() const;
    void gyroZChanged() const;
    void hasMagnetometerChanged() const;
    void magneticXChanged() const;
    void magneticYChanged() const;
    void magneticZChanged() const;
    void magneticBiasXChanged() const;
    void magneticBiasYChanged() const;
    void magneticBiasZChanged() const;
    void hasVolumeUpKeyChanged() const;
    void volumeUpKeyPressed() const;
    void volumeUpKeyReleased() const;
    void hasVolumeDownKeyChanged() const;
    void volumeDownKeyPressed() const;
    void volumeDownKeyReleased() const;
    void hasVolumeMuteKeyChanged() const;
    void volumeMuteKeyPressed() const;
    void volumeMuteKeyReleased() const;
    void hasPowerKeyChanged() const;
    void powerKeyPressed() const;
    void powerKeyReleased() const;
    void hasHomeKeyChanged() const;
    void homeKeyPressed() const;
    void homeKeyReleased() const;
    void hasBackKeyChanged() const;
    void backKeyPressed() const;
    void backKeyReleased() const;
    void hasMenuKeyChanged() const;
    void menuKeyPressed() const;
    void menuKeyReleased() const;

private Q_SLOTS:
    void logSignal();

private:
    QString m_packagePath;
    Sensors m_sensors;
    HardwareKeys m_hardwareKeys;
    float m_accelX;
    float m_accelY;
    float m_accelZ;
    float m_gravityX;
    float m_gravityY;
    float m_gravityZ;
    float m_gyroX;
    float m_gyroY;
    float m_gyroZ;
    float m_magneticX;
    float m_magneticY;
    float m_magneticZ;
    float m_magneticBiasX;
    float m_magneticBiasY;
    float m_magneticBiasZ;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SimApi::HardwareKeys);

