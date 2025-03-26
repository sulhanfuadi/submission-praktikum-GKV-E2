/* Nama File : grabRefactored.cpp
  Deskripsi : Program simulasi robot yang dapat mengambil dan memindahkan kotak
              dengan interaksi keyboard dan efek visual yang ditingkatkan
  Pembuat   : Sulhan Fuadi (24060123130115)
  Tanggal   : 26 Maret 2025
*/

#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14

// Variabel untuk posisi dan orientasi kamera
float angle = 0.0, deltaAngle = 0.0, ratio;
float x = 5.0f, y = 10.0f, z = 40.0f; // posisi awal kamera
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
int deltaMove = 0, h, w;
static int rotAngleX = 0, rotAngleY = 0, rotAngleZ = 0;

// Posisi objek
float posXBadan = 10, posXKotak = 0, posYBadan = 7, posYKotak = 6;
float rotTangan1 = 0.0, rotTangan2 = 0.0, rotTangan3 = 0.0, rotTangan4 = 0.0;

// Status interaksi
int kick = 0, roll = 0, hit = 0, gerakTangan = 0, drop = 0, bring = 0, grab = 0, tabrak = 0;
bool dalamProsesGrab = false; // Flag baru untuk menandai proses grab sedang berlangsung
bool dalamProsesDrop = false; // Flag baru untuk menandai proses drop sedang berlangsung

// Batas collision
float jarakAmbil = 4.0;      // Jarak maksimum untuk mengambil kotak (diperbesar)
float toleransiTabrak = 1.0; // Toleransi untuk tabrakan

// Fisika
float kecepatanJatuh = 0.2;   // Kecepatan jatuh kotak (dipercepat)
float batasLantai = 3.0;      // Posisi Y lantai untuk kotak
float posXMeja = 0.0;         // Posisi X meja
float lebarMeja = 5.0;        // Lebar meja
float sudutRotasiKotak = 0.0; // Untuk animasi rotasi kotak
float faktorkecepatan = 1.0;  // Untuk mempercepat/memperlambat animasi

// Offset relatif kotak dari robot saat dibawa
float offsetXKotak = 0.0;
float offsetYKotak = -2.0;

// Efek visual
float warnaMejaR = 0.1, warnaMejaG = 0.1, warnaMejaB = 0.2;
float warnaKotakR = 0.8, warnaKotakG = 0.3, warnaKotakB = 0.3;
float warnaRobotR = 0.3, warnaRobotG = 0.3, warnaRobotB = 0.8;
float warnaTanganR = 0.2, warnaTanganG = 0.5, warnaTanganB = 0.2;
float deltaWarna = 0.005; // Kecepatan perubahan warna
int efekWarna = 0;        // Flag untuk efek warna

// Pesan status dan feedback
char pesanStatus[100] = "";
int waktuTampilPesan = 0;

GLUquadricObj *IDquadric;

// Variable untuk pencahayaan
const GLfloat light_ambient[] = {0.5f, 0.5f, 0.5f, 0.0f};
const GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_position[] = {0.0f, 20.0f, 10.0f, 1.0f};
const GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
const GLfloat mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
const GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat high_shininess[] = {100.0f};

// Prototipe untuk fungsi pembantu
void tampilkanPesan(const char *pesan, float x, float y);
void aturPesanStatus(const char *pesan);
void animasiPerubahanWarna();
void animasiKotak();
void buatPartikel(float x, float y, float z);

void init(void)
{
  // Inisialisasi pengaturan rendering
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  IDquadric = gluNewQuadric();              // Membuat pointer untuk objek quadric
  gluQuadricNormals(IDquadric, GLU_SMOOTH); // Membuat normal yang halus
  gluQuadricTexture(IDquadric, GL_TRUE);    // Mengaktifkan koordinat tekstur

  // Mengatur seed untuk generator angka acak
  srand(time(NULL));

  // Pesan awal
  aturPesanStatus("Siap. Gunakan tombol O/P untuk bergerak dan G untuk mengambil kotak.");
}

