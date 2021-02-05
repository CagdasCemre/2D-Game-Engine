build:
	g++ \
	./src/*.cpp \
	-o game \
	-I"./lib/lua" \
	-L"./lib/lua" \
	-llua \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer;
