# clang -dynamiclib -o libTest.A.dylib -Wl,-no_pie printer.c
# clang -o test -Wl,-no_pie test.c libTest.A.dylib

#
# arm64 will build, but not run
# arm7
# from: http://www.jailbreakqa.com/questions/188600/command-line-test-program-killed-9
#The program was compiled with clang -O1 -g3 -std=c99 -Wall -Wextra -arch "$IOS_ARCH" -isysroot="$IOS_SYSROOT" t.c -o t.exe. IOS_ARCH is armv7 (the 6.1 SDK only offers armv7 and armv7s). IOS_SYSROOT si /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS6.1.sdk.
#
#
# After building, need to run ldld -S <exename> to codesign
CC=clang
OBJ=kqueue.o
SDK_HOME=SDKs
NO_ASLR_FLAGS=-Wl,-no_pie
DEBUG_FLAGS=-g

# can we use -target arm64-apple-ios?
IOS_TARGET=-target arm64-apple-darwin -isysroot $(SDK_HOME)/iPhoneOS9.3.sdk
MACOS_TARGET=

ARGS = $(MACOS_TARGET) $(DEBUG_FLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(ARGS)

main: $(OBJ)
	$(CC) -o kqueue kqueue.o $(ARGS)

clean:
	rm *.o kqueue
