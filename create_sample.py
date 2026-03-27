import numpy as np
import wave
import struct
import os

os.makedirs('assets/samples', exist_ok=True)

sample_rate = 48000
duration = 0.5
t = np.linspace(0, duration, int(sample_rate * duration))

# Kick drum synthesis
frequency = 150 * np.exp(-t * 20)
waveform = np.sin(2 * np.pi * frequency * t) * np.exp(-t * 8)
audio = np.int16(waveform * 32767)

with wave.open('assets/samples/kick.wav', 'w') as obj:
    obj.setnchannels(1) 
    obj.setsampwidth(2)
    obj.setframerate(sample_rate)
    for value in audio:
        obj.writeframesraw(struct.pack('<h', value))
