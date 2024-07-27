# pz-web-map
A CLI tool generator Project Zomboid map

It's up to future user to setup `config.yaml` with proper values (valid and existing). This tool does little to almost no error checking (apart from this above).
As long as game directory and mod directory are valid, it should work, but it deoes not care if mods are made properly or not (thus if checks mod.info).

For now this project will provide only files for (non-existing for now) web application. And this repo will probably be renamed to `pz-web-map-generator` or something similar. Dunno yet.

## Usage

### Linux
 - 1. Clone the repository
 - 2. Create `build` directory
 - 3. Run `cmake ..` in `build` directory
 - 4. Run `make` in `build` directory 
 - 5. Copy `PZWebMap` to your desired location
 - 6. Copy `config.example.yaml` nexto `PZWebMap` and rename it to `config.yaml`
 - 7. Edit `config.yaml`
 - 8. Run `./PZWebMap` in terminal
 - 9. Enjoy your map

### Windows

- 0. Install Linux
- 1. Install Steam
- 2. Install Project Zomboid
- 3. Follow Linux instructions

I'm joking, but you can try to run it on Windows, but I have no idea if it will work.
If you want to try, you can try to use WSL or MinGW. Since I don't have Windows, I can't test it and will not provide any support for Windows nor will contribute any code.

## Configuration

See `config.example.yaml` for example configuration. You can copy it to `config.yaml` and edit it to your needs.

## Features

### Available
 - none

### Planned
- Generate a tile-based map of Project Zomboid with some visual helpers (as obvioulsy)
- Detects all mods (from Steam Workshop) and tries to determine if they are mod maps or texture packs and checks for dependencies (partially implemented)

## Contributing
If you want to contribute, feel free to fork and submit a pull request. I will review it and merge it if it fits the project. If you have any questions, feel free to ask.

## License
This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

Portions of this project are derived from software originally licensed under other licenses. See respective files for details. Derived software:
 - [yaml-cpp](https://github.com/jbeder/yaml-cpp) - [MIT License](https://github.com/jbeder/yaml-cpp/blob/master/LICENSE)


Project is based on the work of [The Indie Stone](https://theindiestone.com/), the creators of Project Zomboid. This project is not affiliated with The Indie Stone.

## Final notes

This is a project I started to learn C++ and to have some fun. I am not a professional programmer, so the code may not be perfect. If you have any suggestions or improvements, feel free to submit a pull request. I will be happy to review it.