void Reshape(int w1, int h1)
{
  // Fungsi reshape untuk mengatur aspek tampilan saat ukuran jendela berubah
  if (h1 == 0)
    h1 = 1;
  w = w1;
  h = h1;
  ratio = 1.0f * w / h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, w, h);
  gluPerspective(45, ratio, 0.1, 1000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

void orientMe(float ang)
{
  // Fungsi ini untuk memutar arah kamera (tengok kiri/kanan)
  lx = sin(ang / 10);
  lz = -cos(ang / 10);
  glLoadIdentity();
  gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

void moveMeFlat(int i)
{
  // Fungsi ini untuk maju mundur kamera
  x = x + i * (lx) * 0.1;
  z = z + i * (lz) * 0.1;
  glLoadIdentity();
  gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

// Fungsi yang diperbaiki untuk mengecek apakah robot berada dalam jarak untuk mengambil kotak
bool dalamJarakAmbil()
{
  // Menghitung jarak horizontal antara robot dan kotak
  float jarakX = fabs(posXBadan - posXKotak);

  // Menghitung jarak vertikal antara robot dan kotak
  float jarakY = fabs((posYBadan - 3.5) - posYKotak); // Sesuaikan offset vertikal

  // Kotak dapat diambil jika jarak horizontal dan vertikal dalam batas yang wajar
  return jarakX <= jarakAmbil && jarakY <= 4.0;
}

// Fungsi untuk mengecek apakah robot akan menabrak meja
bool akanTabrakMeja(float pergeseran)
{
  // Prediksi posisi robot setelah pergeseran dan periksa tabrakan dengan meja
  float posXBaruBadan = posXBadan + pergeseran;
  float jarakKeMeja = fabs(posXBaruBadan - posXMeja);

  // Tabrakan hanya terjadi jika kotak masih di atas meja
  return jarakKeMeja < (lebarMeja / 2 + 1.5) && posYKotak >= batasLantai + 1.0;
}

// Fungsi untuk mengecek apakah robot akan menabrak kotak
bool akanTabrakKotak(float pergeseran)
{
  // Jika kotak sedang dipegang, tidak akan menabrak
  if (grab == 1 || bring == 1)
    return false;

  // Prediksi posisi robot setelah pergeseran dan periksa tabrakan dengan kotak
  float posXBaruBadan = posXBadan + pergeseran;
  float jarakKeKotak = fabs(posXBaruBadan - posXKotak);

  // Tabrakan hanya terjadi jika kotak sudah ada di lantai (atau hampir)
  return jarakKeKotak < 3.0 + toleransiTabrak && posYKotak <= batasLantai + 0.2;
}

// Fungsi untuk mengatur pesan status dengan timeout
void aturPesanStatus(const char *pesan)
{
  strcpy(pesanStatus, pesan);
  waktuTampilPesan = 100; // Waktu tampil pesan (dalam frame)
}

void keyboard(unsigned char key, int x, int y)
{
  // Handler untuk keyboard biasa
  switch (key)
  {
  case 'w': // Rotasi ke atas
    rotAngleX += 2;
    break;
  case 's': // Rotasi ke bawah
    rotAngleX -= 2;
    break;
  case 'a': // Rotasi ke kiri
    rotAngleY += 2;
    break;
  case 'd': // Rotasi ke kanan
    rotAngleY -= 2;
    break;
  case 'q': // Rotasi searah jarum jam
    rotAngleZ += 2;
    break;
  case 'e': // Rotasi berlawanan jarum jam
    rotAngleZ -= 2;
    break;
  case 'o': // Robot gerak ke kiri
    if (!akanTabrakMeja(-1) && !akanTabrakKotak(-1))
    {
      posXBadan -= 1;
      if (bring == 1) // Jika sedang membawa kotak
      {
        posXKotak = posXBadan + offsetXKotak; // Kotak mengikuti robot dengan offset tetap
      }
    }
    else
    {
      aturPesanStatus("Tidak bisa bergerak ke kiri, ada penghalang!");
    }
    break;
  case 'p': // Robot gerak ke kanan
    if (!akanTabrakMeja(1) && !akanTabrakKotak(1))
    {
      posXBadan += 1;
      if (bring == 1) // Jika sedang membawa kotak
      {
        posXKotak = posXBadan + offsetXKotak; // Kotak mengikuti robot dengan offset tetap
      }
    }
    else
    {
      aturPesanStatus("Tidak bisa bergerak ke kanan, ada penghalang!");
    }
    break;
  case 'g':                                                       // Perintah untuk grab/mengambil kotak
    if (!dalamProsesGrab && !dalamProsesDrop && gerakTangan == 0) // Hanya jika tidak sedang dalam proses grab/drop
    {
      if (dalamJarakAmbil() && !grab && !bring)
      {
        gerakTangan = 1; // Mulai gerakan mengambil
        dalamProsesGrab = true;
        aturPesanStatus("Mencoba mengambil kotak...");
        efekWarna = 1; // Aktifkan efek warna untuk visual feedback
      }
      else if (grab || bring)
      {
        aturPesanStatus("Kotak sudah dipegang!");
      }
      else
      {
        aturPesanStatus("Terlalu jauh dari kotak!");
      }
    }
    break;
  case 't': // Perintah untuk drop/menjatuhkan kotak
    if (!dalamProsesGrab && !dalamProsesDrop && (grab == 1 || bring == 1))
    {
      drop = 1;
      dalamProsesDrop = true;
      gerakTangan = 3; // Mulai proses melepaskan
      aturPesanStatus("Melepaskan kotak...");
      efekWarna = 1; // Aktifkan efek warna untuk visual feedback
    }
    else if (!grab && !bring)
    {
      aturPesanStatus("Tidak ada kotak yang dipegang!");
    }
    break;
  case 'f': // Ganti kecepatan animasi
    faktorkecepatan = (faktorkecepatan == 1.0) ? 2.0 : 1.0;
    aturPesanStatus(faktorkecepatan == 2.0 ? "Kecepatan: Cepat" : "Kecepatan: Normal");
    break;
  case 'c': // Toggle efek warna
    efekWarna = !efekWarna;
    aturPesanStatus(efekWarna ? "Efek warna: Aktif" : "Efek warna: Nonaktif");
    break;
  case 32: // Spasi - Reset simulasi
    rotAngleX = rotAngleY = rotAngleZ = 0;
    posXBadan = 10;
    posXKotak = 0;
    posYBadan = 7;
    posYKotak = 6;
    rotTangan1 = rotTangan2 = rotTangan3 = rotTangan4 = 0;
    kick = roll = gerakTangan = drop = hit = bring = grab = 0;
    dalamProsesGrab = dalamProsesDrop = false;
    sudutRotasiKotak = 0.0;
    // Reset warna ke default
    warnaMejaR = 0.1;
    warnaMejaG = 0.1;
    warnaMejaB = 0.2;
    warnaKotakR = 0.8;
    warnaKotakG = 0.3;
    warnaKotakB = 0.3;
    warnaRobotR = 0.3;
    warnaRobotG = 0.3;
    warnaRobotB = 0.8;
    warnaTanganR = 0.2;
    warnaTanganG = 0.5;
    warnaTanganB = 0.2;
    aturPesanStatus("Simulasi direset!");
    break;
  case 27: // ESC - Keluar dari program
    exit(0);
  default:
    break;
  }
  glutPostRedisplay();
}

void pressKey(int k, int x, int y)
{
  // Fungsi ini akan dijalankan saat tombol keyboard spesial ditekan
  switch (k)
  {
  case GLUT_KEY_UP: // Kamera maju
    deltaMove = 1;
    break;
  case GLUT_KEY_DOWN: // Kamera mundur
    deltaMove = -1;
    break;
  case GLUT_KEY_LEFT: // Kamera putar kiri
    deltaAngle = -0.01f;
    break;
  case GLUT_KEY_RIGHT: // Kamera putar kanan
    deltaAngle = 0.01f;
    break;
  }
}

void releaseKey(int key, int x, int y)
{
  // Fungsi ini akan dijalankan saat tekanan tombol keyboard dilepas
  switch (key)
  {
  case GLUT_KEY_UP:
    if (deltaMove > 0)
      deltaMove = 0;
    break;
  case GLUT_KEY_DOWN:
    if (deltaMove < 0)
      deltaMove = 0;
    break;
  case GLUT_KEY_LEFT:
    if (deltaAngle < 0.0f)
      deltaAngle = 0.0f;
    break;
  case GLUT_KEY_RIGHT:
    if (deltaAngle > 0.0f)
      deltaAngle = 0.0f;
    break;
  }
}

void lighting()
{
  // Fungsi untuk mengatur pencahayaan pada scene
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}

void Grid()
{
  // Fungsi untuk membuat grid garis pada lantai
  double i;
  const float Z_MIN = -50, Z_MAX = 50;
  const float X_MIN = -50, X_MAX = 50;
  const float gap = 2;
  glColor3f(0.5, 0.5, 0.5);
  glBegin(GL_LINES);
  for (i = Z_MIN; i < Z_MAX; i += gap)
  {
    glVertex3f(i, 0, Z_MIN);
    glVertex3f(i, 0, Z_MAX);
  }
  for (i = X_MIN; i < X_MAX; i += gap)
  {
    glVertex3f(X_MIN, 0, i);
    glVertex3f(X_MAX, 0, i);
  }
  glEnd();
}

void Grid2()
{
  // Fungsi untuk membuat lantai solid
  glColor3f(0.9f, 0.9f, 0.9f); // Warna lantai abu-abu terang
  glBegin(GL_QUADS);
  glVertex3f(-50, 0, 50);
  glVertex3f(-50, 0, -50);
  glVertex3f(50, 0, -50);
  glVertex3f(50, 0, 50);
  glEnd();
}

void Balok(float panjang, float lebar, float tinggi)
{
  // Fungsi untuk menggambar balok dengan tekstur
  glPushMatrix();
  float p = panjang / 2;
  float l = lebar / 2;
  float t = tinggi / 2;

  // Sisi depan
  glBegin(GL_QUADS);
  glNormal3f(0, 0, 1); // Normal menghadap ke depan
  glVertex3f(-p, 0, l);
  glVertex3f(p, 0, l);
  glVertex3f(p, -t * 2, l);
  glVertex3f(-p, -t * 2, l);
  glEnd();

  // Sisi belakang
  glBegin(GL_QUADS);
  glNormal3f(0, 0, -1); // Normal menghadap ke belakang
  glVertex3f(-p, 0, -l);
  glVertex3f(p, 0, -l);
  glVertex3f(p, -t * 2, -l);
  glVertex3f(-p, -t * 2, -l);
  glEnd();

  // Sisi atas
  glBegin(GL_QUADS);
  glNormal3f(0, 1, 0); // Normal menghadap ke atas
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-p, 0, -l);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(p, 0, -l);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(p, 0, l);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-p, 0, l);
  glEnd();

  // Sisi bawah
  glBegin(GL_QUADS);
  glNormal3f(0, -1, 0); // Normal menghadap ke bawah
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-p, -t * 2, -l);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(p, -t * 2, -l);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(p, -t * 2, l);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-p, -t * 2, l);
  glEnd();

  // Sisi kanan
  glBegin(GL_QUADS);
  glNormal3f(-1, 0, 0); // Normal menghadap ke kanan
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-p, -t * 2, -l);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-p, -t * 2, l);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-p, 0, l);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-p, 0, -l);
  glEnd();

  // Sisi kiri
  glBegin(GL_QUADS);
  glNormal3f(1, 0, 0); // Normal menghadap ke kiri
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(p, -t * 2, -l);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(p, -t * 2, l);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(p, 0, l);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(p, 0, -l);
  glEnd();
  glPopMatrix();
}

