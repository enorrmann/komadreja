# Implementación de Grabación de Audio en Koala Klone

## Situación Actual

1. **Biblioteca Base:** El proyecto utiliza repozitorios de audio basados en la biblioteca `miniaudio` (en forma de un solo archivo de cabecera).
2. **Motor de Audio (`AudioEngine`):** 
   - Actualmente, el sistema inicializa el dispositivo en modo de **solo reproducción** (`ma_device_type_playback`).
   - El manejador de eventos de audio (`data_callback` y `processAudio`) mezcla las distintas voces de los pads y las envía al búfer de salida (`pOutput`).
   - El parámetro `pInput` en el callback actualmente se ignora o viene vacío, ya que no se está capturando audio.
3. **Gestión de Muestras:** Las muestras de audio (`SampleData`) se leen desde un archivo físico en el disco y se decodifican mediante `ma_decoder_init_memory` a bloques de memoria RAM persistentes almacenados en `QMap<int, SampleData> m_samples;`.

## Pasos a Seguir para Implementar la Grabación

Para permitir que el usuario mantenga pulsado un botón y grabe audio desde el micrófono, se recomiendan los siguientes pasos técnicos:

### 1. Reconfigurar `miniaudio` a Modo Duplex (o Dispositivos Separados)

Para poder capturar audio, existen dos acercamientos con `miniaudio`:
- **Dispositivo Duplex:** Implica cambiar `ma_device_type_playback` a `ma_device_type_duplex` en `AudioEngine::init()`. Esta es típicamente la forma más sencilla para mantener la sincronización, pues el mismo callback recibe `pInput` (micrófono) y provee `pOutput` (altavoz).
- **Dispositivo de Captura Dedicado:** Instanciar otra variable `ma_device m_captureDevice` configurada en `ma_device_type_capture` con un callback distinto dedicado exclusivamente a llenar un búfer de datos.

**Recomendación:** Configurar en modo *Duplex* añadiendo:
```cpp
config.capture.format   = ma_format_f32;
config.capture.channels = 2; // o 1 para micrófono mono
config.capture.shareMode = ma_share_mode_shared;
```

### 2. Modificar el Estado Global en `AudioEngine`

Se necesita agregar al motor un estado para controlar en qué momento se están conservando los datos del micrófono:
- `std::atomic<bool> m_isRecording{false};` (Atómico porque se lee en el hilo de audio y se escribe desde el hilo de la UI).
- Un búfer en disco o en memoria para almacenar las tramas (e.g., `std::vector<float> m_recordBuffer;` pre-reservado con una cantidad de memoria suficiente, o bien guardar a un archivo temporal `.wav` sobre la marcha usando un codificador).

### 3. Adaptar el Callback de Audio (`processAudio`)

En el procesamiento si se es duplex:
- Si `m_isRecording == true`: Leer de la variable `pInput` y copiar esos datos (muestras `float`) al final de `m_recordBuffer`.
- Hay que tener cuidado con las asignaciones dinámicas y los *locks* (como `mutex`) durante el callback, pues éste corre en el hilo prioritario de audio. Es mejor utilizar *lock-free queues* o un simple búfer grande pre-alocado (ej. suficiente para 1 minuto de memoria) con un índice atómico que avanza.

### 4. Exponer Métodos Invocables (`Q_INVOKABLE`) hacia QML

Añadir las funciones que controlarán el flujo desde la interfaz:
- `Q_INVOKABLE void startRecording();`: Prepara el búfer temporal, lo vacía, y cambia `m_isRecording = true`.
- `Q_INVOKABLE void stopRecording();`: Cambia la bandera de grabación a falsa y finaliza el proceso de copiado del búfer.
- (Opcional) `Q_INVOKABLE void assignRecordingToPad(int padIndex);`: Una vez terminada la grabación, este método toma todos los datos recabados en `m_recordBuffer`, instancia un `SampleData` en la RAM, y lo mete al mapa `m_samples`, reemplazando lo que estuviese en el pad seleccionado.

### 5. Integración con la Interfaz Gráfica (QML / UI)

- Programar el componente de la vista para tener un botón que ejecute `startRecording()` en el suceso `onPressed` y `stopRecording()` en `onReleased`, simulando un esquema push-to-talk.
- Añadir el despliegue lógico y visual (indicador en rojo), para que el usuario sepa que la aplicación lo está escuchando.
