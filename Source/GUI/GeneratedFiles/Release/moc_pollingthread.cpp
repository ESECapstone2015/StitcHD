/****************************************************************************
** Meta object code from reading C++ file 'pollingthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../pollingthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pollingthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PollingThread_t {
    QByteArrayData data[12];
    char stringdata[92];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PollingThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PollingThread_t qt_meta_stringdata_PollingThread = {
    {
QT_MOC_LITERAL(0, 0, 13), // "PollingThread"
QT_MOC_LITERAL(1, 14, 14), // "updatePosition"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 4), // "posx"
QT_MOC_LITERAL(4, 35, 4), // "posy"
QT_MOC_LITERAL(5, 40, 6), // "anglex"
QT_MOC_LITERAL(6, 47, 6), // "angley"
QT_MOC_LITERAL(7, 54, 6), // "anglez"
QT_MOC_LITERAL(8, 61, 9), // "sendDebug"
QT_MOC_LITERAL(9, 71, 3), // "msg"
QT_MOC_LITERAL(10, 75, 8), // "pollPort"
QT_MOC_LITERAL(11, 84, 7) // "comPort"

    },
    "PollingThread\0updatePosition\0\0posx\0"
    "posy\0anglex\0angley\0anglez\0sendDebug\0"
    "msg\0pollPort\0comPort"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PollingThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    5,   29,    2, 0x06 /* Public */,
       8,    1,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong, QMetaType::LongLong, QMetaType::LongLong, QMetaType::LongLong,    3,    4,    5,    6,    7,
    QMetaType::Void, QMetaType::QString,    9,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   11,

       0        // eod
};

void PollingThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PollingThread *_t = static_cast<PollingThread *>(_o);
        switch (_id) {
        case 0: _t->updatePosition((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2])),(*reinterpret_cast< qint64(*)>(_a[3])),(*reinterpret_cast< qint64(*)>(_a[4])),(*reinterpret_cast< qint64(*)>(_a[5]))); break;
        case 1: _t->sendDebug((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->pollPort((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (PollingThread::*_t)(qint64 , qint64 , qint64 , qint64 , qint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PollingThread::updatePosition)) {
                *result = 0;
            }
        }
        {
            typedef void (PollingThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PollingThread::sendDebug)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject PollingThread::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PollingThread.data,
      qt_meta_data_PollingThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PollingThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PollingThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PollingThread.stringdata))
        return static_cast<void*>(const_cast< PollingThread*>(this));
    return QObject::qt_metacast(_clname);
}

int PollingThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void PollingThread::updatePosition(qint64 _t1, qint64 _t2, qint64 _t3, qint64 _t4, qint64 _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PollingThread::sendDebug(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