// Prosedur untuk animasi perubahan warna
void animasiPerubahanWarna()
{
  // Jika efek warna aktif, lakukan perubahan warna
  if (efekWarna)
  {
    // Perubahan warna kotak saat interaksi
    if (grab == 1 || drop == 1)
    {
      warnaKotakR = 0.8 + 0.2 * sin(sudutRotasiKotak / 10);
      warnaKotakG = 0.3 + 0.2 * cos(sudutRotasiKotak / 8);
      warnaKotakB = 0.3 + 0.2 * sin(sudutRotasiKotak / 6);
    }

    // Perubahan warna tangan saat bergerak
    if (gerakTangan > 0)
    {
      warnaTanganR = 0.2 + 0.3 * sin(sudutRotasiKotak / 5);
      warnaTanganG = 0.5 + 0.2 * cos(sudutRotasiKotak / 7);
      warnaTanganB = 0.2 + 0.3 * sin(sudutRotasiKotak / 9);
    }

    // Perubahan warna robot saat membawa kotak
    if (bring == 1)
    {
      warnaRobotR = 0.3 + 0.1 * sin(sudutRotasiKotak / 15);
      warnaRobotG = 0.3 + 0.1 * cos(sudutRotasiKotak / 12);
      warnaRobotB = 0.8 + 0.1 * sin(sudutRotasiKotak / 10);
    }
  }
}

