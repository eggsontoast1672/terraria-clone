build:
  cmake --build build

run: build
  build/terraria-clone

clean:
  cmake --build build --target clean
