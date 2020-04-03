TEMPLATE = app
CONFIG += c++11
QT += opengl
QMAKE_CXXFLAGS += -std=c++11

#***** Les bibliothèques Windows *****#
win32:LIBS += C:\Windows\System32\opengl32.dll
win32:LIBS += C:\Windows\System32\freeglut.dll
win32:LIBS += C:\Windows\System32\glew32.dll

#***** Les bibliothèques Unix *****#
unix:LIBS += /usr/lib/x86_64-linux-gnu/libGL.so
unix:LIBS += /usr/lib/x86_64-linux-gnu/libGLU.so
unix:LIBS += /usr/lib/x86_64-linux-gnu/libglut.so
unix:LIBS += /usr/lib/x86_64-linux-gnu/libGLEW.so

#***** Bibliothèques externes *****#
HEADERS += GL/freeglut.h
HEADERS += GL/freeglut_std.h
HEADERS += GL/freeglut_ext.h
HEADERS += GL/glew.h
HEADERS += GL/wglew.h

# Core
HEADERS += \
    src/main.h \
    src/core.h

SOURCES += \
    src/main.cpp \
    src/core.cpp

# Subdivision
HEADERS += \
    src/subdivision/matrice.h \
    src/subdivision/mot.h \
    src/subdivision/schema.h \
        src/subdivision/schemaquads.h \
            src/subdivision/catmullclark.h \
            src/subdivision/doosabin.h \
            src/subdivision/quadratique_non_uniforme.h \
        src/subdivision/schematriangles.h \
            src/subdivision/loop.h \
        src/subdivision/schemaquadsnonuniforme.h

SOURCES += \
    src/subdivision/matrice.cpp \
    src/subdivision/mot.cpp \
    src/subdivision/schema.cpp \
        src/subdivision/schemaquads.cpp \
            src/subdivision/catmullclark.cpp \
            src/subdivision/doosabin.cpp \
            src/subdivision/quadratique_non_uniforme.cpp \
        src/subdivision/schematriangles.cpp \
            src/subdivision/loop.cpp \
        src/subdivision/schemaquadsnonuniforme.cpp

# Interface utilisateur
HEADERS += \
    src/gui/panneaulateral.h \
    src/gui/panneauscene.h \
    src/gui/fenetre.h \
    src/gui/widgetopengl.h

SOURCES += \
    src/gui/panneaulateral.cpp \
    src/gui/panneauscene.cpp \
    src/gui/fenetre.cpp \
    src/gui/widgetopengl.cpp


# Rendu
HEADERS += \
    src/rendu/shader.h \
    src/rendu/shaderprogram.h \
    src/rendu/shaderprogram.tpp \
    src/rendu/camera.h \
    src/rendu/objet.h \
    src/rendu/texture.h \
    src/rendu/lumiere.h \
    src/rendu/scene.h \
    src/rendu/materiau.h

SOURCES += \
    src/rendu/shader.cpp \
    src/rendu/shaderprogram.cpp \
    src/rendu/camera.cpp \
    src/rendu/objet.cpp \
    src/rendu/texture.cpp \
    src/rendu/lumiere.cpp \
    src/rendu/scene.cpp \
    src/rendu/materiau.cpp

# Les shaders
DISTFILES += \
    shaders/adaptative_quads.tcs \
    shaders/basique.geom \
    shaders/basique.vert \
    shaders/blinn_phong.frag \
    shaders/bounce.vert \
    shaders/catmull_clark.tes \
    shaders/doo_sabin.tes \
    shaders/loop.tes \
    shaders/normales.frag \
    shaders/quadratique_non_uniforme.tes \
    shaders/quads.tcs \
    shaders/quads.tes \
    shaders/rouge.frag \
    shaders/texture.frag \
    shaders/triangles.tcs \    
    shaders/triangles.tes \      
    shaders/triangles_04.tcs \
    shaders/triangles_16.tcs \
    shaders/bullets.geom \
    shaders/calcul_normales.geom \
    shaders/couleur.frag
