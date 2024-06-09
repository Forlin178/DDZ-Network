/****************************************************************************
** Meta object code from reading C++ file 'communication.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../ddz-client/thread/communication.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'communication.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Communication_t {
    QByteArrayData data[14];
    char stringdata0[118];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Communication_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Communication_t qt_meta_stringdata_Communication = {
    {
QT_MOC_LITERAL(0, 0, 13), // "Communication"
QT_MOC_LITERAL(1, 14, 6), // "failed"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 7), // "loginOK"
QT_MOC_LITERAL(4, 30, 10), // "registerOK"
QT_MOC_LITERAL(5, 41, 9), // "failedMsg"
QT_MOC_LITERAL(6, 51, 3), // "msg"
QT_MOC_LITERAL(7, 55, 11), // "playerCount"
QT_MOC_LITERAL(8, 67, 6), // "number"
QT_MOC_LITERAL(9, 74, 9), // "startGame"
QT_MOC_LITERAL(10, 84, 9), // "roomExist"
QT_MOC_LITERAL(11, 94, 4), // "flag"
QT_MOC_LITERAL(12, 99, 12), // "someoneLeave"
QT_MOC_LITERAL(13, 112, 5) // "count"

    },
    "Communication\0failed\0\0loginOK\0registerOK\0"
    "failedMsg\0msg\0playerCount\0number\0"
    "startGame\0roomExist\0flag\0someoneLeave\0"
    "count"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Communication[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,
       4,    0,   56,    2, 0x06 /* Public */,
       5,    1,   57,    2, 0x06 /* Public */,
       7,    1,   60,    2, 0x06 /* Public */,
       9,    1,   63,    2, 0x06 /* Public */,
      10,    1,   66,    2, 0x06 /* Public */,
      12,    1,   69,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::QByteArray,    6,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Int,   13,

       0        // eod
};

void Communication::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Communication *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->failed(); break;
        case 1: _t->loginOK(); break;
        case 2: _t->registerOK(); break;
        case 3: _t->failedMsg((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 4: _t->playerCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->startGame((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 6: _t->roomExist((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->someoneLeave((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Communication::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Communication::failed)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Communication::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Communication::loginOK)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Communication::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Communication::registerOK)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Communication::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Communication::failedMsg)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Communication::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Communication::playerCount)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Communication::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Communication::startGame)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Communication::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Communication::roomExist)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (Communication::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Communication::someoneLeave)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Communication::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Communication.data,
    qt_meta_data_Communication,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Communication::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Communication::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Communication.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QRunnable"))
        return static_cast< QRunnable*>(this);
    return QObject::qt_metacast(_clname);
}

int Communication::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void Communication::failed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Communication::loginOK()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Communication::registerOK()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Communication::failedMsg(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Communication::playerCount(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Communication::startGame(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Communication::roomExist(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Communication::someoneLeave(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
