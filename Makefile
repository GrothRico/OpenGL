PACKAGES:=xorg-dev libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev pkg-config libxmu-dev libxi-dev libgl-dev
EXEC_NAME:=Game

default: run

run: compile
	./build/$(EXEC_NAME)

compile: create-build-files
	$(MAKE) -C ./build/

setup-project: install-packages clone-vcpkg bootstrap-vcpkg create-build-files

tear-down-project:
	rm -rf ./vcpkg/ ./vcpkg_installed/ ./build/ ./compile_commands.json
	sudo apt remove $(PACKAGES)

install-packages:
	sudo apt-get update
	sudo apt-get install $(PACKAGES)

create-build-files:
	rm -f compile_commands.json
	cmake -S ./ -B ./build/ && ln -s build/compile_commands.json compile_commands.json

clone-vcpkg:
	git clone https://github.com/microsoft/vcpkg.git

bootstrap-vcpkg:
	./vcpkg/bootstrap-vcpkg.sh

clean:
	rm -r ./build/
