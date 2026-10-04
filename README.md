# Library Arduino Berbahasa Indonesia

Kumpulan library Arduino dengan API, contoh, dan dokumentasi berbahasa Indonesia, untuk pelajar, mahasiswa,
maker, dan peserta lomba robot. Semuanya tersedia di **Library Manager** Arduino IDE: cari namanya, klik Install.

Dibuat oleh **Amadeo Wisesa**, Fakultas Teknik Elektronika dan Komputer (FTEK),
Universitas Kristen Satya Wacana (UKSW). Kontak: wisesaamadeo@gmail.com

*English: Arduino libraries with an Indonesian API and docs. Each library has a short `README.en.md`.*

## Daftar library

### Sensor & robot

| Library | Versi | Isi |
|---|---|---|
| [TimbanganHX711](https://github.com/nothinx/TimbanganHX711) | 1.0.0 | Timbangan HX711 + load cell: tidak pernah menunggu, tara & kalibrasi di latar belakang, deteksi kabel lepas. |
| [ArahMPU6050](https://github.com/nothinx/ArahMPU6050) | 1.0.2 | Arah hadap robot dari IMU MPU6050. Membaca FIFO sensor, jadi putaran tidak hilang walau `loop()` lambat. |
| [PosisiRobot](https://github.com/nothinx/PosisiRobot) | 1.1.0 | Odometri robot dua roda: posisi x, y (cm) dan arah dari encoder, plus navigasi ke titik. |
| [KontrolPID](https://github.com/nothinx/KontrolPID) | 1.0.1 | PID dengan dt nyata, anti-windup, dan filter D untuk motor, suhu, dan line follower. |
| [LogikaFuzzy](https://github.com/nothinx/LogikaFuzzy) | 1.0.1 | Fuzzy Mamdani dan Sugeno; aturan ditulis seperti di laporan: `jika(DINGIN).maka(PELAN)`. |

### Olah data sensor

| Library | Versi | Isi |
|---|---|---|
| [FilterSensor](https://github.com/nothinx/FilterSensor) | 1.0.1 | Rata-rata bergerak, median, EMA, dan Kalman 1D tanpa `malloc`. |
| [KalibrasiSensor](https://github.com/nothinx/KalibrasiSensor) | 1.0.1 | Bacaan mentah → nilai nyata lewat tabel titik ukur (Sharp IR, baterai, termistor), bisa di PROGMEM. |

### Input, waktu, dan alur program

| Library | Versi | Isi |
|---|---|---|
| [TombolPintar](https://github.com/nothinx/TombolPintar) | 1.0.0 | Tombol: debounce, klik, klik ganda, klik N kali, tekan lama. 16 byte RAM per tombol. |
| [TanpaDelay](https://github.com/nothinx/TanpaDelay) | 1.0.1 | Pengganti `delay()` tanpa drift: jalankan beberapa hal bersamaan. |
| [AlurProgram](https://github.com/nothinx/AlurProgram) | 1.0.1 | State machine sederhana dengan `switch-case`, 7 byte RAM. |

### Komunikasi & teks

| Library | Versi | Isi |
|---|---|---|
| [PerintahSerial](https://github.com/nothinx/PerintahSerial) | 1.0.1 | Perintah teks dari Serial Monitor, Bluetooth HC-05, atau Stream apa pun (`LED ON`, `SET kp 2.5`). |
| [SandiMorse](https://github.com/nothinx/SandiMorse) | 1.0.0 | Kirim dan baca sandi Morse tanpa `delay()`. |
| [Terbilang](https://github.com/nothinx/Terbilang) | 1.0.1 | Angka → kata ("seratus dua puluh lima") dan format rupiah (`Rp1.250.000`). |

## Standar yang dipegang semua library

- **Non-blocking**: tanpa `delay()` di dalam library, aman saat `millis()`/`micros()` meluap.
- **Hemat memori**: tanpa `new`, `malloc`, atau `String`; RAM per objek tercantum di setiap README.
- **Teruji**: logika diuji otomatis di PC (`extras/test`) dan di-compile di 7 board (Uno, Mega, ESP32,
  ESP32-C3, ESP32-S3, STM32 Blackpill F411, Bluepill F103) lewat GitHub Actions.
- **Diukur, bukan diklaim**: kecepatan, RAM, dan flash dibandingkan dengan library populer di simulator
  ATmega328P yang akurat per siklus (simavr). Hasilnya ditulis apa adanya di README, termasuk saat kalah.
- **Jujur soal status**: semua library lolos uji logika dan compile, tetapi sebagian besar **belum diuji di
  hardware sungguhan**. Status tiap library ada di README-nya. Laporan uji di hardware sangat membantu,
  silakan buka *issue* di repo library terkait.

## Berikutnya

Library sensor untuk modul yang paling banyak dipakai: `TimbanganHX711` sudah terbit,
berikutnya `JarakHCSR04`, `SuhuDHT`, `KelembabanTanah`, dan seterusnya. Standar tambahan untuk library sensor
(waktu terblokir, ketahanan saat kabel dicabut, pesan error berbahasa Indonesia) ada di
[`alat/SPEK-SENSOR.md`](alat/SPEK-SENSOR.md).

## Isi repo ini

Repo ini hanya katalog dan alat bantu. Kode tiap library ada di repo masing-masing.

| Folder | Isi |
|---|---|
| `alat/SPEK*.md` | Standar penulisan library, simulasi, benchmark, dan sensor |
| `alat/bench/` | Runner benchmark simavr (Docker) dan sketch pengukur siklus |
| `alat/*.sh`, `alat/cek-index.py` | Skrip rilis, menunggu CI, dan cek status di Library Manager |

## Lisensi

MIT © 2026 Amadeo Wisesa. Setiap library memiliki file LICENSE sendiri.
