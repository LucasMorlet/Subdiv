/****************************************************************************
** Meta object code from reading C++ file 'panneauscene.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/gui/panneauscene.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'panneauscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PanneauScene[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      27,   13,   13,   13, 0x08,
      41,   13,   13,   13, 0x08,
      52,   13,   13,   13, 0x08,
      63,   13,   13,   13, 0x08,
      78,   13,   13,   13, 0x08,
      98,   96,   13,   13, 0x08,
     126,   13,   13,   13, 0x08,
     148,   13,   13,   13, 0x08,
     161,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PanneauScene[] = {
    "PanneauScene\0\0changerFPS()\0mise_a_jour()\0"
    "addObjet()\0delObjet()\0changerObjet()\0"
    "changerMateriau()\0n\0changerLumiereCourante(int)\0"
    "modificationLumiere()\0addLumiere()\0"
    "delLumiere()\0"
};

void PanneauScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PanneauScene *_t = static_cast<PanneauScene *>(_o);
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

const QMetaObjectExtraData PanneauScene::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PanneauScene::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PanneauScene,
      qt_meta_data_PanneauScene, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PanneauScene::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PanneauScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PanneauScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PanneauScene))
        return static_cast<void*>(const_cast< PanneauScene*>(this));
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
    }
    return _id;
}
QT_END_MOC_NAMESPACE
