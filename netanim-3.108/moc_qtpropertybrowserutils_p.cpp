/****************************************************************************
** Meta object code from reading C++ file 'qtpropertybrowserutils_p.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qtpropertybrowser/src/qtpropertybrowserutils_p.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtpropertybrowserutils_p.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtBoolEdit[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QtBoolEdit[] = {
    "QtBoolEdit\0\0toggled(bool)\0"
};

void QtBoolEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QtBoolEdit *_t = static_cast<QtBoolEdit *>(_o);
        switch (_id) {
        case 0: _t->toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QtBoolEdit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QtBoolEdit::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtBoolEdit,
      qt_meta_data_QtBoolEdit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtBoolEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtBoolEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtBoolEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtBoolEdit))
        return static_cast<void*>(const_cast< QtBoolEdit*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtBoolEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void QtBoolEdit::toggled(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_QtKeySequenceEdit[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      61,   19,   18,   18, 0x0a,
      90,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtKeySequenceEdit[] = {
    "QtKeySequenceEdit\0\0sequence\0"
    "keySequenceChanged(QKeySequence)\0"
    "setKeySequence(QKeySequence)\0"
    "slotClearShortcut()\0"
};

void QtKeySequenceEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QtKeySequenceEdit *_t = static_cast<QtKeySequenceEdit *>(_o);
        switch (_id) {
        case 0: _t->keySequenceChanged((*reinterpret_cast< const QKeySequence(*)>(_a[1]))); break;
        case 1: _t->setKeySequence((*reinterpret_cast< const QKeySequence(*)>(_a[1]))); break;
        case 2: _t->slotClearShortcut(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QtKeySequenceEdit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QtKeySequenceEdit::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtKeySequenceEdit,
      qt_meta_data_QtKeySequenceEdit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtKeySequenceEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtKeySequenceEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtKeySequenceEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtKeySequenceEdit))
        return static_cast<void*>(const_cast< QtKeySequenceEdit*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtKeySequenceEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void QtKeySequenceEdit::keySequenceChanged(const QKeySequence & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
