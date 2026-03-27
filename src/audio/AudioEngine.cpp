#define MINIAUDIO_IMPLEMENTATION
#include "AudioEngine.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <cstring>

void AudioEngine::data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    AudioEngine* engine = static_cast<AudioEngine*>(pDevice->pUserData);
    engine->processAudio(static_cast<float*>(pOutput), static_cast<const float*>(pInput), frameCount);
}

AudioEngine::AudioEngine(QObject *parent)
    : QObject(parent), m_isInitialized(false)
{
}

AudioEngine::~AudioEngine()
{
    if (m_isInitialized) {
        ma_device_uninit(&m_device);
    }
    
    // Free sample data
    for (auto& sample : m_samples) {
        if (sample.pData) {
            ma_free(sample.pData, nullptr);
        }
    }
}

bool AudioEngine::init()
{
    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format   = ma_format_f32;   // Float 32
    config.playback.channels = 2;               // Stereo
    config.sampleRate        = 48000;
    config.dataCallback      = data_callback;
    config.pUserData         = this;

    if (ma_device_init(NULL, &config, &m_device) != MA_SUCCESS) {
        qWarning() << "Failed to initialize miniaudio device.";
        return false;
    }

    if (ma_device_start(&m_device) != MA_SUCCESS) {
        qWarning() << "Failed to start miniaudio device.";
        ma_device_uninit(&m_device);
        return false;
    }

    m_isInitialized = true;
    qDebug() << "Miniaudio initialized successfully.";
    return true;
}

void AudioEngine::loadSample(int padIndex, const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open sample file:" << filePath;
        return;
    }
    QByteArray fileData = file.readAll();
    file.close();

    // Decode with miniaudio from memory buffer
    ma_decoder decoder;
    ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 2, 48000);
    
    if (ma_decoder_init_memory(fileData.constData(), fileData.size(), &config, &decoder) != MA_SUCCESS) {
        qWarning() << "Failed to decode sample:" << filePath;
        return;
    }

    // Allocate memory for the entire sample
    ma_uint64 frameCount;
    ma_decoder_get_length_in_pcm_frames(&decoder, &frameCount);
    
    float* pData = (float*)ma_malloc(frameCount * 2 * sizeof(float), nullptr);
    if (!pData) {
        ma_decoder_uninit(&decoder);
        return;
    }

    ma_uint64 framesRead;
    ma_decoder_read_pcm_frames(&decoder, pData, frameCount, &framesRead);
    ma_decoder_uninit(&decoder);

    SampleData sample;
    sample.pData = pData;
    sample.frameCount = framesRead;
    sample.channels = 2;
    sample.sampleRate = 48000;

    // Free previous if exists
    if (m_samples.contains(padIndex)) {
        ma_free(m_samples[padIndex].pData, nullptr);
    }

    m_samples[padIndex] = sample;
    qDebug() << "Loaded sample to pad" << padIndex << "frames:" << framesRead;
}

void AudioEngine::playSample(int padIndex, float velocity)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    
    if (!m_samples.contains(padIndex)) {
        return; // No sample loaded
    }

    // Find a free voice
    for (int i = 0; i < MAX_VOICES; ++i) {
        if (!m_voices[i].active) {
            m_voices[i].padIndex = padIndex;
            m_voices[i].currentFrame = 0;
            m_voices[i].volume = velocity;
            m_voices[i].active = true;
            qDebug() << "Playing pad" << padIndex << "on voice" << i;
            return;
        }
    }
    qDebug() << "No free voices to play pad" << padIndex;
}

void AudioEngine::stopSample(int padIndex)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    for (int i = 0; i < MAX_VOICES; ++i) {
        if (m_voices[i].active && m_voices[i].padIndex == padIndex) {
            m_voices[i].active = false;
        }
    }
}

void AudioEngine::processAudio(float* pOutput, const float* pInput, ma_uint32 frameCount)
{
    // Clear output buffer
    std::memset(pOutput, 0, frameCount * 2 * sizeof(float));

    std::lock_guard<std::mutex> lock(m_audioMutex);

    for (int v = 0; v < MAX_VOICES; ++v) {
        Voice& voice = m_voices[v];
        if (!voice.active) continue;

        if (!m_samples.contains(voice.padIndex)) {
            voice.active = false;
            continue;
        }

        const SampleData& sample = m_samples[voice.padIndex];
        float* sampleData = sample.pData;
        
        for (ma_uint32 i = 0; i < frameCount; ++i) {
            if (voice.currentFrame >= sample.frameCount) {
                voice.active = false;
                break;
            }

            // Mixing logic (Interleaved Stereo)
            ma_uint64 sampleIdx = voice.currentFrame * sample.channels;
            ma_uint32 outIdx = i * 2;

            if (sample.channels == 2) {
                pOutput[outIdx] += sampleData[sampleIdx] * voice.volume;
                pOutput[outIdx + 1] += sampleData[sampleIdx + 1] * voice.volume;
            } else if (sample.channels == 1) {
                // Mono to stereo
                float val = sampleData[sampleIdx] * voice.volume;
                pOutput[outIdx] += val;
                pOutput[outIdx + 1] += val;
            }

            voice.currentFrame++;
        }
    }
}
