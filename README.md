# Tugas Besar 1 IF2010 Pemrograman Berorientasi Objek - Nimonspoli

## Anggota Kelompok
- 13524024 Billie Bhaskara Wibawa
- 13524058 Muhammad Naufal Romi Annafi
- 13524072 I Gusti Ngurah Alit Dharma Yuda
- 13524082 Zeki Amani
- 13524083 Mahmudia Kimdaro Amin

## Deskripsi
Nimonspoli adalah simulasi permainan papan monopoli digital yang mendukung 2 hingga 4 pemain untuk berkompetisi dalam mengumpulkan kekayaan. Pemain dapat membeli properti, mengelola aset, berpartisipasi dalam lelang, dan mengatur strategi keuangan. Permainan ini mengintegrasikan mekanisme kompleks seperti penggunaan kartu kemampuan spesial, efek festival, sistem gadai, hingga manajemen likuidasi saat pemain menghadapi kebangkrutan.

## Fitur Utama
- **Papan Permainan Kompleks**: Terdiri dari 40 petak yang mencakup properti street, stasiun (railroad), utilitas, dan berbagai petak aksi.
- **Sistem Pemain**: Mendukung 2–4 pemain dengan penentuan urutan giliran secara acak.
- **Mekanisme Dadu**: Tersedia mode lempar dadu acak maupun pengaturan manual untuk keperluan pengujian.
- **Manajemen Properti**: Fitur pembelian aset, pembangunan rumah dan hotel dengan aturan pemerataan, serta sistem sewa otomatis.
- **Sistem Ekonomi**: Mekanisme lelang otomatis bagi aset yang tidak terbeli, pengenaan pajak (PPH/PBM), dan fitur festival untuk melipatgandakan nilai sewa.
- **Kartu Spesial**: Implementasi 7 jenis Kartu Kemampuan Spesial (seperti MoveCard, ShieldCard, hingga DemolitionCard) serta Kartu Kesempatan dan Dana Umum.
- **Manajemen Aset**: Sistem gadai dan tebus properti untuk menjaga likuiditas pemain.
- **Utilitas Sistem**: Fitur Save/Load permainan melalui file teks terstruktur, Transaction Logger untuk mencatat riwayat aksi, dan antarmuka berbasis terminal dengan dukungan ANSI color.

## Struktur Program
```text
tugas-besar-1-tsr/
├── config/    # File konfigurasi permainan (.txt)
├── include/   # Header file (.hpp)
├── src/       # Kode sumber program (.cpp)
├── data/      # File penyimpanan/save game
├── makefile   # Skrip otomasi kompilasi
└── README.md  # Dokumentasi proyek
```

## Konsep OOP yang Diimplementasikan

### Inheritance & Polymorphism
Diterapkan secara luas pada hierarki **Tile** dan **Card**. Kelas dasar (seperti `Tile` dan `Card`) mendefinisikan kontrak utama melalui metode virtual murni (misal: `onLand()` dan `onDraw()`). Kelas turunan seperti `StreetTile`, `RailroadTile`, atau `MoveSkillCard` mengimplementasikan logika spesifik namun tetap dapat dikelola secara seragam melalui pointer kelas dasar.

### Method & Operator Overloading
Diimplementasikan pada kelas **Player** untuk mempermudah transaksi keuangan. Operator `+=` dan `-=` di-overload agar manipulasi saldo pemain lebih intuitif dan mudah dibaca dalam kode program.

### Template & Generic Classes
Digunakan pada kelas **Deck<T>**. Dengan menggunakan template, satu kelas generik dapat digunakan untuk mengelola berbagai jenis tumpukan kartu (seperti `ActionCard` atau `SkillCard`) secara aman dan efisien.

### Exception Handling
Sistem menangani kesalahan saat runtime menggunakan blok *try-catch*. Terdapat `SaveLoadException` untuk menangani kegagalan akses file dan `GameplayException` untuk menangani kesalahan logika selama permainan berlangsung (seperti penarikan kartu dari deck yang kosong).

### C++ Standard Template Library (STL)
- **Container**: Menggunakan `std::vector` untuk koleksi objek dinamis dan `std::map`/`std::unordered_map` untuk pemetaan perintah serta konfigurasi.
- **Smart Pointers**: Penggunaan `std::unique_ptr` dan `std::shared_ptr` untuk manajemen memori otomatis (RAII) guna mencegah kebocoran memori (*memory leak*).
- **Algorithms**: Pemanfaatan `std::sort`, `std::shuffle`, dan `std::find_if` untuk pemrosesan data koleksi secara efisien.
- **Streams**: Menggunakan `std::stringstream` untuk pemrosesan format data saat melakukan proses Save/Load.

### Composition & Association
Mendefinisikan hubungan antar objek, seperti **Composition** pada `Board` yang memiliki kumpulan `Tile`, serta **Association** antara `Player` dengan `PropertyTile` yang mereka miliki.

### Abstract Base Class
Digunakan sebagai fondasi hierarki kelas agar struktur program lebih modular dan mudah diperluas. Contoh utamanya adalah kelas `Effect`, `Saveable`, `Card`, serta sub-kelasnya seperti `ActionCard` dan `SkillCard`.

## Prasyarat
- Compiler: g++ dengan dukungan standar C++17
- OS: Linux / WSL / macOS
- Tools: make

## Cara Kompilasi
Gunakan perintah berikut di terminal pada direktori utama:
- `make all`: Membangun (build) seluruh program.
- `make rebuild`: Melakukan pembersihan file objek kemudian membangun ulang program.
- `make run`: Menjalankan program yang telah dikompilasi.

## Daftar Perintah
| Perintah | Deskripsi |
| :--- | :--- |
| **CETAK_PAPAN** | Menampilkan visualisasi papan permainan saat ini |
| **LEMPAR_DADU** | Melempar dadu untuk menentukan pergerakan pemain |
| **ATUR_DADU X Y** | Mengatur nilai dadu secara manual (X dan Y antara 1–6) |
| **CETAK_AKTA [KODE]** | Menampilkan informasi detail akta kepemilikan suatu properti |
| **CETAK_PROPERTI** | Menampilkan daftar seluruh properti yang dimiliki pemain |
| **GADAI** | Melakukan penggadaian properti untuk mendapatkan dana |
| **TEBUS** | Menebus properti yang sedang digadaikan |
| **BANGUN** | Membangun rumah atau hotel pada properti street |
| **GUNAKAN_KEMAMPUAN** | Mengaktifkan kartu kemampuan spesial milik pemain |
| **BAYAR_DENDA** | Membayar denda agar bisa keluar dari penjara |
| **SIMPAN [FILE]** | Menyimpan progres permainan ke dalam file |
| **MUAT [FILE]** | Memuat progres permainan dari file yang ada |
| **CETAK_LOG [N]** | Menampilkan N baris riwayat kejadian terakhir dalam game |
