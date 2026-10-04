# Spesifikasi: benchmark & optimasi performa

Tujuan: membuktikan dengan angka (dan memperbaiki bila perlu) bahwa library kita memakai kemampuan MCU sebaik
mungkin dan menutup kelemahan library sebelumnya: kecepatan (siklus CPU), RAM, flash, kompleksitas (Big O),
dan cara memakai. Hasil akhirnya tabel jujur di README, termasuk bila di satu hal kita kalah.

Library sudah dirilis & terdaftar di Library Manager. Folder: `D:\deo\projects\library-arduino\<Nama>\`.
Baca seluruh src/, extras/test/, README.md, README.en.md dulu.

## Alat ukur: simavr (ATmega328P 16 MHz, akurat per siklus)
- Runner: `bash D:/deo/projects/library-arduino/alat/bench/bench.sh <folder-sketch> --library <folder-library> [--library ...]`
  Mencetak `FLASH`, `RAM`, dan `BENCH <nama> <siklus per panggilan>`. Docker (image `simavr`) sudah berjalan.
- Sketch benchmark memakai `#include "Siklus.h"` (bench.sh menyalinnya otomatis ke folder sketch) dan makro
  `UKUR("nama", ulang, kode);` lalu `selesai();` di akhir `setup()`. Contoh: `D:/deo/projects/library-arduino/alat/bench/ukur/coba/coba.ino`.
  Timer0 (millis) dimatikan selama pengukuran; kode yang diuji tidak boleh bergantung pada millis yang maju
  di dalam satu UKUR. Jika kode bergantung waktu (debounce, interval), ukur jalur "tidak ada kejadian"
  (paling sering) dan jalur "ada kejadian" terpisah dengan mengatur kondisi sebelum UKUR.
- Pakai `volatile` untuk masukan/keluaran agar kompiler tidak membuang kode. Siklus mencakup overhead loop
  (beberapa siklus) — sama untuk semua pembanding, jadi adil.
- Konversi ke waktu: µs = siklus / 16.

## Pesaing
- Ambil 2–3 pesaing yang sudah disebut di README (atau yang dominan) **tanpa memasang ke folder libraries
  pengguna**: unduh source ke `D:/deo/projects/library-arduino/alat/bench/pesaing/<Nama>/` (git clone --depth 1 tag rilis, atau zip
  dari GitHub/`downloads.arduino.cc`), lalu `--library` ke folder itu. Catat versi.
- Beban kerja harus setara (fungsi yang sama, ukuran sama, tipe data yang wajar untuk tiap library).
  Jika pesaing memakai int dan kita float, ukur keduanya apa adanya dan jelaskan di README.

## Analisis & optimasi
1. Tulis Big O tiap fungsi publik di jalur panas (per panggilan, waktu & memori).
2. Ukur: siklus jalur panas, RAM per objek, flash tambahan (sketch dengan library − sketch kosong setara).
3. Cari pemborosan nyata, contoh:
   - promosi `double` tak sengaja (literal tanpa `f`, `fabs`/`sqrt`/`sin` versi double): di ESP32/STM32 double
     diemulasi perangkat lunak dan jauh lebih lambat. Compile contoh dengan
     `--build-property "compiler.cpp.extra_flags=-Wdouble-promotion"` di `esp32:esp32:esp32` dan
     `STMicroelectronics:stm32:GenF4:pnum=BLACKPILL_F411CE`; harus nol peringatan dari src/.
   - pembagian/modulo di jalur panas yang bisa diganti perkalian/geser/pembanding; perhitungan berulang yang bisa
     disimpan; pencarian linear yang bisa O(log n) bila n bisa besar; sin/cos yang bisa dihindari; float di AVR
     bila integer setara dan API tidak berubah; fungsi kecil yang sebaiknya `inline`; tabel yang sebaiknya PROGMEM.