// Prosedur untuk animasi rotasi kotak
void animasiKotak()
{
  // Rotasi kotak untuk efek visual
  sudutRotasiKotak += 0.5 * faktorkecepatan;
  if (sudutRotasiKotak > 360)
    sudutRotasiKotak -= 360;
}

// Prosedur untuk mengupdate posisi kotak
void perubahKotak()
{
  // Jika kotak sedang dibawa, posisinya harus selalu mengikuti robot
  if (bring == 1 && grab == 1 && !drop)
  {
    // Update posisi kotak relatif terhadap robot
    posXKotak = posXBadan + offsetXKotak;
    posYKotak = posYBadan + offsetYKotak;
    return;
  }

  // Jika kotak perlu dijatuhkan
  if (drop == 1)
  {
    // Jatuhkan kotak jika perintah drop diberikan
    if (posYKotak > batasLantai)
    {
      posYKotak -= kecepatanJatuh * faktorkecepatan; // Menggunakan faktor kecepatan
    }

    if (posYKotak <= batasLantai)
    {
      posYKotak = batasLantai;               // Pastikan kotak tidak menembus lantai
      buatPartikel(posXKotak, posYKotak, 0); // Efek partikel saat kotak mendarat

      // Reset status
      bring = 0;
      grab = 0;
      drop = 0;
      dalamProsesDrop = false;
      aturPesanStatus("Kotak diletakkan di lantai.");
    }
  }
}

