import wave
import numpy as np
import matplotlib.pyplot as plt
import soundfile as sf
import scipy.signal as signal

from scipy.io import wavfile

def print_audio_signal(file_name, title):
    wav_file = wave.open(file_name, 'rb')
    sample_width = wav_file.getsampwidth()
    sample_rate = wav_file.getframerate()
    num_frames = wav_file.getnframes()
    audio_data = wav_file.readframes(num_frames)
    wav_file.close()
    audio_np = np.frombuffer(audio_data, dtype=np.int16)
    duration = num_frames / sample_rate
    time = np.linspace(0, duration, num=len(audio_np))
    plt.figure(figsize=(10, 4))
    plt.plot(time, audio_np)
    plt.xlabel('Tiempo (s)')
    plt.ylabel('Amplitud')
    plt.title(f'Onda de Audio de {title}')
    plt.grid(True)
    plt.show()

def plot_dual_waveforms(file1, file2):
    # Cargar los archivos WAV
    sample_rate1, waveform1 = wavfile.read(file1)
    sample_rate2, waveform2 = wavfile.read(file2)

    # Calcular los ejes de tiempo para ambas formas de onda
    duration1 = len(waveform1) / sample_rate1
    duration2 = len(waveform2) / sample_rate2
    time1 = np.linspace(0., duration1, len(waveform1))
    time2 = np.linspace(0., duration2, len(waveform2))

    # Crear la figura y los ejes
    fig, ax = plt.subplots()

    # Trazar la primera forma de onda en color azul
    ax.plot(time1, waveform1, color='blue', label='Original')

    # Trazar la segunda forma de onda en color rojo
    ax.plot(time2, waveform2, color='red', label='Filtrado')

    # Configurar etiquetas y título del gráfico
    ax.set_xlabel('Time (s)')
    ax.set_ylabel('Amplitude')
    ax.set_title('Defirencia entre ambas ondas')
    plt.grid(True)
    ax.legend()
    plt.show()

def get_info_from_audio(file_name):
    info = sf.info(file_name)
    print("-------------------------------------\n")
    print(info)
    print("-------------------------------------\n")

def combine_audio_files(file_name1, file_name2, output_file):
    data1, samplerate1 = sf.read(file_name1)
    data2, samplerate2 = sf.read(file_name2)

    if samplerate1 != samplerate2:
        raise ValueError("Las frecuencias de muestreo no coinciden")

    combined_data = np.concatenate((data1, data2))
    sf.write(output_file, combined_data, samplerate1)


def aplicar_filtro_frecuencia(input_file, output_file, freq_min, freq_max):
    # Cargar el archivo WAV
    sample_rate, data = wavfile.read(input_file)
    freq_min /= sample_rate
    freq_max /= sample_rate

    b, a = signal.butter(8, [freq_min, freq_max], btype='bandstop')

    # Aplicar el filtro a los datos
    filtered_data = signal.lfilter(b, a, data)
    
    # Guardar los datos filtrados en un nuevo archivo WAV
    wavfile.write(output_file, sample_rate, np.int16(filtered_data))

def add_echo(input_audio, echo_delay=0.5, echo_gain=0.6):
    sample_rate, audio = wavfile.read(input_audio)

    echo_samples = int(echo_delay * sample_rate)

    echo_signal = np.zeros_like(audio)
    echo_signal[echo_samples:] = audio[:-echo_samples] + echo_gain * audio[echo_samples:]

    return echo_signal, sample_rate

def reverse_audio(input_audio):
    sample_rate, audio = wavfile.read(input_audio)
    reversed_signal = np.flip(audio)

    return reversed_signal, sample_rate

def process_audio_with_echo_and_reverse(input_file, output_file, echo_delay=0.5, echo_gain=0.6):
    # Aplicar eco
    echo_signal, sample_rate = add_echo(input_file, echo_delay, echo_gain)
    sf.write('eco.wav', echo_signal, sample_rate)
    # Invertir el sonido
    reversed_signal, _ = reverse_audio('eco.wav')

    # Guardar la señal invertida como un archivo WAV
    sf.write(output_file, reversed_signal, sample_rate)
    print("Procesamiento completado. Archivo guardado como:", output_file)

###########################################################################3##



# Nombre del archivo de audio para imprimir la onda
file_name = 'nombre.wav'
file_surname = 'apellidos.wav'
################# Ejercicios 1 y 2 #############################################
# Leemos dos archivos wav, nombre y apellido y mostramos sus ondas de sonido
################################################################################

print_audio_signal(file_name, 'nombre')
print_audio_signal(file_surname, 'apellido')

# Nombre del archivo de audio para obtener información de la cabecera
get_info_from_audio(file_name)
get_info_from_audio(file_surname)

# Nombres de los archivos de audio para combinar
output_file = 'audio_combined.wav'
combine_audio_files(file_name, file_surname, output_file)
print_audio_signal(output_file, 'Nombre completo')
# get_info_from_audio(output_file)

mix_wav = 'mezcla.wav'
# Aplicando filtro
aplicar_filtro_frecuencia('audio_combined.wav', 'mezcla.wav', 10000, 20000)
print_audio_signal(mix_wav, ' filtro')
plot_dual_waveforms('audio_combined.wav', 'mezcla.wav')

process_audio_with_echo_and_reverse('audio_combined.wav', 'alreves.wav')
