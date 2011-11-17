OBJS=Engine.o \
Entity.o \
GraphicsEngine.o \
BitmapText.o \
Image.o \
Log.o \
Rect.o \
Scene.o \
ScriptEngine.o \
Sprite.o \
TileMap.o 

GLOBJS=gl/GLGraphicsEngine.o gl/GLImage.o
LUAOBJS=lua/LUAEntity.o lua/LUAManager.o

CFLAGS=-fPIC -shared
CXXFLAGS=$(CFLAGS)


all:$(OBJS)
	g++ $(CXXFLAGS) -o libhandgrenade.so.1.0 $(OBJS)
	ar -rsc libhandgrenade.a $(OBJS)
	cp -v libhandgrenade.so.1.0 libhandgrenade.a ../libs/
	cp -v *.hpp ../include/HandGrenade/

gl:$(GLOBJS)
	g++ $(CXXFLAGS) -o libhandgrenadeGL.so.1.0 $(GLOBJS)
	ar -rsc libhandgrenadeGL.a $(GLOBJS)
	cp -v libhandgrenadeGL.so.1.0 libhandgrenadeGL.a ../libs/
	cp -v gl/*.hpp ../include/HandGrenade/gl

lua:$(LUAOBJS)
	g++ $(CXXFLAGS) -o libhandgrenadeLUA.so.1.0 $(LUAOBJS)
	ar -rsc libhandgrenadeLUA.a $(LUAOBJS)
	cp -v libhandgrenadeLUA.so.1.0 libhandgrenadeLUA.a ../libs/
	cp -v lua/*.hpp ../include/HandGrenade/lua

%o:%cpp
	g++ $(CXXFLAGS) $< -c
