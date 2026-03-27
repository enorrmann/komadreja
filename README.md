# Komadreja 

**Komadreja** is a low-latency audio sampler, built with **C++**, **Qt 6 (QML)**, and **miniaudio**.

##  Features

- **Direct Pad Recording**: Hold any empty pad to record audio from your microphone.
- **Instant Playback**: Tap any assigned pad to play the sample instantly.
- **Low Latency**: High-performance audio engine using `miniaudio` in duplex mode.
- **Cross-platform**: Support for **Linux** and **Android (arm64-v8a)**.
- **Interactive UI**: Responsive and modern design with QML.

##  Requirements

- **Qt 6.10.1** (or compatible)
- **CMake 3.20+**
- **Android NDK r27c** (for Android builds)
- **C++20** compliant compiler

##  Build Instructions

### Linux

To build and run on Linux:

```bash
chmod +x clean_build.sh
./clean_build.sh
./build/appKomadreja
```

### Android

To build the APK for Android (arm64-v8a):

```bash
chmod +x build_android.sh
./build_android.sh
```

The resulting binary will be located in `build_android/libappKomadreja_arm64-v8a.so` (and the APK in `build_android/android-build/`).

##  Usage

1. **Record**: Find an empty (dark) pad. Press and hold it to record audio. The pad will turn red while recording.
2. **Assign**: Release the pad to stop recording. The sample is automatically assigned to that pad.
3. **Play**: Tap the filled pad to play the recorded sample.
4. **Bank selection**: Use the A, B, C, D bank selectors to switch between different sets of pads.

## Project Structure

- `src/audio/`: C++ Audio Engine implementation using `miniaudio`.
- `qml/`: User interface components and screens.
- `assets/`: Default samples and icons.
- `src/models/`: Data models for pads and audio state.

---

### Español

**Komadreja** es un sampler de audio de baja latencia, desarrollado con **C++**, **Qt 6 (QML)**, y **miniaudio**.

**Funciones principales:**
- Grabación directa en los Pads (Mantener presionado para grabar).
- Reproducción instantánea al tocar.
- Soporte nativo para Linux y Android.

**Compilación:**
- Linux: `./clean_build.sh`
- Android: `./build_android.sh`