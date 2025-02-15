<h1 align="center">Load cell as Z-probe</h1>

<p align="center">
  <img alt="Github top language" src="https://img.shields.io/github/languages/top/Leandro-Bertoluzzi/load-cell-as-z-probe?color=56BEB8">

  <img alt="Github language count" src="https://img.shields.io/github/languages/count/Leandro-Bertoluzzi/load-cell-as-z-probe?color=56BEB8">

  <img alt="Repository size" src="https://img.shields.io/github/repo-size/Leandro-Bertoluzzi/load-cell-as-z-probe?color=56BEB8">

  <img alt="License" src="https://img.shields.io/github/license/Leandro-Bertoluzzi/load-cell-as-z-probe?color=56BEB8">

  <!-- <img alt="Github issues" src="https://img.shields.io/github/issues/Leandro-Bertoluzzi/load-cell-as-z-probe?color=56BEB8" /> -->

  <!-- <img alt="Github forks" src="https://img.shields.io/github/forks/Leandro-Bertoluzzi/load-cell-as-z-probe?color=56BEB8" /> -->

  <!-- <img alt="Github stars" src="https://img.shields.io/github/stars/Leandro-Bertoluzzi/load-cell-as-z-probe?color=56BEB8" /> -->
</p>

<p align="center">
  <a href="#dart-about">About</a> &#xa0; | &#xa0;
  <a href="#white_check_mark-requirements">Requirements</a> &#xa0; | &#xa0;
  <a href="#checkered_flag-starting">Starting</a> &#xa0; | &#xa0;
  <a href="#memo-license">License</a> &#xa0; | &#xa0;
  <a href="https://github.com/Leandro-Bertoluzzi" target="_blank">Author</a>
</p>

<br>

## :dart: About ##

Instructions, code, diagrams and other info to use a regular load cell as a Z-probe for your CNC machine.

## :white_check_mark: Requirements ##

Before starting :checkered_flag:, you need to have:
- Either the [Arduino IDE](https://www.arduino.cc/en/software) or VSCode with [PlatformIO](https://platformio.org/) installed.
- An Arduino board*.
- An HX711 board.

*Tested with Arduino Nano v3 and UNO v3.

## :checkered_flag: Starting ##

First, check your wiring:

![wiring diagram](./docs/z-probe-wiring.svg)

Then, you can view the code:

```bash
# Clone this project
$ git clone https://github.com/Leandro-Bertoluzzi/load-cell-as-z-probe

# Access
$ cd load-cell-as-z-probe
```

You can the open the folder `arduino/z_probe` with the Arduino IDE or PlatformIO and load it to your Arduino board.

## :memo: License ##

This project is under license from MIT. For more details, see the [LICENSE](LICENSE.md) file.


Made with :heart: by <a href="https://github.com/Leandro-Bertoluzzi" target="_blank">Leandro Bertoluzzi</a>

&#xa0;

<a href="#top">Back to top</a>
