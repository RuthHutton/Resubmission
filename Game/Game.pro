TARGET=Game
QT+=testlib
CONFIG+=c++11
OBJECTS_DIR=obj

isEqual(QT_MAJOR_VERSION, 5) {
        cache()
        DEFINES +=QT5BUILD
}
MOC_DIR=moc
CONFIG-=app_bundle

# .cpp files to be included
SOURCES+=$$PWD/src/main.cpp \
         $$PWD/src/Window.cpp \
         $$PWD/src/Fish.cpp

# .h files to be included
HEADERS+=$$PWD/include/Window.h \
         $$PWD/include/Fish.h

DESTDIR=./

OTHER_FILES+= shaders/*.glsl \


INCLUDEPATH+=$$PWD/include




CONFIG += console

!equals(PWD, $${OUT_PWD}){
        copydata.commands = echo "creating destination dirs" ;
        copydata.commands += mkdir -p $$OUT_PWD/models ;
        copydata.commands += echo "copying files" ;
        copydata.commands += $(COPY_DIR) $$PWD/models/*.obj $$OUT_PWD/models/ ;
        first.depends = $(first) copydata
        export(first.depends)
        export(copydata.commands)
        QMAKE_EXTRA_TARGETS += first copydata
}




# include UseNGL.pri to use NGL
NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){
       message("including $HOME/NGL")
      include($(HOME)/NGL/UseNGL.pri)
}
else
    {
       message("Using custom NGL location")
      include($(NGLDIR)/UseNGL.pri)
    }
