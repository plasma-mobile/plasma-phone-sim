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

#include <QDebug>
#include <QMetaObject>
#include <QMetaMethod>

SimApi::SimApi(const QString &currentPath, QObject *parent)
    : QObject(parent),
      m_packagePath(currentPath),
      m_accelX(0),
      m_accelY(0),
      m_accelZ(0),
      m_gravityX(0),
      m_gravityY(0),
      m_gravityZ(0),
      m_gyroX(0),
      m_gyroY(0),
      m_gyroZ(0),
      m_magneticX(0),
      m_magneticY(0),
      m_magneticZ(0),
      m_magneticBiasX(0),
      m_magneticBiasY(0),
      m_magneticBiasZ(0)
{
    // connect all signals to the logSignal slot
    // QMetaObject madness^Hmagic
    const QMetaObject *metaObj = metaObject();
    const int numMethods = metaObj->methodCount();
    const int methodOffset = metaObj->methodOffset();
    const int lastMethod = numMethods + methodOffset;
    const QMetaMethod slot(metaObj->method(metaObj->indexOfSlot("logSignal()")));
    for (int i = metaObj->methodOffset(); i < lastMethod; ++i) {
        const QMetaMethod method(metaObj->method(i));
        if (method.methodType() == QMetaMethod::Signal) {
            connect(this, method, this, slot);
        }
    }

//     connect(this, &SimApi::homeKeyPressed,
//             this, &SimApi::logSignal);
//     connect(this, &SimApi::homeKeyReleased,
//             this, &SimApi::logSignal);
}

void SimApi::logSignal()
{
    QObject *s = sender();
    if (!s) {
        return;
    }

    const int signalIndex = senderSignalIndex();
    const QMetaObject *metaObj = s->metaObject();
    const QMetaMethod method(metaObj->method(signalIndex));
    qDebug() << "EVENT!" << method.name();
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

float SimApi::gyroX() const
{
    return m_gyroX;
}

float SimApi::gyroY() const
{
    return m_gyroY;
}

float SimApi::gyroZ() const
{
    return m_gyroZ;
}

void SimApi::setGyro(float X, float Y, float Z)
{
    if (m_gyroX != X) {
        m_gyroX = X;
        emit gyroXChanged();
    }

    if (m_gyroY != Y) {
        m_gyroY = Y;
        emit gyroYChanged();
    }

    if (m_gyroZ != Z) {
        m_gyroZ = Z;
        emit gyroZChanged();
    }
}

bool SimApi::hasMagnetometer() const
{
    return m_sensors & Magnetometer;
}

float SimApi::magneticX() const
{
    return m_magneticX;
}

float SimApi::magneticY() const
{
    return m_magneticY;
}

float SimApi::magneticZ() const
{
    return m_magneticZ;
}

float SimApi::magneticBiasX() const
{
    return m_magneticBiasX;
}

float SimApi::magneticBiasY() const
{
    return m_magneticBiasY;
}

float SimApi::magneticBiasZ() const
{
    return m_magneticZ;
}

void SimApi::setMagentic(float X, float XBias,
                         float Y, float YBias,
                         float Z, float ZBias)
{
    if (m_magneticX != X) {
        m_magneticX = X;
        emit magneticXChanged();
    }

    if (m_magneticBiasX != XBias) {
        m_magneticBiasX = XBias;
        emit magneticBiasXChanged();
    }

    if (m_magneticY != Y) {
        m_magneticY = Y;
        emit magneticYChanged();
    }

    if (m_magneticBiasY != YBias) {
        m_magneticBiasY = YBias;
        emit magneticBiasYChanged();
    }

    if (m_magneticZ != Z) {
        m_magneticZ = Z;
        emit magneticZChanged();
    }

    if (m_magneticBiasZ != ZBias) {
        m_magneticBiasZ = ZBias;
        emit magneticBiasZChanged();
    }
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
