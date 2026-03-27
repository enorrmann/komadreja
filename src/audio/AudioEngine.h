#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>
#include <QString>
#include <QMap>
#include <mutex>
#include <atomic>
#include <vector>
#include "miniaudio.h"

struct SampleData {
    float* pData = nullptr;
    ma_uint64 frameCount = 0;
    ma_uint32 channels = 0;
    ma_uint32 sampleRate = 0;
};

struct Voice {
    int padIndex = -1;
    ma_uint64 currentFrame = 0;
    bool active = false;
    float volume = 1.0f;
};

class AudioEngine : public QObject
{
    Q_OBJECT
public:
    explicit AudioEngine(QObject *parent = nullptr);
    ~AudioEngine();

    Q_INVOKABLE bool init();
    Q_INVOKABLE void loadSample(int padIndex, const QString& filePath);
    Q_INVOKABLE void playSample(int padIndex, float velocity = 1.0f);
    Q_INVOKABLE void stopSample(int padIndex);

    // Recording API
    Q_INVOKABLE void startRecording();
    Q_INVOKABLE void stopRecording();
    Q_INVOKABLE void assignRecordingToPad(int padIndex);

    // Audio callback from miniaudio
    void processAudio(float* pOutput, const float* pInput, ma_uint32 frameCount);

private:
    ma_device m_device;
    bool m_isInitialized;

    // Recording variables
    std::atomic<bool> m_isRecording;
    std::vector<float> m_recordBuffer;
    std::atomic<size_t> m_recordPosition;

    // A map of pad index to sample data
    QMap<int, SampleData> m_samples;

    static const int MAX_VOICES = 32;
    Voice m_voices[MAX_VOICES];
    std::mutex m_audioMutex;

    static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
};

#endif // AUDIOENGINE_H
