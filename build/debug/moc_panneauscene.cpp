/****************************************************************************
** Meta object code from reading C++ file 'panneauscene.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.0.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/gui/panneauscene.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'panneauscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PanneauScene_t {
    const uint offsetsAndSize[26];
    char stringdata0[151];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_PanneauScene_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_PanneauScene_t qt_meta_stringdata_PanneauScene = {
    {
QT_MOC_LITERAL(0, 12), // "PanneauScene"
QT_MOC_LITERAL(13, 10), // "changerFPS"
QT_MOC_LITERAL(24, 0), // ""
QT_MOC_LITERAL(25, 11), // "mise_a_jour"
QT_MOC_LITERAL(37, 8), // "addObjet"
QT_MOC_LITERAL(46, 8), // "delObjet"
QT_MOC_LITERAL(55, 12), // "changerObjet"
QT_MOC_LITERAL(68, 15), // "changerMateriau"
QT_MOC_LITERAL(84, 22), // "changerLumiereCourante"
QT_MOC_LITERAL(107, 1), // "n"
QT_MOC_LITERAL(109, 19), // "modificationLumiere"
QT_MOC_LITERAL(129, 10), // "addLumiere"
QT_MOC_LITERAL(140, 10) // "delLumiere"

    },
    "PanneauScene\0changerFPS\0\0mise_a_jour\0"
    "addObjet\0delObjet\0changerObjet\0"
    "changerMateriau\0changerLumiereCourante\0"
    "n\0modificationLumiere\0addLumiere\0"
    "delLumiere"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PanneauScene[] = {

 // content:
       9,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   74,    2, 0x08,    0 /* Private */,
       3,    0,   75,    2, 0x08,    1 /* Private */,
       4,    0,   76,    2, 0x08,    2 /* Private */,
       5,    0,   77,    2, 0x08,    3 /* Private */,
       6,    0,   78,    2, 0x08,    4 /* Private */,
       7,    0,   79,    2, 0x08,    5 /* Private */,
       8,    1,   80,    2, 0x08,    6 /* Private */,
      10,    0,   83,    2, 0x08,    8 /* Private */,
      11,    0,   84,    2, 0x08,    9 /* Private */,
      12,    0,   85,    2, 0x08,   10 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PanneauScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PanneauScene *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->changerFPS(); break;
        case 1: _t->mise_a_jour(); break;
        case 2: _t->addObjet(); break;
        case 3: _t->delObjet(); break;
        case 4: _t->changerObjet(); break;
        case 5: _t->changerMateriau(); break;
        case 6: _t->changerLumiereCourante((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->modificationLumiere(); break;
        case 8: _t->addLumiere(); break;
        case 9: _t->delLumiere(); break;
        default: ;
        }
    }
}

const QMetaObject PanneauScene::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_PanneauScene.offsetsAndSize,
    qt_meta_data_PanneauScene,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_PanneauScene_t

, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *PanneauScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PanneauScene::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PanneauScene.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PanneauScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