4. Optimasi **hanya di dalam library**. API publik tidak boleh berubah atau rusak (semua contoh dan uji lama tetap
   compile & lolos). Jangan mengorbankan kebenaran/ketelitian; jika ketelitian berubah, uji & dokumentasikan.
   Jangan menambah kerumitan demi < 5% kecuali di jalur yang dipanggil ribuan kali per detik.
5. Tambah uji di extras/test untuk setiap jalur baru (mis. binary search, jalur integer).
6. Ukur ulang setelah optimasi. Simpan angka sebelum/sesudah untuk laporan.
7. Jika library kita kalah di suatu metrik dan tidak masuk akal dikejar (mis. PID float vs fixed-point FastPID),
   jangan paksakan; tulis tarik-ulurnya dengan jujur.

## Aset di repo
- `extras/benchmark/<Nama>Benchmark/<Nama>Benchmark.ino` + `Siklus.h` (salinan) — sketch yang mengukur library
  kita saja (tanpa pesaing, agar repo tidak bergantung library lain). Sketch pembanding pesaing cukup di scratchpad;
  sebut versi pesaing di README.
- Pastikan sketch benchmark tidak ikut dianggap contoh oleh Arduino IDE (extras/ memang tidak) dan tidak memecah lint.

## README
- `README.md`: bagian `## Kecepatan & memori` (setelah Hasil simulasi bila ada). Isi:
  - tabel: fungsi/skenario | TombolPintar(kita) | pesaing A | pesaing B …, satuan siklus dan µs di Uno 16 MHz;
    RAM per objek; flash tambahan. Sebut versi pesaing dan bahwa diukur dengan simavr (simulator ATmega328P
    akurat per siklus), cara mengulang (sketch di extras/benchmark).
  - baris Big O singkat untuk fungsi jalur panas.
  - kalimat jujur bila kalah di suatu metrik + alasannya.
- `README.en.md`: bagian `## Speed & memory` ringkas dengan tabel yang sama.
- Perbarui angka lain di README bila berubah karena optimasi (mis. RAM per objek).

## Versi
- Jika src/ berubah: naikkan `version` di library.properties: perbaikan internal tanpa API baru → patch
  (1.0.0 → 1.0.1); ada fungsi publik baru → minor (1.1.0). Jika hanya dokumentasi → jangan ubah versi.

## Pemeriksaan sebelum commit
1. extras/test lolos (perintah g++ di README bagian Pengujian, dengan -Wall -Wextra -Werror).
2. `python extras/simulasi/gambar.py` (jika ada) tetap jalan; jika angka grafik berubah karena optimasi, commit SVG baru.
3. Semua contoh compile tanpa warning di `arduino:avr:uno`, `esp32:esp32:esp32`,
   `STMicroelectronics:stm32:GenF1:pnum=BLUEPILL_F103C8` (`--warnings all`, `< /dev/null`).
4. Lint dari `D:/deo/projects/library-arduino`:
   `D:/deo/projects/library-arduino/alat/arduino-lint.exe --library-manager submit --compliance strict <Nama>` → bersih (mode submit, karena index belum memuat library baru; LP018 di mode update wajar).

## Git
`git -c user.name="Amadeo Wisesa" -c user.email="wisesaamadeo@gmail.com" commit -qm "<perf|docs>: ..."`
(Conventional Commits, bahasa Indonesia). Boleh 1–2 commit (mis. `perf: ...` lalu `docs: tambah benchmark`).
Tanpa co-author/trailer. **Jangan push, jangan tag, jangan buat release.** Jangan sentuh folder library lain
(kecuali membaca). Jangan memasang library ke folder libraries pengguna.

## Laporan akhir
Per library: tabel sebelum/sesudah (siklus, RAM, flash) vs pesaing (dengan versi), Big O, daftar optimasi yang
dilakukan (dan yang sengaja tidak), hasil -Wdouble-promotion, hasil pemeriksaan 1–4, versi baru (jika berubah),
hash commit, dan di mana kita masih kalah + alasannya.
