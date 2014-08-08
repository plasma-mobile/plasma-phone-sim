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

#include "simapi.h"

SimApi::SimApi(const QString &currentPath, QObject *parent)
    : QObject(parent),
      m_packagePath(currentPath),
      m_accelX(0),
      m_accelY(0),
      m_accelZ(0),
      m_gravityX(0),
      m_gravityY(0),
      m_gravityZ(0)
{
}

QString SimApi::packagePath() const
{
    return m_packagePath;
}

void SimApi::setPackagePath(const QString &path)
{
    if (path != m_packagePath) {
        m_packagePath = path;
        emit packagePathChanged(m_packagePath);
    }
}

void SimApi::setSensors(const Sensors &sensors)
{
    if (m_sensors != sensors) {
        m_sensors = sensors;
        emit hasAccelerometerChanged();
        emit hasGPSChanged();
        emit hasGyroChanged();
        emit hasMagnetometerChanged();
    }
}

bool SimApi::hasAccelerometer() const
{
    return m_sensors & Accelerometer;
}

float SimApi::accelX() const
{
    return m_accelX;
}

float SimApi::accelY() const
{
    return m_accelY;
}

float SimApi::accelZ() const
{
    return m_accelZ;
}

void SimApi::setAccel(float X, float Y, float Z)
{
    if (m_accelX != X) {
        m_accelX = X;
        emit accelXChanged();
    }

    if (m_accelY != Y) {
        m_accelY = Y;
        emit accelYChanged();
    }

    if (m_accelZ != Z) {
        m_accelZ = Z;
        emit accelZChanged();
    }
}

bool SimApi::hasGravitySensor() const
{
    return m_sensors & GravitySensor;
}

float SimApi::gravityX() const
{
    return m_gravityX;
}

float SimApi::gravityY() const
{
    return m_gravityY;
}

float SimApi::gravityZ() const
{
    return m_gravityZ;
}

void SimApi::setGravity(float X, float Y, float Z)
{
    if (m_gravityX != X) {
        m_gravityX = X;
        emit gravityXChanged();
    }

    if (m_gravityY != Y) {
        m_gravityY = Y;
        emit gravityYChanged();
    }

    if (m_gravityZ != Z) {
        m_gravityZ = Z;
        emit gravityZChanged();
    }
}

bool SimApi::hasGPS() const
{
    return m_sensors & GPS;
}

bool SimApi::hasGyro() const
{
    return m_sensors & Gyro;
}

bool SimApi::hasMagnetometer() const
{
    return m_sensors & Magnetometer;
}

void SimApi::setHardwareKeys(const HardwareKeys &keys)
{
    if (m_hardwareKeys != keys) {
        m_hardwareKeys = keys;
        emit hasVolumeUpKeyChanged();
        emit hasVolumeDownKeyChanged();
        emit hasVolumeMuteKeyChanged();
        emit hasPowerKeyChanged();
        emit hasHomeKeyChanged();
        emit hasBackKeyChanged();
        emit hasMenuKeyChanged();
    }
}

bool SimApi::hasVolumeUpKey() const
{
    return m_hardwareKeys & VolumeUpKey;
}

bool SimApi::hasVolumeDownKey() const
{
    return m_hardwareKeys & VolumeDownKey;
}

bool SimApi::hasVolumeMuteKey() const
{
    return m_hardwareKeys & VolumeMuteKey;
}

bool SimApi::hasPowerKey() const
{
    return m_hardwareKeys & PowerKey;
}

bool SimApi::hasHomeKey() const
{
    return m_hardwareKeys & HomeKey;
}

bool SimApi::hasBackKey() const
{
    return m_hardwareKeys & BackKey;
}

bool SimApi::hasMenuKey() const
{
    return m_hardwareKeys & MenuKey;
}

#include "simapi.moc"
