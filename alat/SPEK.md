# Spesifikasi bersama library Arduino berbahasa Indonesia

Folder kerja: `D:\deo\projects\library-arduino\<NamaLibrary>\` (buat baru).
Contoh acuan yang sudah jadi dan lolos semua cek: `D:\deo\projects\library-arduino\TombolPintar\`.
**Baca seluruh isi TombolPintar dulu** (src, examples, extras/test, README.md, README.en.md,
library.properties, keywords.txt, .github/workflows/ci.yml) dan tiru polanya persis: gaya komentar,
gaya README, struktur tabel, nada bahasa.

## Pasar & bahasa
- Target pengguna Indonesia: pelajar, mahasiswa, maker, peserta lomba robot.
- API (nama kelas, fungsi, parameter, enum), komentar kode, contoh, dan `README.md` berbahasa Indonesia
  yang natural (bukan terjemahan kaku). Istilah teknis yang lazim boleh tetap: PID, filter, state, buffer.
- `README.en.md` ringkas dalam bahasa Inggris: contoh cepat, alasan memilih, tabel fungsi Indonesia → Inggris.
  Ditautkan dari baris atas `README.md` (`[English](README.en.md)`), dan sebaliknya.
- `paragraph` di `library.properties` diakhiri `English: <kata kunci Inggris>`.
- Tidak ada topik/fitur SARA (agama, suku). Tidak ada fitur terkait ibadah atau kalender keagamaan.

## Teknis
- Spesifikasi Library 1.5: `library.properties`, `src/`, `examples/`, `keywords.txt`, `README.md`,
  `README.en.md`, `LICENSE` (salin dari TombolPintar), `.github/workflows/ci.yml`, `extras/test/`.
- `library.properties`: version=1.0.0, author & maintainer `Amadeo Wisesa <wisesaamadeo@gmail.com>`,
  url=https://github.com/nothinx/<NamaLibrary>, architectures=*, includes=<Nama>.h, category yang sesuai
  dari daftar resmi (Display, Communication, Signal Input/Output, Sensors, Device Control, Timing,
  Data Storage, Data Processing, Other).
- Non-blocking. Tanpa `delay()` di library. Tanpa alokasi dinamis (`new`, `malloc`, `String`) di library;
  pakai buffer tetap/template. Hemat RAM (Uno hanya 2 KB). Tabel konstan besar di PROGMEM jika berarti.
- `bool` sebagai tanda berhasil/gagal. Aman terhadap luapan `millis()`/`micros()`.
- Header ringkas bergaya TombolPintar: penjelasan singkat di atas, komentar satu baris per fungsi publik.
- Kode sesingkat mungkin yang benar. Tanpa abstraksi spekulatif, tanpa fitur "untuk nanti".
- Compile **tanpa warning** dari kode library dan contoh (`--warnings all`).

## Riset singkat (wajib, sebelum menulis kode)
- Cari 2–3 library pesaing teratas di `C:\Users\HP\AppData\Local\Arduino15\library_index.json`
  (field name/sentence/paragraph/website) lalu baca source-nya di GitHub (WebFetch raw.githubusercontent.com)
  atau issue-nya. Catat kekurangan **nyata** yang bisa ditunjukkan.
- Klaim perbandingan di README hanya yang sudah dibuktikan (dari source, issue, atau pengukuran sendiri).
  Jangan mengarang angka bintang/RAM pesaing. Jika tidak diukur, jangan dicantumkan.

## Uji logika di PC (wajib)
- `extras/test/Arduino.h` tiruan (minimal: tipe, `millis()`/`micros()` palsu, Print/Stream tiruan bila perlu)
  + `extras/test/uji.cpp` berbasis `assert`, mencetak "Semua uji lolos" saat sukses.
- Jalankan: `g++ -std=c++11 -Wall -Wextra -Werror -I. -I../../src uji.cpp ../../src/*.cpp -o $TEMP/uji_<nama>.exe && $TEMP/uji_<nama>.exe`
  (g++ ada di PATH via msys64). Executable jangan ditaruh di folder repo.
- Uji kasus tepi: nilai batas, luapan waktu, input salah, buffer penuh.
- CI: salin `.github/workflows/ci.yml` dari TombolPintar apa adanya (lint + compile 7 board + job uji-logika).
  Sesuaikan perintah g++ job uji-logika bila ada lebih dari satu file .cpp (`../../src/*.cpp`).

## Compile lokal
Untuk tiap contoh, di 3 board (CI di GitHub akan menguji 7 board):
```
arduino-cli compile -b arduino:avr:uno --library <folder> --warnings all <folder>/examples/<Contoh> < /dev/null
arduino-cli compile -b esp32:esp32:esp32 --library <folder> --warnings all ... < /dev/null
arduino-cli compile -b STMicroelectronics:stm32:GenF1:pnum=BLUEPILL_F103C8 --library <folder> --warnings all ... < /dev/null
```
Selalu tambahkan `< /dev/null`. ESP32 DevKit tidak punya `LED_BUILTIN` — beri fallback `#ifndef LED_BUILTIN / #define LED_BUILTIN 2`.
Laporkan RAM yang dipakai library di Uno jika relevan (bandingkan sketch kosong vs sketch dengan objek).

## Lint
`D:/deo/projects/library-arduino/alat/arduino-lint.exe --library-manager submit --compliance strict <NamaLibrary>`
dijalankan dari `D:/deo/projects/library-arduino` (pakai nama folder, bukan `.`).
Satu-satunya error yang boleh: LP042 (URL 404, karena repo belum dibuat).

## Git
```
cd <folder> && git init -q -b main && git add -A
git -c user.name="Amadeo Wisesa" -c user.email="wisesaamadeo@gmail.com" commit -qm "feat: rilis awal <Nama> v1.0.0"
```
Tanpa co-author, tanpa trailer apa pun. **Jangan push, jangan buat repo GitHub, jangan buat PR.**
Jangan sentuh folder library lain.

## README
- Ikuti struktur README TombolPintar: judul, tautan English, deskripsi 1–2 kalimat, Fitur, Board yang didukung,
  Instalasi, Contoh cepat, Referensi fungsi (tabel), Contoh yang tersedia, perbandingan (hanya klaim terbukti),
  Pengujian (`extras/test`), Status (jujur: lolos uji logika di PC + compile; belum diuji di hardware jika memang
  belum; untuk library murni perangkat lunak cukup sebut lolos uji logika), Lisensi.
- Gaya: kalimat pendek, jelas, tanpa basa-basi pemasaran.

## Laporan akhir (balasan kamu)
Per library: nama, ringkasan API (daftar fungsi), daftar contoh, hasil uji PC (jumlah assert/kasus), hasil compile
3 board (lolos/gagal + warning), hasil lint, RAM di Uno, temuan riset pesaing yang dipakai di README, dan hash commit.
Sebutkan juga keputusan desain penting atau keterbatasan yang perlu diketahui.
