/****************************************************************************
** Meta object code from reading C++ file 'panneauscene.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui/panneauscene.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'panneauscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PanneauScene_t {
    QByteArrayData data[13];
    char stringdata0[151];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PanneauScene_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PanneauScene_t qt_meta_stringdata_PanneauScene = {
    {
QT_MOC_LITERAL(0, 0, 12), // "PanneauScene"
QT_MOC_LITERAL(1, 13, 10), // "changerFPS"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 11), // "mise_a_jour"
QT_MOC_LITERAL(4, 37, 8), // "addObjet"
QT_MOC_LITERAL(5, 46, 8), // "delObjet"
QT_MOC_LITERAL(6, 55, 12), // "changerObjet"
QT_MOC_LITERAL(7, 68, 15), // "changerMateriau"
QT_MOC_LITERAL(8, 84, 22), // "changerLumiereCourante"
QT_MOC_LITERAL(9, 107, 1), // "n"
QT_MOC_LITERAL(10, 109, 19), // "modificationLumiere"
QT_MOC_LITERAL(11, 129, 10), // "addLumiere"
QT_MOC_LITERAL(12, 140, 10) // "delLumiere"

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
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    1,   70,    2, 0x08 /* Private */,
      10,    0,   73,    2, 0x08 /* Private */,
      11,    0,   74,    2, 0x08 /* Private */,
      12,    0,   75,    2, 0x08 /* Private */,

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
        PanneauScene *_t = static_cast<PanneauScene *>(_o);
        Q_UNUSED(_t)
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

QT_INIT_METAOBJECT const QMetaObject PanneauScene::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_PanneauScene.data,
    qt_meta_data_PanneauScene,
    qt_static_metacall,
    nullptr,
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