// Prosedur untuk efek partikel sederhana
void buatPartikel(float x, float y, float z)
{
  // Efek visual sederhana saat kotak jatuh ke lantai
  glPushMatrix();
  glTranslatef(x, y, z);
  glDisable(GL_LIGHTING);
  glColor3f(1.0, 1.0, 0.7); // Warna kuning terang

  // Gambar 10 partikel kecil
  for (int i = 0; i < 10; i++)
  {
    float offsetX = (rand() % 100 - 50) / 20.0;
    float offsetZ = (rand() % 100 - 50) / 20.0;

    glBegin(GL_POINTS);
    glVertex3f(offsetX, 0.1, offsetZ);
    glEnd();

    // Lingkaran kecil sebagai efek debu
    glBegin(GL_POLYGON);
    for (int j = 0; j < 8; j++)
    {
      float angle = 2 * PI * j / 8;
      glVertex3f(offsetX + 0.3 * cos(angle), 0.05, offsetZ + 0.3 * sin(angle));
    }
    glEnd();
  }

  glEnable(GL_LIGHTING);
  glPopMatrix();
}

// Prosedur untuk mengubah gerakan tangan dan mendeteksi interaksi
void pengubahTangan()
{
  // Deteksi apakah robot berada dalam jarak untuk mengambil kotak
  if (dalamJarakAmbil())
  {
    hit = 1;
  }
  else
  {
    hit = 0;
  }

  // Gerakan tangan (mekanisme 4 tahap)
  if (gerakTangan == 1)
  {
    // Tahap 1: Pelebaran tangan samping
    if (rotTangan1 > -90)
    {
      rotTangan1 -= 3.0 * faktorkecepatan; // Kecepatan rotasi dipercepat
    }

    if (rotTangan1 <= -90)
    {
      gerakTangan = 2; // Lanjut ke tahap berikutnya
    }
  }

  if (gerakTangan == 2)
  {
    // Tahap 2: Merapatkan tangan depan (grab)
    if (rotTangan2 > -90)
    {
      rotTangan2 -= 3.0 * faktorkecepatan; // Dipercepat
    }

    if (rotTangan2 <= -90)
    {
      if (hit == 1 && !grab)
      {
        // Berhasil mengambil kotak
        grab = 1;
        bring = 1;

        // Atur posisi kotak relatif terhadap robot
        offsetXKotak = posXKotak - posXBadan;
        offsetYKotak = posYKotak - posYBadan;

        aturPesanStatus("Kotak berhasil diambil!");
      }
      else if (!hit && !grab)
      {
        // Gagal mengambil karena tidak ada kotak di dekatnya
        aturPesanStatus("Tidak ada kotak yang dapat diambil.");
      }

      // Lanjut ke tahap berikutnya
      gerakTangan = 3;
    }
  }

  if (gerakTangan == 3)
  {
    // Tahap 3: Melebarkan tangan depan (release jika menjatuhkan kotak)
    if (rotTangan2 < 0)
    {
      rotTangan2 += 3.0 * faktorkecepatan; // Dipercepat
    }

    if (rotTangan2 >= 0)
    {
      gerakTangan = 4; // Lanjut ke tahap berikutnya

      // Jika sedang menjatuhkan, lepaskan genggaman
      if (drop == 1 && grab == 1)
      {
        grab = 0; // Lepaskan genggaman, tapi kotak masih jatuh
      }
    }
  }

  if (gerakTangan == 4)
  {
    // Tahap 4: Merapatkan tangan samping (kembali ke posisi awal)
    if (rotTangan1 < 0)
    {
      rotTangan1 += 3.0 * faktorkecepatan; // Dipercepat
    }

    if (rotTangan1 >= 0)
    {
      gerakTangan = 0;         // Selesai, kembali ke posisi normal
      dalamProsesGrab = false; // Proses grab selesai

      if (dalamProsesDrop && !grab)
      {
        // Jika selesai proses drop (tangan sudah kembali), tapi kotak belum menyentuh lantai
        if (posYKotak > batasLantai)
        {
          aturPesanStatus("Kotak sedang jatuh...");
        }
      }
    }
  }
}

