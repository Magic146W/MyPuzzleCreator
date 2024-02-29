<a name="readme-top"></a>
[![License: LGPL v3](https://img.shields.io/badge/License-LGPL_v3-blue.svg)](https://github.com/Magic146W/Qt_FileExplorer/blob/master/LICENSE)
[![YouTube Link](https://img.shields.io/badge/YouTube-Link-red.svg)](https://youtu.be/8LXdldJvki8)
<br />
<div align="center">
  <h1 align="center">C++ based Puzzle Creator</h1>

Welcome to My Puzzle Creator, a puzzle creation software developed using Qt 6.6.1 and C++, and built with CMake. This tool was created as a learning project, aimed at broadening my knowledge of the QWidget, QPainter, QPainterPath classes, as well as drag-and-drop events.<br>

  <p align="center">
    This README provides an initial overview of the puzzle creation software. More comprehensive and detailed information can be found in the documentation. Link to YouTube presentation in top left corner.
    <br />
  </p>
</div>
  

  <summary><h2>Table of Contents</h2></summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#getting-project-to-run-on-visual-studio">Getting project to run on Visual Studio</a></li>
        <li><a href="#getting-project-to-run-on-qt-creator">Getting project to run on Qt Creator</a></li>
      </ul>
    </li>
    <li>
     <a href="#usage">Usage</a>
      <ul>
        <li><a href="#presentation">Presentation</a></li>
      </ul>
    </li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
  </ol>


## About The Project

My Puzzle Creator is software designed to transform nearly any image into a puzzle of your choice. With customizable options to adjust the size and shape of puzzle pieces, including the ability to change the height and width independently, users have ultimate flexibility in creating their puzzles.
- Versatility: My Puzzle Creator offers a wide range of functionalities, empowering users to craft diverse puzzles according to their preferences. Whether saving them for later enjoyment or printing them out, the possibilities are endless.
- Control and Precision: With customizable settings, users maintain precise control over puzzle generation, ensuring personalized results tailored to their preferences.
- Intuitive Interface: My Puzzle Creator features an intuitive and linear interface, making the puzzle creation process straightforward and enjoyable. Users can seamlessly navigate through the software, simplifying the overall user experience.
- Create and Play: My Puzzle Creator not only facilitates puzzle creation but also provides a platform for users to solve their creations. With drag-and-drop functionality and interactive features, solving puzzles becomes both engaging and effortless.

With My Puzzle Creator, unleash your creativity and enjoy the thrill of both creating and solving unique puzzles with ease.

### Built With

[![Qt](https://img.shields.io/badge/Qt-%23217346.svg?style=for-the-badge&logo=Qt&logoColor=white)](https://www.qt.io/download-open-source) Used as the primary Integrated Development Environment (IDE) for coding the My Puzzle Creator project. Version 6.6.1.
<br>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Getting Started

For the project to function properly, user must have either Qt Creator or Visual Studio IDE 2019 (or newer) installed on their system. These Integrated Development Environments (IDEs) provide the necessary tools and environment to compile, run, and work with the puzzle creator application seamlessly.

### Prerequisites

**QT**<br>
You will need Qt version 6.6.1 or newer, along with the Developer and Designer Tools. If you're unsure whether these tools are installed or if they are not installed, open the Qt Maintenance Tool by navigating to:
"Tools > Qt Maintenance Tool > Start Maintenance Tool" (select the necessary options in the 'Select Components' tab)
<br>

**Visual Studio**<br>
If you prefer using the Visual Studio IDE, ensure that you have the necessary components installed. Access the Visual Studio Installer, modify your Visual Studio IDE, and search for the 'C++ CMake tools for Windows' in the 'Individual Components' tab. Install it if needed. Alternatively, you can install the C++ Windows package, ensuring to select CMake if you haven't installed C++ yet.
    
### Getting project to run on Visual Studio

1. Navigate "File > Clone Repository..." and select folder for repository <a href="https://github.com/Magic146W/MyPuzzleCreator.git">https://github.com/Magic146W/MyPuzzleCreator.git</a>.

2. Setting Up CMake for Visual Studio:
- Open Visual Studio and go to “File &gt; Open &gt; CMake…” and select CMakeLists.txt inside your working directory<br>
  - If you don’t find the CMake option, please go through prerequisites for VS<br>
  -  The first time you open this option, an overview of CMake in Visual Studio will appear. Once done, repeat the process of opening CMake from "File &gt; Open" and select your working directory, then choose CMakeLists.txt.<br>
-  Wait for the project to build.
3. To run the project, either click the green arrow with "Qt Application" written next to it or press F5.


### Getting project to run on Qt Creator

1. Clone the repository to your local machine:
   ```sh
   git clone https://github.com/Magic146W/MyPuzzleCreator.git
   ```
2. Open the project by clicking on CMakeLists.txt inside of the cloned directory.
3. When opening the project for the first time or if not previously configured, select the kit "Desktop Qt 6.6.1 MSVC2019 64bit" or a newer version and click on "Configure Project."
4. Wait for the initial project setup and build to finish.
5. Run the project by clicking the green arrow or using the shortcut Ctrl+R.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Usage

 **Functions:**
 
- File: "Open...", "Save As...", "Print...", "Exit"
- Edit: "Copy", "Paste"
- View: "Zoom in (25%)", "Zoom out (25%)", "Normal Size"
- Puzzle: "Prepare", "Create", "Play"

 **Initiating Actions:**

- The main software functionalities are located within the Puzzle menu bar, guiding the user step by step through the process of creating puzzle shapes to solving the puzzle.
  
 **Hotkey Usage:**
  
- Utilize the designated hotkey for swift access and immediate action execution. Each hotkey is conveniently displayed alongside its corresponding Action button within the menu bar.

### Presentation

[![YouTube Link](https://img.shields.io/badge/YouTube-Link-red.svg)](https://youtu.be/8LXdldJvki8)

## Contributing

Contributions make the open-source community a fantastic space to learn, inspire, and create. Your contributions are **very much appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/YourFeature`)
3. Commit your Changes (`git commit -m 'Add my YourFeature'`)
4. Push to the Branch (`git push origin feature/YourFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## License

Distributed under the LGPL v3 License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>