// Fungsi untuk menampilkan pesan teks pada layar
void tampilkanPesan(const char *pesan, float x, float y)
{
  glDisable(GL_LIGHTING);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0.0, w, 0.0, h);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glColor3f(1.0, 1.0, 1.0); // Warna teks putih
  glRasterPos2f(x, y);

  for (const char *c = pesan; *c != '\0'; c++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
  }

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_LIGHTING);
}

void Object()
{
  // Eksekusi animasi
  animasiKotak();
  animasiPerubahanWarna();

  // Meja
  glPushMatrix();
  glColor3f(warnaMejaR, warnaMejaG, warnaMejaB);
  glTranslatef(posXMeja, 3, 0);
  Balok(lebarMeja, 5, 3);
  glPopMatrix();

  // Kotak (objek yang bisa diambil)
  glPushMatrix();
  perubahKotak(); // Update posisi kotak
  glColor3f(warnaKotakR, warnaKotakG, warnaKotakB);
  glTranslatef(posXKotak, posYKotak, 0);

  // Animasi rotasi kotak saat dibawa oleh robot
  if (bring == 1 && grab == 1)
  {
    glRotatef(sin(sudutRotasiKotak / 20.0) * 5.0, 0, 1, 0); // Bergoyang perlahan
  }

  Balok(3, 3, 3);
  glPopMatrix();

  // Robot dan tangan
  glPushMatrix();
  pengubahTangan(); // Update gerakan tangan

  // Badan robot
  glColor3f(warnaRobotR, warnaRobotG, warnaRobotB);
  glTranslatef(posXBadan, posYBadan, 0);
  Balok(3, 3, 7);

  // Tangan kiri
  glPushMatrix();
  glColor3f(warnaTanganR, warnaTanganG, warnaTanganB);
  glTranslatef(0, -2, 2.5);
  glRotatef(rotTangan1, 1, 0, 0);
  glRotatef(rotTangan2, 0, 0, 1);
  Balok(2, 2, 4);
  glPopMatrix();

  // Tangan kanan
  glPushMatrix();
  glColor3f(warnaTanganR, warnaTanganG, warnaTanganB);
  glTranslatef(0, -2, -2.5);
  glRotatef(-rotTangan1, 1, 0, 0);
  glRotatef(rotTangan2, 0, 0, 1);
  Balok(2, 2, 4);
  glPopMatrix();

  glPopMatrix();

  // Gambar highlight untuk area jangkauan grab jika robot dalam jarak ambil
  if (hit == 1 && !grab && !bring && !dalamProsesGrab && !dalamProsesDrop)
  {
    glPushMatrix();
    glTranslatef(posXKotak, posYKotak - 1.5, 0);
    glDisable(GL_LIGHTING);
    glColor4f(0.0, 1.0, 0.0, 0.3); // Warna hijau transparan

    // Lingkaran untuk menunjukkan area yang bisa di-grab
    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++)
    {
      float angle = 2 * PI * i / 20;
      glVertex3f(2.0 * cos(angle), 0.1, 2.0 * sin(angle));
    }
    glEnd();

    glEnable(GL_LIGHTING);
    glPopMatrix();
  }
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Kalau move dan angle tidak nol, gerakkan kamera...
  if (deltaMove)
    moveMeFlat(deltaMove);
  if (deltaAngle)
  {
    angle += deltaAngle;
    orientMe(angle);
  }

  glPushMatrix();
  glRotated(rotAngleX + 10, 1, 0, 0);
  glRotated(rotAngleY, 0, 1, 0);
  glRotated(rotAngleZ, 0, 0, 1);

  // Gambar grid dan lantai
  Grid();
  Grid2();

  // Gambar objek (robot, kotak, dll)
  Object();

  glPopMatrix();

  // Kurangi waktu tampil pesan jika sedang ada pesan
  if (waktuTampilPesan > 0)
    waktuTampilPesan--;

  // Menampilkan informasi kontrol
  tampilkanPesan("Kontrol: O/P = Gerak Kiri/Kanan, G = Ambil, T = Lepas, F = Kecepatan, C = Efek Warna", 10, 20);

  // Menampilkan status
  char statusPesan[100];
  sprintf(statusPesan, "Status: %s %s %s",
          grab ? "Menggenggam" : "",
          bring ? "Membawa" : "",
          drop ? "Menjatuhkan" : "");
  tampilkanPesan(statusPesan, 10, 40);

  // Tampilkan pesan status jika ada
  if (waktuTampilPesan > 0)
  {
    tampilkanPesan(pesanStatus, 10, h - 20);
  }

  glFlush();
  glutSwapBuffers();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 480);
  glutCreateWindow("Robot Grab Simulator");

  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(display);
  glutReshapeFunc(Reshape);

  lighting();
  init();
  glutMainLoop();
  return (0);
}